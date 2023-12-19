/* @file
 * @brief Internal APIs for PACS handling
 *
 * Copyright (c) 2020 Intel Corporation
 * Copyright (c) 2022 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <bluetooth/audio/audio.h>

#define BT_AUDIO_LOCATION_MASK BIT_MASK(28)

STRUCT_PACKED_PRE
struct bt_pac_codec {
	uint8_t  id;			/* Codec ID */
	uint16_t cid;			/* Company ID */
	uint16_t vid;			/* Vendor specific Codec ID */
} STRUCT_PACKED_POST;

/* TODO: Figure out the capabilities types */
#define BT_CODEC_CAP_PARAMS		0x01
#define BT_CODEC_CAP_DRM		0x0a
#define BT_CODEC_CAP_DRM_VALUE		0x0b

STRUCT_PACKED_PRE
struct bt_pac_codec_capability {
	uint8_t  len;			/* Codec Capability length */
	uint8_t  type;			/* Codec Capability type */
#if 0 /* Walkaround "unknown-size array" error. */
	uint8_t  data[0];		/* Codec Capability data */
#endif
} STRUCT_PACKED_POST;

STRUCT_PACKED_PRE
struct bt_pac_meta {
	uint8_t  len;			/* Metadata Length */
	uint8_t  value[0];		/* Metadata Value */
} STRUCT_PACKED_POST;

STRUCT_PACKED_PRE
struct bt_pac {
	struct bt_pac_codec codec;	/* Codec ID */
	uint8_t  cc_len;		/* Codec Capabilities Length */
#if 0 /* Walkaround "unknown-size array" error. */
	struct bt_pac_codec_capability cc[0]; /* Codec Specific Capabilities */
	struct bt_pac_meta meta[0];	/* Metadata */
#endif
} STRUCT_PACKED_POST;

STRUCT_PACKED_PRE
struct bt_pacs_read_rsp {
	uint8_t  num_pac;		/* Number of PAC Records*/
#if 0 /* Walkaround "unknown-size array" error. */
	struct bt_pac pac[0];
#endif
} STRUCT_PACKED_POST;

STRUCT_PACKED_PRE
struct bt_pacs_context {
	uint16_t  snk;
	uint16_t  src;
} STRUCT_PACKED_POST;

void bt_pacs_add_capability(enum bt_audio_dir dir);
void bt_pacs_remove_capability(enum bt_audio_dir dir);
int bt_pacs_available_contexts_changed(void);
bool bt_pacs_context_available(enum bt_audio_dir dir, uint16_t context);
