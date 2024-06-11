/** @file
 *  @brief Internal APIs for Bluetooth L2CAP handling.
 */

/*
 * Copyright 2021 NXP
 * Copyright (c) 2015-2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __L2CAP_INTERNAL_H__
#define __L2CAP_INTERNAL_H__

#include <bluetooth/l2cap.h>


enum l2cap_conn_list_action {
	BT_L2CAP_CHAN_LOOKUP,
	BT_L2CAP_CHAN_DETACH,
};

#define BT_L2CAP_CID_BR_SIG             0x0001
#define BT_L2CAP_CID_ATT                0x0004
#define BT_L2CAP_CID_LE_SIG             0x0005
#define BT_L2CAP_CID_SMP                0x0006
#define BT_L2CAP_CID_BR_SMP             0x0007

#define BT_L2CAP_PSM_RFCOMM             0x0003

STRUCT_PACKED_PRE
struct bt_l2cap_hdr {
	uint16_t len;
	uint16_t cid;
} STRUCT_PACKED_POST;
STRUCT_PACKED_PRE
struct bt_l2cap_sig_hdr {
	uint8_t  code;
	uint8_t  ident;
	uint16_t len;
} STRUCT_PACKED_POST;

#define BT_L2CAP_REJ_NOT_UNDERSTOOD     0x0000
#define BT_L2CAP_REJ_MTU_EXCEEDED       0x0001
#define BT_L2CAP_REJ_INVALID_CID        0x0002

#define BT_L2CAP_CMD_REJECT             0x01
STRUCT_PACKED_PRE
struct bt_l2cap_cmd_reject {
	uint16_t reason;
	uint8_t  data[0];
} STRUCT_PACKED_POST;
STRUCT_PACKED_PRE
struct bt_l2cap_cmd_reject_cid_data {
	uint16_t scid;
	uint16_t dcid;
} STRUCT_PACKED_POST;


#define BT_L2CAP_DISCONN_REQ            0x06
STRUCT_PACKED_PRE
struct bt_l2cap_disconn_req {
	uint16_t dcid;
	uint16_t scid;
} STRUCT_PACKED_POST;

#define BT_L2CAP_DISCONN_RSP            0x07
STRUCT_PACKED_PRE
struct bt_l2cap_disconn_rsp {
	uint16_t dcid;
	uint16_t scid;
} STRUCT_PACKED_POST;


#define BT_L2CAP_CONN_PARAM_REQ         0x12
STRUCT_PACKED_PRE
struct bt_l2cap_conn_param_req {
	uint16_t min_interval;
	uint16_t max_interval;
	uint16_t latency;
	uint16_t timeout;
} STRUCT_PACKED_POST;

#define BT_L2CAP_CONN_PARAM_ACCEPTED    0x0000
#define BT_L2CAP_CONN_PARAM_REJECTED    0x0001

#define BT_L2CAP_CONN_PARAM_RSP         0x13
STRUCT_PACKED_PRE
struct bt_l2cap_conn_param_rsp {
	uint16_t result;
} STRUCT_PACKED_POST;

#define BT_L2CAP_LE_CONN_REQ            0x14
STRUCT_PACKED_PRE
struct bt_l2cap_le_conn_req {
	uint16_t psm;
	uint16_t scid;
	uint16_t mtu;
	uint16_t mps;
	uint16_t credits;
} STRUCT_PACKED_POST;

/* valid results in conn response on LE */
#define BT_L2CAP_LE_SUCCESS             0x0000
#define BT_L2CAP_LE_ERR_PSM_NOT_SUPP    0x0002
#define BT_L2CAP_LE_ERR_NO_RESOURCES    0x0004
#define BT_L2CAP_LE_ERR_AUTHENTICATION  0x0005
#define BT_L2CAP_LE_ERR_AUTHORIZATION   0x0006
#define BT_L2CAP_LE_ERR_KEY_SIZE        0x0007
#define BT_L2CAP_LE_ERR_ENCRYPTION      0x0008
#define BT_L2CAP_LE_ERR_INVALID_SCID    0x0009
#define BT_L2CAP_LE_ERR_SCID_IN_USE     0x000A
#define BT_L2CAP_LE_ERR_UNACCEPT_PARAMS 0x000B
#define BT_L2CAP_LE_ERR_INVALID_PARAMS  0x000C
#define BT_L2CAP_LE_ERR_NO_FURTHER_INFO_AVAL    0x000D
#define BT_L2CAP_LE_ERR_AUTHENTICATION_PENDING  0x000E
#define BT_L2CAP_LE_ERR_AUTHORIZATION_PENDING   0x000F

#define BT_L2CAP_LE_CONN_RSP            0x15
STRUCT_PACKED_PRE
struct bt_l2cap_le_conn_rsp {
	uint16_t dcid;
	uint16_t mtu;
	uint16_t mps;
	uint16_t credits;
	uint16_t result;
} STRUCT_PACKED_POST;

#define BT_L2CAP_LE_CREDITS             0x16
STRUCT_PACKED_PRE
struct bt_l2cap_le_credits {
	uint16_t cid;
	uint16_t credits;
} STRUCT_PACKED_POST;

#define BT_L2CAP_ECRED_CONN_REQ         0x17
STRUCT_PACKED_PRE
struct bt_l2cap_ecred_conn_req {
	uint16_t psm;
	uint16_t mtu;
	uint16_t mps;
	uint16_t credits;
	uint16_t scid[0];
} STRUCT_PACKED_POST;

#define BT_L2CAP_ECRED_CONN_RSP         0x18
STRUCT_PACKED_PRE
struct bt_l2cap_ecred_conn_rsp {
	uint16_t mtu;
	uint16_t mps;
	uint16_t credits;
	uint16_t result;
	uint16_t dcid[0];
} STRUCT_PACKED_POST;

#define L2CAP_ECRED_CHAN_MAX_PER_REQ 5

#define BT_L2CAP_ECRED_RECONF_REQ       0x19
STRUCT_PACKED_PRE
struct bt_l2cap_ecred_reconf_req {
	uint16_t mtu;
	uint16_t mps;
	uint16_t scid[0];
} STRUCT_PACKED_POST;

#define BT_L2CAP_RECONF_SUCCESS         0x0000
#define BT_L2CAP_RECONF_INVALID_MTU     0x0001
#define BT_L2CAP_RECONF_INVALID_MPS     0x0002
#define BT_L2CAP_RECONF_INVALID_CID     0x0003
#define BT_L2CAP_RECONF_OTHER_UNACCEPT  0x0004

#define BT_L2CAP_ECRED_RECONF_RSP       0x1a
STRUCT_PACKED_PRE
struct bt_l2cap_ecred_reconf_rsp {
	uint16_t result;
} STRUCT_PACKED_POST;

struct bt_l2cap_fixed_chan {
	uint16_t		cid;
	int (*accept)(struct bt_conn *conn, struct bt_l2cap_chan **chan);
	bt_l2cap_chan_destroy_t destroy;
};

#define BT_L2CAP_CHANNEL_DEFINE(_name, _cid, _accept, _destroy)         \
	const STRUCT_SECTION_ITERABLE(bt_l2cap_fixed_chan, (_name)) = { \
				.cid = (_cid),                            \
				.accept = (_accept),                      \
				.destroy = (_destroy),                    \
			}

/* Notify L2CAP channels of a new connection */
void bt_l2cap_connected(struct bt_conn *conn);

/* Notify L2CAP channels of a disconnect event */
void bt_l2cap_disconnected(struct bt_conn *conn);

/* Add channel to the connection */
void bt_l2cap_chan_add(struct bt_conn *conn, struct bt_l2cap_chan *chan,
		       bt_l2cap_chan_destroy_t destroy);

/* Remove channel from the connection */
void bt_l2cap_chan_remove(struct bt_conn *conn, struct bt_l2cap_chan *ch);

/* Delete channel */
void bt_l2cap_chan_del(struct bt_l2cap_chan *chan);

const char *bt_l2cap_chan_state_str(bt_l2cap_chan_state_t state);

#if (defined(CONFIG_BT_DEBUG_L2CAP) && ((CONFIG_BT_DEBUG_L2CAP) > 0U))
void bt_l2cap_chan_set_state_debug(struct bt_l2cap_chan *chan,
				   bt_l2cap_chan_state_t state,
				   const char *func, int line);
#define bt_l2cap_chan_set_state(_chan, _state) \
	bt_l2cap_chan_set_state_debug(_chan, _state, __func__, __LINE__)
#else
void bt_l2cap_chan_set_state(struct bt_l2cap_chan *chan,
			     bt_l2cap_chan_state_t state);
#endif /* CONFIG_BT_DEBUG_L2CAP */
struct bt_l2cap_br_fixed_chan {
	uint16_t		cid;
	int (*accept)(struct bt_conn *conn, struct bt_l2cap_chan **chan);
};

#define BT_L2CAP_BR_CHANNEL_DEFINE(_name, _cid, _accept)		\
	const STRUCT_SECTION_ITERABLE(bt_l2cap_br_fixed_chan, _name) = { \
				.cid = _cid,			\
				.accept = _accept,		\
			}

#define BR_CHAN(_ch) CONTAINER_OF(_ch, struct bt_l2cap_br_chan, chan)

        
/*
 * Notify L2CAP channels of a change in encryption state passing additionally
 * HCI status of performed security procedure.
 */
void bt_l2cap_security_changed(struct bt_conn *conn, uint8_t hci_status);

/* Prepare an L2CAP PDU to be sent over a connection */
struct net_buf *bt_l2cap_create_pdu_timeout(struct net_buf_pool *pool,
					    size_t reserve,
					    size_t timeout);

#define bt_l2cap_create_pdu(_pool, _reserve) \
	bt_l2cap_create_pdu_timeout(_pool, _reserve, osaWaitForever_c)

/* Prepare a L2CAP Response PDU to be sent over a connection */
struct net_buf *bt_l2cap_create_rsp(struct net_buf *buf, size_t reserve);

/* Send L2CAP PDU over a connection
 *
 * Buffer ownership is transferred to stack in case of success.
 */
int bt_l2cap_send_cb(struct bt_conn *conn, uint16_t cid, struct net_buf *buf,
		     bt_conn_tx_cb_t cb, void *user_data);

static inline int bt_l2cap_send(struct bt_conn *conn, uint16_t cid,
				struct net_buf *buf)
{
	return bt_l2cap_send_cb(conn, cid, buf, NULL, NULL);
}

int bt_l2cap_chan_send_cb(struct bt_l2cap_chan *chan, struct net_buf *buf, bt_conn_tx_cb_t cb,
			  void *user_data);

/* Receive a new L2CAP PDU from a connection */
void bt_l2cap_recv(struct bt_conn *conn, struct net_buf *buf, bool complete);

/* Perform connection parameter update request */
int bt_l2cap_update_conn_param(struct bt_conn *conn,
			       const struct bt_le_conn_param *param);

/* Initialize L2CAP and supported channels */
void bt_l2cap_init(void);

/* Lookup channel by Transmission CID */
struct bt_l2cap_chan *bt_l2cap_le_lookup_tx_cid(struct bt_conn *conn,
						uint16_t cid);

/* Lookup channel by Receiver CID */
struct bt_l2cap_chan *bt_l2cap_le_lookup_rx_cid(struct bt_conn *conn,
						uint16_t cid);

struct bt_l2cap_chan *ethermind_find_br_sig_chan(struct bt_conn *conn);

struct bt_l2cap_ecred_cb {
	void (*ecred_conn_rsp)(struct bt_conn *conn, uint16_t result, uint8_t attempted,
			       uint8_t succeeded, uint16_t psm);
	void (*ecred_conn_req)(struct bt_conn *conn, uint16_t result, uint16_t psm);
};

/* Register callbacks for Enhanced Credit based Flow Control */
void bt_l2cap_register_ecred_cb(const struct bt_l2cap_ecred_cb *cb);
/* Returns a server if it exists for given psm. */
struct bt_l2cap_server *bt_l2cap_server_lookup_psm(uint16_t psm);

int l2cap_br_ecred_init(struct bt_conn *conn,
			       struct bt_l2cap_br_chan *ch, uint16_t psm);

void l2cap_ecbfc_conn_req_recovery(struct bt_conn *conn, struct net_buf *buf);

#endif /* __L2CAP_INTERNAL_H__ */
