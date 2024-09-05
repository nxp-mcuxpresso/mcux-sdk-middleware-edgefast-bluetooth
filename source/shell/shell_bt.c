/** @file
 * @brief Bluetooth shell module
 *
 * Provide some Bluetooth shell commands that can be useful to applications.
 */

/*
 * Copyright 2021-2024 NXP
 * Copyright (c) 2017 Intel Corporation
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <errno/errno.h>
#include <zephyr/types.h>
#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
//#include <strings.h>

#include <sys/byteorder.h>
#include <sys/util.h>
#include <porting.h>

#include <bluetooth/hci.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/conn.h>
#if (defined(CONFIG_BT_RFCOMM) && (CONFIG_BT_RFCOMM > 0))
#include <bluetooth/rfcomm.h>
#endif
#if (defined(CONFIG_BT_BREDR) && ((CONFIG_BT_BREDR) > 0U))
#include <bluetooth/sdp.h>
#endif
#include <bluetooth/hci.h>
#include <bluetooth/iso.h>
#if defined(CONFIG_BT_AUDIO) && (CONFIG_BT_AUDIO > 0)
#include <bluetooth/audio/audio.h>
#include <bluetooth/audio/pacs.h>
#endif
#if defined(CONFIG_BT_CSIP) && (CONFIG_BT_CSIP > 0)
#include <bluetooth/audio/csip.h>
#endif

#include "fsl_shell.h"

#include "shell_bt.h"
#include "shell_hci.h"
#if (defined(CONFIG_BT_IND_RESET) && (CONFIG_BT_IND_RESET > 0U))  
#include "controller_ind_reset.h"
#endif /*#define CONFIG_BT_IND_RESET*/
#if (defined(CONFIG_BLE_ADV_REPORT_BUFFER_FILTER) && (CONFIG_BLE_ADV_REPORT_BUFFER_FILTER > 0U))
#include "shell_ble_scan_filter.h"
#endif /* CONFIG_BLE_ADV_REPORT_BUFFER_FILTER */
static bool no_settings_load;

uint8_t selected_id = BT_ID_DEFAULT;
shell_handle_t ctx_shell;
static const char * s_shellBtPrompt = NULL;

#if (defined(CONFIG_BT_CSIP) && (CONFIG_BT_CSIP > 0))
extern struct bt_csis *csis;
#endif

#if (defined(CONFIG_BT_CONN) && (CONFIG_BT_CONN > 0))
struct bt_conn *default_conn;

/* Connection context for BR/EDR legacy pairing in sec mode 3 */
static struct bt_conn *pairing_conn;

static struct bt_le_oob oob_local;
#if (defined(CONFIG_BT_SMP) && (CONFIG_BT_SMP > 0)) || ((defined(CONFIG_BT_BREDR) && (CONFIG_BT_BREDR > 0)))
static struct bt_le_oob oob_remote;
#endif /* CONFIG_BT_SMP || CONFIG_BT_BREDR) */
#endif /* CONFIG_BT_CONN */

#if (defined(CONFIG_BT_SMP) && (CONFIG_BT_SMP > 0))
static struct bt_conn_auth_info_cb auth_info_cb;
#endif /* CONFIG_BT_SMP */

#define NAME_LEN 30

#define KEY_STR_LEN 33

#define ADV_DATA_DELIMITER ", "

#define AD_SIZE 9

/*
 * Based on the maximum number of parameters for HCI_LE_Generate_DHKey
 * See BT Core Spec V5.2 Vol. 4, Part E, section 7.8.37
 */
#define HCI_CMD_MAX_PARAM 65

enum {
	SHELL_ADV_OPT_CONNECTABLE,
	SHELL_ADV_OPT_DISCOVERABLE,
	SHELL_ADV_OPT_EXT_ADV,
	SHELL_ADV_OPT_APPEARANCE,
	SHELL_ADV_OPT_KEEP_RPA,

	SHELL_ADV_OPT_NUM,
};

static ATOMIC_DEFINE(adv_opt, SHELL_ADV_OPT_NUM);
#if (defined(CONFIG_BT_EXT_ADV) && (CONFIG_BT_EXT_ADV > 0))
#if (defined(CONFIG_BT_BROADCASTER) && (CONFIG_BT_BROADCASTER > 0))
uint8_t selected_adv;
struct bt_le_ext_adv *adv_sets[CONFIG_BT_EXT_ADV_MAX_ADV_SET];
static ATOMIC_DEFINE(adv_set_opt, SHELL_ADV_OPT_NUM)[CONFIG_BT_EXT_ADV_MAX_ADV_SET];
#endif /* CONFIG_BT_BROADCASTER */
#endif /* CONFIG_BT_EXT_ADV */

#if ((defined(CONFIG_BT_OBSERVER) && (CONFIG_BT_OBSERVER > 0)) \
  || (defined(CONFIG_BT_USER_PHY_UPDATE) && (CONFIG_BT_USER_PHY_UPDATE > 0)))
static const char *phy2str(uint8_t phy)
{
	switch (phy) {
	case 0: return "No packets";
	case BT_GAP_LE_PHY_1M: return "LE 1M";
	case BT_GAP_LE_PHY_2M: return "LE 2M";
	case BT_GAP_LE_PHY_CODED: return "LE Coded";
	default: return "Unknown";
	}
}
#endif

#if (defined(CONFIG_BT_CONN) && (CONFIG_BT_CONN > 0)) || \
    ((defined(CONFIG_BT_BROADCASTER) && (CONFIG_BT_BROADCASTER > 0)) && \
	 (defined(CONFIG_BT_EXT_ADV) && (CONFIG_BT_EXT_ADV > 0)))
static void print_le_addr(const char *desc, const bt_addr_le_t *addr)
{
	char addr_str[BT_ADDR_LE_STR_LEN];

	const char *addr_desc = bt_addr_le_is_identity(addr) ? "identity" :
				bt_addr_le_is_rpa(addr) ? "resolvable" :
				"non-resolvable";

	bt_addr_le_to_str(addr, addr_str, sizeof(addr_str));

	shell_print(ctx_shell, "%s address: %s (%s)", desc, addr_str,
		    addr_desc);
}
#endif

#if (defined(CONFIG_BT_TRANSMIT_POWER_CONTROL) && (CONFIG_BT_TRANSMIT_POWER_CONTROL > 0))
static const char *tx_power_flag2str(int8_t flag)
{
	switch (flag) {
	case 0:
		return "Neither Max nor Min Tx Power";
	case 1:
		return "Tx Power Level is at minimum";
	case 2:
		return "Tx Power Level is at maximum";
	/* Current Tx Power Level is the only available one*/
	case 3:
		return "Tx Power Level is at minimum & maximum.";
	default:
		return "Unknown";
	}
}

static const char *tx_power_report_reason2str(uint8_t reason)
{
	switch (reason) {
	case BT_HCI_LE_TX_POWER_REPORT_REASON_LOCAL_CHANGED:
		return "Local Tx Power changed";
	case BT_HCI_LE_TX_POWER_REPORT_REASON_REMOTE_CHANGED:
		return "Remote Tx Power changed";
	case BT_HCI_LE_TX_POWER_REPORT_REASON_READ_REMOTE_COMPLETED:
		return "Completed to read remote Tx Power";
	default:
		return "Unknown";
	}
}

static const char *tx_pwr_ctrl_phy2str(enum bt_conn_le_tx_power_phy phy)
{
	switch (phy) {
	case BT_CONN_LE_TX_POWER_PHY_NONE:
		return "None";
	case BT_CONN_LE_TX_POWER_PHY_1M:
		return "LE 1M";
	case BT_CONN_LE_TX_POWER_PHY_2M:
		return "LE 2M";
	case BT_CONN_LE_TX_POWER_PHY_CODED_S8:
		return "LE Coded S8";
	case BT_CONN_LE_TX_POWER_PHY_CODED_S2:
		return "LE Coded S2";
	default:
		return "Unknown";
	}
}

static const char *enabled2str(bool enabled)
{
	if (enabled) {
		return "Enabled";
	} else {
		return "Disabled";
	}
}

#endif /* CONFIG_BT_TRANSMIT_POWER_CONTROL */
#if (defined(CONFIG_BT_CENTRAL) && (CONFIG_BT_CENTRAL> 0))
static shell_status_t cmd_scan_off(shell_handle_t shell);
static shell_status_t cmd_connect_le(shell_handle_t shell, int32_t argc, char *argv[]);
static shell_status_t cmd_scan_filter_clear_name(shell_handle_t shell, int32_t argc, char *argv[]);

static struct bt_auto_connect {
	bt_addr_le_t addr;
	bool addr_set;
	bool connect_name;
} auto_connect;
#endif
#if (defined(CONFIG_BT_OBSERVER) && (CONFIG_BT_OBSERVER > 0))
static struct bt_scan_filter {
	char name[NAME_LEN];
	bool name_set;
	char addr[18]; /* fits xx:xx:xx:xx:xx:xx\0 */
	bool addr_set;
	int8_t rssi;
	bool rssi_set;
	uint16_t pa_interval;
	bool pa_interval_set;
} scan_filter;

static const char scan_response_label[] = "[DEVICE]: ";
static bool scan_verbose_output;

/* armgcc and mcux not defined strncasecmp(), so we need implement one. */
#if (defined(__CC_ARM) || (defined(__ARMCC_VERSION))) || \
    (defined(__GNUC__) || (defined(_LINKER) && defined(__GCC_LINKER_CMD__)))

static int my_strncasecmp(const char*s1, const char*s2, int n)
{
	if(n == 0)
	{
		return 0;
	}
    
	do {
		if(tolower(*s1) != tolower(*s2))
		{
			return (int)(tolower(*s1) - tolower(*s2));
		}

		if(*s1++ == 0)
		{
			break;
		}

		s2++;

	} while(--n != 0);
    
	return 0;
}

#endif

/**
 * @brief Compares two strings without case sensitivy
 *
 * @param substr The substring
 * @param str The string to find the substring in
 *
 * @return true if @substr is a substring of @p, else false
 */
static bool is_substring(const char *substr, const char *str)
{
	const size_t str_len = strlen(str);
	const size_t sub_str_len = strlen(substr);

	if (sub_str_len > str_len) {
		return false;
	}

	for (size_t pos = 0; pos < str_len; pos++) {
		unsigned char a = substr[0];
		unsigned char b = str[pos];
		if (tolower(a) == tolower(b)) {
			if (pos + sub_str_len > str_len) {
				return false;
			}

/* armgcc and mcux not defined strncasecmp(), so we need a implement one. */
#if (defined(__CC_ARM) || (defined(__ARMCC_VERSION))) || \
    (defined(__GNUC__) || (defined(_LINKER) && defined(__GCC_LINKER_CMD__)))
			if (my_strncasecmp(substr, &str[pos], sub_str_len) == 0) {
#else
			if (strncasecmp(substr, &str[pos], sub_str_len) == 0) {
#endif
				return true;
			}
		}
	}

	return false;
}

static bool data_cb(struct bt_data *data, void *user_data)
{
	char *name = (char *)user_data;

	switch (data->type) {
	case BT_DATA_NAME_SHORTENED:
	case BT_DATA_NAME_COMPLETE:
	case BT_DATA_BROADCAST_NAME:
		memcpy(name, data->data, MIN(data->data_len, NAME_LEN - 1));
		return false;
	default:
		return true;
	}
}

static void print_data_hex(const uint8_t *data, uint8_t len)
{
    char dataBuf[128] = {'\0'};
    char hexBuf[128] = {'\0'};
	if (len == 0)
		return;
        
	/* Reverse the byte order when printing as advertising data is LE
	 * and the MSB should be first in the printed output.
	 */
	for (int16_t i = len - 1; i >= 0; i--) {
		sprintf(hexBuf, "0x%02x ", data[i]);
        strcat(dataBuf, hexBuf);
	}

        shell_print(ctx_shell,"%s", dataBuf);
}

static void print_data_set(uint8_t set_value_len,
			   const uint8_t *scan_data, uint8_t scan_data_len)
{
	uint8_t idx = 0;
    //char dataBuf[128] = {'\0'};

	if (scan_data_len == 0 || set_value_len > scan_data_len) {
		return;
	}

	do {
		if (idx > 0) {
			shell_print(ctx_shell, ADV_DATA_DELIMITER);
            //sprintf(dataBuf, ADV_DATA_DELIMITER);
		}

		print_data_hex(&scan_data[idx], set_value_len);
		idx += set_value_len;
	} while (idx + set_value_len <= scan_data_len);

	if (idx < scan_data_len) {
		shell_print(ctx_shell, " Excess data: ");
		print_data_hex(&scan_data[idx], scan_data_len - idx);
	}
}

static bool data_verbose_cb(struct bt_data *data, void *user_data)
{
	shell_print(ctx_shell, "%*sType 0x%02x: ",
		      strlen(scan_response_label), "", data->type);

	switch (data->type) {
	case BT_DATA_UUID16_SOME:
	case BT_DATA_UUID16_ALL:
	case BT_DATA_SOLICIT16:
		print_data_set(BT_UUID_SIZE_16, data->data, data->data_len);
		break;
	case BT_DATA_SVC_DATA16:
		/* Data starts with a UUID16 (2 bytes),
		 * the rest is unknown and printed as single bytes
		 */
		if (data->data_len < BT_UUID_SIZE_16) {
			shell_print(ctx_shell,
				      "BT_DATA_SVC_DATA16 data length too short (%u)",
				      data->data_len);
			break;
		}
		print_data_set(BT_UUID_SIZE_16, data->data, BT_UUID_SIZE_16);
		if (data->data_len > BT_UUID_SIZE_16) {
			shell_print(ctx_shell, ADV_DATA_DELIMITER);
			print_data_set(1, data->data + BT_UUID_SIZE_16,
				       data->data_len - BT_UUID_SIZE_16);
		}
		break;
	case BT_DATA_UUID32_SOME:
	case BT_DATA_UUID32_ALL:
		print_data_set(BT_UUID_SIZE_32, data->data, data->data_len);
		break;
	case BT_DATA_SVC_DATA32:
		/* Data starts with a UUID32 (4 bytes),
		 * the rest is unknown and printed as single bytes
		 */
		if (data->data_len < BT_UUID_SIZE_32) {
			shell_print(ctx_shell,
				      "BT_DATA_SVC_DATA32 data length too short (%u)",
				      data->data_len);
			break;
		}
		print_data_set(BT_UUID_SIZE_32, data->data, BT_UUID_SIZE_32);
		if (data->data_len > BT_UUID_SIZE_32) {
			shell_print(ctx_shell, ADV_DATA_DELIMITER);
			print_data_set(1, data->data + BT_UUID_SIZE_32,
				       data->data_len - BT_UUID_SIZE_32);
		}
		break;
	case BT_DATA_UUID128_SOME:
	case BT_DATA_UUID128_ALL:
	case BT_DATA_SOLICIT128:
		print_data_set(BT_UUID_SIZE_128, data->data, data->data_len);
		break;
	case BT_DATA_SVC_DATA128:
		/* Data starts with a UUID128 (16 bytes),
		 * the rest is unknown and printed as single bytes
		 */
		if (data->data_len < BT_UUID_SIZE_128) {
			shell_print(ctx_shell,
				      "BT_DATA_SVC_DATA128 data length too short (%u)",
				      data->data_len);
			break;
		}
		print_data_set(BT_UUID_SIZE_128, data->data, BT_UUID_SIZE_128);
		if (data->data_len > BT_UUID_SIZE_128) {
			shell_print(ctx_shell, ADV_DATA_DELIMITER);
			print_data_set(1, data->data + BT_UUID_SIZE_128,
				       data->data_len - BT_UUID_SIZE_128);
		}
		break;
	case BT_DATA_NAME_SHORTENED:
	case BT_DATA_NAME_COMPLETE:
	case BT_DATA_BROADCAST_NAME:
		shell_print(ctx_shell, "%.*s", data->data_len, data->data);
		break;
	case BT_DATA_PUB_TARGET_ADDR:
	case BT_DATA_RAND_TARGET_ADDR:
	case BT_DATA_LE_BT_DEVICE_ADDRESS:
		print_data_set(BT_ADDR_SIZE, data->data, data->data_len);
		break;
	case BT_DATA_CSIS_RSI:
		print_data_set(3, data->data, data->data_len);
		break;
	default:
		print_data_set(1, data->data, data->data_len);
	}

	shell_print(ctx_shell, "\n");

	return true;
}

static const char *scan_response_type_txt(uint8_t type)
{
	switch (type) {
	case BT_GAP_ADV_TYPE_ADV_IND:
		return "ADV_IND";
	case BT_GAP_ADV_TYPE_ADV_DIRECT_IND:
		return "ADV_DIRECT_IND";
	case BT_GAP_ADV_TYPE_ADV_SCAN_IND:
		return "ADV_SCAN_IND";
	case BT_GAP_ADV_TYPE_ADV_NONCONN_IND:
		return "ADV_NONCONN_IND";
	case BT_GAP_ADV_TYPE_SCAN_RSP:
		return "SCAN_RSP";
	case BT_GAP_ADV_TYPE_EXT_ADV:
		return "EXT_ADV";
	default:
		return "UNKNOWN";
	}
}

bool passes_scan_filter(const struct bt_le_scan_recv_info *info, const struct net_buf_simple *buf)
{

	if (scan_filter.rssi_set && (scan_filter.rssi > info->rssi)) {
		return false;
	}

	if (scan_filter.pa_interval_set &&
	    (scan_filter.pa_interval > BT_CONN_INTERVAL_TO_MS(info->interval))) {
		return false;
	}

	if (scan_filter.addr_set) {
		char le_addr[BT_ADDR_LE_STR_LEN] = {0};

		bt_addr_le_to_str(info->addr, le_addr, sizeof(le_addr));

		if (!is_substring(scan_filter.addr, le_addr)) {
			return false;
		}
	}

	if (scan_filter.name_set) {
		struct net_buf_simple buf_copy;
		char name[NAME_LEN] = {0};

		/* call to bt_data_parse consumes netbufs so shallow clone for verbose output */
		net_buf_simple_clone(buf, &buf_copy);
		bt_data_parse(&buf_copy, data_cb, name);

		if (!is_substring(scan_filter.name, name)) {
			return false;
		}
	}

	return true;
}

static void scan_recv(const struct bt_le_scan_recv_info *info,
		      struct net_buf_simple *buf)
{
	char le_addr[BT_ADDR_LE_STR_LEN];
	char name[NAME_LEN];
	struct net_buf_simple buf_copy;

	if (!passes_scan_filter(info, buf)) {
		return;
	}

	if (scan_verbose_output) {
		/* call to bt_data_parse consumes netbufs so shallow clone for verbose output */
		net_buf_simple_clone(buf, &buf_copy);
	}

	(void)memset(name, 0, sizeof(name));

	bt_data_parse(buf, data_cb, name);
	bt_addr_le_to_str(info->addr, le_addr, sizeof(le_addr));

#if (defined(CONFIG_BLE_ADV_REPORT_BUFFER_FILTER) && (CONFIG_BLE_ADV_REPORT_BUFFER_FILTER > 0U))
    if (kStatus_SHELL_Success != shell_le_scan_filter_handler(info))
    {
        return;
    }
#endif /* CONFIG_BLE_ADV_REPORT_BUFFER_FILTER */

	PRINTF("[DEVICE]: %s, AD evt type %u, RSSI %i %s "
		    "C:%u S:%u D:%d SR:%u E:%u Prim: %s, Secn: %s, "
		    "Interval: 0x%04x (%u us), SID: 0x%x\n",
		    le_addr, info->adv_type, info->rssi, name,
		    (info->adv_props & BT_GAP_ADV_PROP_CONNECTABLE) != 0,
		    (info->adv_props & BT_GAP_ADV_PROP_SCANNABLE) != 0,
		    (info->adv_props & BT_GAP_ADV_PROP_DIRECTED) != 0,
		    (info->adv_props & BT_GAP_ADV_PROP_SCAN_RESPONSE) != 0,
		    (info->adv_props & BT_GAP_ADV_PROP_EXT_ADV) != 0,
		    phy2str(info->primary_phy), phy2str(info->secondary_phy),
		    info->interval, BT_CONN_INTERVAL_TO_US(info->interval),
		    info->sid);

	if (scan_verbose_output) {
		shell_info(ctx_shell,
			   "%*s[SCAN DATA START - %s]",
			   strlen(scan_response_label), "",
			   scan_response_type_txt(info->adv_type));
		bt_data_parse(&buf_copy, data_verbose_cb, NULL);
		shell_info(ctx_shell, "%*s[SCAN DATA END]", strlen(scan_response_label), "");
	}
	
	/* Store address for later use */
#if (defined(CONFIG_BT_CENTRAL) && (CONFIG_BT_CENTRAL> 0))
	if ((info->adv_props & BT_GAP_ADV_PROP_CONNECTABLE) != 0U) {
		struct bt_conn *conn = bt_conn_lookup_addr_le(selected_id, info->addr);

		/* Only store auto-connect address for devices we are not already connected to */
		if (conn == NULL) {
			/* Store address for later use */
			auto_connect.addr_set = true;
			bt_addr_le_copy(&auto_connect.addr, info->addr);

			/* Use the above auto_connect.addr address to automatically connect */
			if (auto_connect.connect_name) {
				auto_connect.connect_name = false;

				cmd_scan_off(ctx_shell);

				/* "name" is what would be in argv[0] normally */
				cmd_scan_filter_clear_name(ctx_shell, 1, (char *[]){"name"});

				/* "connect" is what would be in argv[0] normally */
				cmd_connect_le(ctx_shell, 1, (char *[]){"connect"});
			}
		} else {
			bt_conn_unref(conn);
		}
	}
#endif /* CONFIG_BT_CENTRAL */
}

static void scan_timeout(void)
{
	shell_print(ctx_shell, "Scan timeout");
#if (defined(CONFIG_BT_CENTRAL) && (CONFIG_BT_CENTRAL> 0))
	if (auto_connect.connect_name) {
		auto_connect.connect_name = false;
		/* "name" is what would be in argv[0] normally */
		cmd_scan_filter_clear_name(ctx_shell, 1, (char *[]){ "name" });
	}
#endif /* CONFIG_BT_CENTRAL */
}
#endif /* CONFIG_BT_OBSERVER */

#if (defined(CONFIG_BT_EXT_ADV) && (CONFIG_BT_EXT_ADV > 0))
#if (defined(CONFIG_BT_BROADCASTER) && (CONFIG_BT_BROADCASTER > 0))
static void adv_sent(struct bt_le_ext_adv *adv,
		     struct bt_le_ext_adv_sent_info *info)
{
	shell_print(ctx_shell, "Advertiser[%d] %p sent %d",
		    bt_le_ext_adv_get_index(adv), adv, info->num_sent);
}

static void adv_scanned(struct bt_le_ext_adv *adv,
			struct bt_le_ext_adv_scanned_info *info)
{
	char str[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(info->addr, str, sizeof(str));

	shell_print(ctx_shell, "Advertiser[%d] %p scanned by %s",
		    bt_le_ext_adv_get_index(adv), adv, str);
}
#endif /* CONFIG_BT_BROADCASTER */

#if (defined(CONFIG_BT_PERIPHERAL) && (CONFIG_BT_PERIPHERAL > 0))
static void adv_connected(struct bt_le_ext_adv *adv,
			  struct bt_le_ext_adv_connected_info *info)
{
	char str[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(bt_conn_get_dst(info->conn), str, sizeof(str));

	shell_print(ctx_shell, "Advertiser[%d] %p connected by %s",
		    bt_le_ext_adv_get_index(adv), adv, str);
}
#endif /* CONFIG_BT_PERIPHERAL */
#if (defined(CONFIG_BT_PRIVACY) && (CONFIG_BT_PRIVACY > 0))
static bool adv_rpa_expired(struct bt_le_ext_adv *adv)
{
	uint8_t adv_index = bt_le_ext_adv_get_index(adv);

	bool keep_rpa = atomic_test_bit(adv_set_opt[adv_index],
					  SHELL_ADV_OPT_KEEP_RPA);
	shell_print(ctx_shell, "Advertiser[%d] %p RPA %s",
		    adv_index, adv,
		    keep_rpa ? "not expired" : "expired");

	return keep_rpa;
}
#endif /* defined(CONFIG_BT_PRIVACY) */
#endif /* CONFIG_BT_EXT_ADV */

#if !(defined(CONFIG_BT_CONN) && (CONFIG_BT_CONN > 0))
#if 1 /* FIXME: Add support for changing prompt */
static const char *current_prompt(void)
{
	return NULL;
}
#endif
#endif /* !CONFIG_BT_CONN */

#if (defined(CONFIG_BT_CONN) && (CONFIG_BT_CONN > 0))
#if 1 /* FIXME: Add support for changing prompt */
static const char *current_prompt(void)
{
	static char str[BT_ADDR_LE_STR_LEN + 2];
	static struct bt_conn_info info;

	memset(str, 0, sizeof(str));
	do
	{
		if (!default_conn) {
			break;
		}

		if (bt_conn_get_info(default_conn, &info) < 0) {
			break;
		}

		if (info.type != BT_CONN_TYPE_LE) {
			break;
		}

		bt_addr_le_to_str(info.le.dst, str, sizeof(str) - 2);
	} while (0);
	strcat(str, "@bt> ");
	return str;
}
#endif

void conn_addr_str(struct bt_conn *conn, char *addr, size_t len)
{
	struct bt_conn_info info;

	if (bt_conn_get_info(conn, &info) < 0) {
		addr[0] = '\0';
		return;
	}

	switch (info.type) {
#if (defined(CONFIG_BT_BREDR) && (CONFIG_BT_BREDR > 0))
	case BT_CONN_TYPE_BR:
		bt_addr_to_str(info.br.dst, addr, len);
		break;
#endif
	case BT_CONN_TYPE_LE:
		bt_addr_le_to_str(info.le.dst, addr, len);
		break;
	default:
		break;
	}
}

static void print_le_oob(shell_handle_t shell, struct bt_le_oob *oob)
{
	char addr[BT_ADDR_LE_STR_LEN];
	char c[KEY_STR_LEN];
	char r[KEY_STR_LEN];

	bt_addr_le_to_str(&oob->addr, addr, sizeof(addr));

	bin2hex(oob->le_sc_data.c, sizeof(oob->le_sc_data.c), c, sizeof(c));
	bin2hex(oob->le_sc_data.r, sizeof(oob->le_sc_data.r), r, sizeof(r));

	shell_print(shell, "OOB data:");
	shell_print(shell, "%-29s %-32s %-32s", "addr", "random", "confirm");
	shell_print(shell, "%29s %32s %32s", addr, r, c);
}

static void connected(struct bt_conn *conn, uint8_t err)
{
	char addr[BT_ADDR_LE_STR_LEN];
	struct bt_conn_info info;
	int info_err;

	conn_addr_str(conn, addr, sizeof(addr));

	if (err) {
		shell_error(ctx_shell, "Failed to connect to %s (0x%02x)", addr,
			     err);
		goto done;
	}

	shell_print(ctx_shell, "Connected: %s", addr);

	info_err = bt_conn_get_info(conn, &info);
	if (info_err != 0) {
		shell_error(ctx_shell, "Failed to connection information: %d", info_err);
		goto done;
	}

	if (info.role == BT_CONN_ROLE_CENTRAL) {
		if (default_conn != NULL) {
			bt_conn_unref(default_conn);
		}

		default_conn = bt_conn_ref(conn);
	} else if (info.role == BT_CONN_ROLE_PERIPHERAL) {
		if (default_conn == NULL) {
			default_conn = bt_conn_ref(conn);
		}
	}
    s_shellBtPrompt = current_prompt();
    SHELL_ChangePrompt(ctx_shell, (char *)s_shellBtPrompt);

done:
	/* clear connection reference for sec mode 3 pairing */
	if (pairing_conn) {
		bt_conn_unref(pairing_conn);
		pairing_conn = NULL;
	}
}

static void disconnected_set_new_default_conn_cb(struct bt_conn *conn, void *user_data)
{
	struct bt_conn_info info;

	if (default_conn != NULL) {
		/* nop */
		return;
	}

	if (bt_conn_get_info(conn, &info) != 0) {
		shell_error(ctx_shell, "Unable to get info: conn %p", conn);
		return;
	}

	if (info.state == BT_CONN_STATE_CONNECTED) {
		char addr_str[BT_ADDR_LE_STR_LEN];

		default_conn = bt_conn_ref(conn);

		bt_addr_le_to_str(info.le.dst, addr_str, sizeof(addr_str));
		shell_print(ctx_shell, "Selected conn is now: %s", addr_str);
	}
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
	char addr[BT_ADDR_LE_STR_LEN];

	conn_addr_str(conn, addr, sizeof(addr));
	shell_print(ctx_shell, "Disconnected: %s (reason 0x%02x)", addr, reason);

	if (default_conn == conn) {
		bt_conn_unref(default_conn);
		default_conn = NULL;
		/* If we are connected to other devices, set one of them as default */
		bt_conn_foreach(BT_CONN_TYPE_LE, disconnected_set_new_default_conn_cb, NULL);
	}
    s_shellBtPrompt = current_prompt();
    SHELL_ChangePrompt(ctx_shell, (char *)s_shellBtPrompt);
}

static bool le_param_req(struct bt_conn *conn, struct bt_le_conn_param *param)
{
	shell_print(ctx_shell, "LE conn  param req: int (0x%04x, 0x%04x) lat %d"
		    " to %d", param->interval_min, param->interval_max,
		    param->latency, param->timeout);

	return true;
}

static void le_param_updated(struct bt_conn *conn, uint16_t interval,
			     uint16_t latency, uint16_t timeout)
{
	shell_print(ctx_shell, "LE conn param updated: int 0x%04x lat %d "
		     "to %d", interval, latency, timeout);
}

#if (defined(CONFIG_BT_SMP) && (CONFIG_BT_SMP > 0))
static void identity_resolved(struct bt_conn *conn, const bt_addr_le_t *rpa,
			      const bt_addr_le_t *identity)
{
	char addr_identity[BT_ADDR_LE_STR_LEN];
	char addr_rpa[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(identity, addr_identity, sizeof(addr_identity));
	bt_addr_le_to_str(rpa, addr_rpa, sizeof(addr_rpa));

	shell_print(ctx_shell, "Identity resolved %s -> %s", addr_rpa,
	      addr_identity);
}
#endif

#if (defined(CONFIG_BT_SMP) && (CONFIG_BT_SMP > 0)) || ((defined(CONFIG_BT_BREDR) && (CONFIG_BT_BREDR > 0)))
static const char *security_err_str(enum bt_security_err err)
{
	switch (err) {
	case BT_SECURITY_ERR_SUCCESS:
		return "Success";
	case BT_SECURITY_ERR_AUTH_FAIL:
		return "Authentication failure";
	case BT_SECURITY_ERR_PIN_OR_KEY_MISSING:
		return "PIN or key missing";
	case BT_SECURITY_ERR_OOB_NOT_AVAILABLE:
		return "OOB not available";
	case BT_SECURITY_ERR_AUTH_REQUIREMENT:
		return "Authentication requirements";
	case BT_SECURITY_ERR_PAIR_NOT_SUPPORTED:
		return "Pairing not supported";
	case BT_SECURITY_ERR_PAIR_NOT_ALLOWED:
		return "Pairing not allowed";
	case BT_SECURITY_ERR_INVALID_PARAM:
		return "Invalid parameters";
	case BT_SECURITY_ERR_UNSPECIFIED:
		return "Unspecified";
	default:
		return "Unknown";
	}
}

static void security_changed(struct bt_conn *conn, bt_security_t level,
			     enum bt_security_err err)
{
	char addr[BT_ADDR_LE_STR_LEN];

	conn_addr_str(conn, addr, sizeof(addr));

	if (!err) {
		shell_print(ctx_shell, "Security changed: %s level %u", addr,
			    level);
	} else {
		shell_print(ctx_shell, "Security failed: %s level %u "
			    "reason: %s (%d)",
			    addr, level, security_err_str(err), err);
	}
}
#endif

#if (defined(CONFIG_BT_REMOTE_INFO) && (CONFIG_BT_REMOTE_INFO > 0))
static const char *ver_str(uint8_t ver)
{
	const char * const str[] = {
		"1.0b", "1.1", "1.2", "2.0", "2.1", "3.0", "4.0", "4.1", "4.2",
		"5.0", "5.1", "5.2", "5.3", "5.4"
	};

	if (ver < ARRAY_SIZE(str)) {
		return str[ver];
	}

	return "unknown";
}

static void remote_info_available(struct bt_conn *conn,
				  struct bt_conn_remote_info *remote_info)
{
	struct bt_conn_info info;

	bt_conn_get_info(conn, &info);

	if (IS_ENABLED(CONFIG_BT_REMOTE_VERSION)) {
		shell_print(ctx_shell,
			    "Remote LMP version %s (0x%02x) subversion 0x%04x "
			    "manufacturer 0x%04x", ver_str(remote_info->version),
			    remote_info->version, remote_info->subversion,
			    remote_info->manufacturer);
	}

	if (info.type == BT_CONN_TYPE_LE) {
		uint8_t features[8];
		char features_str[2 * sizeof(features) +  1];

		sys_memcpy_swap(features, remote_info->le.features,
				sizeof(features));
		bin2hex(features, sizeof(features),
			features_str, sizeof(features_str));
		shell_print(ctx_shell, "LE Features: 0x%s ", features_str);
	}
}
#endif /* defined(CONFIG_BT_REMOTE_INFO) */

#if (defined(CONFIG_BT_USER_DATA_LEN_UPDATE) && (CONFIG_BT_USER_DATA_LEN_UPDATE > 0))
void le_data_len_updated(struct bt_conn *conn,
			 struct bt_conn_le_data_len_info *info)
{
	shell_print(ctx_shell,
		    "LE data len updated: TX (len: %d time: %d)"
		    " RX (len: %d time: %d)", info->tx_max_len,
		    info->tx_max_time, info->rx_max_len, info->rx_max_time);
}
#endif

#if (defined(CONFIG_BT_USER_PHY_UPDATE) && (CONFIG_BT_USER_PHY_UPDATE > 0))
void le_phy_updated(struct bt_conn *conn,
		    struct bt_conn_le_phy_info *info)
{
	shell_print(ctx_shell, "LE PHY updated: TX PHY %s, RX PHY %s",
		    phy2str(info->tx_phy), phy2str(info->rx_phy));
}
#endif

#if (defined(CONFIG_BT_TRANSMIT_POWER_CONTROL) && (CONFIG_BT_TRANSMIT_POWER_CONTROL > 0))
void tx_power_report(struct bt_conn *conn,
		    const struct bt_conn_le_tx_power_report *report)
{
	shell_print(ctx_shell, "Tx Power Report: Reason: %s, PHY: %s, Tx Power Level: %d",
		    tx_power_report_reason2str(report->reason), tx_pwr_ctrl_phy2str(report->phy),
		    report->tx_power_level);
	shell_print(ctx_shell, "Tx Power Level Flag Info: %s, Delta: %d",
		    tx_power_flag2str(report->tx_power_level_flag), report->delta);
}
#endif
static struct bt_conn_cb conn_callbacks = {
	.connected = connected,
	.disconnected = disconnected,
	.le_param_req = le_param_req,
	.le_param_updated = le_param_updated,
#if (defined(CONFIG_BT_SMP) && (CONFIG_BT_SMP > 0))
	.identity_resolved = identity_resolved,
#endif
#if (defined(CONFIG_BT_SMP) && (CONFIG_BT_SMP > 0)) || ((defined(CONFIG_BT_BREDR) && (CONFIG_BT_BREDR > 0)))
	.security_changed = security_changed,
#endif
#if (defined(CONFIG_BT_REMOTE_INFO) && (CONFIG_BT_REMOTE_INFO > 0))
	.remote_info_available = remote_info_available,
#endif
#if (defined(CONFIG_BT_USER_DATA_LEN_UPDATE) && (CONFIG_BT_USER_DATA_LEN_UPDATE > 0))
	.le_data_len_updated = le_data_len_updated,
#endif
#if (defined(CONFIG_BT_USER_PHY_UPDATE) && (CONFIG_BT_USER_PHY_UPDATE > 0))
	.le_phy_updated = le_phy_updated,
#endif
#if (defined(CONFIG_BT_TRANSMIT_POWER_CONTROL) && (CONFIG_BT_TRANSMIT_POWER_CONTROL > 0))
	.tx_power_report = tx_power_report,
#endif
};
#endif /* CONFIG_BT_CONN */

#if (defined(CONFIG_BT_OBSERVER) && (CONFIG_BT_OBSERVER > 0))
static struct bt_le_scan_cb scan_callbacks = {
	.recv = scan_recv,
	.timeout = scan_timeout,
};
#endif /* defined(CONFIG_BT_OBSERVER) */

#if (defined(CONFIG_BT_EXT_ADV) && (CONFIG_BT_EXT_ADV > 0))
#if (defined(CONFIG_BT_BROADCASTER) && (CONFIG_BT_BROADCASTER > 0))
static struct bt_le_ext_adv_cb adv_callbacks = {
	.sent = adv_sent,
	.scanned = adv_scanned,
#if (defined(CONFIG_BT_PERIPHERAL) && (CONFIG_BT_PERIPHERAL > 0))
	.connected = adv_connected,
#endif /* CONFIG_BT_PERIPHERAL */
#if (defined(CONFIG_BT_PRIVACY) && (CONFIG_BT_PRIVACY > 0))
	.rpa_expired = adv_rpa_expired,
#endif /* defined(CONFIG_BT_PRIVACY) */
};
#endif /* CONFIG_BT_BROADCASTER */
#endif /* CONFIG_BT_EXT_ADV */


#if (defined(CONFIG_BT_PER_ADV_SYNC) && (CONFIG_BT_PER_ADV_SYNC > 0))
struct bt_le_per_adv_sync *per_adv_syncs[CONFIG_BT_PER_ADV_SYNC_MAX];
size_t selected_per_adv_sync;

static void per_adv_sync_sync_cb(struct bt_le_per_adv_sync *sync,
				 struct bt_le_per_adv_sync_synced_info *info)
{
	const bool is_past_peer = info->conn != NULL;
	char le_addr[BT_ADDR_LE_STR_LEN];
	char past_peer[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(info->addr, le_addr, sizeof(le_addr));

	if (is_past_peer) {
		conn_addr_str(info->conn, past_peer, sizeof(past_peer));
	}

	shell_print(ctx_shell, "PER_ADV_SYNC[%u]: [DEVICE]: %s synced, "
		    "Interval 0x%04x (%u us), PHY %s, SD 0x%04X, PAST peer %s",
		    bt_le_per_adv_sync_get_index(sync), le_addr,
		    info->interval, BT_CONN_INTERVAL_TO_US(info->interval),
		    phy2str(info->phy), info->service_data,
		    is_past_peer ? past_peer : "not present");

	if (info->conn) { /* if from PAST */
		for (int i = 0; i < ARRAY_SIZE(per_adv_syncs); i++) {
			if (!per_adv_syncs[i]) {
				per_adv_syncs[i] = sync;
				break;
			}
		}
	}
}

static void per_adv_sync_terminated_cb(
	struct bt_le_per_adv_sync *sync,
	const struct bt_le_per_adv_sync_term_info *info)
{
	char le_addr[BT_ADDR_LE_STR_LEN];

	for (int i = 0; i < ARRAY_SIZE(per_adv_syncs); i++) {
		if (per_adv_syncs[i] == sync) {
			per_adv_syncs[i] = NULL;
			break;
		}
	}

	bt_addr_le_to_str(info->addr, le_addr, sizeof(le_addr));
	shell_print(ctx_shell, "PER_ADV_SYNC[%u]: [DEVICE]: %s sync terminated",
		    bt_le_per_adv_sync_get_index(sync), le_addr);
}

static void per_adv_sync_recv_cb(
	struct bt_le_per_adv_sync *sync,
	const struct bt_le_per_adv_sync_recv_info *info,
	struct net_buf_simple *buf)
{
	char le_addr[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(info->addr, le_addr, sizeof(le_addr));
	shell_print(ctx_shell, "PER_ADV_SYNC[%u]: [DEVICE]: %s, tx_power %i, "
		    "RSSI %i, CTE %u, data length %u",
		    bt_le_per_adv_sync_get_index(sync), le_addr, info->tx_power,
		    info->rssi, info->cte_type, buf->len);
}

static void per_adv_sync_biginfo_cb(struct bt_le_per_adv_sync *sync,
				    const struct bt_iso_biginfo *biginfo)
{
	char le_addr[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(biginfo->addr, le_addr, sizeof(le_addr));
	shell_print(ctx_shell, "BIG_INFO PER_ADV_SYNC[%u]: [DEVICE]: %s, sid 0x%02x, num_bis %u, "
		    "nse 0x%02x, interval 0x%04x (%u us), bn 0x%02x, pto 0x%02x, irc 0x%02x, "
		    "max_pdu 0x%04x, sdu_interval 0x%04x, max_sdu 0x%04x, phy %s, framing 0x%02x, "
		    "%sencrypted",
		    bt_le_per_adv_sync_get_index(sync), le_addr, biginfo->sid, biginfo->num_bis,
		    biginfo->sub_evt_count, biginfo->iso_interval,
		    BT_CONN_INTERVAL_TO_US(biginfo->iso_interval), biginfo->burst_number,
		    biginfo->offset, biginfo->rep_count, biginfo->max_pdu, biginfo->sdu_interval,
		    biginfo->max_sdu, phy2str(biginfo->phy), biginfo->framing,
		    biginfo->encryption ? "" : "not ");
}

static struct bt_le_per_adv_sync_cb per_adv_sync_cb = {
	.synced = per_adv_sync_sync_cb,
	.term = per_adv_sync_terminated_cb,
	.recv = per_adv_sync_recv_cb,
	.biginfo = per_adv_sync_biginfo_cb,
};
#endif /* CONFIG_BT_PER_ADV_SYNC */

static void bt_ready(int err)
{
	if (err) {
		shell_error(ctx_shell, "Bluetooth init failed (err %d)", err);
		return;
	}

	shell_print(ctx_shell, "Bluetooth initialized");

	if (IS_ENABLED(CONFIG_BT_SETTINGS) && !no_settings_load) {
		settings_load();
		shell_print(ctx_shell, "Settings Loaded");
	}

	if (IS_ENABLED(CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY)) {
		bt_le_oob_set_legacy_flag(true);
	}

#if (defined(CONFIG_BT_OBSERVER) && (CONFIG_BT_OBSERVER > 0))
	bt_le_scan_cb_register(&scan_callbacks);
#endif

#if (defined(CONFIG_BT_CONN) && (CONFIG_BT_CONN > 0))
	default_conn = NULL;

	bt_conn_cb_register(&conn_callbacks);
#endif /* CONFIG_BT_CONN */

#if (defined(CONFIG_BT_PER_ADV_SYNC) && (CONFIG_BT_PER_ADV_SYNC > 0))
	bt_le_per_adv_sync_cb_register(&per_adv_sync_cb);
#endif /* CONFIG_BT_PER_ADV_SYNC */

#if (defined(CONFIG_BT_SMP) && (CONFIG_BT_SMP > 0))
	bt_conn_auth_info_cb_register(&auth_info_cb);
#endif /* CONFIG_BT_SMP */
}

static shell_status_t cmd_init(shell_handle_t shell, int32_t argc, char *argv[])
{
	int err;
	bool sync = false;

	ctx_shell = shell;

	for (size_t argn = 1; argn < argc; argn++) {
		const char *arg = argv[argn];

		if (!strcmp(arg, "no-settings-load")) {
			no_settings_load = true;
		} else if (!strcmp(arg, "sync")) {
			sync = true;
		} else {
			shell_help(shell);
			return kStatus_SHELL_PrintCmdHelp;
		}
	}

	if (sync) {
		err = bt_enable(NULL);
		bt_ready(err);
	} else {
		err = bt_enable(bt_ready);
		if (err) {
			shell_error(shell, "Bluetooth init failed (err %d)",
				    err);
		}
	}

	return (shell_status_t)err;
}

#if (defined(CONFIG_BT_SETTINGS) && (CONFIG_BT_SETTINGS > 0))
static shell_status_t cmd_settings_load(shell_handle_t shell, int32_t argc, char *argv[])
{
	int err;

	err = settings_load();
	if (err) {
		shell_error(shell, "Settings load failed (err %d)", err);
		return (shell_status_t)err;
	}

	shell_print(shell, "Settings loaded");
	return kStatus_SHELL_Success;
}
#endif

#if (defined(CONFIG_BT_HCI) && (CONFIG_BT_HCI > 0))
static shell_status_t cmd_hci_cmd(shell_handle_t shell, int32_t argc, char *argv[])
{
	uint8_t ogf;
	uint16_t ocf;
	struct net_buf *buf = NULL, *rsp;
	int err;
	static uint8_t hex_data[HCI_CMD_MAX_PARAM];
	int hex_data_len;

	hex_data_len = 0;
	ogf = strtoul(argv[1], NULL, 16);
	ocf = strtoul(argv[2], NULL, 16);

	if (argc > 3) {
		size_t len;

		if (strlen(argv[3]) > 2 * HCI_CMD_MAX_PARAM) {
			shell_error(shell, "Data field too large\n");
			return kStatus_SHELL_Error;
		}

		len = hex2bin(argv[3], strlen(argv[3]), &hex_data[hex_data_len],
			      sizeof(hex_data) - hex_data_len);
		if (!len) {
			shell_error(shell, "HCI command illegal data field\n");
			return kStatus_SHELL_Error;
		}

		buf = bt_hci_cmd_create(BT_OP(ogf, ocf), len);
		net_buf_add_mem(buf, hex_data, len);
	}

	err = bt_hci_cmd_send_sync(BT_OP(ogf, ocf), buf, &rsp);
	if (err) {
		shell_error(shell, "HCI command failed (err %d)", err);
		return (shell_status_t)err;
	} else {
		shell_hexdump(shell, rsp->data, rsp->len);
		net_buf_unref(rsp);
	}

	return kStatus_SHELL_Success;
}
#endif /* CONFIG_BT_HCI */

static shell_status_t cmd_name(shell_handle_t shell, int32_t argc, char *argv[])
{
	int err;

	if (argc < 2) {
		shell_print(shell, "Bluetooth Local Name: %s", bt_get_name());
		return kStatus_SHELL_Success;
	}

	err = bt_set_name(argv[1]);
	if (err) {
		shell_error(shell, "Unable to set name %s (err %d)", argv[1],
			    err);
		return (shell_status_t)err;
	}

	return kStatus_SHELL_Success;
}

static shell_status_t cmd_appearance(shell_handle_t shell, int32_t argc, char *argv[])
{
	if (argc == 1) {
		shell_print(shell, "Bluetooth Appearance: 0x%04x", bt_get_appearance());
		return kStatus_SHELL_Success;
	}

#if (defined(CONFIG_BT_DEVICE_APPEARANCE_DYNAMIC) && (CONFIG_BT_DEVICE_APPEARANCE_DYNAMIC> 0))
	uint16_t app;
	int err;
	const char *val;

	val = argv[1];
	if (strlen(val) != 6 || strncmp(val, "0x", 2)) {
		shell_error(sh, "Argument must be 0x followed by exactly 4 hex digits.");
		return -EINVAL;
	}

	app = shell_strtoul(val, 16, &err);
	if (err) {
		shell_error(sh, "Argument must be 0x followed by exactly 4 hex digits.");
		return -EINVAL;
	}

	err = bt_set_appearance(app);
	if (err) {
		shell_error(shell, "bt_set_appearance(0x%04x) failed with err %d", app, err);
		return (shell_status_t)err;
	}
#endif /* defined(CONFIG_BT_DEVICE_APPEARANCE_DYNAMIC) */

	return kStatus_SHELL_Success;
}

static shell_status_t cmd_id_create(shell_handle_t shell, int32_t argc, char *argv[])
{
	char addr_str[BT_ADDR_LE_STR_LEN];
	bt_addr_le_t addr;
	int err;

	if (argc > 1) {
		err = bt_addr_le_from_str(argv[1], "random", &addr);
		if (err) {
			shell_error(shell, "Invalid address");
		}
	} else {
		bt_addr_le_copy(&addr, BT_ADDR_LE_ANY);
	}

	err = bt_id_create(&addr, NULL);
	if (err < 0) {
		shell_error(shell, "Creating new ID failed (err %d)", err);
	}

	bt_addr_le_to_str(&addr, addr_str, sizeof(addr_str));
	shell_print(shell, "New identity (%d) created: %s", err, addr_str);

	return kStatus_SHELL_Success;
}

static shell_status_t cmd_id_reset(shell_handle_t shell, int32_t argc, char *argv[])
{
	char addr_str[BT_ADDR_LE_STR_LEN];
	bt_addr_le_t addr;
	uint8_t id;
	int err;

	if (argc < 2) {
		shell_error(shell, "Identity identifier not specified");
		return kStatus_SHELL_Error;
	}

	id = strtol(argv[1], NULL, 10);

	if (argc > 2) {
		err = bt_addr_le_from_str(argv[2], "random", &addr);
		if (err) {
			shell_print(shell, "Invalid address");
			return (shell_status_t)err;
		}
	} else {
		bt_addr_le_copy(&addr, BT_ADDR_LE_ANY);
	}

	err = bt_id_reset(id, &addr, NULL);
	if (err < 0) {
		shell_print(shell, "Resetting ID %u failed (err %d)", id, err);
		return (shell_status_t)err;
	}

	bt_addr_le_to_str(&addr, addr_str, sizeof(addr_str));
	shell_print(shell, "Identity %u reset: %s", id, addr_str);

	return kStatus_SHELL_Success;
}

static shell_status_t cmd_id_delete(shell_handle_t shell, int32_t argc, char *argv[])
{
	uint8_t id;
	int err;

	if (argc < 2) {
		shell_error(shell, "Identity identifier not specified");
		return kStatus_SHELL_Error;
	}

	id = strtol(argv[1], NULL, 10);

	err = bt_id_delete(id);
	if (err < 0) {
		shell_error(shell, "Deleting ID %u failed (err %d)", id, err);
		return kStatus_SHELL_Error;
	}

	shell_print(shell, "Identity %u deleted", id);

	return kStatus_SHELL_Success;
}

static shell_status_t cmd_id_show(shell_handle_t shell, int32_t argc, char *argv[])
{
	bt_addr_le_t addrs[CONFIG_BT_ID_MAX];
	size_t i, count = CONFIG_BT_ID_MAX;

	bt_id_get(addrs, &count);

	for (i = 0; i < count; i++) {
		char addr_str[BT_ADDR_LE_STR_LEN];

		bt_addr_le_to_str(&addrs[i], addr_str, sizeof(addr_str));
		shell_print(shell, "%s%lu: %s", i == selected_id ? "*" : " ", i,
		      addr_str);
	}

	return kStatus_SHELL_Success;
}

static shell_status_t cmd_id_select(shell_handle_t shell, int32_t argc, char *argv[])
{
	char addr_str[BT_ADDR_LE_STR_LEN];
	bt_addr_le_t addrs[CONFIG_BT_ID_MAX];
	size_t count = CONFIG_BT_ID_MAX;
	uint8_t id;

	id = strtol(argv[1], NULL, 10);

	bt_id_get(addrs, &count);
	if (count <= id) {
		shell_error(shell, "Invalid identity");
		return kStatus_SHELL_Error;
	}

	bt_addr_le_to_str(&addrs[id], addr_str, sizeof(addr_str));
	shell_print(shell, "Selected identity: %s", addr_str);
	selected_id = id;

	return kStatus_SHELL_Success;
}

#if (defined(CONFIG_BT_OBSERVER) && (CONFIG_BT_OBSERVER > 0))
static shell_status_t cmd_active_scan_on(shell_handle_t shell, uint32_t options,
			      uint16_t timeout)
{
	int err;
	struct bt_le_scan_param param = {
			.type       = BT_LE_SCAN_TYPE_ACTIVE,
			.options    = BT_LE_SCAN_OPT_FILTER_DUPLICATE,
			.interval   = BT_GAP_SCAN_FAST_INTERVAL,
			.window     = BT_GAP_SCAN_FAST_WINDOW,
			.timeout    = timeout, };

	param.options |= options;

#if (defined(CONFIG_BLE_ADV_REPORT_BUFFER_FILTER) && (CONFIG_BLE_ADV_REPORT_BUFFER_FILTER > 0U))
    shell_le_init_adv_list();
#endif /* CONFIG_BLE_ADV_REPORT_BUFFER_FILTER */

	err = bt_le_scan_start(&param, NULL);
	if (err) {
		shell_error(shell, "Bluetooth set active scan failed "
		      "(err %d)", err);
		return (shell_status_t)err;
	} else {
		shell_print(shell, "Bluetooth active scan enabled");
	}

	return kStatus_SHELL_Success;
}

static shell_status_t cmd_passive_scan_on(shell_handle_t shell, uint32_t options,
			       uint16_t timeout)
{
	struct bt_le_scan_param param = {
			.type       = BT_LE_SCAN_TYPE_PASSIVE,
			.options    = BT_LE_SCAN_OPT_NONE,
			.interval   = 0x10,
			.window     = 0x10,
			.timeout    = timeout, };
	int err;

	param.options |= options;

#if (defined(CONFIG_BLE_ADV_REPORT_BUFFER_FILTER) && (CONFIG_BLE_ADV_REPORT_BUFFER_FILTER > 0U))
    shell_le_init_adv_list();
#endif /* CONFIG_BLE_ADV_REPORT_BUFFER_FILTER */

	err = bt_le_scan_start(&param, NULL);
	if (err) {
		shell_error(shell, "Bluetooth set passive scan failed "
			    "(err %d)", err);
		return (shell_status_t)err;
	} else {
		shell_print(shell, "Bluetooth passive scan enabled");
	}

	return kStatus_SHELL_Success;
}

static shell_status_t cmd_scan_off(shell_handle_t shell)
{
	int err;

	err = bt_le_scan_stop();
	if (err) {
		shell_error(shell, "Stopping scanning failed (err %d)", err);
		return (shell_status_t)err;
	} else {
		shell_print(shell, "Scan successfully stopped");
	}

	return kStatus_SHELL_Success;
}

static shell_status_t cmd_scan(shell_handle_t shell, int32_t argc, char *argv[])
{
	const char *action;
	uint32_t options = 0;
	uint16_t timeout = 0;

	/* Parse duplicate filtering data */
	for (size_t argn = 2; argn < argc; argn++) {
		const char *arg = argv[argn];

		if (!strcmp(arg, "dups")) {
			options |= BT_LE_SCAN_OPT_FILTER_DUPLICATE;
		} else if (!strcmp(arg, "nodups")) {
			options &= ~BT_LE_SCAN_OPT_FILTER_DUPLICATE;
		} else if (!strcmp(arg, "fal")) {
			options |= BT_LE_SCAN_OPT_FILTER_ACCEPT_LIST;
		} else if (!strcmp(arg, "coded")) {
			options |= BT_LE_SCAN_OPT_CODED;
		} else if (!strcmp(arg, "no-1m")) {
			options |= BT_LE_SCAN_OPT_NO_1M;
		} else if (!strcmp(arg, "timeout")) {
			if (++argn == argc) {
				shell_help(shell);
				return kStatus_SHELL_PrintCmdHelp;
			}

			timeout = strtoul(argv[argn], NULL, 16);
		} else {
			shell_help(shell);
			return kStatus_SHELL_PrintCmdHelp;
		}
	}

	action = argv[1];
	if (!strcmp(action, "on")) {
		return cmd_active_scan_on(shell, options, timeout);
	} else if (!strcmp(action, "off")) {
		return cmd_scan_off(shell);
	} else if (!strcmp(action, "passive")) {
		return cmd_passive_scan_on(shell, options, timeout);
	} else {
		shell_help(shell);
		return kStatus_SHELL_PrintCmdHelp;
	}
}
static shell_status_t cmd_scan_verbose_output(shell_handle_t shell, int32_t argc,
				    char *argv[])
{
	const char *verbose_state;

	verbose_state = argv[1];
	if (!strcmp(verbose_state, "on")) {
		scan_verbose_output = true;
	} else if (!strcmp(verbose_state, "off")) {
		scan_verbose_output = false;
	} else {
		shell_help(shell);
		return kStatus_SHELL_PrintCmdHelp;
	}

	return kStatus_SHELL_Success;
}
static shell_status_t cmd_scan_filter_set_name(shell_handle_t shell, int32_t argc,
				    char *argv[])
{
	const char *name_arg = argv[1];

	if (strlen(name_arg) >= sizeof(scan_filter.name)) {
		shell_error(ctx_shell, "Name is too long (max %u): %s\n",
			    sizeof(scan_filter.name), name_arg);
		return kStatus_SHELL_Error;
	}

	strcpy(scan_filter.name, name_arg);
	scan_filter.name_set = true;

	return kStatus_SHELL_Success;
}

static shell_status_t cmd_scan_filter_set_addr(shell_handle_t shell, int32_t argc,
				    char *argv[])
{
	const char *addr_arg = argv[1];

	/* Validate length including null terminator. */
	if (strlen(addr_arg) >= sizeof(scan_filter.addr)) {
		shell_error(ctx_shell, "Invalid address string: %s\n",
			    addr_arg);
		return kStatus_SHELL_Error;
	}

	/* Validate input to check if valid (subset of) BT address */
	for (size_t i = 0; i < strlen(addr_arg); i++) {
		const char c = addr_arg[i];
		uint8_t tmp;

		if (c != ':' && char2hex(c, &tmp) < 0) {
			shell_error(ctx_shell,
					"Invalid address string: %s\n",
					addr_arg);
			return kStatus_SHELL_Error;
		}
	}

	strcpy(scan_filter.addr, addr_arg);
	scan_filter.addr_set = true;

	return kStatus_SHELL_Success;
}

static shell_status_t cmd_scan_filter_set_rssi(shell_handle_t shell, int32_t argc, char *argv[])
{
	int err = 0;
	long rssi;

	rssi = shell_strtol(argv[1], 10, &err);

	if (!err) {
		if (IN_RANGE(rssi, INT8_MIN, INT8_MAX)) {
			scan_filter.rssi = (int8_t)rssi;
			scan_filter.rssi_set = true;
			shell_print(shell, "RSSI cutoff set at %d dB", scan_filter.rssi);

			return kStatus_SHELL_Success;
		}

		shell_print(shell, "value out of bounds (%d to %d)", INT8_MIN, INT8_MAX);
		err = kStatus_SHELL_Error;
	}

	shell_print(shell, "error %d", err);
	shell_help(shell);

	return kStatus_SHELL_Success;
}
static shell_status_t cmd_scan_filter_set_pa_interval(shell_handle_t shell, int32_t argc,
					   char *argv[])
{
	unsigned long pa_interval;
	int err = 0;

	pa_interval = shell_strtoul(argv[1], 10, &err);

	if (!err) {
		if (IN_RANGE(pa_interval,
			     BT_GAP_PER_ADV_MIN_INTERVAL,
			     BT_GAP_PER_ADV_MAX_INTERVAL)) {
			scan_filter.pa_interval = (uint16_t)pa_interval;
			scan_filter.pa_interval_set = true;
			shell_print(shell, "PA interval cutoff set at %u",
				    scan_filter.pa_interval);

			return kStatus_SHELL_Success;
		}

		shell_print(shell, "value out of bounds (%d to %d)",
			    BT_GAP_PER_ADV_MIN_INTERVAL,
			    BT_GAP_PER_ADV_MAX_INTERVAL);

		err = (shell_status_t)-ERANGE;
	}

	shell_print(shell, "error %d", err);
	shell_help(shell);

	return kStatus_SHELL_PrintCmdHelp;
}

static shell_status_t cmd_scan_filter_clear_all(shell_handle_t shell, int32_t argc,
				     char *argv[])
{
	(void)memset(&scan_filter, 0, sizeof(scan_filter));

	return kStatus_SHELL_Success;
}

static shell_status_t cmd_scan_filter_clear_name(shell_handle_t shell, int32_t argc,
				      char *argv[])
{
	(void)memset(scan_filter.name, 0, sizeof(scan_filter.name));
	scan_filter.name_set = false;

	return kStatus_SHELL_Success;
}

static shell_status_t cmd_scan_filter_clear_addr(shell_handle_t shell, int32_t argc,
				      char *argv[])
{
	(void)memset(scan_filter.addr, 0, sizeof(scan_filter.addr));
	scan_filter.addr_set = false;

	return kStatus_SHELL_Success;
}

#endif /* CONFIG_BT_OBSERVER */

#if (defined(CONFIG_BT_BROADCASTER) && (CONFIG_BT_BROADCASTER > 0))
static ssize_t ad_init(struct bt_data *data_array, const size_t data_array_size,
		       const atomic_t *adv_options)
{
	const bool discoverable = atomic_test_bit(adv_options, SHELL_ADV_OPT_DISCOVERABLE);
	const bool appearance = atomic_test_bit(adv_options, SHELL_ADV_OPT_APPEARANCE);
	const bool adv_ext = atomic_test_bit(adv_options, SHELL_ADV_OPT_EXT_ADV);
	static uint8_t ad_flags;
	size_t ad_len = 0;

	/* Set BR/EDR Not Supported if LE-only device */
	ad_flags = IS_ENABLED(CONFIG_BT_BREDR) ? 0 : BT_LE_AD_NO_BREDR;

	if (discoverable) {
		/* A privacy-enabled Set Member should advertise RSI values only when in
		 * the GAP Limited Discoverable mode.
		 */
#if (defined(CONFIG_BT_CSIP) && (CONFIG_BT_CSIP > 0))
		if (IS_ENABLED(CONFIG_BT_PRIVACY) &&
		    IS_ENABLED(CONFIG_BT_CSIP_SET_MEMBER) &&
		    svc_inst != NULL) {
			ad_flags |= BT_LE_AD_LIMITED;
		} else 
#endif
                {
			ad_flags |= BT_LE_AD_GENERAL;
		}
	}

	if (ad_flags != 0) {
		__ASSERT(data_array_size > ad_len, "No space for AD_FLAGS");
		data_array[ad_len].type = BT_DATA_FLAGS;
		data_array[ad_len].data_len = sizeof(ad_flags);
		data_array[ad_len].data = &ad_flags;
		ad_len++;
	}
	if (appearance) {
		const uint16_t appearance2 = bt_get_appearance();
		static uint8_t appearance_data[sizeof(appearance2)];

		__ASSERT(data_array_size > ad_len, "No space for appearance");
		sys_put_le16(appearance2, appearance_data);
		data_array[ad_len].type = BT_DATA_GAP_APPEARANCE;
		data_array[ad_len].data_len = sizeof(appearance_data);
		data_array[ad_len].data = appearance_data;
		ad_len++;
	}

#if (defined(CONFIG_BT_CSIP) && (CONFIG_BT_CSIP > 0))
	if (IS_ENABLED(CONFIG_BT_CSIP)) {
		ssize_t csis_ad_len;

		csis_ad_len = csis_ad_data_add(&data_array[ad_len],
					       data_array_size - ad_len, discoverable);
		if (csis_ad_len < 0) {
			shell_error(ctx_shell, "Failed to add CSIS data (err %d)", csis_ad_len);
			return ad_len;
		}

		ad_len += csis_ad_len;
	}
#endif

#if (defined(CONFIG_BT_AUDIO) && (CONFIG_BT_AUDIO > 0))
	if (IS_ENABLED(CONFIG_BT_AUDIO) && IS_ENABLED(CONFIG_BT_EXT_ADV) && adv_ext) {
		const bool connectable = atomic_test_bit(adv_options, SHELL_ADV_OPT_CONNECTABLE);
		size_t audio_ad_len;

		audio_ad_len = audio_ad_data_add(&data_array[ad_len], data_array_size - ad_len,
						 discoverable, connectable);
#if 0 /* pointless comparison of unsigned integer with zero. */
		if (audio_ad_len < 0) {
			return audio_ad_len;
		}
#endif

		ad_len += audio_ad_len;
	}
#endif

	(void)adv_ext;   /* Unused variable */

	return ad_len;
}

static shell_status_t cmd_advertise(shell_handle_t shell, int32_t argc, char *argv[])
{
	struct bt_le_adv_param param = {0};
	struct bt_data ad[2];
	bool discoverable = true;
	bool appearance = false;
	ssize_t ad_len;
	int err;

	if (!strcmp(argv[1], "off")) {
		if (bt_le_adv_stop() < 0) {
			shell_error(shell, "Failed to stop advertising");
			return kStatus_SHELL_Error;
		} else {
			shell_print(shell, "Advertising stopped");
		}

		return kStatus_SHELL_Success;
	}

	param.id = selected_id;
	param.interval_min = BT_GAP_ADV_FAST_INT_MIN_2;
	param.interval_max = BT_GAP_ADV_FAST_INT_MAX_2;

	if (!strcmp(argv[1], "on")) {
		param.options = (BT_LE_ADV_OPT_CONNECTABLE |
				 BT_LE_ADV_OPT_USE_NAME);
	} else if (!strcmp(argv[1], "scan")) {
		param.options = BT_LE_ADV_OPT_USE_NAME;
	} else if (!strcmp(argv[1], "nconn")) {
		param.options = 0U;
	} else {
		goto fail;
	}


	for (size_t argn = 2; argn < argc; argn++) {
		const char *arg = argv[argn];

		if (!strcmp(arg, "discov")) {
			discoverable = true;
		} else if (!strcmp(arg, "non_discov")) {
			discoverable = false;
		} else if (!strcmp(arg, "appearance")) {
			appearance = true;
		} else if (!strcmp(arg, "fal")) {
			param.options |= BT_LE_ADV_OPT_FILTER_SCAN_REQ;
			param.options |= BT_LE_ADV_OPT_FILTER_CONN;
		} else if (!strcmp(arg, "fal-scan")) {
			param.options |= BT_LE_ADV_OPT_FILTER_SCAN_REQ;
		} else if (!strcmp(arg, "fal-conn")) {
			param.options |= BT_LE_ADV_OPT_FILTER_CONN;
		} else if (!strcmp(arg, "identity")) {
			param.options |= BT_LE_ADV_OPT_USE_IDENTITY;
		} else if (!strcmp(arg, "no-name")) {
			param.options &= ~BT_LE_ADV_OPT_USE_NAME;
		} else if (!strcmp(arg, "name-ad")) {
			param.options |= BT_LE_ADV_OPT_USE_NAME;
			param.options |= BT_LE_ADV_OPT_FORCE_NAME_IN_AD;
		} else if (!strcmp(arg, "one-time")) {
			param.options |= BT_LE_ADV_OPT_ONE_TIME;
		} else if (!strcmp(arg, "disable-37")) {
			param.options |= BT_LE_ADV_OPT_DISABLE_CHAN_37;
		} else if (!strcmp(arg, "disable-38")) {
			param.options |= BT_LE_ADV_OPT_DISABLE_CHAN_38;
		} else if (!strcmp(arg, "disable-39")) {
			param.options |= BT_LE_ADV_OPT_DISABLE_CHAN_39;
		} else {
			goto fail;
		}
	}

	atomic_clear(adv_opt);
	atomic_set_bit_to(adv_opt, SHELL_ADV_OPT_CONNECTABLE,
			  (param.options & BT_LE_ADV_OPT_CONNECTABLE) > 0);
	atomic_set_bit_to(adv_opt, SHELL_ADV_OPT_DISCOVERABLE, discoverable);
	atomic_set_bit_to(adv_opt, SHELL_ADV_OPT_APPEARANCE, appearance);

	ad_len = ad_init(ad, ARRAY_SIZE(ad), adv_opt);
	if (ad_len < 0) {
		return kStatus_SHELL_Error;
	}
	err = bt_le_adv_start(&param, ad_len > 0 ? ad : NULL, ad_len, NULL, 0);
	if (err < 0) {
		shell_error(shell, "Failed to start advertising (err %d)",
			    err);
		return (shell_status_t)err;
	} else {
		shell_print(shell, "Advertising started");
	}

	return kStatus_SHELL_Success;

fail:
	shell_help(shell);
	return kStatus_SHELL_Error;
}

#if (defined(CONFIG_BT_PERIPHERAL) && (CONFIG_BT_PERIPHERAL > 0))
static shell_status_t cmd_directed_adv(shell_handle_t shell,
			     int32_t argc, char *argv[])
{
	int err;
	bt_addr_le_t addr;
	struct bt_le_adv_param param;

	err = bt_addr_le_from_str(argv[1], argv[2], &addr);
	param = *BT_LE_ADV_CONN_DIR(&addr);
	if (err) {
		shell_error(shell, "Invalid peer address (err %d)", err);
		return (shell_status_t)err;
	}

	for (size_t argn = 3; argn < argc; argn++) {
		const char *arg = argv[argn];

		if (!strcmp(arg, "low")) {
			param.options |= BT_LE_ADV_OPT_DIR_MODE_LOW_DUTY;
			param.interval_max = BT_GAP_ADV_FAST_INT_MAX_2;
			param.interval_min = BT_GAP_ADV_FAST_INT_MIN_2;
		} else if (!strcmp(arg, "identity")) {
			param.options |= BT_LE_ADV_OPT_USE_IDENTITY;
		} else if (!strcmp(arg, "dir-rpa")) {
			param.options |= BT_LE_ADV_OPT_DIR_ADDR_RPA;
		} else if (!strcmp(arg, "disable-37")) {
			param.options |= BT_LE_ADV_OPT_DISABLE_CHAN_37;
		} else if (!strcmp(arg, "disable-38")) {
			param.options |= BT_LE_ADV_OPT_DISABLE_CHAN_38;
		} else if (!strcmp(arg, "disable-39")) {
			param.options |= BT_LE_ADV_OPT_DISABLE_CHAN_39;
		} else {
			shell_help(shell);
			return kStatus_SHELL_Error;
		}
	}

	if (0 == (param.options & BT_LE_ADV_OPT_DIR_MODE_LOW_DUTY))
	{
		param.interval_max = BT_GAP_ADV_FAST_INT_MAX_1;
		param.interval_min = BT_GAP_ADV_FAST_INT_MIN_1;
	}

	err = bt_le_adv_start(&param, NULL, 0, NULL, 0);
	if (err) {
		shell_error(shell, "Failed to start directed advertising (%d)",
			    err);
		return kStatus_SHELL_Error;
	} else {
		shell_print(shell, "Started directed advertising");
	}

	return kStatus_SHELL_Success;
}
#endif /* CONFIG_BT_PERIPHERAL */

#if (defined(CONFIG_BT_EXT_ADV) && (CONFIG_BT_EXT_ADV > 0))
static bool adv_param_parse(int32_t argc, char *argv[],
			   struct bt_le_adv_param *param)
{
	memset(param, 0, sizeof(struct bt_le_adv_param));

	if (!strcmp(argv[1], "conn-scan")) {
		param->options |= BT_LE_ADV_OPT_CONNECTABLE;
		param->options |= BT_LE_ADV_OPT_SCANNABLE;
	} else if (!strcmp(argv[1], "conn-nscan")) {
		param->options |= BT_LE_ADV_OPT_CONNECTABLE;
	} else if (!strcmp(argv[1], "nconn-scan")) {
		param->options |= BT_LE_ADV_OPT_SCANNABLE;
	} else if (!strcmp(argv[1], "nconn-nscan")) {
		/* Acceptable option, nothing to do */
	} else {
		return false;
	}

	for (size_t argn = 2; argn < argc; argn++) {
		const char *arg = argv[argn];

		if (!strcmp(arg, "ext-adv")) {
			param->options |= BT_LE_ADV_OPT_EXT_ADV;
		} else if (!strcmp(arg, "coded")) {
			param->options |= BT_LE_ADV_OPT_CODED;
		} else if (!strcmp(arg, "no-2m")) {
			param->options |= BT_LE_ADV_OPT_NO_2M;
		} else if (!strcmp(arg, "anon")) {
			param->options |= BT_LE_ADV_OPT_ANONYMOUS;
		} else if (!strcmp(arg, "tx-power")) {
			param->options |= BT_LE_ADV_OPT_USE_TX_POWER;
		} else if (!strcmp(arg, "scan-reports")) {
			param->options |= BT_LE_ADV_OPT_NOTIFY_SCAN_REQ;
		} else if (!strcmp(arg, "fal")) {
			param->options |= BT_LE_ADV_OPT_FILTER_SCAN_REQ;
			param->options |= BT_LE_ADV_OPT_FILTER_CONN;
		} else if (!strcmp(arg, "fal-scan")) {
			param->options |= BT_LE_ADV_OPT_FILTER_SCAN_REQ;
		} else if (!strcmp(arg, "fal-conn")) {
			param->options |= BT_LE_ADV_OPT_FILTER_CONN;
		} else if (!strcmp(arg, "identity")) {
			param->options |= BT_LE_ADV_OPT_USE_IDENTITY;
		} else if (!strcmp(arg, "name")) {
			param->options |= BT_LE_ADV_OPT_USE_NAME;
		} else if (!strcmp(arg, "name-ad")) {
			param->options |= BT_LE_ADV_OPT_USE_NAME;
			param->options |= BT_LE_ADV_OPT_FORCE_NAME_IN_AD;
		} else if (!strcmp(arg, "low")) {
			param->options |= BT_LE_ADV_OPT_DIR_MODE_LOW_DUTY;
		} else if (!strcmp(arg, "dir-rpa")) {
			param->options |= BT_LE_ADV_OPT_DIR_ADDR_RPA;
		} else if (!strcmp(arg, "disable-37")) {
			param->options |= BT_LE_ADV_OPT_DISABLE_CHAN_37;
		} else if (!strcmp(arg, "disable-38")) {
			param->options |= BT_LE_ADV_OPT_DISABLE_CHAN_38;
		} else if (!strcmp(arg, "disable-39")) {
			param->options |= BT_LE_ADV_OPT_DISABLE_CHAN_39;
		} else if (!strcmp(arg, "directed")) {
			static bt_addr_le_t addr;

			if ((argn + 2) >= argc) {
				return false;
			}

			if (bt_addr_le_from_str(argv[argn + 1], argv[argn + 2],
						&addr)) {
				return false;
			}

			param->peer = &addr;
			argn += 2;
		} else {
			return false;
		}
	}

	param->id = selected_id;
	param->sid = 0;
	if (param->peer &&
	    !(param->options & BT_LE_ADV_OPT_DIR_MODE_LOW_DUTY)) {
		param->interval_min = 0;
		param->interval_max = 0;
	} else {
		param->interval_min = BT_GAP_ADV_FAST_INT_MIN_2;
		param->interval_max = BT_GAP_ADV_FAST_INT_MAX_2;
	}

	return true;
}

static shell_status_t cmd_adv_create(shell_handle_t shell, int32_t argc, char *argv[])
{
	struct bt_le_adv_param param;
	struct bt_le_ext_adv *adv;
	uint8_t adv_index;
	int err;

	if (!adv_param_parse(argc, argv, &param)) {
		shell_help(shell);
		return kStatus_SHELL_Error;
	}

	err = bt_le_ext_adv_create(&param, &adv_callbacks, &adv);
	if (err) {
		shell_error(shell, "Failed to create advertiser set (%d)", err);
		return kStatus_SHELL_Error;
	}

	adv_index = bt_le_ext_adv_get_index(adv);
	adv_sets[adv_index] = adv;

	atomic_clear(adv_set_opt[adv_index]);
	atomic_set_bit_to(adv_set_opt[adv_index], SHELL_ADV_OPT_CONNECTABLE,
			  (param.options & BT_LE_ADV_OPT_CONNECTABLE) > 0);
	atomic_set_bit_to(adv_set_opt[adv_index], SHELL_ADV_OPT_EXT_ADV,
			  (param.options & BT_LE_ADV_OPT_EXT_ADV) > 0);

	shell_print(shell, "Created adv id: %d, adv: %p", adv_index, adv);

	return kStatus_SHELL_Success;
}

static shell_status_t cmd_adv_param(shell_handle_t shell, int32_t argc, char *argv[])
{
	struct bt_le_ext_adv *adv = adv_sets[selected_adv];
	struct bt_le_adv_param param;
	int err;

	if (!adv_param_parse(argc, argv, &param)) {
		shell_help(shell);
		return kStatus_SHELL_Error;
	}

	err = bt_le_ext_adv_update_param(adv, &param);
	if (err) {
		shell_error(shell, "Failed to update advertiser set (%d)", err);
		return kStatus_SHELL_Error;
	}

	return kStatus_SHELL_Success;
}

static shell_status_t cmd_adv_data(shell_handle_t shell, int32_t argc, char *argv[])
{
	struct bt_le_ext_adv *adv = adv_sets[selected_adv];
	static uint8_t hex_data[1650];
        bool appearance = false;
	struct bt_data *data;
	struct bt_data ad[AD_SIZE];
	struct bt_data sd[AD_SIZE];
	size_t hex_data_len;
	size_t ad_len = 0;
	size_t sd_len = 0;
	int32_t len = 0;
	bool discoverable = false;
	size_t *data_len;
	int err;

	if (!adv) {
		return kStatus_SHELL_Error;
	}

	hex_data_len = 0;
	data = ad;
	data_len = &ad_len;

	for (size_t argn = 1; argn < argc; argn++) {
		const char *arg = argv[argn];

		if (strcmp(arg, "scan-response") &&
		    *data_len == ARRAY_SIZE(ad)) {
			/* Maximum entries limit reached. */
			shell_print(shell, "Failed to set advertising data: "
					   "Maximum entries limit reached");

			return kStatus_SHELL_Error;
		}

		if (!strcmp(arg, "discov")) {
			discoverable = true;
		} else if (!strcmp(arg, "non_discov")) {
			discoverable = false;
		} else if (!strcmp(arg, "appearance")) {
			appearance = true;
		} else if (!strcmp(arg, "scan-response")) {
			if (data == sd) {
				shell_print(shell, "Failed to set advertising data: "
						   "duplicate scan-response option");
				return kStatus_SHELL_Error;
			}

			data = sd;
			data_len = &sd_len;
		} else {
			len = hex2bin(arg, strlen(arg), &hex_data[hex_data_len],
				      sizeof(hex_data) - hex_data_len);

			if (!len || (len - 1) != (hex_data[hex_data_len])) {
				shell_print(shell, "Failed to set advertising data: "
						   "malformed hex data");
				return kStatus_SHELL_Error;
			}

			data[*data_len].type = hex_data[hex_data_len + 1];
			data[*data_len].data_len = len - 2;
			data[*data_len].data = &hex_data[hex_data_len + 2];
			(*data_len)++;
			hex_data_len += len;
		}
	}

	atomic_set_bit_to(adv_set_opt[selected_adv], SHELL_ADV_OPT_DISCOVERABLE, discoverable);
	atomic_set_bit_to(adv_set_opt[selected_adv], SHELL_ADV_OPT_APPEARANCE,
			  appearance);

	len = ad_init(&data[*data_len], AD_SIZE - *data_len, adv_set_opt[selected_adv]);
	if (len < 0) {
		shell_error(shell, "Failed to initialize stack advertising data");

        return kStatus_SHELL_Error;
	}

	if (data == ad) {
		ad_len += len;
	} else {
		sd_len += len;
	}

	err = bt_le_ext_adv_set_data(adv, ad_len > 0 ? ad : NULL, ad_len,
					  sd_len > 0 ? sd : NULL, sd_len);
	if (err) {
		shell_print(shell, "Failed to set advertising set data (%d)",
			    err);
		return kStatus_SHELL_Error;
	}

	return kStatus_SHELL_Success;
}

static shell_status_t cmd_adv_start(shell_handle_t shell, int32_t argc, char *argv[])
{
	struct bt_le_ext_adv *adv = adv_sets[selected_adv];
	struct bt_le_ext_adv_start_param param;
	uint8_t num_events = 0;
	int32_t timeout = 0;
	int err;

	if (!adv) {
		shell_print(shell, "Advertiser[%d] not created", selected_adv);
		return kStatus_SHELL_Error;
	}

	for (size_t argn = 1; argn < argc; argn++) {
		const char *arg = argv[argn];

		if (!strcmp(arg, "timeout")) {
			if (++argn == argc) {
				goto fail_show_help;
			}

			timeout = strtoul(argv[argn], NULL, 16);
		}

		if (!strcmp(arg, "num-events")) {
			if (++argn == argc) {
				goto fail_show_help;
			}

			num_events = strtoul(argv[argn], NULL, 16);
		}
	}

	param.timeout = timeout;
	param.num_events = num_events;

	err = bt_le_ext_adv_start(adv, &param);
	if (err) {
		shell_print(shell, "Failed to start advertising set (%d)", err);
		return kStatus_SHELL_Error;
	}

	shell_print(shell, "Advertiser[%d] %p set started", selected_adv, adv);
	return kStatus_SHELL_Success;

fail_show_help:
	shell_help(shell);
	return kStatus_SHELL_Error;
}

static shell_status_t cmd_adv_stop(shell_handle_t shell, int32_t argc, char *argv[])
{
	struct bt_le_ext_adv *adv = adv_sets[selected_adv];
	int err;

	if (!adv) {
		shell_print(shell, "Advertiser[%d] not created", selected_adv);
		return kStatus_SHELL_Error;
	}

	err = bt_le_ext_adv_stop(adv);
	if (err) {
		shell_print(shell, "Failed to stop advertising set (%d)", err);
		return kStatus_SHELL_Error;
	}

	shell_print(shell, "Advertiser set stopped");
	return kStatus_SHELL_Success;
}

static shell_status_t cmd_adv_delete(shell_handle_t shell, int32_t argc, char *argv[])
{
	struct bt_le_ext_adv *adv = adv_sets[selected_adv];
	int err;

	if (!adv) {
		shell_print(shell, "Advertiser[%d] not created", selected_adv);
		return kStatus_SHELL_Error;
	}

	err = bt_le_ext_adv_delete(adv);
	if (err) {
		shell_error(ctx_shell, "Failed to delete advertiser set");
		return (shell_status_t)err;
	}

	adv_sets[selected_adv] = NULL;
	return kStatus_SHELL_Success;
}

static shell_status_t cmd_adv_select(shell_handle_t shell, int32_t argc, char *argv[])
{
	if (argc == 2) {
		uint8_t id = strtol(argv[1], NULL, 10);

		if (!(id < ARRAY_SIZE(adv_sets))) {
			return kStatus_SHELL_Error;
		}

		selected_adv = id;
		return kStatus_SHELL_Success;
	}

	for (int i = 0; i < ARRAY_SIZE(adv_sets); i++) {
		if (adv_sets[i]) {
			shell_print(shell, "Advertiser[%d] %p", i, adv_sets[i]);
		}
	}

	return kStatus_SHELL_Error;
}

static shell_status_t cmd_adv_info(shell_handle_t shell, int32_t argc, char *argv[])
{
	struct bt_le_ext_adv *adv = adv_sets[selected_adv];
	struct bt_le_ext_adv_info info;
	int err;

	if (!adv) {
		return kStatus_SHELL_Error;
	}

	err = bt_le_ext_adv_get_info(adv, &info);
	if (err) {
		shell_error(shell, "OOB data failed");
		return (shell_status_t)err;
	}

	shell_print(shell, "Advertiser[%d] %p", selected_adv, adv);
	shell_print(shell, "Id: %d, TX power: %d dBm", info.id, info.tx_power);
	print_le_addr("Address", info.addr);

	return kStatus_SHELL_Success;
}

#if (defined(CONFIG_BT_PERIPHERAL) && (CONFIG_BT_PERIPHERAL > 0))
static shell_status_t cmd_adv_oob(shell_handle_t shell, int32_t argc, char *argv[])
{
	struct bt_le_ext_adv *adv = adv_sets[selected_adv];
	int err;

	if (!adv) {
		return kStatus_SHELL_Error;
	}

	err = bt_le_ext_adv_oob_get_local(adv, &oob_local);
	if (err) {
		shell_error(shell, "OOB data failed");
		return (shell_status_t)err;
	}

	print_le_oob(shell, &oob_local);

	return kStatus_SHELL_Success;
}
#endif /* CONFIG_BT_PERIPHERAL */

#if (defined(CONFIG_BT_PRIVACY) && (CONFIG_BT_PRIVACY > 0))
static shell_status_t cmd_adv_rpa_expire(shell_handle_t shell, int32_t argc, char *argv[])
{
	if (!strcmp(argv[1], "on")) {
		atomic_clear_bit(adv_set_opt[selected_adv], SHELL_ADV_OPT_KEEP_RPA);
		shell_print(shell, "RPA will expire on next timeout");
	} else if (!strcmp(argv[1], "off")) {
		atomic_set_bit(adv_set_opt[selected_adv], SHELL_ADV_OPT_KEEP_RPA);
		shell_print(shell, "RPA will not expire on RPA timeout");
	} else {
		shell_error(shell, "Invalid argument: %s", argv[1]);
		return (shell_status_t)EINVAL;
	}

	return kStatus_SHELL_Success;
}
#endif
#if (defined(CONFIG_BT_PER_ADV) && (CONFIG_BT_PER_ADV > 0))
static shell_status_t cmd_per_adv(shell_handle_t shell, int32_t argc, char *argv[])
{
	struct bt_le_ext_adv *adv = adv_sets[selected_adv];

	if (!adv) {
		shell_error(shell, "No extended advertisement set selected");
		return (shell_status_t)EINVAL;
	}

	if (!strcmp(argv[1], "off")) {
		if (bt_le_per_adv_stop(adv) < 0) {
			shell_error(shell,
				    "Failed to stop periodic advertising");
		} else {
			shell_print(shell, "Periodic advertising stopped");
		}
	} else if (!strcmp(argv[1], "on")) {
		if (bt_le_per_adv_start(adv) < 0) {
			shell_error(shell,
				    "Failed to start periodic advertising");
		} else {
			shell_print(shell, "Periodic advertising started");
		}
	} else {
		shell_error(shell, "Invalid argument: %s", argv[1]);
		return (shell_status_t)EINVAL;
	}

	return kStatus_SHELL_Success;
}

static shell_status_t cmd_per_adv_param(shell_handle_t shell, int32_t argc,
			     char *argv[])
{
	struct bt_le_ext_adv *adv = adv_sets[selected_adv];
	struct bt_le_per_adv_param param;
	int err;

	if (!adv) {
		shell_error(shell, "No extended advertisement set selected");
		return (shell_status_t)EINVAL;
	}

	if (argc > 1) {
		param.interval_min = strtol(argv[1], NULL, 16);
	} else {
		param.interval_min = BT_GAP_ADV_SLOW_INT_MIN;
	}

	if (argc > 2) {
		param.interval_max = strtol(argv[2], NULL, 16);
	} else {
		param.interval_max = (uint16_t)(param.interval_min * 1.2);

	}

	if (param.interval_min > param.interval_max) {
		shell_error(shell,
			    "Min interval shall be less than max interval");
		return (shell_status_t)EINVAL;
	}

	if (argc > 3 && !strcmp(argv[3], "tx-power")) {
		param.options = BT_LE_ADV_OPT_USE_TX_POWER;
	} else {
		param.options = 0;
	}

	err = bt_le_per_adv_set_param(adv, &param);
	if (err) {
		shell_error(shell, "Failed to set periodic advertising "
			    "parameters (%d)", err);
		return kStatus_SHELL_Error;
	}

	return kStatus_SHELL_Success;
}
static ssize_t pa_ad_init(struct bt_data *data_array,
			  const size_t data_array_size)
{
	size_t ad_len = 0;

	if (IS_ENABLED(CONFIG_BT_AUDIO)) {
		ssize_t audio_pa_ad_len;

		audio_pa_ad_len = audio_pa_data_add(&data_array[ad_len],
						    data_array_size - ad_len);
#if 0
		if (audio_pa_ad_len < 0U) {
			return audio_pa_ad_len;
		}
#endif

		ad_len += audio_pa_ad_len;
	}

	return ad_len;
}

static shell_status_t cmd_per_adv_data(shell_handle_t shell, int32_t argc,
			    char *argv[])
{
	struct bt_le_ext_adv *adv = adv_sets[selected_adv];
	static uint8_t hex_data[256];
	static struct bt_data ad[2U];
	ssize_t stack_ad_len;
	uint8_t ad_len = 0;
	int err;

	if (!adv) {
		shell_error(shell, "No extended advertisement set selected");
		return (shell_status_t)EINVAL;
	}

	if (argc > 1) {
		size_t hex_len = 0U;

		(void)memset(hex_data, 0, sizeof(hex_data));
		hex_len = hex2bin(argv[1U], strlen(argv[1U]), hex_data,
				  sizeof(hex_data));

		if (hex_len == 0U) {
			shell_error(shell, "Could not parse adv data");

			return (shell_status_t)ENOEXEC;
		}

		ad[ad_len].data_len = hex_data[0U];
		ad[ad_len].type = hex_data[1U];
		ad[ad_len].data = &hex_data[2U];
		ad_len++;
	}

	stack_ad_len = pa_ad_init(&ad[ad_len], ARRAY_SIZE(ad) - ad_len);
	if (stack_ad_len < 0) {
		shell_error(shell, "Failed to get stack PA data");

		return (shell_status_t)ENOEXEC;
	}
	ad_len += stack_ad_len;
	err = bt_le_per_adv_set_data(adv, ad, ad_len);
	if (err) {
		shell_error(shell,
			    "Failed to set periodic advertising data (%d)",
			    err);
		return kStatus_SHELL_Error;
	}

	return kStatus_SHELL_Success;
}
#endif /* CONFIG_BT_PER_ADV */
#endif /* CONFIG_BT_EXT_ADV */
#endif /* CONFIG_BT_BROADCASTER */

#if (defined(CONFIG_BT_PER_ADV_SYNC) && (CONFIG_BT_PER_ADV_SYNC > 0))

static shell_status_t cmd_per_adv_sync_create(shell_handle_t shell, int32_t argc,
				   char *argv[])
{
	struct bt_le_per_adv_sync *per_adv_sync = per_adv_syncs[selected_per_adv_sync];
	int err;
	struct bt_le_per_adv_sync_param create_params = { 0 };
	uint32_t options = 0;

	if (per_adv_sync != NULL) {
		shell_error(shell, "Selected per-adv-sync is not NULL");
		return kStatus_SHELL_Error;

	}

	err = bt_addr_le_from_str(argv[1], argv[2], &create_params.addr);
	if (err) {
		shell_error(shell, "Invalid peer address (err %d)", err);
		return kStatus_SHELL_Error;
	}

	/* Default values */
	create_params.timeout = 1000; /* 10 seconds */
	create_params.skip = 10;

	create_params.sid = strtol(argv[3], NULL, 16);

	for (int j = 4; j < argc; j++) {
		if (!strcmp(argv[j], "aoa")) {
			options |= BT_LE_PER_ADV_SYNC_OPT_DONT_SYNC_AOA;
		} else if (!strcmp(argv[j], "aod_1us")) {
			options |= BT_LE_PER_ADV_SYNC_OPT_DONT_SYNC_AOD_1US;
		} else if (!strcmp(argv[j], "aod_2us")) {
			options |= BT_LE_PER_ADV_SYNC_OPT_DONT_SYNC_AOD_2US;
		} else if (!strcmp(argv[j], "only_cte")) {
			options |=
				BT_LE_PER_ADV_SYNC_OPT_SYNC_ONLY_CONST_TONE_EXT;
		} else if (!strcmp(argv[j], "timeout")) {
			if (++j == argc) {
				shell_help(shell);
				return kStatus_SHELL_PrintCmdHelp;
			}

			create_params.timeout = strtoul(argv[j], NULL, 16);
		} else if (!strcmp(argv[j], "skip")) {
			if (++j == argc) {
				shell_help(shell);
				return kStatus_SHELL_PrintCmdHelp;
			}

			create_params.skip = strtoul(argv[j], NULL, 16);
		} else {
			shell_help(shell);
			return kStatus_SHELL_PrintCmdHelp;
		}

		/* TODO: add support to parse using the per adv list */
	}

	create_params.options = options;

	err = bt_le_per_adv_sync_create(&create_params, &per_adv_syncs[selected_per_adv_sync]);
	if (err) {
		shell_error(shell, "Per adv sync failed (%d)", err);
	} else {
		shell_print(shell, "Per adv sync pending");
	}

	return kStatus_SHELL_Success;
}

static shell_status_t cmd_per_adv_sync_delete(shell_handle_t shell, int32_t argc,
				   char *argv[])
{
	struct bt_le_per_adv_sync *per_adv_sync = per_adv_syncs[selected_per_adv_sync];
	int err;

	if (!per_adv_sync) {
		shell_error(shell, "Selected per-adv-sync is NULL");
		return kStatus_SHELL_Error;
	}

	err = bt_le_per_adv_sync_delete(per_adv_sync);

	if (err) {
		shell_error(shell, "Per adv sync delete failed (%d)", err);
	} else {
		shell_print(shell, "Per adv sync deleted");
		per_adv_syncs[selected_per_adv_sync] = NULL;
	}

	return kStatus_SHELL_Success;
}
static shell_status_t cmd_per_adv_sync_select(shell_handle_t shell, int32_t argc, char *argv[])
{
	if (argc == 2) {
		unsigned long id;
		int err = 0;

		id = shell_strtoul(argv[1], 0, &err);
		if (err != 0) {
			shell_error(shell, "Could not parse id: %d", err);
			return kStatus_SHELL_Error;
		}

		if (id > ARRAY_SIZE(adv_sets)) {
			shell_error(shell, "Invalid id: %lu", id);
			return kStatus_SHELL_Error;
		}

		selected_per_adv_sync = id;
		return kStatus_SHELL_Success;
	}

	for (size_t i = 0U; i < ARRAY_SIZE(adv_sets); i++) {
		if (adv_sets[i]) {
			shell_print(shell, "PER_ADV_SYNC[%zu] %p", i, adv_sets[i]);
		}
	}

	return kStatus_SHELL_Error;
}

#if (defined(CONFIG_BT_PER_ADV_SYNC_TRANSFER_RECEIVER) && (CONFIG_BT_PER_ADV_SYNC_TRANSFER_RECEIVER > 0))
static shell_status_t cmd_past_subscribe(shell_handle_t shell, int32_t argc,
				   char *argv[])
{
	struct bt_le_per_adv_sync_transfer_param param;
	int err;
	int i = 0;
	bool global = true;

	if (i == ARRAY_SIZE(per_adv_syncs)) {
		shell_error(shell, "Cannot create more per adv syncs");
		return (shell_status_t)kStatus_SHELL_Error;
	}

	/* Default values */
	param.options = 0;
	param.timeout = 1000; /* 10 seconds */
	param.skip = 10;

	for (int j = 1; j < argc; j++) {
		if (!strcmp(argv[j], "aoa")) {
			param.options |=
				BT_LE_PER_ADV_SYNC_TRANSFER_OPT_SYNC_NO_AOA;
		} else if (!strcmp(argv[j], "aod_1us")) {
			param.options |=
				BT_LE_PER_ADV_SYNC_TRANSFER_OPT_SYNC_NO_AOD_1US;
		} else if (!strcmp(argv[j], "aod_2us")) {
			param.options |=
				BT_LE_PER_ADV_SYNC_TRANSFER_OPT_SYNC_NO_AOD_2US;
		} else if (!strcmp(argv[j], "only_cte")) {
			param.options |=
				BT_LE_PER_ADV_SYNC_TRANSFER_OPT_SYNC_ONLY_CTE;
		} else if (!strcmp(argv[j], "timeout")) {
			if (++j == argc) {
				shell_help(shell);
				return kStatus_SHELL_PrintCmdHelp;
			}

			param.timeout = strtoul(argv[j], NULL, 16);
		} else if (!strcmp(argv[j], "skip")) {
			if (++j == argc) {
				shell_help(shell);
				return kStatus_SHELL_PrintCmdHelp;
			}

			param.skip = strtoul(argv[j], NULL, 16);
		} else if (!strcmp(argv[j], "conn")) {
			if (!default_conn) {
				shell_print(shell, "Not connected");
				return (shell_status_t)EINVAL;
			}
			global = false;
		} else {
			shell_help(shell);
			return kStatus_SHELL_PrintCmdHelp;
		}
	}

	bt_le_per_adv_sync_cb_register(&per_adv_sync_cb);

	err = bt_le_per_adv_sync_transfer_subscribe(
		global ? NULL : default_conn, &param);

	if (err) {
		shell_error(shell, "PAST subscribe failed (%d)", err);
	} else {
		shell_print(shell, "Subscribed to PAST");
	}

	return kStatus_SHELL_Success;
}

static shell_status_t cmd_past_unsubscribe(shell_handle_t shell, int32_t argc,
				   char *argv[])
{
	int err;

	if (argc > 1) {
		if (!strcmp(argv[1], "conn")) {
			if (default_conn) {
				err =
					bt_le_per_adv_sync_transfer_unsubscribe(
						default_conn);
			} else {
				shell_print(shell, "Not connected");
				return (shell_status_t)EINVAL;
			}
		} else {
			shell_help(shell);
			return kStatus_SHELL_PrintCmdHelp;
		}
	} else {
		err = bt_le_per_adv_sync_transfer_unsubscribe(NULL);
	}

	if (err) {
		shell_error(shell, "PAST unsubscribe failed (%d)", err);
	}

	return (shell_status_t)err;
}
#endif /* CONFIG_BT_PER_ADV_SYNC_TRANSFER_RECEIVER */

#if (defined(CONFIG_BT_PER_ADV_SYNC_TRANSFER_SENDER) && (CONFIG_BT_PER_ADV_SYNC_TRANSFER_SENDER > 0))
static shell_status_t cmd_per_adv_sync_transfer(shell_handle_t shell, int32_t argc, char *argv[])
{
	int err;
	int index;
	struct bt_le_per_adv_sync *per_adv_sync;

	if (argc > 1) {
		index = strtol(argv[1], NULL, 10);
	} else {
		index = 0;
	}

	if (index >= ARRAY_SIZE(per_adv_syncs)) {
		shell_error(shell, "Maximum index is %zu but %d was requested",
			    ARRAY_SIZE(per_adv_syncs) - 1, index);
	}

	per_adv_sync = per_adv_syncs[index];
	if (!per_adv_sync) {
		return kStatus_SHELL_Error;
	}

	err = bt_le_per_adv_sync_transfer(per_adv_sync, default_conn, 0);
	if (err) {
		shell_error(shell, "Periodic advertising sync transfer failed (%d)", err);
	}

	return (shell_status_t)err;
}
#endif /* CONFIG_BT_PER_ADV_SYNC_TRANSFER_SENDER */
#endif /* CONFIG_BT_PER_ADV_SYNC */

#if (defined(CONFIG_BT_PER_ADV_SYNC_TRANSFER_SENDER) && (CONFIG_BT_PER_ADV_SYNC_TRANSFER_SENDER > 0)) && \
    (defined(CONFIG_BT_PER_ADV) && (CONFIG_BT_PER_ADV > 0))
static shell_status_t cmd_per_adv_set_info_transfer(shell_handle_t shell, int32_t argc,
					 char *argv[])
{
	const struct bt_le_ext_adv *adv = adv_sets[selected_adv];
	int err;

	if (default_conn == NULL) {
#if 0
		shell_error(shell, "%s: at least, one connection is required",
			    shell->ctx->active_cmd.syntax);
#else
		shell_error(shell, "At least, one connection is required");
#endif
		return kStatus_SHELL_Error;
	}

	err = bt_le_per_adv_set_info_transfer(adv, default_conn, 0U);
	if (err) {
		shell_error(shell, "Periodic advertising sync transfer failed (%d)", err);
	}

	return (shell_status_t)err;
}
#endif /* CONFIG_BT_PER_ADV_SYNC_TRANSFER_SENDER && CONFIG_BT_PER_ADV */
#if (defined(CONFIG_BT_TRANSMIT_POWER_CONTROL) && (CONFIG_BT_TRANSMIT_POWER_CONTROL > 0))
static shell_status_t cmd_read_remote_tx_power(shell_handle_t shell, int32_t argc, char *argv[])
{
	if (argc < 3) {
		int err = 0;
		enum bt_conn_le_tx_power_phy phy = (enum bt_conn_le_tx_power_phy)strtoul((const char *)argv[1], NULL, 16);

		err = bt_conn_le_get_remote_tx_power_level(default_conn, phy);

		if (!err) {
			shell_print(shell, "Read Remote TX Power for PHY %s",
				    tx_pwr_ctrl_phy2str(phy));
		} else {
			shell_print(shell, "error %d", err);
		}
	} else {
		shell_help(sh);
		return kStatus_SHELL_PrintCmdHelp;
	}
	return kStatus_SHELL_Success;
}

static shell_status_t cmd_read_local_tx_power(shell_handle_t shell, int32_t argc, char *argv[])
{
	int err = 0;

	if (argc < 3) {
		struct bt_conn_le_tx_power tx_power_level;

		tx_power_level.phy = strtoul(argv[1], NULL, 16);

		int8_t unachievable_current_level = -100;
		/* Arbitrary, these are output parameters.*/
		tx_power_level.current_level = unachievable_current_level;
		tx_power_level.max_level = 6;

		if (default_conn == NULL) {
			shell_error(shell, "Conn handle error, at least one connection is required.");
			return (shell_status_t)ENOEXEC;
		}
		err = bt_conn_le_get_tx_power_level(default_conn, &tx_power_level);
		if (err) {
			shell_print(shell, "Commad returned error error %d", err);
			return (shell_status_t)err;
		}
		if (tx_power_level.current_level == unachievable_current_level) {
			shell_print(shell, "We received no current tx power level.");
			return (shell_status_t)EIO;
		}
		shell_print(shell, "Read local TX Power: current level: %d, PHY: %s, Max Level: %d",
			    tx_power_level.current_level,
			    tx_pwr_ctrl_phy2str((enum bt_conn_le_tx_power_phy)tx_power_level.phy),
			    tx_power_level.max_level);
	} else {
		shell_help(shell);
		return kStatus_SHELL_PrintCmdHelp;
	}

	return (shell_status_t)err;
}

static shell_status_t cmd_set_power_report_enable(shell_handle_t shell, int32_t argc, char *argv[])
{
	if (argc < 4) {
		int err = 0;
		bool local_enable = 0;
		bool remote_enable = 0;

		if (*argv[1] == '1') {
			local_enable = 1;
		}
		if (*argv[2] == '1') {
			remote_enable = 1;
		}
		if (default_conn == NULL) {
			shell_error(shell, "Conn handle error, at least one connection is required.");
			return (shell_status_t)-ENOEXEC;
		}
		err = bt_conn_le_set_tx_power_report_enable(default_conn, local_enable,
							     remote_enable);
		if (!err) {
			shell_print(shell, "Tx Power Report: local: %s, remote: %s",
				    enabled2str(local_enable), enabled2str(remote_enable));
		} else {
			shell_print(shell, "error %d", err);
		}
	} else {
		shell_help(sh);
		return kStatus_SHELL_PrintCmdHelp;
	}
	return kStatus_SHELL_Success;
}

#endif
#if (defined(CONFIG_BT_CONN) && (CONFIG_BT_CONN > 0))
#if (defined(CONFIG_BT_CENTRAL) && (CONFIG_BT_CENTRAL > 0))
static shell_status_t cmd_connect_le(shell_handle_t shell, int32_t argc, char *argv[])
{
	int err;
	bt_addr_le_t addr;
	struct bt_conn *conn;
	uint32_t options = 0;

	/* When no arguments are specified, connect to the last scanned device. */
	if (argc == 1) {
		if (auto_connect.addr_set) {
			bt_addr_le_copy(&addr, &auto_connect.addr);
		} else {
			shell_error(shell, "No connectable adv stored, please trigger a scan first.");
			shell_help(shell);

			return kStatus_SHELL_PrintCmdHelp;
		}
	} else {
		err = bt_addr_le_from_str(argv[1], argv[2], &addr);
		if (err) {
			shell_error(shell, "Invalid peer address (err %d)", err);
			return (shell_status_t)err;
		}
	}

#if (defined(CONFIG_BT_EXT_ADV) && (CONFIG_BT_EXT_ADV > 0))
	for (size_t argn = 3; argn < argc; argn++) {
		const char *arg = argv[argn];

		if (!strcmp(arg, "coded")) {
			options |= BT_CONN_LE_OPT_CODED;
		} else if (!strcmp(arg, "no-1m")) {
			options |= BT_CONN_LE_OPT_NO_1M;
		} else {
			shell_help(shell);
			return kStatus_SHELL_PrintCmdHelp;
		}
	}
#endif /* defined(CONFIG_BT_EXT_ADV) */

	struct bt_conn_le_create_param *create_params =
		BT_CONN_LE_CREATE_PARAM(options,
					BT_GAP_SCAN_FAST_INTERVAL,
					BT_GAP_SCAN_FAST_INTERVAL);

	err = bt_conn_le_create(&addr, create_params, BT_LE_CONN_PARAM_DEFAULT,
				&conn);
	if (err) {
		shell_error(shell, "Connection failed (%d)", err);
		return kStatus_SHELL_Error;
	} else {

		shell_print(shell, "Connection pending");

		/* unref connection obj in advance as app user */
		bt_conn_unref(conn);
	}

	return kStatus_SHELL_Success;
}

#if !(defined(CONFIG_BT_FILTER_ACCEPT_LIST) && (CONFIG_BT_FILTER_ACCEPT_LIST > 0))
static shell_status_t cmd_auto_conn(shell_handle_t shell, int32_t argc, char *argv[])
{
	bt_addr_le_t addr;
	int err;

	err = bt_addr_le_from_str(argv[1], argv[2], &addr);
	if (err) {
		shell_error(shell, "Invalid peer address (err %d)", err);
		return (shell_status_t)err;
	}

	if (argc < 4) {
		return (shell_status_t)bt_le_set_auto_conn(&addr, BT_LE_CONN_PARAM_DEFAULT);
	} else if (!strcmp(argv[3], "on")) {
		return (shell_status_t)bt_le_set_auto_conn(&addr, BT_LE_CONN_PARAM_DEFAULT);
	} else if (!strcmp(argv[3], "off")) {
		return (shell_status_t)bt_le_set_auto_conn(&addr, NULL);
	} else {
		shell_help(shell);
		return kStatus_SHELL_PrintCmdHelp;
	}
#if 0
	return kStatus_SHELL_Success;
#endif
}
#endif /* !defined(CONFIG_BT_FILTER_ACCEPT_LIST) */

static shell_status_t cmd_connect_le_name(shell_handle_t shell, int32_t argc, char *argv[])
{
	const uint16_t timeout_seconds = 10;
	const struct bt_le_scan_param param = {
		.type       = BT_LE_SCAN_TYPE_ACTIVE,
		.options    = BT_LE_SCAN_OPT_NONE,
		.interval   = BT_GAP_SCAN_FAST_INTERVAL,
		.window     = BT_GAP_SCAN_FAST_WINDOW,
		.timeout    = timeout_seconds * 100, /* 10ms units */
	};
	int err;

	/* Set the name filter which we will use in the scan callback to
	 * automatically connect to the first device that passes the filter
	 */
	err = cmd_scan_filter_set_name(shell, argc, argv);
	if (err) {
		shell_error(shell,
			    "Bluetooth set scan filter name to %s failed (err %d)",
			    argv[1], err);
		return (shell_status_t)err;
	}

	err = bt_le_scan_start(&param, NULL);
	if (err) {
		shell_error(shell, "Bluetooth scan failed (err %d)", err);
		return (shell_status_t)err;
	}

	shell_print(shell, "Bluetooth active scan enabled");

	/* Set boolean to tell the scan callback to connect to this name */
	auto_connect.connect_name = true;

	return kStatus_SHELL_Success;
}
#endif /* CONFIG_BT_CENTRAL */

static shell_status_t cmd_disconnect(shell_handle_t shell, int32_t argc, char *argv[])
{
	struct bt_conn *conn;
	int err;

	if (default_conn && argc < 3) {
		conn = bt_conn_ref(default_conn);
	} else {
		bt_addr_le_t addr;

		if (argc < 3) {
			shell_help(shell);
			return kStatus_SHELL_PrintCmdHelp;
		}

		err = bt_addr_le_from_str(argv[1], argv[2], &addr);
		if (err) {
			shell_error(shell, "Invalid peer address (err %d)",
				    err);
			return (shell_status_t)err;
		}

		conn = bt_conn_lookup_addr_le(selected_id, &addr);
	}

	if (!conn) {
		shell_error(shell, "Not connected");
		return kStatus_SHELL_Error;
	}

	err = bt_conn_disconnect(conn, BT_HCI_ERR_REMOTE_USER_TERM_CONN);
	if (err) {
		shell_error(shell, "Disconnection failed (err %d)", err);
		return (shell_status_t)err;
	}

	bt_conn_unref(conn);

	return kStatus_SHELL_Success;
}

static shell_status_t cmd_select(shell_handle_t shell, int32_t argc, char *argv[])
{
	struct bt_conn *conn;
	bt_addr_le_t addr;
	int err;

	err = bt_addr_le_from_str(argv[1], argv[2], &addr);
	if (err) {
		shell_error(shell, "Invalid peer address (err %d)", err);
		return (shell_status_t)err;
	}

	conn = bt_conn_lookup_addr_le(selected_id, &addr);
	if (!conn) {
		shell_error(shell, "No matching connection found");
		return kStatus_SHELL_Error;
	}

	if (default_conn) {
		bt_conn_unref(default_conn);
	}

	default_conn = conn;

    s_shellBtPrompt = current_prompt();
    SHELL_ChangePrompt(shell, (char *)s_shellBtPrompt);

	return kStatus_SHELL_Success;
}

static const char *get_conn_type_str(uint8_t type)
{
	switch (type) {
	case BT_CONN_TYPE_LE: return "LE";
	case BT_CONN_TYPE_BR: return "BR/EDR";
	case BT_CONN_TYPE_SCO: return "SCO";
	default: return "Invalid";
	}
}

static const char *get_conn_role_str(uint8_t role)
{
	switch (role) {
	case BT_CONN_ROLE_CENTRAL: return "central";
	case BT_CONN_ROLE_PERIPHERAL: return "peripheral";
	default: return "Invalid";
	}
}

static shell_status_t cmd_info(shell_handle_t shell, int32_t argc, char *argv[])
{
	struct bt_conn *conn = NULL;
	struct bt_conn_info info;
	bt_addr_le_t addr;
	int err;

    memset(&addr, 0, sizeof(addr));
	switch (argc) {
	case 1:
		if (default_conn) {
			conn = bt_conn_ref(default_conn);
		}
		break;
	case 2:
		addr.type = BT_ADDR_LE_PUBLIC;
		err = bt_addr_from_str(argv[1], &addr.a);
		if (err) {
			shell_error(shell, "Invalid peer address (err %d)",
				    err);
			return (shell_status_t)err;
		}
		conn = bt_conn_lookup_addr_le(selected_id, &addr);
		break;
	case 3:
		err = bt_addr_le_from_str(argv[1], argv[2], &addr);

		if (err) {
			shell_error(shell, "Invalid peer address (err %d)",
				    err);
			return (shell_status_t)err;
		}
		conn = bt_conn_lookup_addr_le(selected_id, &addr);
		break;
	}

	if (!conn) {
		shell_error(shell, "Not connected");
		return kStatus_SHELL_Error;
	}

	err = bt_conn_get_info(conn, &info);
	if (err) {
		shell_print(ctx_shell, "Failed to get info");
		goto done;
	}

	shell_print(ctx_shell, "Type: %s, Role: %s, Id: %u",
		    get_conn_type_str(info.type),
		    get_conn_role_str(info.role),
		    info.id);

	if (info.type == BT_CONN_TYPE_LE) {
		print_le_addr("Remote", info.le.dst);
		print_le_addr("Local", info.le.src);
		print_le_addr("Remote on-air", info.le.remote);
		print_le_addr("Local on-air", info.le.local);

		shell_print(ctx_shell, "Interval: 0x%04x (%u us)",
			    info.le.interval,
			    BT_CONN_INTERVAL_TO_US(info.le.interval));
		shell_print(ctx_shell, "Latency: 0x%04x",
			    info.le.latency);
		shell_print(ctx_shell, "Supervision timeout: 0x%04x (%d ms)",
			    info.le.timeout, info.le.timeout * 10);
#if (defined(CONFIG_BT_USER_PHY_UPDATE) && (CONFIG_BT_USER_PHY_UPDATE > 0))
		shell_print(ctx_shell, "LE PHY: TX PHY %s, RX PHY %s",
			    phy2str(info.le.phy->tx_phy),
			    phy2str(info.le.phy->rx_phy));
#endif
#if (defined(CONFIG_BT_USER_DATA_LEN_UPDATE) && (CONFIG_BT_USER_DATA_LEN_UPDATE > 0))
		shell_print(ctx_shell, "LE data len: TX (len: %d time: %d)"
			    " RX (len: %d time: %d)",
			    info.le.data_len->tx_max_len,
			    info.le.data_len->tx_max_time,
			    info.le.data_len->rx_max_len,
			    info.le.data_len->rx_max_time);
#endif
	}

#if (defined(CONFIG_BT_BREDR) && (CONFIG_BT_BREDR > 0))
	if (info.type == BT_CONN_TYPE_BR) {
		char addr_str[BT_ADDR_STR_LEN];

		bt_addr_to_str(info.br.dst, addr_str, sizeof(addr_str));
		shell_print(ctx_shell, "Peer address %s", addr_str);
	}
#endif /* defined(CONFIG_BT_BREDR) */

done:
	bt_conn_unref(conn);

	return (shell_status_t)err;
}

static shell_status_t cmd_conn_update(shell_handle_t shell, int32_t argc, char *argv[])
{
	struct bt_le_conn_param param;
	int err;

	if (default_conn == NULL) {
		shell_error(shell, "%s: at least, one connection is required");
		return kStatus_SHELL_Error;
	}

	param.interval_min = strtoul(argv[1], NULL, 16);
	param.interval_max = strtoul(argv[2], NULL, 16);
	param.latency = strtoul(argv[3], NULL, 16);
	param.timeout = strtoul(argv[4], NULL, 16);

	err = bt_conn_le_param_update(default_conn, &param);
	if (err) {
		shell_error(shell, "conn update failed (err %d).", err);
	} else {
		shell_print(shell, "conn update initiated.");
	}

	return (shell_status_t)err;
}

#if (defined(CONFIG_BT_USER_DATA_LEN_UPDATE) && (CONFIG_BT_USER_DATA_LEN_UPDATE > 0))
static uint16_t tx_time_calc(uint8_t phy, uint16_t max_len)
{
	/* Access address + header + payload + MIC + CRC */
	uint16_t total_len = 4 + 2 + max_len + 4 + 3;

	switch (phy) {
	case BT_GAP_LE_PHY_1M:
		/* 1 byte preamble, 8 us per byte */
		return 8 * (1 + total_len);
	case BT_GAP_LE_PHY_2M:
		/* 2 byte preamble, 4 us per byte */
		return 4 * (2 + total_len);
	case BT_GAP_LE_PHY_CODED:
		/* S8: Preamble + CI + TERM1 + 64 us per byte + TERM2 */
		return 80 + 16 + 24 + 64 * (total_len) + 24;
	default:
		return kStatus_SHELL_Success;
	}
}

static shell_status_t cmd_conn_data_len_update(shell_handle_t shell, int32_t argc,
				    char *argv[])
{
	struct bt_conn_le_data_len_param param;
	int err;

	if (default_conn == NULL) {
		shell_error(shell, "%s: at least, one connection is required");
		return kStatus_SHELL_Error;
	}

	param.tx_max_len = strtoul(argv[1], NULL, 10);

	if (argc > 2) {
		param.tx_max_time = strtoul(argv[2], NULL, 10);
	} else {
		/* Assume 1M if not able to retrieve PHY */
		uint8_t phy = BT_GAP_LE_PHY_1M;

#if (defined(CONFIG_BT_USER_PHY_UPDATE) && (CONFIG_BT_USER_PHY_UPDATE > 0))
		struct bt_conn_info info;

		err = bt_conn_get_info(default_conn, &info);
		if (!err) {
			phy = info.le.phy->tx_phy;
		}
#endif
		param.tx_max_time = tx_time_calc(phy, param.tx_max_len);
		shell_print(shell, "Calculated tx time: %d", param.tx_max_time);
	}



	err = bt_conn_le_data_len_update(default_conn, &param);
	if (err) {
		shell_error(shell, "data len update failed (err %d).", err);
	} else {
		shell_print(shell, "data len update initiated.");
	}

	return (shell_status_t)err;
}
#endif

#if (defined(CONFIG_BT_USER_PHY_UPDATE) && (CONFIG_BT_USER_PHY_UPDATE > 0))
static shell_status_t cmd_conn_phy_update(shell_handle_t shell, int32_t argc,
			       char *argv[])
{
	struct bt_conn_le_phy_param param;
	int err;

	if (default_conn == NULL) {
		shell_error(shell, "at least, one connection is required");
		return kStatus_SHELL_Error;
	}

	param.pref_tx_phy = strtoul(argv[1], NULL, 16);
	param.pref_rx_phy = param.pref_tx_phy;
	param.options = BT_CONN_LE_PHY_OPT_NONE;

	for (size_t argn = 2; argn < argc; argn++) {
		const char *arg = argv[argn];

		if (!strcmp(arg, "s2")) {
			param.options |= BT_CONN_LE_PHY_OPT_CODED_S2;
		} else if (!strcmp(arg, "s8")) {
			param.options |= BT_CONN_LE_PHY_OPT_CODED_S8;
		} else {
			param.pref_rx_phy = strtoul(arg, NULL, 16);
		}
	}

	err = bt_conn_le_phy_update(default_conn, &param);
	if (err) {
		shell_error(shell, "PHY update failed (err %d).", err);
	} else {
		shell_print(shell, "PHY update initiated.");
	}

	return (shell_status_t)err;
}
#endif

#if (defined(CONFIG_BT_CENTRAL) && (CONFIG_BT_CENTRAL > 0))
static shell_status_t cmd_chan_map(shell_handle_t shell, int32_t argc, char *argv[])
{
	uint8_t chan_map[5] = {0};
	int err;

	if (hex2bin(argv[1], strlen(argv[1]), chan_map, 5) == 0) {
		shell_error(shell, "Invalid channel map");
		return kStatus_SHELL_Error;
	}
	sys_mem_swap(chan_map, 5);

	err = bt_le_set_chan_map(chan_map);
	if (err) {
		shell_error(shell, "Failed to set channel map (err %d)", err);
	} else {
		shell_print(shell, "Channel map set");
	}

	return (shell_status_t)err;
}
#endif /* CONFIG_BT_CENTRAL */

static shell_status_t cmd_oob(shell_handle_t shell, int32_t argc, char *argv[])
{
	int err;

	err = bt_le_oob_get_local(selected_id, &oob_local);
	if (err) {
		shell_error(shell, "OOB data failed");
		return (shell_status_t)err;
	}

	print_le_oob(shell, &oob_local);

	return kStatus_SHELL_Success;
}

#if (defined(CONFIG_BT_SMP) && (CONFIG_BT_SMP > 0)) || ((defined(CONFIG_BT_BREDR) && (CONFIG_BT_BREDR > 0)))
static shell_status_t cmd_oob_remote(shell_handle_t shell, int32_t argc,
			     char *argv[])
{
	int err;
	bt_addr_le_t addr;

	err = bt_addr_le_from_str(argv[1], argv[2], &addr);
	if (err) {
		shell_error(shell, "Invalid peer address (err %d)", err);
		return (shell_status_t)err;
	}

	bt_addr_le_copy(&oob_remote.addr, &addr);

	if (argc == 5) {
		hex2bin(argv[3], strlen(argv[3]), oob_remote.le_sc_data.r,
			sizeof(oob_remote.le_sc_data.r));
		hex2bin(argv[4], strlen(argv[4]), oob_remote.le_sc_data.c,
			sizeof(oob_remote.le_sc_data.c));
		bt_le_oob_set_sc_flag(true);
	} else {
		shell_help(shell);
		return kStatus_SHELL_Error;
	}

	return kStatus_SHELL_Success;
}

static shell_status_t cmd_oob_clear(shell_handle_t shell, int32_t argc, char *argv[])
{
	memset(&oob_remote, 0, sizeof(oob_remote));
	bt_le_oob_set_sc_flag(false);

	return kStatus_SHELL_Success;
}
#endif /* CONFIG_BT_SMP || CONFIG_BT_BREDR) */

static shell_status_t cmd_clear(shell_handle_t shell, int32_t argc, char *argv[])
{
	bt_addr_le_t addr;
	int err;

	if (strcmp(argv[1], "all") == 0) {
		err = bt_unpair(selected_id, NULL);
		if (err) {
			shell_error(shell, "Failed to clear pairings (err %d)",
			      err);
			return (shell_status_t)err;
		} else {
			shell_print(shell, "Pairings successfully cleared");
		}

		return kStatus_SHELL_Success;
	}

    memset(&addr, 0, sizeof(addr));
	if (argc < 3) {
#if (defined(CONFIG_BT_BREDR) && (CONFIG_BT_BREDR > 0))
		addr.type = BT_ADDR_LE_PUBLIC;
		err = bt_addr_from_str(argv[1], &addr.a);
#else
		shell_print(shell, "Both address and address type needed");
		return kStatus_SHELL_Error;
#endif
	} else {
		err = bt_addr_le_from_str(argv[1], argv[2], &addr);
	}

	if (err) {
		shell_print(shell, "Invalid address");
		return (shell_status_t)err;
	}

	err = bt_unpair(selected_id, &addr);
	if (err) {
		shell_error(shell, "Failed to clear pairing (err %d)", err);
	} else {
		shell_print(shell, "Pairing successfully cleared");
	}

	return (shell_status_t)err;
}
#endif /* CONFIG_BT_CONN */

#if (defined(CONFIG_BT_SMP) && (CONFIG_BT_SMP > 0)) || ((defined(CONFIG_BT_BREDR) && (CONFIG_BT_BREDR > 0)))
static shell_status_t cmd_security(shell_handle_t shell, int32_t argc, char *argv[])
{
	int err, sec;
	struct bt_conn_info info;

	if (!default_conn || (bt_conn_get_info(default_conn, &info) < 0)) {
		shell_error(shell, "Not connected");
		return kStatus_SHELL_Error;
	}

	if (argc < 2) {
		shell_print(shell, "BT_SECURITY_L%d", bt_conn_get_security(default_conn));

		return kStatus_SHELL_Success;
	}

	sec = *argv[1] - '0';

	if ((info.type == BT_CONN_TYPE_BR &&
	    (sec < BT_SECURITY_L0 || sec > BT_SECURITY_L3))) {
		shell_error(shell, "Invalid BR/EDR security level (%d)", sec);
		return kStatus_SHELL_Error;
	}

	if ((info.type == BT_CONN_TYPE_LE &&
	    (sec < BT_SECURITY_L1 || sec > BT_SECURITY_L4))) {
		shell_error(shell, "Invalid LE security level (%d)", sec);
		return kStatus_SHELL_Error;
	}

	if (argc > 2) {
		if (!strcmp(argv[2], "force-pair")) {
			sec |= BT_SECURITY_FORCE_PAIR;
		} else {
			shell_help(shell);
			return kStatus_SHELL_Error;
		}
	}

	err = bt_conn_set_security(default_conn, (bt_security_t)sec);
	if (err) {
		shell_error(shell, "Setting security failed (err %d)", err);
	}

	return (shell_status_t)err;
}

static shell_status_t cmd_bondable(shell_handle_t shell, int32_t argc, char *argv[])
{
	const char *bondable;

	bondable = argv[1];
	if (!strcmp(bondable, "on")) {
		bt_set_bondable(true);
	} else if (!strcmp(bondable, "off")) {
		bt_set_bondable(false);
	} else {
		shell_help(shell);
		return kStatus_SHELL_PrintCmdHelp;
	}

	return kStatus_SHELL_Success;
}

static void bond_info(const struct bt_bond_info *info, void *user_data)
{
	char addr[BT_ADDR_LE_STR_LEN];
	int *bond_count = (int *)user_data;

	bt_addr_le_to_str(&info->addr, addr, sizeof(addr));
	shell_print(ctx_shell, "Remote Identity: %s", addr);
	(*bond_count)++;
}

static shell_status_t cmd_bonds(shell_handle_t shell, int32_t argc, char *argv[])
{
	int bond_count = 0;

	shell_print(shell, "Bonded devices:");
	bt_foreach_bond(selected_id, bond_info, &bond_count);
	shell_print(shell, "Total %d", bond_count);

	return kStatus_SHELL_Success;
}

static const char *role_str(uint8_t role)
{
	switch (role) {
	case BT_CONN_ROLE_CENTRAL:
		return "Central";
	case BT_CONN_ROLE_PERIPHERAL:
		return "Peripheral";
	}

	return "Unknown";
}

static void connection_info(struct bt_conn *conn, void *user_data)
{
	char addr[BT_ADDR_LE_STR_LEN];
	int *conn_count = user_data;
	struct bt_conn_info info;

	if (bt_conn_get_info(conn, &info) < 0) {
		shell_error(ctx_shell, "Unable to get info: conn %p", conn);
		return;
	}

	switch (info.type) {
#if (defined(CONFIG_BT_BREDR) && (CONFIG_BT_BREDR> 0))
	case BT_CONN_TYPE_BR:
		bt_addr_to_str(info.br.dst, addr, sizeof(addr));
		shell_print(ctx_shell, "#%u [BR][%s] %s", info.id,
			    role_str(info.role), addr);
		break;
#endif
	case BT_CONN_TYPE_LE:
		bt_addr_le_to_str(info.le.dst, addr, sizeof(addr));
		shell_print(ctx_shell, "#%u [LE][%s] %s: Interval %u latency %u"
			    " timeout %u", info.id, role_str(info.role), addr,
			    info.le.interval, info.le.latency, info.le.timeout);
		break;
#if (defined(CONFIG_BT_ISO) && (CONFIG_BT_ISO> 0))
	case BT_CONN_TYPE_ISO:
		bt_addr_le_to_str(info.le.dst, addr, sizeof(addr));
		shell_print(ctx_shell, "#%u [ISO][%s] %s", info.id,
			    role_str(info.role), addr);
		break;
#endif
	default:
		break;
	}

	(*conn_count)++;
}

static shell_status_t cmd_connections(shell_handle_t shell, int32_t argc, char *argv[])
{
	int conn_count = 0;

	shell_print(shell, "Connected devices:");
	bt_conn_foreach(BT_CONN_TYPE_ALL, connection_info, &conn_count);
	shell_print(shell, "Total %d", conn_count);

	return kStatus_SHELL_Success;
}

static void auth_passkey_display(struct bt_conn *conn, unsigned int passkey)
{
	char addr[BT_ADDR_LE_STR_LEN];
	char passkey_str[7];

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	snprintk(passkey_str, 7, "%06u", passkey);

	shell_print(ctx_shell, "Passkey for %s: %s", addr, passkey_str);
}

#if (defined(CONFIG_BT_PASSKEY_KEYPRESS) && (CONFIG_BT_PASSKEY_KEYPRESS > 0))
static void auth_passkey_display_keypress(struct bt_conn *conn,
					  enum bt_conn_auth_keypress type)
{
	char addr[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	shell_print(ctx_shell, "Passkey keypress notification from %s: type %d",
		    addr, type);
}
#endif
static void auth_passkey_confirm(struct bt_conn *conn, unsigned int passkey)
{
	char addr[BT_ADDR_LE_STR_LEN];
	char passkey_str[7];

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	snprintk(passkey_str, 7, "%06u", passkey);

	shell_print(ctx_shell, "Confirm passkey for %s: %s", addr, passkey_str);
}

static void auth_passkey_entry(struct bt_conn *conn)
{
	char addr[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	shell_print(ctx_shell, "Enter passkey for %s", addr);
}

static void auth_cancel(struct bt_conn *conn)
{
	char addr[BT_ADDR_LE_STR_LEN];

	conn_addr_str(conn, addr, sizeof(addr));

	shell_print(ctx_shell, "Pairing cancelled: %s", addr);

	/* clear connection reference for sec mode 3 pairing */
	if (pairing_conn) {
		bt_conn_unref(pairing_conn);
		pairing_conn = NULL;
	}
}

static void auth_pairing_confirm(struct bt_conn *conn)
{
	char addr[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	shell_print(ctx_shell, "Confirm pairing for %s", addr);
}

#if !(defined(CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY) && (CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY > 0))
static const char *oob_config_str(int oob_config)
{
	switch (oob_config) {
	case BT_CONN_OOB_LOCAL_ONLY:
		return "Local";
	case BT_CONN_OOB_REMOTE_ONLY:
		return "Remote";
	case BT_CONN_OOB_BOTH_PEERS:
		return "Local and Remote";
	case BT_CONN_OOB_NO_DATA:
	default:
		return "no";
	}
}
#endif /* !defined(CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY) */

static void auth_pairing_oob_data_request(struct bt_conn *conn,
					  struct bt_conn_oob_info *oob_info)
{
	char addr[BT_ADDR_LE_STR_LEN];
	struct bt_conn_info info;
	int err;

	err = bt_conn_get_info(conn, &info);
	if (err) {
		return;
	}

#if !(defined(CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY) && (CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY > 0))
	if (oob_info->type == BT_CONN_OOB_LE_SC) {
		struct bt_le_oob_sc_data *oobd_local =
			oob_info->lesc.oob_config != BT_CONN_OOB_REMOTE_ONLY
						  ? &oob_local.le_sc_data
						  : NULL;
		struct bt_le_oob_sc_data *oobd_remote =
			oob_info->lesc.oob_config != BT_CONN_OOB_LOCAL_ONLY
						  ? &oob_remote.le_sc_data
						  : NULL;

		if (oobd_remote &&
		    !bt_addr_le_eq(info.le.remote, &oob_remote.addr)) {
			bt_addr_le_to_str(info.le.remote, addr, sizeof(addr));
			shell_print(ctx_shell,
				    "No OOB data available for remote %s",
				    addr);
			bt_conn_auth_cancel(conn);
			return;
		}

		if (oobd_local &&
		    !bt_addr_le_eq(info.le.local, &oob_local.addr)) {
			bt_addr_le_to_str(info.le.local, addr, sizeof(addr));
			shell_print(ctx_shell,
				    "No OOB data available for local %s",
				    addr);
			bt_conn_auth_cancel(conn);
			return;
		}

		bt_le_oob_set_sc_data(conn, oobd_local, oobd_remote);

		bt_addr_le_to_str(info.le.dst, addr, sizeof(addr));
		shell_print(ctx_shell, "Set %s OOB SC data for %s, ",
			    oob_config_str(oob_info->lesc.oob_config), addr);
		return;
	}
#endif /* CONFIG_BT_SMP_OOB_LEGACY_PAIR_ONLY */

	bt_addr_le_to_str(info.le.dst, addr, sizeof(addr));
	shell_print(ctx_shell, "Legacy OOB TK requested from remote %s", addr);
}

static void auth_pairing_complete(struct bt_conn *conn, bool bonded)
{
	char addr[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	shell_print(ctx_shell, "%s with %s", bonded ? "Bonded" : "Paired",
		    addr);
}

static void auth_pairing_failed(struct bt_conn *conn,
				enum bt_security_err reason)
{
	char addr[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	shell_print(ctx_shell, "Pairing failed with %s reason %d", addr,
		    reason);
}

#if (defined(CONFIG_BT_BREDR) && (CONFIG_BT_BREDR > 0))
static void auth_pincode_entry(struct bt_conn *conn, bool highsec)
{
	char addr[BT_ADDR_STR_LEN];
	struct bt_conn_info info;

	if (bt_conn_get_info(conn, &info) < 0) {
		return;
	}

	if (info.type != BT_CONN_TYPE_BR) {
		return;
	}

	bt_addr_to_str(info.br.dst, addr, sizeof(addr));

	if (highsec) {
		shell_print(ctx_shell, "Enter 16 digits wide PIN code for %s",
			    addr);
	} else {
		shell_print(ctx_shell, "Enter PIN code for %s", addr);
	}

	/*
	 * Save connection info since in security mode 3 (link level enforced
	 * security) PIN request callback is called before connected callback
	 */
	if (!default_conn && !pairing_conn) {
		pairing_conn = bt_conn_ref(conn);
	}
}
#endif

#if (defined(CONFIG_BT_SMP_APP_PAIRING_ACCEPT) && (CONFIG_BT_SMP_APP_PAIRING_ACCEPT > 0))
enum bt_security_err pairing_accept(
	struct bt_conn *conn, const struct bt_conn_pairing_feat *const feat)
{
	shell_print(ctx_shell, "Remote pairing features: "
			       "IO: 0x%02x, OOB: %d, AUTH: 0x%02x, Key: %d, "
			       "Init Kdist: 0x%02x, Resp Kdist: 0x%02x",
			       feat->io_capability, feat->oob_data_flag,
			       feat->auth_req, feat->max_enc_key_size,
			       feat->init_key_dist, feat->resp_key_dist);

	return BT_SECURITY_ERR_SUCCESS;
}
#endif /* CONFIG_BT_SMP_APP_PAIRING_ACCEPT */

void bond_deleted(uint8_t id, const bt_addr_le_t *peer)
{
	char addr[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(peer, addr, sizeof(addr));
	shell_print(ctx_shell, "Bond deleted for %s, id %u", addr, id);
}

static struct bt_conn_auth_cb auth_cb_display = {
	.passkey_display = auth_passkey_display,
#if (defined(CONFIG_BT_PASSKEY_KEYPRESS) && (CONFIG_BT_PASSKEY_KEYPRESS > 0))
	.passkey_display_keypress = auth_passkey_display_keypress,
#endif
	.passkey_entry = NULL,
	.passkey_confirm = NULL,
#if (defined(CONFIG_BT_BREDR) && (CONFIG_BT_BREDR > 0))
	.pincode_entry = auth_pincode_entry,
#endif
	.oob_data_request = NULL,
	.cancel = auth_cancel,
	.pairing_confirm = auth_pairing_confirm,
#if (defined(CONFIG_BT_SMP_APP_PAIRING_ACCEPT) && (CONFIG_BT_SMP_APP_PAIRING_ACCEPT > 0))
	.pairing_accept = pairing_accept,
#endif
};

static struct bt_conn_auth_cb auth_cb_display_yes_no = {
	.passkey_display = auth_passkey_display,
	.passkey_entry = NULL,
	.passkey_confirm = auth_passkey_confirm,
#if (defined(CONFIG_BT_BREDR) && (CONFIG_BT_BREDR > 0))
	.pincode_entry = auth_pincode_entry,
#endif
	.oob_data_request = NULL,
	.cancel = auth_cancel,
	.pairing_confirm = auth_pairing_confirm,
#if (defined(CONFIG_BT_SMP_APP_PAIRING_ACCEPT) && (CONFIG_BT_SMP_APP_PAIRING_ACCEPT > 0))
	.pairing_accept = pairing_accept,
#endif
};

static struct bt_conn_auth_cb auth_cb_input = {
	.passkey_display = NULL,
	.passkey_entry = auth_passkey_entry,
	.passkey_confirm = NULL,
#if (defined(CONFIG_BT_BREDR) && (CONFIG_BT_BREDR > 0))
	.pincode_entry = auth_pincode_entry,
#endif
	.oob_data_request = NULL,
	.cancel = auth_cancel,
	.pairing_confirm = auth_pairing_confirm,
#if (defined(CONFIG_BT_SMP_APP_PAIRING_ACCEPT) && (CONFIG_BT_SMP_APP_PAIRING_ACCEPT > 0))
	.pairing_accept = pairing_accept,
#endif
};

static struct bt_conn_auth_cb auth_cb_confirm = {
#if (defined(CONFIG_BT_BREDR) && (CONFIG_BT_BREDR > 0))
	.pincode_entry = auth_pincode_entry,
#endif
	.oob_data_request = NULL,
	.cancel = auth_cancel,
	.pairing_confirm = auth_pairing_confirm,
#if (defined(CONFIG_BT_SMP_APP_PAIRING_ACCEPT) && (CONFIG_BT_SMP_APP_PAIRING_ACCEPT > 0))
	.pairing_accept = pairing_accept,
#endif
};

static struct bt_conn_auth_cb auth_cb_all = {
	.passkey_display = auth_passkey_display,
	.passkey_entry = auth_passkey_entry,
	.passkey_confirm = auth_passkey_confirm,
#if (defined(CONFIG_BT_BREDR) && (CONFIG_BT_BREDR > 0))
	.pincode_entry = auth_pincode_entry,
#endif
	.oob_data_request = auth_pairing_oob_data_request,
	.cancel = auth_cancel,
	.pairing_confirm = auth_pairing_confirm,
#if (defined(CONFIG_BT_SMP_APP_PAIRING_ACCEPT) && (CONFIG_BT_SMP_APP_PAIRING_ACCEPT > 0))
	.pairing_accept = pairing_accept,
#endif
};

static struct bt_conn_auth_cb auth_cb_oob = {
	.passkey_display = NULL,
	.passkey_entry = NULL,
	.passkey_confirm = NULL,
#if (defined(CONFIG_BT_BREDR) && (CONFIG_BT_BREDR > 0))
	.pincode_entry = NULL,
#endif
	.oob_data_request = auth_pairing_oob_data_request,
	.cancel = auth_cancel,
	.pairing_confirm = NULL,
#if (defined(CONFIG_BT_SMP_APP_PAIRING_ACCEPT) && (CONFIG_BT_SMP_APP_PAIRING_ACCEPT > 0))
	.pairing_accept = pairing_accept,
#endif
};

#if (defined(CONFIG_BT_SMP_APP_PAIRING_ACCEPT) && (CONFIG_BT_SMP_APP_PAIRING_ACCEPT > 0))
static struct bt_conn_auth_cb auth_cb_status = {
	.pairing_accept = pairing_accept,
};
#endif

static struct bt_conn_auth_info_cb auth_info_cb = {
	.pairing_failed = auth_pairing_failed,
	.pairing_complete = auth_pairing_complete,
	.bond_deleted = bond_deleted,
};

static shell_status_t cmd_auth(shell_handle_t shell, int32_t argc, char *argv[])
{
	int err = kStatus_SHELL_Error;

	if (!strcmp(argv[1], "all")) {
		err = bt_conn_auth_cb_register(&auth_cb_all);
	} else if (!strcmp(argv[1], "input")) {
		err = bt_conn_auth_cb_register(&auth_cb_input);
	} else if (!strcmp(argv[1], "display")) {
		err = bt_conn_auth_cb_register(&auth_cb_display);
	} else if (!strcmp(argv[1], "yesno")) {
		err = bt_conn_auth_cb_register(&auth_cb_display_yes_no);
	} else if (!strcmp(argv[1], "confirm")) {
		err = bt_conn_auth_cb_register(&auth_cb_confirm);
	} else if (!strcmp(argv[1], "oob")) {
		err = bt_conn_auth_cb_register(&auth_cb_oob);
	} else if (!strcmp(argv[1], "status")) {
#if (defined(CONFIG_BT_SMP_APP_PAIRING_ACCEPT) && (CONFIG_BT_SMP_APP_PAIRING_ACCEPT > 0))
		err = bt_conn_auth_cb_register(&auth_cb_status);
#endif
	} else if (!strcmp(argv[1], "none")) {
		err = bt_conn_auth_cb_register(NULL);
	} else {
		shell_help(shell);
		return kStatus_SHELL_PrintCmdHelp;
	}

	if (err) {
		shell_error(shell, "Failed to set auth handlers (%d)", err);
	}

	return (shell_status_t)err;
}

static shell_status_t cmd_auth_cancel(shell_handle_t shell,
			   int32_t argc, char *argv[])
{
	struct bt_conn *conn;

	if (default_conn) {
		conn = default_conn;
	} else if (pairing_conn) {
		conn = pairing_conn;
	} else {
		conn = NULL;
	}

	if (!conn) {
		shell_print(shell, "Not connected");
		return kStatus_SHELL_Error;
	}

	bt_conn_auth_cancel(conn);

	return kStatus_SHELL_Success;
}

static shell_status_t cmd_auth_passkey_confirm(shell_handle_t shell,
				    int32_t argc, char *argv[])
{
	if (!default_conn) {
		shell_print(shell, "Not connected");
		return kStatus_SHELL_Error;
	}

	bt_conn_auth_passkey_confirm(default_conn);
	return kStatus_SHELL_Success;
}

static shell_status_t cmd_auth_pairing_confirm(shell_handle_t shell,
				    int32_t argc, char *argv[])
{
	if (!default_conn) {
		shell_print(shell, "Not connected");
		return kStatus_SHELL_Error;
	}

	bt_conn_auth_pairing_confirm(default_conn);
	return kStatus_SHELL_Success;
}

#if (defined(CONFIG_BT_FILTER_ACCEPT_LIST) && (CONFIG_BT_FILTER_ACCEPT_LIST > 0))
static shell_status_t cmd_fal_add(shell_handle_t shell, int32_t argc, char *argv[])
{
	bt_addr_le_t addr;
	int err;

	err = bt_addr_le_from_str(argv[1], argv[2], &addr);
	if (err) {
		shell_error(shell, "Invalid peer address (err %d)", err);
		return (shell_status_t)err;
	}

	err = bt_le_filter_accept_list_add(&addr);
	if (err) {
		shell_error(shell, "Add to fa list failed (err %d)", err);
		return (shell_status_t)err;
	}

	return kStatus_SHELL_Success;
}

static shell_status_t cmd_fal_rem(shell_handle_t shell, int32_t argc, char *argv[])
{
	bt_addr_le_t addr;
	int err;

	err = bt_addr_le_from_str(argv[1], argv[2], &addr);
	if (err) {
		shell_error(shell, "Invalid peer address (err %d)", err);
		return (shell_status_t)err;
	}

	err = bt_le_filter_accept_list_remove(&addr);
	if (err) {
		shell_error(shell, "Remove from fa list failed (err %d)",
			    err);
		return (shell_status_t)err;
	}
	return kStatus_SHELL_Success;
}

static shell_status_t cmd_fal_clear(shell_handle_t shell, int32_t argc, char *argv[])
{
	int err;

	err = bt_le_filter_accept_list_clear();
	if (err) {
		shell_error(shell, "Clearing fa list failed (err %d)", err);
		return (shell_status_t)err;
	}

	return kStatus_SHELL_Success;
}

#if (defined(CONFIG_BT_CENTRAL) && (CONFIG_BT_CENTRAL > 0))
static shell_status_t cmd_fal_connect(shell_handle_t shell, int32_t argc, char *argv[])
{
	int err;
	const char *action = argv[1];
	uint32_t options = 0;

#if (defined(CONFIG_BT_EXT_ADV) && (CONFIG_BT_EXT_ADV > 0))
	for (size_t argn = 2; argn < argc; argn++) {
		const char *arg = argv[argn];

		if (!strcmp(arg, "coded")) {
			options |= BT_CONN_LE_OPT_CODED;
		} else if (!strcmp(arg, "no-1m")) {
			options |= BT_CONN_LE_OPT_NO_1M;
		} else {
			shell_help(shell);
			return kStatus_SHELL_PrintCmdHelp;
		}
	}
#endif /* defined(CONFIG_BT_EXT_ADV) */
	struct bt_conn_le_create_param *create_params =
		BT_CONN_LE_CREATE_PARAM(options,
					BT_GAP_SCAN_FAST_INTERVAL,
					BT_GAP_SCAN_FAST_WINDOW);

	if (!strcmp(action, "on")) {
		err = bt_conn_le_create_auto(create_params,
					     BT_LE_CONN_PARAM_DEFAULT);
		if (err) {
			shell_error(shell, "Auto connect failed (err %d)", err);
			return (shell_status_t)err;
		}
	} else if (!strcmp(action, "off")) {
		err = bt_conn_create_auto_stop();
		if (err) {
			shell_error(shell, "Auto connect stop failed (err %d)",
				    err);
		}
		return (shell_status_t)err;
	}

	return kStatus_SHELL_Success;
}
#endif /* CONFIG_BT_CENTRAL */
#endif /* defined(CONFIG_BT_FILTER_ACCEPT_LIST) */

#if (defined(CONFIG_BT_FIXED_PASSKEY) && (CONFIG_BT_FIXED_PASSKEY > 0))
static shell_status_t cmd_fixed_passkey(shell_handle_t shell,
			     int32_t argc, char *argv[])
{
	unsigned int passkey;
	int err;

	if (argc < 2) {
		bt_passkey_set(BT_PASSKEY_INVALID);
		shell_print(shell, "Fixed passkey cleared");
		return kStatus_SHELL_Success;
	}

	passkey = atoi(argv[1]);
	if (passkey > 999999) {
		shell_print(shell, "Passkey should be between 0-999999");
		return kStatus_SHELL_Error;
	}

	err = bt_passkey_set(passkey);
	if (err) {
		shell_print(shell, "Setting fixed passkey failed (err %d)",
			    err);
	}

	return (shell_status_t)err;
}
#endif

static shell_status_t cmd_auth_passkey(shell_handle_t shell,
			    int32_t argc, char *argv[])
{
	unsigned int passkey;
	int err;

	if (!default_conn) {
		shell_print(shell, "Not connected");
		return kStatus_SHELL_Error;
	}

	passkey = atoi(argv[1]);
	if (passkey > 999999) {
		shell_print(shell, "Passkey should be between 0-999999");
		return (shell_status_t)EINVAL;
	}

	err = bt_conn_auth_passkey_entry(default_conn, passkey);
	if (err) {
		shell_error(shell, "Failed to set passkey (%d)", err);
		return (shell_status_t)err;
	}

	return kStatus_SHELL_Success;
}

#if (defined(CONFIG_BT_PASSKEY_KEYPRESS) && (CONFIG_BT_PASSKEY_KEYPRESS > 0))
static shell_status_t cmd_auth_passkey_notify(shell_handle_t shell, int32_t argc, char *argv[])
{
	unsigned long type;
	int err;

	if (!default_conn) {
		shell_print(shell, "Not connected");
		return kStatus_SHELL_Error;
	}

	err = 0;
	type = shell_strtoul(argv[1], 0, &err);
	if (err ||
	    (type != BT_CONN_AUTH_KEYPRESS_ENTRY_STARTED &&
	     type != BT_CONN_AUTH_KEYPRESS_DIGIT_ENTERED &&
	     type != BT_CONN_AUTH_KEYPRESS_DIGIT_ERASED && type != BT_CONN_AUTH_KEYPRESS_CLEARED &&
	     type != BT_CONN_AUTH_KEYPRESS_ENTRY_COMPLETED)) {
		shell_error(sh, "<type> must be a value of enum bt_conn_auth_keypress");
		return (shell_status_t)EINVAL;
	}

	err = bt_conn_auth_keypress_notify(default_conn, type);
	if (err) {
		shell_error(shell, "bt_conn_auth_keypress_notify errno %d", err);
		return (shell_status_t)err;
	}

	return kStatus_SHELL_Success;
}
#endif /* CONFIG_BT_PASSKEY_KEYPRESS */
#if !(defined(CONFIG_BT_SMP_SC_PAIR_ONLY) && (CONFIG_BT_SMP_SC_PAIR_ONLY > 0))
static shell_status_t cmd_auth_oob_tk(shell_handle_t shell, int32_t argc, char *argv[])
{
	uint8_t tk[16];
	size_t len;
	int err;

	len = hex2bin(argv[1], strlen(argv[1]), tk, sizeof(tk));
	if (len != sizeof(tk)) {
		shell_error(shell, "TK should be 16 bytes");
		return (shell_status_t)EINVAL;
	}

	err = bt_le_oob_set_legacy_tk(default_conn, tk);
	if (err) {
		shell_error(shell, "Failed to set TK (%d)", err);
		return (shell_status_t)err;
	}

	return kStatus_SHELL_Success;
}
#endif /* !defined(CONFIG_BT_SMP_SC_PAIR_ONLY) */
#endif /* CONFIG_BT_SMP) || CONFIG_BT_BREDR */

static shell_status_t cmd_default_handler(shell_handle_t shell, int32_t argc, char *argv[])
{
	if (argc == 1) {
		shell_help(shell);
		return kStatus_SHELL_PrintCmdHelp;
	}

	shell_error(shell, "%s unknown parameter: %s", argv[0], argv[1]);

	return kStatus_SHELL_Error;
}
#define HELP_NONE "[none]"
#define HELP_ONOFF "<on, off>"
#define HELP_ADDR "<address: XX:XX:XX:XX:XX:XX>"
#define HELP_ADDR_LE "<address: XX:XX:XX:XX:XX:XX> <type: (public|random)>"

#if (defined(CONFIG_BT_EXT_ADV) && (CONFIG_BT_EXT_ADV > 0))
#define EXT_ADV_SCAN_OPT " [coded] [no-1m]"
#define EXT_ADV_PARAM "<type: conn-scan conn-nscan, nconn-scan nconn-nscan> " \
	"[ext-adv] [no-2m] [coded] [anon] [tx-power] [scan-reports] "                              \
	"[filter-accept-list: fal, fal-scan, fal-conn] [identity] [name] "                         \
	"[name-ad] [directed " HELP_ADDR_LE "] [mode: low] [dir-rpa] "                             \
	"[disable-37] [disable-38] [disable-39]"
#else
#define EXT_ADV_SCAN_OPT ""
#endif /* defined(CONFIG_BT_EXT_ADV) */

#if (defined(CONFIG_BT_OBSERVER) && (CONFIG_BT_OBSERVER > 0))
SHELL_STATIC_SUBCMD_SET_CREATE(bt_scan_filter_set_cmds,
	SHELL_CMD_ARG(name, NULL, "<name>", cmd_scan_filter_set_name, 2, 0),
	SHELL_CMD_ARG(addr, NULL, HELP_ADDR, cmd_scan_filter_set_addr, 2, 0),
	SHELL_CMD_ARG(rssi, NULL, "<rssi>", cmd_scan_filter_set_rssi, 2, 0),
	SHELL_CMD_ARG(pa_interval, NULL, "<pa_interval>",
		      cmd_scan_filter_set_pa_interval, 2, 0),
	SHELL_SUBCMD_SET_END
);

SHELL_STATIC_SUBCMD_SET_CREATE(bt_scan_filter_clear_cmds,
	SHELL_CMD_ARG(all, NULL, "", cmd_scan_filter_clear_all, 1, 0),
	SHELL_CMD_ARG(name, NULL, "", cmd_scan_filter_clear_name, 1, 0),
	SHELL_CMD_ARG(addr, NULL, "", cmd_scan_filter_clear_addr, 1, 0),
	SHELL_SUBCMD_SET_END
);
#endif /* CONFIG_BT_OBSERVER */

#if (defined(CONFIG_BT_IND_RESET) && (CONFIG_BT_IND_RESET > 0U))
static shell_status_t cmd_bt_ind_reset(shell_handle_t shell, int32_t argc, char *argv[])
{
	const char *reset_type;

	reset_type = argv[1];
	if (!
            strcmp(reset_type, "inband")) {
                controler_ind_reset(IND_RESET_INBAND);
	} else if (!strcmp(reset_type, "oob")) {
                controler_ind_reset(IND_RESET_OOB);
        } else if (!strcmp(reset_type, "disable")) {
                controler_ind_reset(IND_RESET_DISABLE);
	} else {
		shell_help(shell);
		return kStatus_SHELL_PrintCmdHelp;
	}

	return kStatus_SHELL_Success;
}
#endif /* CONFIG_BT_IND_RESET */
SHELL_STATIC_SUBCMD_SET_CREATE(bt_cmds,
	SHELL_CMD_ARG(init, NULL, "[no-settings-load], [sync]",
		      cmd_init, 1, 2),
#if (defined(CONFIG_BT_SETTINGS) && (CONFIG_BT_SETTINGS > 0))
	SHELL_CMD_ARG(settings-load, NULL, HELP_NONE, cmd_settings_load, 1, 0),
#endif
#if (defined(CONFIG_BT_HCI) && (CONFIG_BT_HCI > 0U))
	SHELL_CMD_ARG(hci-cmd, NULL, "<ogf> <ocf> [data]", cmd_hci_cmd, 3, 1),
#endif
	SHELL_CMD_ARG(id-create, NULL, HELP_ADDR, cmd_id_create, 1, 1),
	SHELL_CMD_ARG(id-reset, NULL, "<id> "HELP_ADDR, cmd_id_reset, 2, 1),
	SHELL_CMD_ARG(id-delete, NULL, "<id>", cmd_id_delete, 2, 0),
	SHELL_CMD_ARG(id-show, NULL, HELP_NONE, cmd_id_show, 1, 0),
	SHELL_CMD_ARG(id-select, NULL, "<id>", cmd_id_select, 2, 0),
	SHELL_CMD_ARG(name, NULL, "[name]", cmd_name, 1, 1),
#if (defined(CONFIG_BT_DEVICE_APPEARANCE_DYNAMIC) && (CONFIG_BT_DEVICE_APPEARANCE_DYNAMIC> 0))
	SHELL_CMD_ARG(appearance, NULL, "[new appearance value]", cmd_appearance, 1, 1),
#else
	SHELL_CMD_ARG(appearance, NULL, HELP_NONE, cmd_appearance, 1, 0),
#endif /* CONFIG_BT_DEVICE_APPEARANCE_DYNAMIC */
#if (defined(CONFIG_BT_OBSERVER) && (CONFIG_BT_OBSERVER > 0))
	SHELL_CMD_ARG(scan, NULL,
		      "<value: on, passive, off> [filter: dups, nodups] [fal]"
		      EXT_ADV_SCAN_OPT,
		      cmd_scan, 2, 4),
	SHELL_CMD(scan-filter-set, (struct _shell_command **)(void *)&bt_scan_filter_set_cmds,
		      "Scan filter set commands",
		      cmd_default_handler),
	SHELL_CMD(scan-filter-clear, (struct _shell_command **)(void *)&bt_scan_filter_clear_cmds,
		      "Scan filter clear commands",
		      cmd_default_handler),
	SHELL_CMD_ARG(scan-verbose-output, NULL, "<value: on, off>", cmd_scan_verbose_output, 2, 0),
#endif /* CONFIG_BT_OBSERVER */
#if (defined(CONFIG_BT_TRANSMIT_POWER_CONTROL) && (CONFIG_BT_TRANSMIT_POWER_CONTROL > 0))
	SHELL_CMD_ARG(read-remote-tx-power, NULL, HELP_NONE, cmd_read_remote_tx_power, 2, 0),
	SHELL_CMD_ARG(read-local-tx-power, NULL, HELP_NONE, cmd_read_local_tx_power, 2, 0),
	SHELL_CMD_ARG(set-power-report-enable, NULL, HELP_NONE, cmd_set_power_report_enable, 3, 0),
#endif

#if (defined(CONFIG_BT_BROADCASTER) && (CONFIG_BT_BROADCASTER > 0))
	SHELL_CMD_ARG(advertise, NULL,
		      "<type: off, on, scan, nconn> [mode: discov, non_discov] "
		      "[filter-accept-list: fal, fal-scan, fal-conn] [identity] [no-name] "
		      "[one-time] [name-ad] [appearance] "
		      "[disable-37] [disable-38] [disable-39]",
		      cmd_advertise, 2, 8),
#if (defined(CONFIG_BT_PERIPHERAL) && (CONFIG_BT_PERIPHERAL > 0))
	SHELL_CMD_ARG(directed-adv, NULL, HELP_ADDR_LE " [mode: low] "
		      "[identity] [dir-rpa]",
		      cmd_directed_adv, 3, 6),
#endif /* CONFIG_BT_PERIPHERAL */
#if (defined(CONFIG_BT_EXT_ADV) && (CONFIG_BT_EXT_ADV > 0))
	SHELL_CMD_ARG(adv-create, NULL, EXT_ADV_PARAM, cmd_adv_create, 2, 11),
	SHELL_CMD_ARG(adv-param, NULL, EXT_ADV_PARAM, cmd_adv_param, 2, 11),
	SHELL_CMD_ARG(adv-data, NULL, "<data> [scan-response <data>] "
				      "<type: discov, hex> [appearance] ",
		      cmd_adv_data, 1, 16),
	SHELL_CMD_ARG(adv-start, NULL,
		"[timeout <timeout>] [num-events <num events>]",
		cmd_adv_start, 1, 4),
	SHELL_CMD_ARG(adv-stop, NULL, HELP_NONE, cmd_adv_stop, 1, 0),
	SHELL_CMD_ARG(adv-delete, NULL, HELP_NONE, cmd_adv_delete, 1, 0),
	SHELL_CMD_ARG(adv-select, NULL, "[adv]", cmd_adv_select, 1, 1),
	SHELL_CMD_ARG(adv-info, NULL, HELP_NONE, cmd_adv_info, 1, 0),
#if (defined(CONFIG_BT_PERIPHERAL) && (CONFIG_BT_PERIPHERAL > 0))
	SHELL_CMD_ARG(adv-oob, NULL, HELP_NONE, cmd_adv_oob, 1, 0),
#endif /* CONFIG_BT_PERIPHERAL */
#if (defined(CONFIG_BT_PRIVACY) && (CONFIG_BT_PRIVACY > 0))
	SHELL_CMD_ARG(adv-rpa-expire, NULL, HELP_ONOFF, cmd_adv_rpa_expire, 2, 0),
#endif
#if (defined(CONFIG_BT_PER_ADV) && (CONFIG_BT_PER_ADV > 0))
	SHELL_CMD_ARG(per-adv, NULL, HELP_ONOFF, cmd_per_adv, 2, 0),
	SHELL_CMD_ARG(per-adv-param, NULL,
		      "[<interval-min> [<interval-max> [tx_power]]]",
		      cmd_per_adv_param, 1, 3),
	SHELL_CMD_ARG(per-adv-data, NULL, "[data]", cmd_per_adv_data, 1, 1),
#endif /* CONFIG_BT_PER_ADV */
#endif /* CONFIG_BT_EXT_ADV */
#endif /* CONFIG_BT_BROADCASTER */
#if (defined(CONFIG_BT_PER_ADV_SYNC) && (CONFIG_BT_PER_ADV_SYNC > 0))
	SHELL_CMD_ARG(per-adv-sync-create, NULL,
		      HELP_ADDR_LE " <sid> [skip <count>] [timeout <ms>] [aoa] "
		      "[aod_1us] [aod_2us] [cte_only]",
		      cmd_per_adv_sync_create, 4, 6),
	SHELL_CMD_ARG(per-adv-sync-delete, NULL, "[<index>]",
		      cmd_per_adv_sync_delete, 1, 1),
	SHELL_CMD_ARG(per-adv-sync-select, NULL, "[adv]", cmd_per_adv_sync_select, 1, 1),
#endif /* defined(CONFIG_BT_PER_ADV_SYNC) */
#if (defined(CONFIG_BT_CONN) && (CONFIG_BT_CONN > 0))
#if (defined(CONFIG_BT_PER_ADV_SYNC_TRANSFER_RECEIVER) && (CONFIG_BT_PER_ADV_SYNC_TRANSFER_RECEIVER > 0))
	SHELL_CMD_ARG(past-subscribe, NULL, "[conn] [skip <count>] "
		      "[timeout <ms>] [aoa] [aod_1us] [aod_2us] [cte_only]",
		      cmd_past_subscribe, 1, 7),
	SHELL_CMD_ARG(past-unsubscribe, NULL, "[conn]",
		      cmd_past_unsubscribe, 1, 1),
#endif /* CONFIG_BT_PER_ADV_SYNC_TRANSFER_RECEIVER */
#if (defined(CONFIG_BT_PER_ADV_SYNC_TRANSFER_SENDER) && (CONFIG_BT_PER_ADV_SYNC_TRANSFER_SENDER > 0))
#if (defined(CONFIG_BT_PER_ADV_SYNC) && (CONFIG_BT_PER_ADV_SYNC > 0))
	SHELL_CMD_ARG(per-adv-sync-transfer, NULL, "[<index>]",
		      cmd_per_adv_sync_transfer, 1, 1),
#endif /* CONFIG_BT_PER_ADV_SYNC */
         
#if (defined(CONFIG_BT_PER_ADV) && (CONFIG_BT_PER_ADV > 0))
	SHELL_CMD_ARG(per-adv-set-info-transfer, NULL, "",
		      cmd_per_adv_set_info_transfer, 1, 0),
#endif /* CONFIG_BT_PER_ADV */
#endif /* CONFIG_BT_PER_ADV_SYNC_TRANSFER_SENDER */
#if (defined(CONFIG_BT_CENTRAL) && (CONFIG_BT_CENTRAL > 0))
	SHELL_CMD_ARG(connect, NULL, HELP_ADDR_LE EXT_ADV_SCAN_OPT,
		      cmd_connect_le, 1, 3),
#if !(defined(CONFIG_BT_FILTER_ACCEPT_LIST) && (CONFIG_BT_FILTER_ACCEPT_LIST > 0))
	SHELL_CMD_ARG(auto-conn, NULL, HELP_ADDR_LE, cmd_auto_conn, 3, 0),
#endif /* !defined(CONFIG_BT_FILTER_ACCEPT_LIST) */
	SHELL_CMD_ARG(connect-name, NULL, "<name filter>",
		      cmd_connect_le_name, 2, 0),
#endif /* CONFIG_BT_CENTRAL */
	SHELL_CMD_ARG(disconnect, NULL, HELP_ADDR_LE, cmd_disconnect, 1, 2),
	SHELL_CMD_ARG(select, NULL, HELP_ADDR_LE, cmd_select, 3, 0),
	SHELL_CMD_ARG(info, NULL, HELP_ADDR_LE, cmd_info, 1, 2),
	SHELL_CMD_ARG(conn-update, NULL, "<min> <max> <latency> <timeout>",
		      cmd_conn_update, 5, 0),
#if (defined(CONFIG_BT_USER_DATA_LEN_UPDATE) && (CONFIG_BT_USER_DATA_LEN_UPDATE > 0))
	SHELL_CMD_ARG(data-len-update, NULL, "<tx_max_len> [tx_max_time]",
		      cmd_conn_data_len_update, 2, 1),
#endif /* CONFIG_BT_USER_DATA_LEN_UPDATE */
#if (defined(CONFIG_BT_USER_PHY_UPDATE) && (CONFIG_BT_USER_PHY_UPDATE > 0))
	SHELL_CMD_ARG(phy-update, NULL, "<tx_phy> [rx_phy] [s2] [s8]",
		      cmd_conn_phy_update, 2, 3),
#endif /* CONFIG_BT_USER_PHY_UPDATE */
        
#if (defined(CONFIG_BT_CENTRAL) && (CONFIG_BT_CENTRAL > 0))
	SHELL_CMD_ARG(channel-map, NULL, "<channel-map: XXXXXXXXXX> (36-0)",
		      cmd_chan_map, 2, 1),
#endif /* CONFIG_BT_CENTRAL */
	SHELL_CMD_ARG(oob, NULL, HELP_NONE, cmd_oob, 1, 0),
	SHELL_CMD_ARG(clear, NULL, "[all] ["HELP_ADDR_LE"]", cmd_clear, 2, 1),
#if (defined(CONFIG_BT_SMP) && (CONFIG_BT_SMP > 0)) || ((defined(CONFIG_BT_BREDR) && (CONFIG_BT_BREDR > 0)))
	SHELL_CMD_ARG(security, NULL, "<security level BR/EDR: 0 - 3, "
				      "LE: 1 - 4> [force-pair]",
		      cmd_security, 1, 2),
	SHELL_CMD_ARG(bondable, NULL, HELP_ONOFF, cmd_bondable,
		      2, 0),
	SHELL_CMD_ARG(bonds, NULL, HELP_NONE, cmd_bonds, 1, 0),
	SHELL_CMD_ARG(connections, NULL, HELP_NONE, cmd_connections, 1, 0),
	SHELL_CMD_ARG(auth, NULL,
		      "<method: all, input, display, yesno, confirm, "
		      "oob, status, none>",
		      cmd_auth, 2, 0),
	SHELL_CMD_ARG(auth-cancel, NULL, HELP_NONE, cmd_auth_cancel, 1, 0),
	SHELL_CMD_ARG(auth-passkey, NULL, "<passkey>", cmd_auth_passkey, 2, 0),
#if (defined(CONFIG_BT_PASSKEY_KEYPRESS) && (CONFIG_BT_PASSKEY_KEYPRESS > 0))
	/HELL_CMD_ARG(auth-passkey-notify, NULL, "<type>",
		      cmd_auth_passkey_notify, 2, 0),
#endif /* CONFIG_BT_PASSKEY_KEYPRESS */
	SHELL_CMD_ARG(auth-passkey-confirm, NULL, HELP_NONE,
		      cmd_auth_passkey_confirm, 1, 0),
	SHELL_CMD_ARG(auth-pairing-confirm, NULL, HELP_NONE,
		      cmd_auth_pairing_confirm, 1, 0),
#if !(defined(CONFIG_BT_SMP_SC_PAIR_ONLY) && (CONFIG_BT_SMP_SC_PAIR_ONLY > 0))
	SHELL_CMD_ARG(auth-oob-tk, NULL, "<tk>", cmd_auth_oob_tk, 2, 0),
#endif /* !defined(CONFIG_BT_SMP_SC_PAIR_ONLY) */
	SHELL_CMD_ARG(oob-remote, NULL,
		      HELP_ADDR_LE" <oob rand> <oob confirm>",
		      cmd_oob_remote, 3, 2),
	SHELL_CMD_ARG(oob-clear, NULL, HELP_NONE, cmd_oob_clear, 1, 0),
#if (defined(CONFIG_BT_FILTER_ACCEPT_LIST) && (CONFIG_BT_FILTER_ACCEPT_LIST > 0))
	SHELL_CMD_ARG(fal-add, NULL, HELP_ADDR_LE, cmd_fal_add, 3, 0),
	SHELL_CMD_ARG(fal-rem, NULL, HELP_ADDR_LE, cmd_fal_rem, 3, 0),
	SHELL_CMD_ARG(fal-clear, NULL, HELP_NONE, cmd_fal_clear, 1, 0),
#if (defined(CONFIG_BT_CENTRAL) && (CONFIG_BT_CENTRAL > 0))
	SHELL_CMD_ARG(fal-connect, NULL, HELP_ONOFF EXT_ADV_SCAN_OPT,
		      cmd_fal_connect, 2, 3),
#endif /* CONFIG_BT_CENTRAL */
#endif /* defined(CONFIG_BT_FILTER_ACCEPT_LIST) */
#if (defined(CONFIG_BT_FIXED_PASSKEY) && (CONFIG_BT_FIXED_PASSKEY > 0))
	SHELL_CMD_ARG(fixed-passkey, NULL, "[passkey]", cmd_fixed_passkey,
		      1, 1),
#endif
#endif /* CONFIG_BT_SMP || CONFIG_BT_BREDR) */
#endif /* CONFIG_BT_CONN */
#if (defined(CONFIG_BT_HCI_MESH_EXT) && (CONFIG_BT_HCI_MESH_EXT > 0))
	SHELL_CMD(mesh_adv, NULL, HELP_ONOFF, cmd_mesh_adv),
#endif /* CONFIG_BT_HCI_MESH_EXT */
#if (defined(CONFIG_BT_CTLR_ADV_EXT) && (CONFIG_BT_CTLR_ADV_EXT > 0))
#if (defined(CONFIG_BT_BROADCASTER) && (CONFIG_BT_BROADCASTER > 0))
	SHELL_CMD_ARG(advx, NULL,
		      "<on hdcd ldcd off> [coded] [anon] [txp] [ad]",
		      cmd_advx, 2, 4),
#endif /* CONFIG_BT_BROADCASTER */
#if (defined(CONFIG_BT_OBSERVER) && (CONFIG_BT_OBSERVER > 0))
	SHELL_CMD_ARG(scanx, NULL, "<on passive off> [coded]", cmd_scanx,
		      2, 1),
#endif /* CONFIG_BT_OBSERVER */
#endif /* CONFIG_BT_CTLR_ADV_EXT */
#if (defined(CONFIG_BT_CTLR_DTM) && (CONFIG_BT_CTLR_DTM > 0))
	SHELL_CMD_ARG(test_tx, NULL, "<chan> <len> <type> <phy>", cmd_test_tx,
		      5, 0),
	SHELL_CMD_ARG(test_rx, NULL, "<chan> <phy> <mod_idx>", cmd_test_rx,
		      4, 0),
	SHELL_CMD_ARG(test_end, NULL, HELP_NONE, cmd_test_end, 1, 0),
#endif /* CONFIG_BT_CTLR_DTM */
#if (defined(CONFIG_BT_IND_RESET) && (CONFIG_BT_IND_RESET > 0U))
        SHELL_CMD_ARG(ind_reset, NULL, HELP_NONE, cmd_bt_ind_reset, 2, 0),
#endif /*#define CONFIG_BT_IND_RESET*/
	SHELL_SUBCMD_SET_END
);

static shell_status_t cmd_bt(shell_handle_t shell, int32_t argc, char **argv)
{
	if (argc == 1) {
		shell_help(shell);
		return kStatus_SHELL_PrintCmdHelp;
	}

	shell_error(shell, "%s unknown parameter: %s", argv[0], argv[1]);

	return kStatus_SHELL_Error;
}

SHELL_CMD_REGISTER(bt, bt_cmds, "command entry", cmd_bt, 1, 0);

void bt_CommandInit(shell_handle_t shell)
{
    if ((shell_status_t)kStatus_Success != SHELL_RegisterCommand(shell, &g_shellCommandbt))
    {
        shell_print(shell, "Shell register command %s failed!", g_shellCommandbt.pcCommand);
    }
    bt_ShellGattInit(shell);
    bt_ShellL2capInit(shell);
#if (defined(CONFIG_BT_ISO) && (CONFIG_BT_ISO > 0))
    bt_ShellIsoInit(shell);
#endif
#if (defined(CONFIG_BT_BAP_STREAM) && (CONFIG_BT_BAP_STREAM > 0))
	bt_ShellBapInit(shell);
#endif
#if (defined(CONFIG_BT_BAP_BROADCAST_ASSISTANT) && (CONFIG_BT_BAP_BROADCAST_ASSISTANT > 0))
	bt_ShellBapBroadcastAssistantInit(shell);
#endif
#if (defined(CONFIG_BT_BAP_SCAN_DELEGATOR) && (CONFIG_BT_BAP_SCAN_DELEGATOR > 0))
	bt_ShellBapScanDelegatorInit(shell);
#endif
#if (defined(CONFIG_BT_CAP_INITIATOR) && (CONFIG_BT_CAP_INITIATOR > 0))
	bt_ShellCapInitiatorInit(shell);
#endif
#if (defined(CONFIG_BT_CAP_ACCEPTOR) && (CONFIG_BT_CAP_ACCEPTOR > 0))
	bt_ShellCapAcceptorInit(shell);
#endif
#if (defined(CONFIG_BT_CAP_COMMANDER) && (CONFIG_BT_CAP_COMMANDER > 0))
	bt_ShellCapCommanderInit(shell);
#endif
#if (defined(CONFIG_BT_CSIP_SET_MEMBER) && (CONFIG_BT_CSIP_SET_MEMBER > 0))
	bt_ShellCsipSetMemberInit(shell);
#endif
#if (defined(CONFIG_BT_HAS_CLIENT) && (CONFIG_BT_HAS_CLIENT > 0))
	bt_ShellHasClientInit(shell);
#endif
#if (defined(CONFIG_BT_HAS_PRESET_SUPPORT) && (CONFIG_BT_HAS_PRESET_SUPPORT > 0))
	bt_ShellHasInit(shell);
#endif
#if (defined(CONFIG_BT_MCC) && (CONFIG_BT_MCC > 0))
	bt_ShellMccInit(shell);
#endif
#if (defined(CONFIG_BT_MCS) && (CONFIG_BT_MCS > 0))
	bt_ShellMediaInit(shell);
#endif
#if (defined(CONFIG_BT_MICP_MIC_CTLR) && (CONFIG_BT_MICP_MIC_CTLR > 0))
	bt_ShellMicpMicCtlrInit(shell);
#endif
#if (defined(CONFIG_BT_MICP_MIC_DEV) && (CONFIG_BT_MICP_MIC_DEV > 0))
	bt_ShellMicpInit(shell);
#endif
#if (defined(CONFIG_BT_MPL) && (CONFIG_BT_MPL > 0))
	bt_ShellMplInit(shell);
#endif
#if (defined(CONFIG_BT_TBS_CLIENT) && (CONFIG_BT_TBS_CLIENT > 0))
	bt_ShellTbsClientInit(shell);
#endif
#if (defined(CONFIG_BT_TBS) && (CONFIG_BT_TBS > 0))
	bt_ShellTbsInit(shell);
#endif
#if (defined(CONFIG_BT_TMAP) && (CONFIG_BT_TMAP > 0))
	bt_ShellTmapInit(shell);
#endif
#if (defined(CONFIG_BT_VCP_VOL_REND) && (CONFIG_BT_VCP_VOL_REND > 0))
	bt_ShellVcpVolRendInit(shell);
#endif
#if (defined(CONFIG_BT_VCP_VOL_CTLR) && (CONFIG_BT_VCP_VOL_CTLR > 0))
	bt_ShellVcpVolCtlrInit(shell);
#endif
#if (defined(CONFIG_BT_BREDR) && (CONFIG_BT_BREDR > 0))
    bt_ShellBrEdrInit(shell);

#if (defined(CONFIG_BT_RFCOMM) && (CONFIG_BT_RFCOMM > 0))
    bt_ShellRfcommInit(shell);
#endif /* CONFIG_BT_RFCOMM */

#if (defined(CONFIG_BT_A2DP) && (CONFIG_BT_A2DP > 0))
    bt_ShellA2dpInit(shell);
#endif /* CONFIG_BT_A2DP */

#if (defined(CONFIG_BT_AVRCP) && (CONFIG_BT_AVRCP > 0))
    bt_ShellAvrcpInit(shell);
#endif /* CONFIG_BT_AVRCP */
#if (defined(CONFIG_BT_HFP_HF) && (CONFIG_BT_HFP_HF > 0))
    bt_ShellHfpInit(shell);
#endif /* CONFIG_BT_HFP */
#if (defined(CONFIG_BT_PBAP_PCE) && (CONFIG_BT_PBAP_PCE > 0))
    bt_ShellPbapInit(shell);
#endif /* CONGIF_BT_PBAP */
#if (defined(CONFIG_BT_MAP_MCE) && (CONFIG_BT_MAP_MCE > 0))
    bt_ShellMapInit(shell);
#endif /* CONFIG_BT_MAP_MCE */
#endif /* CONFIG_BT_BREDR */
#if (defined(CONFIG_BT_RF_TEST_MODE) && (CONFIG_BT_RF_TEST_MODE > 0))
    bt_ShellTestModeInit(shell);
#endif /* CONFIG_BT_RF_TEST_MODE */
    ctx_shell = shell;
    s_shellBtPrompt = current_prompt();
    SHELL_ChangePrompt(shell, (char *)s_shellBtPrompt);
}

void shell_hexdump(shell_handle_t shell, const uint8_t *data, size_t len)
{
    shell_dump(shell, " Data: ");

    while (len--) {
        shell_dump(shell, "%02X ", *data++);
    }
	shell_dump(shell, "\r\n");
}

unsigned long shell_strtoul(const char *str, int base, int *err)
{
	unsigned long val;
	char *endptr = NULL;

	if (*str == '-') {
		*err = -EINVAL;
		return 0;
	}

	val = strtoul(str, &endptr, base);

	return val;
}

unsigned long long shell_strtoull(const char *str, int base, int *err)
{
	unsigned long long val;
	char *endptr = NULL;

	if (*str == '-') {
		*err = -EINVAL;
		return 0;
	}

	val = strtoull(str, &endptr, base);

	return val;
}

long shell_strtol(const char *str, int base, int *err)
{
	long val;
	char *endptr = NULL;

	val = strtol(str, &endptr, base);
	if (endptr == str || *endptr) {
		*err = -EINVAL;
		return 0;
	}

	return val;
}

bool shell_strtobool(const char *str, int base, int *err)
{
	if (!strcmp(str, "on") || !strcmp(str, "enable") || !strcmp(str, "true")) {
		return true;
	}

	if (!strcmp(str, "off") || !strcmp(str, "disable") || !strcmp(str, "false")) {
		return false;
	}

	return shell_strtoul(str, base, err);
}