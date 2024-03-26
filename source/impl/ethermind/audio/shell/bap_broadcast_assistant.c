/**
 * @file
 * @brief Shell APIs for Bluetooth BASS client
 *
 * Copyright (c) 2020-2023 Nordic Semiconductor ASA
 * Copyright (C) 2022-2023 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#if (defined(CONFIG_BT_BAP_BROADCAST_ASSISTANT) && (CONFIG_BT_BAP_BROADCAST_ASSISTANT > 0))

#include <porting.h>
#include <zephyr/types.h>
#include "fsl_shell.h"
#include <stdlib.h>
#include <bluetooth/gatt.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/audio/audio.h>
#include <bluetooth/audio/bap.h>
#include <sys/byteorder.h>

#include "shell_bt.h"
#include "../host/bt_pal_hci_core.h"
#include "audio.h"

#define INVALID_BROADCAST_ID 0xFFFFFFFFU
/* BIS sync is a 32-bit bitfield where BIT(0) is not allowed */
#define VALID_BIS_SYNC(_bis_sync) ((bis_sync & BIT(0)) == 0U && bis_sync < UINT32_MAX)

static uint8_t received_base[UINT8_MAX];
static uint8_t received_base_size;

static struct bt_auto_scan {
	uint32_t broadcast_id;
	bool pa_sync;
	struct bt_bap_scan_delegator_subgroup subgroup;
} auto_scan = {
	.broadcast_id = INVALID_BROADCAST_ID,
};

static bool pa_decode_base(struct bt_data *data, void *user_data)
{
	const struct bt_bap_base *base = bt_bap_base_get_base_from_ad(data);

	/* Base is NULL if the data does not contain a valid BASE */
	if (base == NULL) {
		return true;
	}

	/* Compare BASE and print if different */
	if (data->data_len != received_base_size ||
	    memcmp(data->data, received_base, data->data_len) != 0) {
		(void)memcpy(&received_base, data->data, data->data_len);
		received_base_size = data->data_len;

		print_base(base);
	}

	return false;
}

static void pa_recv(struct bt_le_per_adv_sync *sync,
		    const struct bt_le_per_adv_sync_recv_info *info,
		    struct net_buf_simple *buf)
{
	bt_data_parse(buf, pa_decode_base, NULL);
}

static void bap_broadcast_assistant_discover_cb(struct bt_conn *conn, int err,
						uint8_t recv_state_count)
{
	if (err != 0) {
		shell_error(ctx_shell, "BASS discover failed (%d)", err);
	} else {
		shell_print(ctx_shell, "BASS discover done with %u recv states",
			    recv_state_count);
	}

}

static void bap_broadcast_assistant_scan_cb(const struct bt_le_scan_recv_info *info,
					    uint32_t broadcast_id)
{
	char le_addr[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(info->addr, le_addr, sizeof(le_addr));
	shell_print(ctx_shell,
		    "[DEVICE]: %s, broadcast_id 0x%06X, interval (ms) %u), SID 0x%x, RSSI %i",
		    le_addr, broadcast_id, BT_GAP_PER_ADV_INTERVAL_TO_MS(info->interval), info->sid,
		    info->rssi);
}

static bool metadata_entry(struct bt_data *data, void *user_data)
{
	char metadata[512];

	bin2hex(data->data, data->data_len, metadata, sizeof(metadata));

	shell_print(ctx_shell, "\t\tMetadata length %u, type %u, data: %s",
		    data->data_len, data->type, metadata);

	return true;
}

static void bap_broadcast_assistant_recv_state_cb(
	struct bt_conn *conn, int err,
	const struct bt_bap_scan_delegator_recv_state *state)
{
	char le_addr[BT_ADDR_LE_STR_LEN];
	char bad_code[33];
	bool is_bad_code;

	if (err != 0) {
		shell_error(ctx_shell, "BASS recv state read failed (%d)", err);
		return;
	}

	bt_addr_le_to_str(&state->addr, le_addr, sizeof(le_addr));
	bin2hex(state->bad_code, BT_AUDIO_BROADCAST_CODE_SIZE,
		bad_code, sizeof(bad_code));

	is_bad_code = state->encrypt_state == BT_BAP_BIG_ENC_STATE_BAD_CODE;
	shell_print(
		ctx_shell,
		"BASS recv state: src_id %u, addr %s, sid %u, sync_state %u, encrypt_state %u%s%s",
		state->src_id, le_addr, state->adv_sid, state->pa_sync_state, state->encrypt_state,
		is_bad_code ? ", bad code" : "", is_bad_code ? bad_code : "");

	for (int i = 0; i < state->num_subgroups; i++) {
		const struct bt_bap_scan_delegator_subgroup *subgroup = &state->subgroups[i];
		struct net_buf_simple buf;

		shell_print(ctx_shell, "\t[%d]: BIS sync 0x%04X, metadata_len %zu", i,
			    subgroup->bis_sync, subgroup->metadata_len);

		net_buf_simple_init_with_data(&buf, (void *)subgroup->metadata,
					      subgroup->metadata_len);
		bt_data_parse(&buf, metadata_entry, NULL);
	}


	if (state->pa_sync_state == BT_BAP_PA_STATE_INFO_REQ) {
		struct bt_le_per_adv_sync *per_adv_sync = NULL;
		struct bt_le_ext_adv *ext_adv = NULL;

		/* Lookup matching PA sync */
		for (int i = 0; i < ARRAY_SIZE(per_adv_syncs); i++) {
			if (per_adv_syncs[i] &&
			    bt_addr_le_eq(&per_adv_syncs[i]->addr, &state->addr)) {
				per_adv_sync = per_adv_syncs[i];
				break;
			}
		}

		if (per_adv_sync && IS_ENABLED(CONFIG_BT_PER_ADV_SYNC_TRANSFER_SENDER)) {
			shell_print(ctx_shell, "Sending PAST");

			err = bt_le_per_adv_sync_transfer(per_adv_sync,
							  conn,
							  BT_UUID_BASS_VAL);

			if (err != 0) {
				shell_error(ctx_shell, "Could not transfer periodic adv sync: %d",
					    err);
			}

			return;
		}

		/* If no PA sync was found, check for local advertisers */
		for (int i = 0; i < ARRAY_SIZE(adv_sets); i++) {
			struct bt_le_oob oob_local;

			if (adv_sets[i] == NULL) {
				continue;
			}

			err = bt_le_ext_adv_oob_get_local(adv_sets[i],
							  &oob_local);

			if (err != 0) {
				shell_error(ctx_shell,
					    "Could not get local OOB %d",
					    err);
				return;
			}

			if (bt_addr_le_eq(&oob_local.addr, &state->addr)) {
				ext_adv = adv_sets[i];
				break;
			}
		}

		if (ext_adv != NULL && IS_ENABLED(CONFIG_BT_PER_ADV) &&
		    IS_ENABLED(CONFIG_BT_PER_ADV_SYNC_TRANSFER_SENDER)) {
			shell_print(ctx_shell, "Sending local PAST");

			err = bt_le_per_adv_set_info_transfer(ext_adv, conn,
							      BT_UUID_BASS_VAL);

			if (err != 0) {
				shell_error(ctx_shell,
					    "Could not transfer per adv set info: %d",
					    err);
			}
		} else {
			shell_error(ctx_shell,
				    "Could not send PA to Scan Delegator");
		}
	}
}

static void bap_broadcast_assistant_recv_state_removed_cb(struct bt_conn *conn,
							  int err,
							  uint8_t src_id)
{
	if (err != 0) {
		shell_error(ctx_shell, "BASS recv state removed failed (%d)",
			    err);
	} else {
		shell_print(ctx_shell, "BASS recv state %u removed", src_id);
	}
}

static void bap_broadcast_assistant_scan_start_cb(struct bt_conn *conn, int err)
{
	if (err != 0) {
		shell_error(ctx_shell, "BASS scan start failed (%d)", err);
	} else {
		shell_print(ctx_shell, "BASS scan start successful");
	}
}

static void bap_broadcast_assistant_scan_stop_cb(struct bt_conn *conn, int err)
{
	if (err != 0) {
		shell_error(ctx_shell, "BASS scan stop failed (%d)", err);
	} else {
		shell_print(ctx_shell, "BASS scan stop successful");
	}
}

static void bap_broadcast_assistant_add_src_cb(struct bt_conn *conn, int err)
{
	if (err != 0) {
		shell_error(ctx_shell, "BASS add source failed (%d)", err);
	} else {
		shell_print(ctx_shell, "BASS add source successful");
	}
}

static void bap_broadcast_assistant_mod_src_cb(struct bt_conn *conn, int err)
{
	if (err != 0) {
		shell_error(ctx_shell, "BASS modify source failed (%d)", err);
	} else {
		shell_print(ctx_shell, "BASS modify source successful");
	}
}

static void bap_broadcast_assistant_broadcast_code_cb(struct bt_conn *conn,
						      int err)
{
	if (err != 0) {
		shell_error(ctx_shell, "BASS broadcast code failed (%d)", err);
	} else {
		shell_print(ctx_shell, "BASS broadcast code successful");
	}
}

static void bap_broadcast_assistant_rem_src_cb(struct bt_conn *conn, int err)
{
	if (err != 0) {
		shell_error(ctx_shell, "BASS remove source failed (%d)", err);
	} else {
		shell_print(ctx_shell, "BASS remove source successful");
	}
}

static struct bt_bap_broadcast_assistant_cb cbs = {
	.discover = bap_broadcast_assistant_discover_cb,
	.scan = bap_broadcast_assistant_scan_cb,
	.recv_state = bap_broadcast_assistant_recv_state_cb,
	.recv_state_removed = bap_broadcast_assistant_recv_state_removed_cb,
	.scan_start = bap_broadcast_assistant_scan_start_cb,
	.scan_stop = bap_broadcast_assistant_scan_stop_cb,
	.add_src = bap_broadcast_assistant_add_src_cb,
	.mod_src = bap_broadcast_assistant_mod_src_cb,
	.broadcast_code = bap_broadcast_assistant_broadcast_code_cb,
	.rem_src = bap_broadcast_assistant_rem_src_cb,
};

static shell_status_t cmd_bap_broadcast_assistant_discover(shell_handle_t sh, int32_t argc, char *argv[])
{
	static bool registered;
	int result;

	if (!registered) {
		static struct bt_le_per_adv_sync_cb cb = {
			.recv = pa_recv,
		};

		bt_le_per_adv_sync_cb_register(&cb);

		bt_bap_broadcast_assistant_register_cb(&cbs);

		registered = true;
	}

	result = bt_bap_broadcast_assistant_discover(default_conn);
	if (result) {
		shell_print(sh, "Fail: %d", result);
	}

	return (shell_status_t)result;
}

static shell_status_t cmd_bap_broadcast_assistant_scan_start(shell_handle_t sh, int32_t argc, char *argv[])
{
	int result;
	int start_scan = false;

	if (argc > 1) {
		result = 0;

		start_scan = shell_strtobool(argv[1], 0, &result);
		if (result != 0) {
			shell_error(sh, "Could not parse start_scan: %d",
				    result);

			return kStatus_SHELL_Error;
		}
	}

	result = bt_bap_broadcast_assistant_scan_start(default_conn,
						       (bool)start_scan);
	if (result) {
		shell_print(sh, "Fail: %d", result);
	}

	return (shell_status_t)result;
}

static shell_status_t cmd_bap_broadcast_assistant_scan_stop(shell_handle_t sh, int32_t argc, char *argv[])
{
	int result;

	result = bt_bap_broadcast_assistant_scan_stop(default_conn);
	if (result) {
		shell_print(sh, "Fail: %d", result);
	}

	return (shell_status_t)result;
}

static shell_status_t cmd_bap_broadcast_assistant_add_src(shell_handle_t sh, int32_t argc, char *argv[])
{
	struct bt_bap_broadcast_assistant_add_src_param param = { 0 };
	struct bt_bap_scan_delegator_subgroup subgroup = { 0 };
	unsigned long broadcast_id;
	unsigned long adv_sid;
	int result;

	result = bt_addr_le_from_str(argv[1], argv[2], &param.addr);
	if (result) {
		shell_error(sh, "Invalid peer address (err %d)", result);

		return kStatus_SHELL_Error;
	}

	adv_sid = shell_strtoul(argv[3], 0, &result);
	if (result != 0) {
		shell_error(sh, "Could not parse adv_sid: %d", result);

		return kStatus_SHELL_Error;
	}

	if (adv_sid > BT_GAP_SID_MAX) {
		shell_error(sh, "Invalid adv_sid: %lu", adv_sid);

		return kStatus_SHELL_Error;
	}

	param.adv_sid = adv_sid;

	param.pa_sync = shell_strtobool(argv[4], 0, &result);
	if (result != 0) {
		shell_error(sh, "Could not parse adv_sid: %d", result);

		return kStatus_SHELL_Error;
	}

	broadcast_id = shell_strtoul(argv[5], 0, &result);
	if (result != 0) {
		shell_error(sh, "Could not parse broadcast_id: %d", result);

		return kStatus_SHELL_Error;
	}

	if (broadcast_id > BT_AUDIO_BROADCAST_ID_MAX) {
		shell_error(sh, "Invalid broadcast_id: %lu", broadcast_id);

		return kStatus_SHELL_Error;
	}

	param.broadcast_id = broadcast_id;

	if (argc > 6) {
		unsigned long pa_interval;

		pa_interval = shell_strtoul(argv[6], 0, &result);
		if (result) {
			shell_error(sh, "Could not parse pa_interval: %d",
				    result);

			return kStatus_SHELL_Error;
		}

		if (!IN_RANGE(pa_interval,
			      BT_GAP_PER_ADV_MIN_INTERVAL,
			      BT_GAP_PER_ADV_MAX_INTERVAL)) {
			shell_error(sh, "Invalid pa_interval: %lu",
				    pa_interval);

			return kStatus_SHELL_Error;
		}

		param.pa_interval = pa_interval;
	} else {
		param.pa_interval = BT_BAP_PA_INTERVAL_UNKNOWN;
	}

	/* TODO: Support multiple subgroups */
	if (argc > 7) {
		unsigned long bis_sync;

		bis_sync = shell_strtoul(argv[7], 0, &result);
		if (result) {
			shell_error(sh, "Could not parse bis_sync: %d", result);

			return kStatus_SHELL_Error;
		}

		if (!VALID_BIS_SYNC(bis_sync)) {
			shell_error(sh, "Invalid bis_sync: %lu", bis_sync);

			return kStatus_SHELL_Error;
		}

		subgroup.bis_sync = bis_sync;
	}

	if (argc > 8) {
		size_t metadata_len;

		metadata_len = hex2bin(argv[8], strlen(argv[8]),
				       subgroup.metadata,
				       sizeof(subgroup.metadata));

		if (metadata_len == 0U) {
			shell_error(sh, "Could not parse metadata");

			return kStatus_SHELL_Error;
		}

		/* sizeof(subgroup.metadata) can always fit in uint8_t */

		subgroup.metadata_len = metadata_len;
	}

	param.num_subgroups = 1;
	param.subgroups = &subgroup;

	result = bt_bap_broadcast_assistant_add_src(default_conn, &param);
	if (result) {
		shell_print(sh, "Fail: %d", result);
	}

	return (shell_status_t)result;
}

static bool broadcast_source_found(struct bt_data *data, void *user_data)
{
	struct bt_bap_broadcast_assistant_add_src_param param = { 0 };
	const struct bt_le_scan_recv_info *info = user_data;
	char addr_str[BT_ADDR_LE_STR_LEN];
	struct bt_uuid_16 adv_uuid;
	uint32_t broadcast_id;
	int err;

	/* Verify that it is a BAP broadcaster*/

	if (data->type != BT_DATA_SVC_DATA16) {
		return true;
	}

	if (data->data_len < BT_UUID_SIZE_16 + BT_AUDIO_BROADCAST_ID_SIZE) {
		return true;
	}

	if (!bt_uuid_create(&adv_uuid.uuid, data->data, BT_UUID_SIZE_16)) {
		return true;
	}

	if (bt_uuid_cmp(&adv_uuid.uuid, BT_UUID_BROADCAST_AUDIO) != 0) {
		return true;
	}

	broadcast_id = sys_get_le24(data->data + BT_UUID_SIZE_16);

	if (broadcast_id != auto_scan.broadcast_id) {
		/* Not the one we want */
		return false;
	}

	bt_addr_le_to_str(info->addr, addr_str, sizeof(addr_str));
	shell_print(ctx_shell, "Found BAP broadcast source with address %s and ID 0x%06X\n",
		    addr_str, broadcast_id);

	err = bt_le_scan_stop();
	if (err) {
		shell_error(ctx_shell, "Failed to stop scan: %d", err);
	}

	bt_addr_le_copy(&param.addr, info->addr);
	param.adv_sid = info->sid;
	param.pa_interval = info->interval;
	param.broadcast_id = broadcast_id;
	param.pa_sync = auto_scan.pa_sync;
	param.num_subgroups = 1;
	param.subgroups = &auto_scan.subgroup;

	err = bt_bap_broadcast_assistant_add_src(default_conn, &param);
	if (err) {
		shell_print(ctx_shell, "Failed to add source: %d", err);
	}

	memset(&auto_scan, 0, sizeof(auto_scan));
	auto_scan.broadcast_id = INVALID_BROADCAST_ID;

	return false;
}

static void scan_recv_cb(const struct bt_le_scan_recv_info *info,
			 struct net_buf_simple *ad)
{
	if (auto_scan.broadcast_id == INVALID_BROADCAST_ID) {
		/* no op */
		return;
	}

	/* We are only interested in non-connectable periodic advertisers */
	if ((info->adv_props & BT_GAP_ADV_PROP_CONNECTABLE) != 0 ||
	    info->interval == 0) {
		return;
	}

	if (!passes_scan_filter(info, ad)) {
		return;
	}

	bt_data_parse(ad, broadcast_source_found, (void *)info);
}

static void scan_timeout_cb(void)
{
	shell_print(ctx_shell, "Scan timeout");

	if (auto_scan.broadcast_id != INVALID_BROADCAST_ID) {
		memset(&auto_scan, 0, sizeof(auto_scan));
		auto_scan.broadcast_id = INVALID_BROADCAST_ID;
	}
}

static struct bt_le_scan_cb scan_callbacks = {
	.recv = scan_recv_cb,
	.timeout = scan_timeout_cb,
};

static shell_status_t cmd_bap_broadcast_assistant_add_broadcast_id(shell_handle_t sh, int32_t argc, char *argv[])
{
	struct bt_bap_scan_delegator_subgroup subgroup = { 0 };
	static bool scan_cbs_registered;
	unsigned long broadcast_id;
	unsigned long pa_sync;
	int err = 0;

	if (!scan_cbs_registered) {
		bt_le_scan_cb_register(&scan_callbacks);
		scan_cbs_registered = true;
	}

	if (auto_scan.broadcast_id != INVALID_BROADCAST_ID) {
		shell_info(sh, "Already scanning, wait for sync or timeout");

		return kStatus_SHELL_Error;
	}

	broadcast_id = shell_strtoul(argv[1], 0, &err);
	if (err != 0) {
		shell_error(sh, "failed to parse broadcast_id: %d", err);

		return kStatus_SHELL_Error;
	} else if (broadcast_id > 0xFFFFFF /* 24 bits */) {
		shell_error(sh, "Broadcast ID maximum 24 bits (was %lu)", broadcast_id);

		return kStatus_SHELL_Error;
	}

	pa_sync = shell_strtoul(argv[2], 0, &err);
	if (err != 0) {
		shell_error(sh, "failed to parse pa_sync: %d", err);

		return kStatus_SHELL_Error;
	} else if (pa_sync != 0U && pa_sync != 1U) {
		shell_error(sh, "pa_sync shall be boolean: %lu", pa_sync);

		return kStatus_SHELL_Error;
	}

	/* TODO: Support multiple subgroups */
	if (argc > 3) {
		const unsigned long bis_sync = shell_strtoul(argv[3], 0, &err);

		if (err != 0) {
			shell_error(sh, "failed to parse bis_sync: %d", err);

			return kStatus_SHELL_Error;
		} else if (!VALID_BIS_SYNC(bis_sync)) {
			shell_error(sh, "Invalid bis_sync: %lu", bis_sync);

			return kStatus_SHELL_Error;
		}

		subgroup.bis_sync = bis_sync;
	}

	if (argc > 4) {
		subgroup.metadata_len = hex2bin(argv[4], strlen(argv[4]), subgroup.metadata,
						sizeof(subgroup.metadata));

		if (subgroup.metadata_len == 0U) {
			shell_error(sh, "Could not parse metadata");

			return kStatus_SHELL_Error;
		}
	}

	err = bt_le_scan_start(BT_LE_SCAN_PASSIVE, NULL);
	if (err) {
		shell_print(sh, "Fail to start scanning: %d", err);

		return kStatus_SHELL_Error;
	}

	/* Store results in the `auto_scan` struct */
	auto_scan.broadcast_id = broadcast_id;
	auto_scan.pa_sync = pa_sync;
	memcpy(&auto_scan.subgroup, &subgroup, sizeof(subgroup));

	return (shell_status_t)0;
}

static shell_status_t cmd_bap_broadcast_assistant_mod_src(shell_handle_t sh, int32_t argc, char *argv[])
{
	struct bt_bap_broadcast_assistant_mod_src_param param = { 0 };
	struct bt_bap_scan_delegator_subgroup subgroup = { 0 };
	unsigned long src_id;
	int result = 0;

	src_id = shell_strtoul(argv[1], 0, &result);
	if (result != 0) {
		shell_error(sh, "Could not parse src_id: %d", result);

		return kStatus_SHELL_Error;
	}

	if (src_id > UINT8_MAX) {
		shell_error(sh, "Invalid src_id: %lu", src_id);

		return kStatus_SHELL_Error;
	}
	param.src_id = src_id;

	param.pa_sync = shell_strtobool(argv[2], 0, &result);
	if (result != 0) {
		shell_error(sh, "Could not parse adv_sid: %d", result);

		return kStatus_SHELL_Error;
	}

	if (argc > 3) {
		unsigned long pa_interval;

		pa_interval = shell_strtoul(argv[3], 0, &result);
		if (result) {
			shell_error(sh, "Could not parse pa_interval: %d", result);

			return kStatus_SHELL_Error;
		}

		if (!IN_RANGE(pa_interval,
			      BT_GAP_PER_ADV_MIN_INTERVAL,
			      BT_GAP_PER_ADV_MAX_INTERVAL)) {
			shell_error(sh, "Invalid pa_interval: %lu", pa_interval);

			return kStatus_SHELL_Error;
		}

		param.pa_interval = pa_interval;
	} else {
		param.pa_interval = BT_BAP_PA_INTERVAL_UNKNOWN;
	}

	/* TODO: Support multiple subgroups */
	if (argc > 4) {
		unsigned long bis_sync;

		bis_sync = shell_strtoul(argv[4], 0, &result);
		if (result) {
			shell_error(sh, "Could not parse bis_sync: %d", result);

			return kStatus_SHELL_Error;
		}

		if (!VALID_BIS_SYNC(bis_sync)) {
			shell_error(sh, "Invalid bis_sync: %lu", bis_sync);

			return kStatus_SHELL_Error;
		}

		subgroup.bis_sync = bis_sync;
	}

	if (argc > 5) {
		size_t metadata_len;

		metadata_len = hex2bin(argv[5], strlen(argv[5]),
				       subgroup.metadata,
				       sizeof(subgroup.metadata));

		if (metadata_len == 0U) {
			shell_error(sh, "Could not parse metadata");

			return kStatus_SHELL_Error;
		}

		/* sizeof(subgroup.metadata) can always fit in uint8_t */

		subgroup.metadata_len = metadata_len;
	}

	param.num_subgroups = 1;
	param.subgroups = &subgroup;

	result = bt_bap_broadcast_assistant_mod_src(default_conn, &param);
	if (result) {
		shell_print(sh, "Fail: %d", result);
	}

	return (shell_status_t)result;
}

static inline bool add_pa_sync_base_subgroup_bis_cb(const struct bt_bap_base_subgroup_bis *bis,
						    void *user_data)
{
	struct bt_bap_scan_delegator_subgroup *subgroup_param = user_data;

	subgroup_param->bis_sync |= BIT(bis->index);

	return true;
}

static inline bool add_pa_sync_base_subgroup_cb(const struct bt_bap_base_subgroup *subgroup,
						void *user_data)
{
	struct bt_bap_broadcast_assistant_add_src_param *param = user_data;
	struct bt_bap_scan_delegator_subgroup *subgroup_param;
	uint8_t *data;
	int ret;

	ret = bt_bap_base_get_subgroup_codec_meta(subgroup, &data);
	if (ret < 0) {
		return false;
	}

	subgroup_param = &param->subgroups[param->num_subgroups];

	if (ret > ARRAY_SIZE(subgroup_param->metadata)) {
		shell_info(ctx_shell, "Cannot fit %d octets into subgroup param with size %zu", ret,
			   ARRAY_SIZE(subgroup_param->metadata));
		return false;
	}

	ret = bt_bap_base_subgroup_foreach_bis(subgroup, add_pa_sync_base_subgroup_bis_cb,
					       subgroup_param);
	if (ret < 0) {
		return false;
	}

	param->num_subgroups++;

	return true;
}

static shell_status_t cmd_bap_broadcast_assistant_add_pa_sync(shell_handle_t sh, int32_t argc, char *argv[])
{
	struct bt_bap_broadcast_assistant_add_src_param param = { 0 };
	/* TODO: Add support to select which PA sync to BIG sync to */
	struct bt_le_per_adv_sync *pa_sync = per_adv_syncs[0];
	struct bt_le_per_adv_sync_info pa_info;
	unsigned long broadcast_id;
	unsigned long pa_sync_req;
	uint32_t bis_bitfield_req;
	int err;

	if (pa_sync == NULL) {
		shell_error(sh, "PA not synced");

		return kStatus_SHELL_Error;
	}

	err = bt_le_per_adv_sync_get_info(pa_sync, &pa_info);
	if (err != 0) {
		shell_error(sh, "Could not get PA sync info: %d", err);

		return kStatus_SHELL_Error;
	}

	bt_addr_le_copy(&param.addr, &pa_info.addr);
	param.adv_sid = pa_info.sid;
	param.pa_interval = pa_info.interval;

	pa_sync_req = shell_strtoul(argv[1], 0, &err);
	if (err != 0) {
		shell_error(sh, "failed to parse pa_sync: %d", err);

		return kStatus_SHELL_Error;
	} else if (pa_sync_req != 0U && pa_sync_req != 1U) {
		shell_error(sh, "pa_sync_req shall be boolean: %lu", pa_sync_req);

		return kStatus_SHELL_Error;
	}

	param.pa_sync = (bool)pa_sync_req;

	broadcast_id = shell_strtoul(argv[2], 0, &err);
	if (err != 0) {
		shell_error(sh, "failed to parse broadcast_id: %d", err);

		return kStatus_SHELL_Error;
	} else if (broadcast_id > BT_AUDIO_BROADCAST_ID_MAX /* 24 bits */) {
		shell_error(sh, "Invalid Broadcast ID: %x",
			    param.broadcast_id);

		return kStatus_SHELL_Error;
	}

	param.broadcast_id = broadcast_id;

	bis_bitfield_req = 0U;
	for (size_t i = 3U; i < argc; i++) {
		const unsigned long index = shell_strtoul(argv[i], 16, &err);

		if (err != 0) {
			shell_error(sh, "failed to parse index: %d", err);

			return kStatus_SHELL_Error;
		}

		if (index < BT_ISO_BIS_INDEX_MIN ||
		    index > BT_ISO_BIS_INDEX_MAX) {
			shell_error(sh, "Invalid index: %ld", index);

			return kStatus_SHELL_Error;
		}

		bis_bitfield_req |= BIT(index);
	}

	err = bt_bap_base_foreach_subgroup((const struct bt_bap_base *)received_base,
					   add_pa_sync_base_subgroup_cb, &param);
	if (err < 0) {
		shell_error(ctx_shell, "Could not add BASE to params %d", err);
		return kStatus_SHELL_Error;
	}

	err = bt_bap_broadcast_assistant_add_src(default_conn, &param);
	if (err != 0) {
		shell_print(sh, "Fail: %d", err);

		return kStatus_SHELL_Error;
	}

	return (shell_status_t)0;
}

static shell_status_t cmd_bap_broadcast_assistant_broadcast_code(shell_handle_t sh, int32_t argc, char *argv[])
{
	uint8_t broadcast_code[BT_AUDIO_BROADCAST_CODE_SIZE] = { 0 };
	size_t broadcast_code_len;
	unsigned long src_id;
	int result = 0;

	src_id = shell_strtoul(argv[1], 0, &result);
	if (result != 0) {
		shell_error(sh, "Could not parse src_id: %d", result);

		return kStatus_SHELL_Error;
	}

	if (src_id > UINT8_MAX) {
		shell_error(sh, "Invalid src_id: %lu", src_id);

		return kStatus_SHELL_Error;
	}

	broadcast_code_len = strlen(argv[2]);
	if (!IN_RANGE(broadcast_code_len, 1, BT_AUDIO_BROADCAST_CODE_SIZE)) {
		shell_error(sh, "Invalid broadcast code length: %zu", broadcast_code_len);

		return kStatus_SHELL_Error;
	}

	memcpy(broadcast_code, argv[2], broadcast_code_len);

	shell_info(sh, "Sending broadcast code:");
	shell_hexdump(sh, broadcast_code, sizeof(broadcast_code));

	result = bt_bap_broadcast_assistant_set_broadcast_code(default_conn,
							       src_id,
							       broadcast_code);
	if (result) {
		shell_print(sh, "Fail: %d", result);
	}

	return (shell_status_t)result;
}

static shell_status_t cmd_bap_broadcast_assistant_rem_src(shell_handle_t sh, int32_t argc, char *argv[])
{
	unsigned long src_id;
	int result = 0;

	src_id = shell_strtoul(argv[1], 0, &result);
	if (result != 0) {
		shell_error(sh, "Could not parse src_id: %d", result);

		return kStatus_SHELL_Error;
	}

	if (src_id > UINT8_MAX) {
		shell_error(sh, "Invalid src_id: %lu", src_id);

		return kStatus_SHELL_Error;
	}

	result = bt_bap_broadcast_assistant_rem_src(default_conn, src_id);
	if (result) {
		shell_print(sh, "Fail: %d", result);
	}

	return (shell_status_t)result;
}

static shell_status_t cmd_bap_broadcast_assistant_read_recv_state(shell_handle_t sh, int32_t argc, char *argv[])
{
	unsigned long idx;
	int result = 0;

	idx = shell_strtoul(argv[1], 0, &result);
	if (result != 0) {
		shell_error(sh, "Could not parse idx: %d", result);

		return kStatus_SHELL_Error;
	}

	if (idx > UINT8_MAX) {
		shell_error(sh, "Invalid idx: %lu", idx);

		return kStatus_SHELL_Error;
	}

	result = bt_bap_broadcast_assistant_read_recv_state(default_conn, idx);
	if (result) {
		shell_print(sh, "Fail: %d", result);
	}

	return (shell_status_t)result;
}

static shell_status_t cmd_bap_broadcast_assistant(shell_handle_t sh, int32_t argc, char *argv[])
{
	if (argc > 1) {
		shell_error(sh, "%s unknown parameter: %s",
			    argv[0], argv[1]);
	} else {
		shell_error(sh, "%s Missing subcommand", argv[0]);
	}

	return kStatus_SHELL_Error;
}

SHELL_STATIC_SUBCMD_SET_CREATE(
	bap_broadcast_assistant_cmds,
	SHELL_CMD_ARG(discover, NULL, "Discover BASS on the server",
		      cmd_bap_broadcast_assistant_discover, 1, 0),
	SHELL_CMD_ARG(scan_start, NULL, "Start scanning for broadcasters",
		      cmd_bap_broadcast_assistant_scan_start, 1, 1),
	SHELL_CMD_ARG(scan_stop, NULL, "Stop scanning for BISs",
		      cmd_bap_broadcast_assistant_scan_stop, 1, 0),
	SHELL_CMD_ARG(add_src, NULL,
		      "Add a source <address: XX:XX:XX:XX:XX:XX> "
		      "<type: public/random> <adv_sid> <sync_pa> "
		      "<broadcast_id> [<pa_interval>] [<sync_bis>] "
		      "[<metadata>]",
		      cmd_bap_broadcast_assistant_add_src, 6, 3),
	SHELL_CMD_ARG(add_broadcast_id, NULL,
		      "Add a source by broadcast ID <broadcast_id> <sync_pa> "
		      "[<sync_bis>] [<metadata>]",
		      cmd_bap_broadcast_assistant_add_broadcast_id, 3, 2),
	SHELL_CMD_ARG(add_pa_sync, NULL,
		      "Add a PA sync as a source <sync_pa> <broadcast_id> "
		      "[bis_index [bis_index [bix_index [...]]]]>",
		      cmd_bap_broadcast_assistant_add_pa_sync, 3, BT_ISO_MAX_GROUP_ISO_COUNT),
	SHELL_CMD_ARG(mod_src, NULL,
		      "Set sync <src_id> <sync_pa> [<pa_interval>] "
		      "[<sync_bis>] [<metadata>]",
		      cmd_bap_broadcast_assistant_mod_src, 3, 2),
	SHELL_CMD_ARG(broadcast_code, NULL,
		      "Send a space separated broadcast code of up to 16 bytes "
		      "<src_id> <broadcast code>",
		      cmd_bap_broadcast_assistant_broadcast_code, 3, 0),
	SHELL_CMD_ARG(rem_src, NULL, "Remove a source <src_id>",
		      cmd_bap_broadcast_assistant_rem_src, 2, 0),
	SHELL_CMD_ARG(read_state, NULL, "Remove a source <index>",
		      cmd_bap_broadcast_assistant_read_recv_state, 2, 0),
	SHELL_SUBCMD_SET_END);

SHELL_CMD_REGISTER(bap_broadcast_assistant, bap_broadcast_assistant_cmds,
		       "Bluetooth BAP broadcast assistant client shell commands",
		       cmd_bap_broadcast_assistant, 1, 1);

void bt_ShellBapBroadcastAssistantInit(shell_handle_t shell)
{
    if ((shell_status_t)kStatus_Success != SHELL_RegisterCommand(shell, &g_shellCommandbap_broadcast_assistant))
    {
        shell_print(shell, "Shell register command %s failed!", g_shellCommandbap_broadcast_assistant.pcCommand);
    }
}

#endif /* CONFIG_BT_BAP_BROADCAST_ASSISTANT */