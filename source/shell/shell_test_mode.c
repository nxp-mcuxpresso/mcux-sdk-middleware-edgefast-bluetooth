/*
 * Copyright 2021-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdlib.h>
#include <string.h>
#include <porting.h>
#include "fsl_shell.h"

#include <sys/byteorder.h>

#include <bluetooth/hci.h>
#include <bluetooth/hci_vs.h>
#include <bluetooth/conn.h>
#include "shell_bt.h"
#include "shell_test_mode.h"

#if (defined(CONFIG_BT_RF_TEST_MODE) && (CONFIG_BT_RF_TEST_MODE > 0))
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/


/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

static void shell_print_response(const struct shell *sh, const uint8_t *data, size_t len)
{
    shell_dump(sh, " HCI Command Response : ");

    while (len > 0) {
        len--;
        shell_dump(sh, "%02X ", *data++);
    }
	shell_dump(sh, "\r\n");
}

#if (defined(CONFIG_BT_CLASSIC) && (CONFIG_BT_CLASSIC > 0))

static int bt_enter_test_mode(const struct shell *sh, size_t argc, char **argv)
{
    int err;
    struct net_buf *rsp=NULL;

    shell_print(sh, "Enable device under test mode \r\n");

    err = bt_hci_cmd_send_sync(BT_HCI_OP_ENABLE_TEST_MODE, NULL, &rsp);
    if (err)
    {
        shell_print(sh, "Enable device under test mode command failed (err %d)\n", err);
        return -EINVAL;
    }
    else
    {
    	shell_print_response(sh,rsp->data,rsp->len);
    	net_buf_unref(rsp);
        return 0;
    }
}

static int bt_tx_test(const struct shell *sh, size_t argc, char **argv)
{
    int err = 0;
    long val;
    struct net_buf *buf = NULL;
    struct net_buf *rsp = NULL;

    if (argc < 1)
    {
        shell_print(sh, "the parameter count is wrong\r\n");
    }

    if (argc < 15)
    {
        shell_print(sh, "the parameter count is wrong\r\n");
        shell_print(sh, "Usage: bt_test.tx_test test_scenario[1] hopping_mode[1] tx_channel[1] rx_channel[1] tx_test_interval[1] pkt_type[1] data_length[2] whitening[1] num_pkt[4] tx_pwr[1]\n");
        return -EINVAL;
    }

    val = shell_strtol(argv[5], 16, &err);
    if (err) {
        shell_print(sh, "Invalid tx_test_interval parameter\n");
        return -EINVAL;
    }
    if (val > 0x0E)
    {
        shell_print(sh, "tx_test_interval= %lx\n", val);
        shell_print(sh,"invalid argument parameter for tx_test_interval. The value should be less than or equal to 0x0E\n");
        return -EINVAL;
    }

    struct bt_hci_cp_tx_test *cp;
    buf = bt_hci_cmd_create(BT_HCI_OP_TX_TEST, sizeof(*cp));
    if (buf != NULL)
    {
        cp = net_buf_add(buf, sizeof(*cp));
        /*Set to default*/
        cp->rx_on_start = 0x80;
        shell_print(sh, "rx_on_start default set to=%x\n",cp->rx_on_start);

        cp->synt_on_start = 0x80;
        shell_print(sh, "synt_on_start default set to=%x\n",cp->synt_on_start);

        cp->tx_on_start = 0x80;
        shell_print(sh, "tx_on_start default set to=%x\n",cp->tx_on_start);

        cp->phd_off_start = 0x80;
        shell_print(sh, "phd_off_start default set to=%x\n",cp->phd_off_start);

        val = shell_strtol(argv[1], 16, &err);
        if (err || val < 0 || val > UINT8_MAX) {
            shell_print(sh, "Invalid test_scenario parameter\n");
            net_buf_unref(buf);
            return -EINVAL;
        }
        cp->test_scenario = (uint8_t)val;
        shell_print(sh, "test_scenario= %x\n", cp->test_scenario);

        val = shell_strtol(argv[2], 16, &err);
        if (err || val < 0 || val > UINT8_MAX) {
            shell_print(sh, "Invalid hopping_mode parameter\n");
            net_buf_unref(buf);
            return -EINVAL;
        }
        cp->hopping_mode = (uint8_t)val;
        shell_print(sh, "hopping_mode= %x\n", cp->hopping_mode);

        val = shell_strtol(argv[3], 16, &err);
        if (err || val < 0 || val > UINT8_MAX) {
            shell_print(sh, "Invalid tx_channel parameter\n");
            net_buf_unref(buf);
            return -EINVAL;
        }
        cp->tx_channel = (uint8_t)val;
        shell_print(sh, "tx_channel= %x\n", cp->tx_channel);

        val = shell_strtol(argv[4], 16, &err);
        if (err || val < 0 || val > UINT8_MAX) {
            shell_print(sh, "Invalid rx_channel parameter\n");
            net_buf_unref(buf);
            return -EINVAL;
        }
        cp->rx_channel = (uint8_t)val;
        shell_print(sh, "rx_channel= %x\n", cp->rx_channel);

        val = shell_strtol(argv[5], 16, &err);
        if (err || val < 0 || val > UINT8_MAX) {
            shell_print(sh, "Invalid tx_test_interval parameter\n");
            net_buf_unref(buf);
            return -EINVAL;
        }
        cp->tx_test_interval = (uint8_t)val;
        shell_print(sh, "tx_test_interval= %x\n", cp->tx_test_interval);

        val = shell_strtol(argv[6], 16, &err);
        if (err || val < 0 || val > UINT8_MAX) {
            shell_print(sh, "Invalid pkt_type parameter\n");
            net_buf_unref(buf);
            return -EINVAL;
        }
        cp->pkt_type = (uint8_t)val;
        shell_print(sh, "pkt_type= %x\n", cp->pkt_type);

        val = shell_strtol(argv[7], 16, &err);
        if (err || val < 0 || val > UINT8_MAX) {
            shell_print(sh, "Invalid data_length[0] parameter\n");
            net_buf_unref(buf);
            return -EINVAL;
        }
        cp->data_length[0] = (uint8_t)val;

        val = shell_strtol(argv[8], 16, &err);
        if (err || val < 0 || val > UINT8_MAX) {
            shell_print(sh, "Invalid data_length[1] parameter\n");
            net_buf_unref(buf);
            return -EINVAL;
        }
        cp->data_length[1] = (uint8_t)val;
        shell_print(sh, "data_length= %x %x\n", cp->data_length[0],cp->data_length[1]);

        val = shell_strtol(argv[9], 16, &err);
        if (err || val < 0 || val > UINT8_MAX) {
            shell_print(sh, "Invalid whitening parameter\n");
            net_buf_unref(buf);
            return -EINVAL;
        }
        cp->whitening = (uint8_t)val;
        shell_print(sh, "whitening= %x\n", cp->whitening);

        val = shell_strtol(argv[10], 16, &err);
        if (err || val < 0 || val > UINT8_MAX) {
            shell_print(sh, "Invalid num_pkt[0] parameter\n");
            net_buf_unref(buf);
            return -EINVAL;
        }
        cp->num_pkt[0] = (uint8_t)val;

        val = shell_strtol(argv[11], 16, &err);
        if (err || val < 0 || val > UINT8_MAX) {
            shell_print(sh, "Invalid num_pkt[1] parameter\n");
            net_buf_unref(buf);
            return -EINVAL;
        }
        cp->num_pkt[1] = (uint8_t)val;

        val = shell_strtol(argv[12], 16, &err);
        if (err || val < 0 || val > UINT8_MAX) {
            shell_print(sh, "Invalid num_pkt[2] parameter\n");
            net_buf_unref(buf);
            return -EINVAL;
        }
        cp->num_pkt[2] = (uint8_t)val;

        val = shell_strtol(argv[13], 16, &err);
        if (err || val < 0 || val > UINT8_MAX) {
            shell_print(sh, "Invalid num_pkt[3] parameter\n");
            net_buf_unref(buf);
            return -EINVAL;
        }
        cp->num_pkt[3] = (uint8_t)val;
        shell_print(sh, "num_pkt= %x %x %x %x\n", cp->num_pkt[0],cp->num_pkt[1],cp->num_pkt[2],cp->num_pkt[3]);

        val = shell_strtol(argv[14], 16, &err);
        if (err || val < INT8_MIN || val > INT8_MAX) {
            shell_print(sh, "Invalid tx_pwr parameter\n");
            net_buf_unref(buf);
            return -EINVAL;
        }
        cp->tx_pwr = (uint8_t)val;
        shell_print(sh, "tx_pwr= %x\n", cp->tx_pwr);

        err = bt_hci_cmd_send_sync(BT_HCI_OP_TX_TEST, buf, &rsp);
    }
    else
    {
        err = -ENOBUFS;
        shell_print(sh, "No buffer space available\r\n");
    }

    if (err)
    {
        shell_print(sh, "BR/EDR transmitter test command failed (err %d)\n", err);
        if (rsp) {
            net_buf_unref(rsp);
        }
        return -EINVAL;
    }
    else
    {
        shell_print_response(sh,rsp->data,rsp->len);
        net_buf_unref(rsp);
        return 0;
    }
}

static int bt_rx_test(const struct shell *sh, size_t argc, char **argv)
{
    int err = 0;
    long val;
    struct net_buf *buf = NULL;
    struct net_buf *rsp = NULL;

    if (argc < 1)
    {
        shell_print(sh, "the parameter count is wrong\r\n");
    }

    if (argc < 18)
    {
        shell_print(sh, "the parameter count is wrong\r\n");
        shell_print(sh, "Usage: bt_test.rx_test test_scenario[1] tx_channel[1] rx_channel[1] pkt_type[1] num_pkt[4] data_length[2] tx_addr[6] report_err_pkt[1]\r\n");
        return -EINVAL;
    }

    uint8_t index;
    struct bt_hci_cp_rx_test *cp;
    buf = bt_hci_cmd_create(BT_HCI_OP_RX_TEST, sizeof(*cp));
    if (buf != NULL)
    {
        cp = net_buf_add(buf, sizeof(*cp));

        val = shell_strtol(argv[1], 16, &err);
        if (err || val < 0 || val > UINT8_MAX) {
            shell_print(sh, "Invalid test_scenario parameter\n");
            net_buf_unref(buf);
            return -EINVAL;
        }
        cp->test_scenario = (uint8_t)val;
        shell_print(sh, "test_scenario= %x\n", cp->test_scenario);

        val = shell_strtol(argv[2], 16, &err);
        if (err || val < 0 || val > UINT8_MAX) {
            shell_print(sh, "Invalid tx_channel parameter\n");
            net_buf_unref(buf);
            return -EINVAL;
        }
        cp->tx_channel = (uint8_t)val;
        shell_print(sh, "tx_channel= %x\n", cp->tx_channel);

        val = shell_strtol(argv[3], 16, &err);
        if (err || val < 0 || val > UINT8_MAX) {
            shell_print(sh, "Invalid rx_channel parameter\n");
            net_buf_unref(buf);
            return -EINVAL;
        }
        cp->rx_channel = (uint8_t)val;
        shell_print(sh, "rx_channel= %x\n", cp->rx_channel);

        val = shell_strtol(argv[4], 16, &err);
        if (err || val < 0 || val > UINT8_MAX) {
            shell_print(sh, "Invalid pkt_type parameter\n");
            net_buf_unref(buf);
            return -EINVAL;
        }
        cp->pkt_type = (uint8_t)val;
        shell_print(sh, "pkt_type= %x\n", cp->pkt_type);

        val = shell_strtol(argv[5], 16, &err);
        if (err || val < 0 || val > UINT8_MAX) {
            shell_print(sh, "Invalid num_pkt[0] parameter\n");
            net_buf_unref(buf);
            return -EINVAL;
        }
        cp->num_pkt[0] = (uint8_t)val;

        val = shell_strtol(argv[6], 16, &err);
        if (err || val < 0 || val > UINT8_MAX) {
            shell_print(sh, "Invalid num_pkt[1] parameter\n");
            net_buf_unref(buf);
            return -EINVAL;
        }
        cp->num_pkt[1] = (uint8_t)val;

        val = shell_strtol(argv[7], 16, &err);
        if (err || val < 0 || val > UINT8_MAX) {
            shell_print(sh, "Invalid num_pkt[2] parameter\n");
            net_buf_unref(buf);
            return -EINVAL;
        }
        cp->num_pkt[2] = (uint8_t)val;

        val = shell_strtol(argv[8], 16, &err);
        if (err || val < 0 || val > UINT8_MAX) {
            shell_print(sh, "Invalid num_pkt[3] parameter\n");
            net_buf_unref(buf);
            return -EINVAL;
        }
        cp->num_pkt[3] = (uint8_t)val;
        shell_print(sh, "num_pkt= %x %x %x %x\n", cp->num_pkt[0], cp->num_pkt[1], cp->num_pkt[2], cp->num_pkt[3]);

        val = shell_strtol(argv[9], 16, &err);
        if (err || val < 0 || val > UINT8_MAX) {
            shell_print(sh, "Invalid data_length[0] parameter\n");
            net_buf_unref(buf);
            return -EINVAL;
        }
        cp->data_length[0] = (uint8_t)val;

        val = shell_strtol(argv[10], 16, &err);
        if (err || val < 0 || val > UINT8_MAX) {
            shell_print(sh, "Invalid data_length[1] parameter\n");
            net_buf_unref(buf);
            return -EINVAL;
        }
        cp->data_length[1] = (uint8_t)val;
        shell_print(sh, "data_length= %x %x\n", cp->data_length[0], cp->data_length[1]);

        /*Set to default*/
        cp->tx_am_addr = 0x01;
        shell_print(sh, "tx_am_addr default set to= %x\n", cp->tx_am_addr);

        for (index = 0; index < BT_BD_ADDR_SIZE; index++)
        {
            val = shell_strtol(argv[11 + index], 16, &err);
            if (err || val < 0 || val > UINT8_MAX) {
                shell_print(sh, "Invalid tx_addr[%d] parameter\n", index);
                net_buf_unref(buf);
                return -EINVAL;
            }
            cp->tx_addr[index] = (uint8_t)val;
        }

        shell_print(sh, "tx_addr: ");
        for (index = 0; index < BT_BD_ADDR_SIZE; index++)
        {
            shell_print(sh, "%x ", cp->tx_addr[index]);
        }
        shell_print(sh, "\n");

        val = shell_strtol(argv[17], 16, &err);
        if (err || val < 0 || val > UINT8_MAX) {
            shell_print(sh, "Invalid report_err_pkt parameter\n");
            net_buf_unref(buf);
            return -EINVAL;
        }
        cp->report_err_pkt = (uint8_t)val;
        shell_print(sh, "report_err_pkt= %x\n", cp->report_err_pkt);

        err = bt_hci_cmd_send_sync(BT_HCI_OP_RX_TEST, buf, &rsp);
    }
    else
    {
        err = -ENOBUFS;
        shell_print(sh, "No buffer space available\r\n");
    }

    if (err)
    {
        shell_print(sh, "BR/EDR receiver test command failed (err %d)\n", err);
        if (rsp) {
            net_buf_unref(rsp);
        }
        return -EINVAL;
    }
    else
    {
        shell_print_response(sh, rsp->data, rsp->len);
        net_buf_unref(rsp);
        return 0;
    }
}

static int bt_reset(const struct shell *sh, size_t argc, char **argv)
{
    int err;

    if (argc < 1)
    {
        shell_print(sh, "the parameter count is wrong\r\n");
    }

    err = bt_hci_cmd_send_sync(BT_HCI_OP_RESET,NULL,NULL);
    if (err)
    {
        shell_print(sh, "HCI reset command failed (err %d)\n", err);
        return -EINVAL;
    }
    else
    {
        shell_print(sh, "API returned success...\r\n");
    	return 0;
    }
}

#endif /* CONFIG_BT_CLASSIC */

static int hci_cmd_interface(const struct shell *sh, size_t argc, char **argv)
{
    int err;
    int parse_err = 0;
    long val;
    struct net_buf *buf = NULL;
    struct net_buf *rsp = NULL;
    struct bt_hci_command command_buffer;

    if (argc < 3)
    {
        shell_print(sh, "the parameter count is wrong\r\n");
        shell_print(sh, "Usage: hci.generic_command ogf[1] ocf[1] params....\n");
        return -EINVAL;
    }

    /* Check param_len overflow before casting to uint8_t */
    if ((argc - 3U) > 255)
    {
        shell_print(sh, "Too many parameters\r\n");
        return -EINVAL;
    }

    val = shell_strtol(argv[1], 16, &parse_err);
    if (parse_err || val < 0 || val > 0x3F)
    {
        shell_print(sh, "Invalid ogf parameter (must be 0x00-0x3F)\n");
        return -EINVAL;
    }
    command_buffer.ogf = (uint8_t)val;

    val = shell_strtol(argv[2], 16, &parse_err);
    if (parse_err || val < 0 || val > 0x3FF)
    {
        shell_print(sh, "Invalid ocf parameter (must be 0x000-0x3FF)\n");
        return -EINVAL;
    }
    command_buffer.ocf = (uint16_t)val;

    command_buffer.opcode = (uint16_t)((command_buffer.ocf & 0x3FFU) | ((uint16_t)command_buffer.ogf << 10U));
    command_buffer.param_len = (uint8_t)(argc - 3U);

    uint8_t *bt_hci_cmd_params = NULL;

    buf = bt_hci_cmd_create(command_buffer.opcode, command_buffer.param_len);
    if (buf != NULL)
    {
        bt_hci_cmd_params = net_buf_add(buf, command_buffer.param_len);

        for (int i = 0; i < command_buffer.param_len; i++)
        {
            val = shell_strtol(argv[i + 3], 16, &parse_err);
            if (parse_err || val < 0 || val > UINT8_MAX)
            {
                shell_print(sh, "Invalid parameter[%d] (must be 0x00-0xFF)\n", i);
                net_buf_unref(buf);
                return -EINVAL;
            }
            bt_hci_cmd_params[i] = (uint8_t)val;
        }

        err = bt_hci_cmd_send_sync(command_buffer.opcode, buf, &rsp);
        if (command_buffer.opcode == BT_HCI_OP_LE_TEST_TX_COUNT)
        {
            shell_dump(sh, "LE TX pkt number: %d\r\n", sys_get_le16(&rsp->data[1]));
        }
    }
    else
    {
        err = -ENOBUFS;
        shell_print(sh, "No buffer space available\r\n");
    }

    if (err)
    {
        shell_print(sh, "HCI command failed (err %d)\n", err);
        if (rsp)
        {
            net_buf_unref(rsp);
        }
        return -EINVAL;
    }
    else
    {
        shell_print_response(sh, rsp->data, rsp->len);
        net_buf_unref(rsp);
        return 0;
    }
}

static int le_set_tx_power(const struct shell *sh, size_t argc, char **argv)
{
    int err = 0;
    long val;
    struct net_buf *buf = NULL, *rsp=NULL;

    if (argc < 3)
    {
        shell_print(sh, "the parameter count is wrong\r\n");
        shell_print(sh, "Usage: le_test.set_tx_power tx_power[1] feloss[2]\n");
        return -EINVAL;
    }

    struct bt_hci_le_config *cp;
    buf = bt_hci_cmd_create(BT_HCI_OP_LE_SET_TX_POWER, sizeof(*cp));
    if (buf != NULL)
    {
        cp = net_buf_add(buf, sizeof(*cp));

        val = shell_strtol(argv[1], 16, &err);
        if (err || val > INT8_MAX || val < INT8_MIN) {
            shell_print(sh, "Invalid tx_power parameter\n");
            net_buf_unref(buf);
            return -EINVAL;
        }

        cp->tx_power = (uint8_t)val;

        val = shell_strtol(argv[2], 16, &err);
        if (err || val < 0 || val > UINT8_MAX) {
            shell_print(sh, "Invalid feloss parameter\n");
            net_buf_unref(buf);
            return -EINVAL;
        }

        cp->feloss = (uint8_t)val;
        if(cp->feloss >= 1)
        {
            cp->feloss = 1;
        }

        shell_print(sh, "tx_power= %x\n", cp->tx_power);
        shell_print(sh, "feloss= %x\n", cp->feloss);

        err = bt_hci_cmd_send_sync(BT_HCI_OP_LE_SET_TX_POWER, buf, &rsp);
    }
    else
    {
        err = -ENOBUFS;
        shell_print(sh, "No buffer space available\r\n");
    }

    if (err)
    {
        shell_print(sh, "LE Set TX Power command failed (err %d)\n", err);
        if (rsp) {
            net_buf_unref(rsp);
        }
        return -EINVAL;
    }
    else
    {
        shell_print_response(sh,rsp->data,rsp->len);
        net_buf_unref(rsp);
        return 0;
    }
}

static int le_tx_test(const struct shell *sh, size_t argc, char **argv)
{
    int err;
    int parse_err = 0;
    long val;
    struct net_buf *buf = NULL;
    struct net_buf *rsp = NULL;

    if (argc < 1)
    {
        shell_print(sh, "the parameter count is wrong\r\n");
        return -EINVAL;
    }

    if (argc < 5)
    {
        shell_print(sh, "the parameter count is wrong\r\n");
        shell_print(sh, "Usage: le_test.tx_test tx_channel[1] test_data_len[1] pkt_payload[1] phy[1]\r\n");
        return -EINVAL;
    }

    struct bt_hci_cp_le_enh_tx_test *cp;
    buf = bt_hci_cmd_create(BT_HCI_OP_LE_ENH_TX_TEST, sizeof(*cp));
    if (buf != NULL)
    {
        cp = net_buf_add(buf, sizeof(*cp));

        val = shell_strtol(argv[1], 16, &parse_err);
        if (parse_err || val < 0 || val > UINT8_MAX) {
            shell_print(sh, "Invalid tx_channel parameter\n");
            net_buf_unref(buf);
            return -EINVAL;
        }
        cp->tx_ch = (uint8_t)val;
        shell_print(sh, "tx_channel= %x\n", cp->tx_ch);

        val = shell_strtol(argv[2], 16, &parse_err);
        if (parse_err || val < 0 || val > UINT8_MAX) {
            shell_print(sh, "Invalid test_data_len parameter\n");
            net_buf_unref(buf);
            return -EINVAL;
        }
        cp->test_data_len = (uint8_t)val;
        shell_print(sh, "test_data_len= %x\n", cp->test_data_len);

        val = shell_strtol(argv[3], 16, &parse_err);
        if (parse_err || val < 0 || val > UINT8_MAX) {
            shell_print(sh, "Invalid pkt_payload parameter\n");
            net_buf_unref(buf);
            return -EINVAL;
        }
        cp->pkt_payload = (uint8_t)val;
        shell_print(sh, "pkt_payload= %x\n", cp->pkt_payload);

        val = shell_strtol(argv[4], 16, &parse_err);
        if (parse_err || val < 0 || val > UINT8_MAX) {
            shell_print(sh, "Invalid phy parameter\n");
            net_buf_unref(buf);
            return -EINVAL;
        }
        cp->phy = (uint8_t)val;
        shell_print(sh, "phy= %x\n", cp->phy);

        err = bt_hci_cmd_send_sync(BT_HCI_OP_LE_ENH_TX_TEST, buf, &rsp);
    }
    else
    {
        err = -ENOBUFS;
        shell_print(sh, "No buffer space available\r\n");
    }

    if (err)
    {
        shell_print(sh, "LE enhanced transmitter test command failed (err %d)\n", err);
        if (rsp) {
            net_buf_unref(rsp);
        }
        return -EINVAL;
    }
    else
    {
        shell_print_response(sh,rsp->data,rsp->len);
        net_buf_unref(rsp);
        return 0;
    }
}

static int le_rx_test(const struct shell *sh, size_t argc, char **argv)
{
    int err;
    int parse_err = 0;
    long val;
    struct net_buf *buf = NULL;
    struct net_buf *rsp = NULL;

    if (argc < 1)
    {
        shell_print(sh, "the parameter count is wrong\r\n");
        return -EINVAL;
    }

    if (argc < 4)
    {
        shell_print(sh, "the parameter count is wrong\r\n");
        shell_print(sh, "Usage: le_test.rx_test rx_channel[1] phy[1] modulation_index[1]\r\n");
        return -EINVAL;
    }

    struct bt_hci_cp_le_enh_rx_test *cp;
    buf = bt_hci_cmd_create(BT_HCI_OP_LE_ENH_RX_TEST, sizeof(*cp));
    if (buf != NULL)
    {
        cp = net_buf_add(buf, sizeof(*cp));

        val = shell_strtol(argv[1], 16, &parse_err);
        if (parse_err || val < 0 || val > UINT8_MAX) {
            shell_print(sh, "Invalid rx_channel parameter\n");
            net_buf_unref(buf);
            return -EINVAL;
        }
        cp->rx_ch = (uint8_t)val;
        shell_print(sh, "rx_channel= %x\n", cp->rx_ch);

        val = shell_strtol(argv[2], 16, &parse_err);
        if (parse_err || val < 0 || val > UINT8_MAX) {
            shell_print(sh, "Invalid phy parameter\n");
            net_buf_unref(buf);
            return -EINVAL;
        }
        cp->phy = (uint8_t)val;
        shell_print(sh, "phy= %x\n", cp->phy);

        val = shell_strtol(argv[3], 16, &parse_err);
        if (parse_err || val < 0 || val > UINT8_MAX) {
            shell_print(sh, "Invalid modulation_index parameter\n");
            net_buf_unref(buf);
            return -EINVAL;
        }
        cp->mod_index = (uint8_t)val;
        shell_print(sh, "modulation_index= %x\n", cp->mod_index);

        err = bt_hci_cmd_send_sync(BT_HCI_OP_LE_ENH_RX_TEST, buf, &rsp);
    }
    else
    {
        err = -ENOBUFS;
        shell_print(sh, "No buffer space available\r\n");
    }

    if (err)
    {
        shell_print(sh, "LE enhanced receiver test command failed (err %d)\n", err);
        if (rsp) {
            net_buf_unref(rsp);
        }
        return -EINVAL;
    }
    else
    {
        shell_print_response(sh,rsp->data,rsp->len);
        net_buf_unref(rsp);
        return 0;
    }
}

static int le_end_test(const struct shell *sh, size_t argc, char **argv)
{
    int err;
    struct net_buf *rsp;
    struct bt_hci_rp_le_test_end *rp;

    if (argc < 1)
    {
        shell_print(sh, "the parameter count is wrong\r\n");
        return -EINVAL;
    }

    err = bt_hci_cmd_send_sync(BT_HCI_OP_LE_TEST_END, NULL, &rsp);
    if (err)
    {
        shell_print(sh, "LE test end command failed (err %d)\n", err);
        return -EINVAL;
    }
    else
    {
        rp = (void *)rsp->data;
        shell_print(sh, "Number of packets received: %d\n", (int)rp->rx_pkt_count);
        net_buf_unref(rsp);
        shell_print(sh, "API returned success...\n");
    	return 0;
    }
}


SHELL_STATIC_SUBCMD_SET_CREATE(hci_cmds,
    SHELL_CMD_ARG(generic_command, NULL, "ogf[1] ocf[1] params....", hci_cmd_interface, 2, 255),
    SHELL_SUBCMD_SET_END
);

SHELL_STATIC_SUBCMD_SET_CREATE(le_cmds,
    SHELL_CMD_ARG(set_tx_power, NULL, "tx_power[1] feloss[2]", le_set_tx_power, 3, 0),
    SHELL_CMD_ARG(tx_test, NULL, "tx_channel[1] data_length[1] payload[1] phy[1]", le_tx_test, 2, 6),
    SHELL_CMD_ARG(rx_test, NULL, "rc_channel[1] phy[1] modulation[1]", le_rx_test, 2, 5),
    SHELL_CMD_ARG(end_test, NULL, "end the le test", le_end_test, 1, 0),
    SHELL_SUBCMD_SET_END
);

#if (defined(CONFIG_BT_CLASSIC) && (CONFIG_BT_CLASSIC > 0))
SHELL_STATIC_SUBCMD_SET_CREATE(br_cmds,
    SHELL_CMD_ARG(enter_test_mode, NULL, "Enable device under test mode", bt_enter_test_mode, 1, 0),
    SHELL_CMD_ARG(tx_test, NULL, "test_scenario[1] hopping_mode[1] tx_channel[1] rx_channel[1] tx_test_interval[1] pkt_type[1] data_length[2] whitening[1] num_pkt[4] tx_pwr[1]", bt_tx_test, 2, 19),
    SHELL_CMD_ARG(rx_test, NULL, "test_scenario[1] tx_channel[1] rx_channel[1] pkt_type[1] num_pkt[4] data_length[2] tx_addr[6] report_err_pkt[1]", bt_rx_test, 2, 19),
    SHELL_CMD_ARG(reset, NULL, " Reset the HCI interface", bt_reset, 1, 0),
    SHELL_SUBCMD_SET_END
);
#endif

static int cmd_bt_test(const struct shell *sh, size_t argc, char **argv)
{
    if (argc == 1) {
        shell_help(sh);
        return SHELL_CMD_HELP_PRINTED;
    }

    shell_error(sh, "%s unknown parameter: %s", argv[0], argv[1]);

    return -EINVAL;
}

#if (defined(CONFIG_BT_CLASSIC) && (CONFIG_BT_CLASSIC > 0))
SHELL_CMD_ARG_REGISTER(bt_test, br_cmds, "Bluetooth BR/EDR test mode commands",
               cmd_bt_test, 1, 17);
#endif
SHELL_CMD_ARG_REGISTER(le_test, le_cmds, "Bluetooth BLE test mode commands",
               cmd_bt_test, 1, 6);

SHELL_CMD_ARG_REGISTER(hci, hci_cmds, "Bluetooth HCI Command interface",
               cmd_bt_test, 1, 255);


void bt_ShellTestModeInit(shell_handle_t shell)
{
    /* Add new command to commands list */
#if (defined(CONFIG_BT_CLASSIC) && (CONFIG_BT_CLASSIC > 0))
    if ((shell_status_t)kStatus_Success != SHELL_RegisterCommand(shell, &g_shellCommandbt_test))
    {
        shell_print(shell, "Shell register command %s failed!", g_shellCommandbt_test.pcCommand);
    }
#endif /* CONFIG_BT_CLASSIC */
    if ((shell_status_t)kStatus_Success != SHELL_RegisterCommand(shell, &g_shellCommandle_test))
    {
        shell_print(shell, "Shell register command %s failed!", g_shellCommandle_test.pcCommand);
    }

    if ((shell_status_t)kStatus_Success != SHELL_RegisterCommand(shell, &g_shellCommandhci))
    {
        shell_print(shell, "Shell register command %s failed!", g_shellCommandle_test.pcCommand);
    }

}
#endif /* CONFIG_BT_RF_TEST_MODE */