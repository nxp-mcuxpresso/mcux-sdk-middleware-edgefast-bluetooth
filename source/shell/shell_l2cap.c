/** @file
 * @brief Bluetooth shell module
 *
 * Provide some Bluetooth shell commands that can be useful to applications.
 */

/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <errno/errno.h>
#include <zephyr/types.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/byteorder.h>
#include <porting.h>


#include <bluetooth/hci.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/conn.h>
#include <bluetooth/l2cap.h>
#if (defined(CONFIG_BT_RFCOMM) && (CONFIG_BT_RFCOMM > 0))
#include <bluetooth/rfcomm.h>
#endif
#if (defined(CONFIG_BT_BREDR) && ((CONFIG_BT_BREDR) > 0U))
#include <bluetooth/sdp.h>
#endif

#include "fsl_shell.h"

#include "shell_bt.h"

#define MAX_L2CAP_CHANNEL 1

#define CREDITS			10
#define DATA_MTU		(23 * CREDITS)

#define L2CAP_POLICY_NONE		0x00
#define L2CAP_POLICY_ALLOWLIST		0x01
#define L2CAP_POLICY_16BYTE_KEY		0x02

NET_BUF_POOL_FIXED_DEFINE(data_tx_pool, 1,
			  BT_L2CAP_SDU_BUF_SIZE(DATA_MTU), NULL);
NET_BUF_POOL_FIXED_DEFINE(data_rx_pool, 1, DATA_MTU, NULL);

static uint8_t l2cap_policy;
static struct bt_conn *l2cap_allowlist[CONFIG_BT_MAX_CONN];

static uint32_t l2cap_rate;
static uint32_t l2cap_recv_delay_ms;
osa_msgq_handle_t l2cap_recv_fifo;
OSA_MSGQ_HANDLE_DEFINE(l2cap_recv_fifo_handle, CONFIG_BT_MSG_QUEUE_COUNT, sizeof(void*));
struct l2ch {
	bool used;
	struct k_work_delayable recv_work;
	struct bt_l2cap_le_chan ch;
};
#define L2CH_CHAN(_chan) CONTAINER_OF(_chan, struct l2ch, ch.chan)
#define L2CH_WORK(_work) CONTAINER_OF(_work, struct l2ch, recv_work)
#define L2CAP_CHAN(_chan) _chan->ch.chan

static bool metrics;

#if (defined(CONFIG_BT_L2CAP_UNBLOCK_SEND) && (CONFIG_BT_L2CAP_UNBLOCK_SEND > 0))
static bool unblock_send_timer_initialized = false;
static bool unblock_send_start_flag = false;
static bool tx_throughput_print_flag = false;
static bool rx_throughput_print_flag = false;
static uint32_t unblock_send_count;
static uint8_t unblock_send_length;
static struct k_work_delayable unblock_send_timer;

static uint16_t req_data_tx_chunk_len = 0x0000;
static uint16_t req_data_tx_count     = 0x0000;
static uint16_t exp_data_rx_chunk_len = 0x0000;
static uint16_t exp_data_rx_count     = 0x0000;
static uint16_t curr_data_rx_count    = 0x0000;

#define L2CAP_RELATIVE_TIME_INIT_VALUE 0xFFFFFFFF
extern uint64_t SHELL_get_us_timestamp(void);
static uint64_t first_tx_inst = L2CAP_RELATIVE_TIME_INIT_VALUE;
static uint64_t last_tx_inst  = L2CAP_RELATIVE_TIME_INIT_VALUE;
static uint64_t first_rx_inst = L2CAP_RELATIVE_TIME_INIT_VALUE;
static uint64_t last_rx_inst  = L2CAP_RELATIVE_TIME_INIT_VALUE;
#endif

#if (defined(CONFIG_BT_L2CAP_UNBLOCK_SEND) && (CONFIG_BT_L2CAP_UNBLOCK_SEND > 0))
static uint64_t l2cap_get_current_relative_time (void)
{
    return (uint64_t)SHELL_get_us_timestamp();
}

static uint32_t l2cap_calculate_data_rate
       (
            uint32_t              data_count,
            uint64_t             time_elapsed
       )
{
    uint64_t tmp_data_count;

    /* Converting the Data Count to Larger Data Type */
    tmp_data_count = data_count;
    tmp_data_count *= (1000000);

    /**
     * TODO: Check if this below code is needed.
     * Defaults to 1 microsecond.
     */
    time_elapsed = (0 == time_elapsed) ? 1 : time_elapsed;

    return (uint32_t)((tmp_data_count)/(time_elapsed));
}

static uint32_t l2cap_calculate_data_rate_in_kbps
       (
            uint32_t              data_count,
            uint64_t             time_elapsed
       )
{
    uint64_t tmp_data_count;

    /* Converting the Data Count to Larger Data Type */
    tmp_data_count = data_count;
    tmp_data_count *= (1000000);

    /**
     * TODO: Check if this below code is needed.
     * Defaults to 1 microsecond.
     */
    time_elapsed = (0 == time_elapsed) ? 1 : time_elapsed;

    return (uint32_t)(((((tmp_data_count)/(time_elapsed)) * 8) /1024));
}

static uint16_t l2cap_display_tx_stat (void)
{
    uint64_t           duration;
    uint32_t           total_byte_count;

    if ((L2CAP_RELATIVE_TIME_INIT_VALUE == first_tx_inst) &&
        (L2CAP_RELATIVE_TIME_INIT_VALUE == last_tx_inst))
    {
        /* Do Nothing */
    }
    else
    {
        duration = (last_tx_inst - first_tx_inst);
        total_byte_count = (req_data_tx_chunk_len * req_data_tx_count);
        shell_print (ctx_shell,
        "\n---------------------------- TX Session ----------------------------\n");
        shell_print (ctx_shell,
        "  First Packet Transmit at: %lld microseconds\n", first_tx_inst);
        shell_print (ctx_shell,
        "  Last Packet Transmit at : %lld microseconds\n", last_tx_inst);
        shell_print (ctx_shell,
        "  Session Duration        : %lld microseconds\n", duration);
        shell_print (ctx_shell,
        "  Total Bytes Transmitted : %d bytes (%d * %d)\n", total_byte_count,
        req_data_tx_chunk_len, req_data_tx_count);
        shell_print (ctx_shell,
        "  Data Rate               : %d bytes per second (%d kbps)\n",
        l2cap_calculate_data_rate (total_byte_count, duration),
        l2cap_calculate_data_rate_in_kbps (total_byte_count, duration));

        shell_print (ctx_shell,
        "\n---------------------------------------------------------------------\n");
    }

    return 0;
}

static uint16_t l2cap_display_rx_stat (void)
{
    uint64_t 			 duration;
    uint32_t             total_byte_count;

    if ((L2CAP_RELATIVE_TIME_INIT_VALUE == first_rx_inst) &&
        (L2CAP_RELATIVE_TIME_INIT_VALUE == last_rx_inst))
    {
        /* Do Nothing */
    }
    else
    {
        duration = (last_rx_inst - first_rx_inst);
        total_byte_count = (exp_data_rx_chunk_len * exp_data_rx_count);
        shell_print (ctx_shell,
        "\n---------------------------- RX Session ----------------------------\n");
        shell_print (ctx_shell,
        "  First Packet Received at: %lld microseconds\n", first_rx_inst);
        shell_print (ctx_shell,
        "  Last Packet Received at : %lld microseconds\n", last_rx_inst);
        shell_print (ctx_shell,
        "  Session Duration        : %lld microseconds\n", duration);
        shell_print (ctx_shell,
        "  Total Bytes Received    : %d bytes (%d * %d)\n", total_byte_count,
        exp_data_rx_chunk_len, exp_data_rx_count);
        shell_print (ctx_shell,
        "  Data Rate               : %d bytes per second (%d kbps)\n",
        l2cap_calculate_data_rate (total_byte_count, duration),
        l2cap_calculate_data_rate_in_kbps (total_byte_count, duration));

        shell_print (ctx_shell,
        "\n---------------------------------------------------------------------\n");
    }

    return 0;
}
#endif

static void l2cap_channel_free(struct l2ch *chan);

static int l2cap_recv_metrics(struct bt_l2cap_chan *chan, struct net_buf *buf)
{
	static uint32_t len;
	static uint32_t cycle_stamp;
	uint32_t delta;

	delta = (uint32_t)OSA_TimeGetMsec() - cycle_stamp;
	delta = (uint32_t)(delta * 1000);

	/* if last data rx-ed was greater than 1 second in the past,
	 * reset the metrics.
	 */
	if (delta > 1000000000) {
		len = 0U;
		l2cap_rate = 0U;
		cycle_stamp = (uint32_t)OSA_TimeGetMsec();
	} else {
		len += buf->len;
		l2cap_rate = ((uint64_t)len << 3) * 1000000000U / delta;
	}

	return kStatus_SHELL_Success;
}

static void l2cap_recv_cb(struct k_work *work)
{
	struct l2ch *c = L2CH_WORK(work);
	struct net_buf *buf;

	while ((buf = net_buf_get(l2cap_recv_fifo, osaWaitNone_c))) {
		shell_print(ctx_shell, "Confirming reception");
		bt_l2cap_chan_recv_complete(&c->ch.chan, buf);
	}
}

static int l2cap_recv(struct bt_l2cap_chan *chan, struct net_buf *buf)
{
	struct l2ch *l2ch = L2CH_CHAN(chan);

	if (metrics) {
		return l2cap_recv_metrics(chan, buf);
	}

	shell_print(ctx_shell, "Incoming data channel %p len %u", chan,
		    buf->len);

#if (defined(CONFIG_BT_L2CAP_UNBLOCK_SEND) && (CONFIG_BT_L2CAP_UNBLOCK_SEND > 0))
	if (rx_throughput_print_flag)
	{
		if (0x0000 == curr_data_rx_count)
		{
			/* Get first instance of Reception */
			first_rx_inst = l2cap_get_current_relative_time();
		}

		/* Increment the count */
		curr_data_rx_count++;

		if (exp_data_rx_count == curr_data_rx_count)
		{
			shell_print(ctx_shell,
			"\n%d Bytes of Data, received for %d times Successfully!\n",
			exp_data_rx_chunk_len, exp_data_rx_count);

			/* Get last instance of Reception */
			last_rx_inst = l2cap_get_current_relative_time();

			/* Print Statistics here */
			l2cap_display_rx_stat();

			/* Reset Transient Variables here */
			curr_data_rx_count = 0;
			first_rx_inst = L2CAP_RELATIVE_TIME_INIT_VALUE;
			last_rx_inst  = L2CAP_RELATIVE_TIME_INIT_VALUE;

			rx_throughput_print_flag = false;
		}
	}
#endif

	if (buf->len) {
		shell_hexdump(ctx_shell, buf->data, buf->len);
	}

	if (l2cap_recv_delay_ms > 0) {
		/* Submit work only if queue is empty */
		if (0 == OSA_MsgQAvailableMsgs(l2cap_recv_fifo)) {
			shell_print(ctx_shell, "Delaying response in %u ms...",
				    l2cap_recv_delay_ms);
			bt_delayed_work_submit(&l2ch->recv_work,
					      BT_MSEC(l2cap_recv_delay_ms));
		}
		net_buf_put(l2cap_recv_fifo, buf);
		return -EINPROGRESS;
	}

	return kStatus_SHELL_Success;
}

static void l2cap_sent(struct bt_l2cap_chan *chan)
{
	shell_print(ctx_shell, "Outgoing data channel %p transmitted", chan);
}

static void l2cap_status(struct bt_l2cap_chan *chan, atomic_t *status)
{
	shell_print(ctx_shell, "Channel %p status %u", chan, (uint32_t)*status);
}

static void l2cap_connected(struct bt_l2cap_chan *chan)
{
	struct l2ch *c = L2CH_CHAN(chan);

	bt_delayed_work_init(&c->recv_work, l2cap_recv_cb);

	shell_print(ctx_shell, "Channel %p connected", chan);
}

static void l2cap_disconnected(struct bt_l2cap_chan *chan)
{
	struct l2ch *l2ch = L2CH_CHAN(chan);

	shell_print(ctx_shell, "Channel %p disconnected", chan);
	l2cap_channel_free(l2ch);
}

static struct net_buf *l2cap_alloc_buf(struct bt_l2cap_chan *chan)
{
	/* print if metrics is disabled */
	if (!metrics) {
		shell_print(ctx_shell, "Channel %p requires buffer", chan);
	}

	return net_buf_alloc(&data_rx_pool, osaWaitForever_c);
}

static const struct bt_l2cap_chan_ops l2cap_ops = {
	.alloc_buf	= l2cap_alloc_buf,
	.recv		= l2cap_recv,
	.sent		= l2cap_sent,
	.status		= l2cap_status,
	.connected	= l2cap_connected,
	.disconnected	= l2cap_disconnected,
};

static struct l2ch l2ch_chan[MAX_L2CAP_CHANNEL];

static struct l2ch * l2cap_channel_create_new(void)
{
	for (int i = 0;i < MAX_L2CAP_CHANNEL;i++)
	{
		if (false == l2ch_chan[i].used)
		{
			l2ch_chan[i].used = true;
			return &l2ch_chan[i];
		}
	}
	return NULL;
}

static void l2cap_channel_free(struct l2ch *chan)
{
	if (true == chan->used)
	{
		chan->used = false;
	}
}

static struct l2ch * l2cap_channel_lookup_conn(struct bt_conn *conn)
{
	for (int i = 0;i < MAX_L2CAP_CHANNEL;i++)
	{
		if ((true == l2ch_chan[i].used) && (conn == l2ch_chan[i].ch.chan.conn))
		{
			return &l2ch_chan[i];
		}
	}
	return NULL;
}

static void l2cap_allowlist_remove(struct bt_conn *conn, uint8_t reason)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(l2cap_allowlist); i++) {
		if (l2cap_allowlist[i] == conn) {
			bt_conn_unref(l2cap_allowlist[i]);
			l2cap_allowlist[i] = NULL;
		}
	}
}

static struct bt_conn_cb l2cap_conn_callbacks = {
	.disconnected = l2cap_allowlist_remove,
};

static int l2cap_accept_policy(struct bt_conn *conn)
{
	int i;

	if (l2cap_policy == L2CAP_POLICY_16BYTE_KEY) {
#if ((defined(CONFIG_BT_SMP) && ((CONFIG_BT_SMP) > 0U)) || (defined(CONFIG_BT_BREDR) && ((CONFIG_BT_BREDR) > 0U)))
		uint8_t enc_key_size = bt_conn_enc_key_size(conn);

		if (enc_key_size && enc_key_size < BT_ENC_KEY_SIZE_MAX) {
			return -EPERM;
		}
#endif
	} else if (l2cap_policy == L2CAP_POLICY_ALLOWLIST) {
		for (i = 0; i < ARRAY_SIZE(l2cap_allowlist); i++) {
			if (l2cap_allowlist[i] == conn) {
				return 0;
			}
		}

		return -EACCES;
	}

	return 0;
}

static int l2cap_accept(struct bt_conn *conn, struct bt_l2cap_server *server,
			struct bt_l2cap_chan **chan)
{
	struct l2ch *l2cap_channel;
	int err;

	shell_print(ctx_shell, "Incoming conn %p", conn);

	err = l2cap_accept_policy(conn);
	if (err < 0) {
		return err;
	}

	l2cap_channel = l2cap_channel_create_new();
	if (NULL == l2cap_channel) {
		shell_print(ctx_shell, "No channels available");
		return -ENOMEM;
	}

	*chan = &l2cap_channel->ch.chan;

	return 0;
}

static struct bt_l2cap_server server = {
	.accept		= l2cap_accept,
};

static shell_status_t cmd_register(shell_handle_t shell, int32_t argc, char *argv[])
{
	const char *policy;

	if (server.psm) {
		shell_error(shell, "Already registered");
		return kStatus_SHELL_Error;
	}

	server.psm = strtoul(argv[1], NULL, 16);

	if (argc > 2) {
		server.sec_level = (bt_security_t)strtoul(argv[2], NULL, 10);
	}

	if (argc > 3) {
		policy = argv[3];

		if (!strcmp(policy, "allowlist")) {
			l2cap_policy = L2CAP_POLICY_ALLOWLIST;
		} else if (!strcmp(policy, "16byte_key")) {
			l2cap_policy = L2CAP_POLICY_16BYTE_KEY;
		} else {
			return (shell_status_t)-EINVAL;
		}
	}

	if (bt_l2cap_server_register(&server) < 0) {
		shell_error(shell, "Unable to register psm");
		server.psm = 0U;
		return kStatus_SHELL_Error;
	} else {
		bt_conn_cb_register(&l2cap_conn_callbacks);

		shell_print(shell, "L2CAP psm %u sec_level %u registered",
			    server.psm, server.sec_level);
	}

	return kStatus_SHELL_Success;
}

#if (defined(CONFIG_BT_L2CAP_ECRED) && (CONFIG_BT_L2CAP_ECRED> 0))
static shell_status_t cmd_ecred_reconfigure(shell_handle_t shell, int32_t argc, char *argv[])
{
	struct bt_l2cap_chan *l2cap_ecred_chans[] = { &l2ch_chan[0].ch.chan, NULL };
	uint16_t mtu;
	int err = 0;

	if (!default_conn) {
		shell_error(shell, "Not connected");
		return kStatus_SHELL_Error;
	}

	if (!l2ch_chan[0].ch.chan.conn) {
		shell_error(shell, "Channel not connected");
		return kStatus_SHELL_Error;
	}

	mtu = shell_strtoul(argv[1], 10, &err);
	if (err) {
		shell_error(shell, "Unable to parse MTU (err %d)", err);

		return kStatus_SHELL_Error;
	}

	err = bt_l2cap_ecred_chan_reconfigure(l2cap_ecred_chans, mtu);
	if (err < 0) {
		shell_error(shell, "Unable to reconfigure channel (err %d)", err);
	} else {
		shell_print(shell, "L2CAP reconfiguration pending");
	}

	return (shell_status_t)err;
}

static shell_status_t cmd_ecred_connect(shell_handle_t shell, int32_t argc, char *argv[])
{
	struct bt_l2cap_chan *l2cap_ecred_chans[] = { &l2ch_chan[0].ch.chan, NULL };
	uint16_t psm;
	int err = 0;

	if (!default_conn) {
		shell_error(shell, "Not connected");

		return kStatus_SHELL_Error;
	}

	if (l2ch_chan[0].ch.chan.conn) {
		shell_error(shell, "Channel already in use");

		return kStatus_SHELL_Error;
	}

	psm = shell_strtoul(argv[1], 16, &err);
	if (err) {
		shell_error(shell, "Unable to parse PSM (err %d)", err);

		return (shell_status_t)err;
	}

	if (argc > 2) {
		int sec;

		sec = shell_strtoul(argv[2], 10, &err);
		if (err) {
			shell_error(shell, "Unable to parse security level (err %d)", err);

			return (shell_status_t)err;
		}


		l2ch_chan[0].ch.required_sec_level = (bt_security_t)sec;
	}

	err = bt_l2cap_ecred_chan_connect(default_conn, l2cap_ecred_chans, psm);
	if (err < 0) {
		shell_error(shell, "Unable to connect to psm %u (err %d)", psm,
			    err);
	} else {
		shell_print(shell, "L2CAP connection pending");
	}

	return (shell_status_t)err;
}
#endif /* CONFIG_BT_L2CAP_ECRED */

static shell_status_t cmd_connect(shell_handle_t shell, int32_t argc, char *argv[])
{
	struct l2ch *l2cap_channel;
	uint16_t psm;
	int err;

	if (!default_conn) {
		shell_error(shell, "Not connected");
		return kStatus_SHELL_Error;
	}

	l2cap_channel = l2cap_channel_create_new();
	if (NULL == l2cap_channel) {
		shell_error(shell, "Channel already in use");
		return kStatus_SHELL_Error;
	}

	psm = strtoul(argv[1], NULL, 16);

	if (argc > 2) {
		int sec;

		sec = *argv[2] - '0';
#if (defined(CONFIG_BT_L2CAP_DYNAMIC_CHANNEL) && (CONFIG_BT_L2CAP_DYNAMIC_CHANNEL > 0))
		l2cap_channel->ch.required_sec_level = (bt_security_t)sec;
#endif
	}

	err = bt_l2cap_chan_connect(default_conn, &l2cap_channel->ch.chan, psm);
	if (err < 0) {
		shell_error(shell, "Unable to connect to psm %u (err %d)", psm,
			    err);
	} else {
		shell_print(shell, "L2CAP connection pending");
	}

	return (shell_status_t)err;
}

static shell_status_t cmd_disconnect(shell_handle_t shell, int32_t argc, char *argv[])
{
	struct l2ch *l2cap_channel;
	int err;

	l2cap_channel = l2cap_channel_lookup_conn(default_conn);
	if (NULL == l2cap_channel) {
		shell_error(shell, "Channel is not found");
		return kStatus_SHELL_Error;
	}

	err = bt_l2cap_chan_disconnect(&l2cap_channel->ch.chan);
	if (err) {
		shell_print(shell, "Unable to disconnect: %u", -err);
	}

	return (shell_status_t)err;
}

#if (defined(CONFIG_BT_L2CAP_UNBLOCK_SEND) && (CONFIG_BT_L2CAP_UNBLOCK_SEND > 0))
static void unblock_send_timer_cb(struct k_work *work)
{
    struct l2ch *l2cap_channel;
	static uint8_t buf_data[DATA_MTU] = { [0 ... (DATA_MTU - 1)] = 0xff };
	int ret, len = DATA_MTU;
	struct net_buf *buf;
        
	l2cap_channel = l2cap_channel_lookup_conn(default_conn);
	len = MIN(l2cap_channel->ch.tx.mtu, unblock_send_length);
	buf = net_buf_alloc(&data_tx_pool, K_NO_WAIT);
	if (buf) {
		net_buf_reserve(buf, BT_L2CAP_SDU_CHAN_SEND_RESERVE);
		net_buf_add_mem(buf, buf_data, len);

		ret = bt_l2cap_chan_send(&l2cap_channel->ch.chan, buf);
        if((!unblock_send_start_flag) && (tx_throughput_print_flag))
        {
        	first_tx_inst = l2cap_get_current_relative_time();
            unblock_send_start_flag = true;
        } 
		if (ret >= 0) {
			unblock_send_count--;
		}
        else {
			net_buf_unref(buf);     
		}
	}
    k_work_reschedule(&unblock_send_timer,30);
    if (unblock_send_count == 0) {
		k_work_cancel_delayable(&unblock_send_timer);
		if(tx_throughput_print_flag)
		{
			/* Get last instance of Transmission */
      		last_tx_inst = l2cap_get_current_relative_time();
      		unblock_send_start_flag = false;
      		/* Print Statistics here */
      		l2cap_display_tx_stat();

      		/* Reset Transient Variables here */
      		first_tx_inst = L2CAP_RELATIVE_TIME_INIT_VALUE;
		}
		tx_throughput_print_flag = false;
	}
}

static shell_status_t cmd_unblock_send(shell_handle_t shell, int32_t argc, char *argv[])
{
	if (argc > 1) {
		unblock_send_count = strtoul(argv[1], NULL, 10);
		if (unblock_send_count <= 0) {
			shell_print(shell,
			"Number should be greater than 0");
			return kStatus_SHELL_Error;
		}
        req_data_tx_count = unblock_send_count;
	}
	if (argc > 2) {
		unblock_send_length = strtoul(argv[2], NULL, 10);
        req_data_tx_chunk_len = unblock_send_length;
		if (unblock_send_length > DATA_MTU) {
			shell_print(shell,
			"Length exceeds TX MTU for the channel");
			return kStatus_SHELL_Error;
		}
	}
	if (argc > 3) {
		tx_throughput_print_flag = strtoul(argv[3], NULL, 10);
	}
    if(!unblock_send_timer_initialized)
    {
        k_work_init_delayable(&unblock_send_timer, unblock_send_timer_cb);
        unblock_send_timer_initialized = true;
    }

    k_work_schedule(&unblock_send_timer, 30);
	return kStatus_SHELL_Success;
}

static shell_status_t cmd_rx_calculate_tput(shell_handle_t shell, int32_t argc, char *argv[])
{
	if (argc > 1) {
		exp_data_rx_count = strtoul(argv[1], NULL, 10);
		if (exp_data_rx_count <= 0) {
			shell_print(shell,
			"Number should be greater than 0");
			return kStatus_SHELL_Error;
		}
	}
	if (argc > 2) {
		exp_data_rx_chunk_len = strtoul(argv[2], NULL, 10);
		if (exp_data_rx_chunk_len > DATA_MTU) {
			shell_print(shell,
			"Length exceeds TX MTU for the channel");
			return kStatus_SHELL_Error;
		}
	}
	if (argc > 3) {
		rx_throughput_print_flag = strtoul(argv[3], NULL, 10);
	}

	return kStatus_SHELL_Success;
}
#endif

static shell_status_t cmd_send(shell_handle_t shell, int32_t argc, char *argv[])
{
	struct l2ch *l2cap_channel;
	static uint8_t buf_data[DATA_MTU] = { [0 ... (DATA_MTU - 1)] = 0xff };
	int ret, len, count = 1;
	struct net_buf *buf;

	if (argc > 1) {
		count = strtoul(argv[1], NULL, 10);
	}

	l2cap_channel = l2cap_channel_lookup_conn(default_conn);
	if (NULL == l2cap_channel) {
		shell_error(shell, "Channel is not found");
		return kStatus_SHELL_Error;
	}

	len = MIN(l2cap_channel->ch.tx.mtu, DATA_MTU - BT_L2CAP_CHAN_SEND_RESERVE);

	while (count--) {
		shell_print(shell, "Rem %d", count);
		buf = net_buf_alloc(&data_tx_pool, BT_SECONDS(2));
		if (!buf) {
			if (l2ch_chan[0].ch.state != BT_L2CAP_CONNECTED) {
				shell_print(shell, "Channel disconnected, stopping TX");

				return kStatus_SHELL_Error;
			}
			shell_print(shell, "Allocation timeout, stopping TX");

			return kStatus_SHELL_Error;
		}
		net_buf_reserve(buf, BT_L2CAP_SDU_CHAN_SEND_RESERVE);

		net_buf_add_mem(buf, buf_data, len);
		ret = bt_l2cap_chan_send(&l2cap_channel->ch.chan, buf);
		if (ret < 0) {
			shell_print(shell, "Unable to send: %d", -ret);
			net_buf_unref(buf);
			return kStatus_SHELL_Error;
		}
	}

	return kStatus_SHELL_Success;
}

static shell_status_t cmd_recv(shell_handle_t shell, int32_t argc, char *argv[])
{
	if (argc > 1) {
		l2cap_recv_delay_ms = strtoul(argv[1], NULL, 10);
	} else {
		shell_print(shell, "l2cap receive delay: %u ms",
			    l2cap_recv_delay_ms);
	}

	return kStatus_SHELL_Success;
}

static shell_status_t cmd_metrics(shell_handle_t shell, int32_t argc, char *argv[])
{
	const char *action;

	if (argc < 2) {
		shell_print(shell, "l2cap rate: %u bps.", l2cap_rate);

		return kStatus_SHELL_Success;
	}

	action = argv[1];

	if (!strcmp(action, "on")) {
		metrics = true;
	} else if (!strcmp(action, "off")) {
		metrics = false;
	} else {
		shell_help(shell);
		return kStatus_SHELL_Success;
	}

	shell_print(shell, "l2cap metrics %s.", action);
	return kStatus_SHELL_Success;
}

static shell_status_t cmd_allowlist_add(shell_handle_t shell, int32_t argc, char *argv[])
{
	int i;

	if (!default_conn) {
		shell_error(shell, "Not connected");
		return kStatus_SHELL_Success;
	}

	for (i = 0; i < ARRAY_SIZE(l2cap_allowlist); i++) {
		if (l2cap_allowlist[i] == NULL) {
			l2cap_allowlist[i] = bt_conn_ref(default_conn);
			return kStatus_SHELL_Success;
		}
	}

	return (shell_status_t)-ENOMEM;
}

static shell_status_t cmd_allowlist_remove(shell_handle_t shell, int32_t argc, char *argv[])
{
	if (!default_conn) {
		shell_error(shell, "Not connected");
		return kStatus_SHELL_Success;
	}

	l2cap_allowlist_remove(default_conn, 0);

	return kStatus_SHELL_Success;
}

#define HELP_NONE "[none]"

SHELL_STATIC_SUBCMD_SET_CREATE(allowlist_cmds,
	SHELL_CMD_ARG(add, NULL, HELP_NONE, cmd_allowlist_add, 1, 0),
	SHELL_CMD_ARG(remove, NULL, HELP_NONE, cmd_allowlist_remove, 1, 0),
	SHELL_SUBCMD_SET_END
);

SHELL_STATIC_SUBCMD_SET_CREATE(l2cap_cmds,
	SHELL_CMD_ARG(connect, NULL, "<psm> [sec_level]", cmd_connect, 2, 1),
	SHELL_CMD_ARG(disconnect, NULL, HELP_NONE, cmd_disconnect, 1, 0),
	SHELL_CMD_ARG(metrics, NULL, "<value on, off>", cmd_metrics, 2, 0),
	SHELL_CMD_ARG(recv, NULL, "[delay (in milliseconds)", cmd_recv, 1, 1),
	SHELL_CMD_ARG(register, NULL, "<psm> [sec_level] "
		      "[policy: allowlist, 16byte_key]", cmd_register, 2, 2),
	SHELL_CMD_ARG(send, NULL, "<number of packets>", cmd_send, 2, 0),
	SHELL_CMD_ARG(allowlist, allowlist_cmds, HELP_NONE, NULL, 1, 0),
#if (defined(CONFIG_BT_L2CAP_ECRED) && (CONFIG_BT_L2CAP_ECRED> 0))
	SHELL_CMD_ARG(ecred-connect, NULL, "<psm (hex)> [sec_level (dec)]",
		cmd_ecred_connect, 2, 1),
	SHELL_CMD_ARG(ecred-reconfigure, NULL, "<mtu (dec)>",
		cmd_ecred_reconfigure, 1, 1),
#endif /* CONFIG_BT_L2CAP_ECRED */
#if (defined(CONFIG_BT_L2CAP_UNBLOCK_SEND) && (CONFIG_BT_L2CAP_UNBLOCK_SEND > 0))
    SHELL_CMD_ARG(unblock_send, NULL, "<number of packets> <length of packet(s)> [enable/disable the throughput printing 1, 0]",
		cmd_unblock_send, 3, 1),
	SHELL_CMD_ARG(rx_calculate_tput, NULL, "<number of packets> <length of packet(s)> [enable/disable the throughput printing 1, 0]",
		cmd_rx_calculate_tput, 3, 1),
#endif /* CONFIG_BT_L2CAP_UNBLOCK_SEND */
	SHELL_SUBCMD_SET_END
);

static shell_status_t cmd_l2cap(shell_handle_t shell, int32_t argc, char *argv[])
{
	if (argc == 1) {
		shell_help(shell);
		/* shell returns 1 when help is printed */
		return kStatus_SHELL_PrintCmdHelp;
	}

	shell_error(shell, "%s unknown parameter: %s", argv[0], argv[1]);

	return kStatus_SHELL_Error;
}

SHELL_CMD_REGISTER(l2cap, l2cap_cmds, "Bluetooth L2CAP shell commands",
		       cmd_l2cap, 1, 1);

void bt_ShellL2capInit(shell_handle_t shell)
{
    osa_status_t ret;

    for (int i = 0;i < MAX_L2CAP_CHANNEL;i++)
    {
        l2ch_chan[i].ch.chan.ops = &l2cap_ops;
        l2ch_chan[i].ch.rx.mtu = DATA_MTU;
    }

    if ((shell_status_t)kStatus_Success != SHELL_RegisterCommand(shell, &g_shellCommandl2cap))
    {
        shell_print(shell, "Shell register command %s failed!", g_shellCommandl2cap.pcCommand);
    }
    else
    {
        ret = OSA_MsgQCreate((osa_msgq_handle_t)l2cap_recv_fifo_handle, CONFIG_BT_MSG_QUEUE_COUNT, sizeof(void *));
        if (KOSA_StatusSuccess == ret)
        {
            l2cap_recv_fifo = (osa_msgq_handle_t)l2cap_recv_fifo_handle;
        }
        else
        {
            shell_print(shell, "Message queue of %s create failed (%d)!", g_shellCommandl2cap.pcCommand, ret);
        }
    }
}
