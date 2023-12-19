/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __CONFIG_BAPS_H__
#define __CONFIG_BAPS_H__

/*! @brief Unicast Audio function
 */
#ifndef CONFIG_BT_AUDIO_UNICAST
    #define CONFIG_BT_AUDIO_UNICAST 0
#endif

#if (defined(CONFIG_BT_AUDIO_UNICAST) && (CONFIG_BT_AUDIO_UNICAST > 0))
    
    #if (!defined(CONFIG_BT_CONN) || !(CONFIG_BT_CONN > 0))
        #error CONFIG_BT_CONN must be enabled for CONFIG_BT_AUDIO_UNICAST.
    #endif

    #if (!defined(CONFIG_BT_SMP) || !(CONFIG_BT_SMP > 0))
        #error CONFIG_BT_SMP must be enabled for CONFIG_BT_AUDIO_UNICAST.
    #endif

    #if (!defined(CONFIG_BT_ISO_UNICAST) || !(CONFIG_BT_ISO_UNICAST > 0))
        #error CONFIG_BT_ISO_UNICAST must be enabled for CONFIG_BT_AUDIO_UNICAST.
    #endif

#endif /* CONFIG_BT_AUDIO_UNICAST */

/*! @brief Bluetooth Unicast Audio Server Support
 * This option enables support for Bluetooth Unicast Audio Server using Isochronous channels.
 */
#if (defined(CONFIG_BT_AUDIO_UNICAST_SERVER) && (CONFIG_BT_AUDIO_UNICAST_SERVER > 0))

    #if (!defined(CONFIG_BT_PERIPHERAL) || !(CONFIG_BT_PERIPHERAL > 0))
        #error CONFIG_BT_AUDIO_UNICAST_SERVER depands on CONFIG_BT_PERIPHERAL.
    #endif

    #if (!defined(CONFIG_BT_AUDIO_UNICAST) || !(CONFIG_BT_AUDIO_UNICAST > 0))
        #error CONFIG_BT_AUDIO_UNICAST_SERVER depands on CONFIG_BT_AUDIO_UNICAST.
    #endif

    #if (!defined(CONFIG_BT_ISO_PERIPHERAL) || !(CONFIG_BT_ISO_PERIPHERAL > 0))
        #error CONFIG_BT_AUDIO_UNICAST_SERVER depands on CONFIG_BT_ISO_PERIPHERAL.
    #endif

    #if (!defined(CONFIG_BT_GATT_DYNAMIC_DB) || !(CONFIG_BT_GATT_DYNAMIC_DB > 0))
        #error CONFIG_BT_AUDIO_UNICAST_SERVER depands on CONFIG_BT_GATT_DYNAMIC_DB.
    #endif

    #if (!defined(CONFIG_BT_GATT_CACHING) || !(CONFIG_BT_GATT_CACHING > 0))
        #error CONFIG_BT_AUDIO_UNICAST_SERVER depands on CONFIG_BT_GATT_CACHING.
    #endif

    #if (!defined(CONFIG_BT_ASCS) || !(CONFIG_BT_ASCS > 0))
        #error CONFIG_BT_AUDIO_UNICAST_SERVER depands on CONFIG_BT_ASCS.
    #endif

#endif /* CONFIG_BT_AUDIO_UNICAST_SERVER */

/*! @brief Bluetooth Unicast Audio Client Support
 * This option enables support for Bluetooth Unicast Audio Client using Isochronous channels.
 */
#if (defined(CONFIG_BT_AUDIO_UNICAST_CLIENT) && (CONFIG_BT_AUDIO_UNICAST_CLIENT > 0))

    #if (!defined(CONFIG_BT_AUDIO_UNICAST) || !(CONFIG_BT_AUDIO_UNICAST > 0))
        #error CONFIG_BT_AUDIO_UNICAST_CLIENT depands on CONFIG_BT_AUDIO_UNICAST.
    #endif

    #if (!defined(CONFIG_BT_ISO_CENTRAL) || !(CONFIG_BT_ISO_CENTRAL > 0))
        #error CONFIG_BT_AUDIO_UNICAST_CLIENT depands on CONFIG_BT_ISO_CENTRAL.
    #endif

    #if (!defined(CONFIG_BT_CENTRAL) || !(CONFIG_BT_CENTRAL > 0))
        #error CONFIG_BT_AUDIO_UNICAST_CLIENT depands on CONFIG_BT_CENTRAL.
    #endif

    #if (!defined(CONFIG_BT_GATT_CLIENT) || !(CONFIG_BT_GATT_CLIENT > 0))
        #error CONFIG_BT_AUDIO_UNICAST_CLIENT depands on CONFIG_BT_GATT_CLIENT.
    #endif

    #if (!defined(CONFIG_BT_GATT_AUTO_DISCOVER_CCC) || !(CONFIG_BT_GATT_AUTO_DISCOVER_CCC > 0))
        #error CONFIG_BT_AUDIO_UNICAST_CLIENT depands on CONFIG_BT_GATT_AUTO_DISCOVER_CCC.
    #endif

    #if (!defined(CONFIG_BT_GATT_AUTO_UPDATE_MTU) || !(CONFIG_BT_GATT_AUTO_UPDATE_MTU > 0))
        #error CONFIG_BT_AUDIO_UNICAST_CLIENT depands on CONFIG_BT_GATT_AUTO_UPDATE_MTU.
    #endif

#endif /* CONFIG_BT_AUDIO_UNICAST_CLIENT */

/*! @brief Codec Capabilities Data Count
 * This option defines the maximum number of LTV entries a codec can store.
 *
 * Valid range 1 ~ 128
 */
#ifndef CONFIG_BT_CODEC_MAX_DATA_COUNT
    #define CONFIG_BT_CODEC_MAX_DATA_COUNT 5
#endif

/*! @brief Codec Capabilities Data Length
 * This option defines the maximum value length of an LTV entry a codec can store.
 *
 * Valid range 1 ~ 128
 */
#ifndef CONFIG_BT_CODEC_MAX_DATA_LEN
    #define CONFIG_BT_CODEC_MAX_DATA_LEN 4
#endif

/*! @brief Codec Metadata Count
 * This option defines the maximum number of LTV entries a metadata can store.
 *
 * Valid range 1 ~ 128
 */
#ifndef CONFIG_BT_CODEC_MAX_METADATA_COUNT
    #define CONFIG_BT_CODEC_MAX_METADATA_COUNT 2
#endif

/*! @brief Codec Metadata Length
 * This option defines the maximum value length of an LTV entry a metadata can store.
 *
 * Valid range 1 ~ 128
 */
#ifndef CONFIG_BT_CODEC_MAX_METADATA_LEN
    #define CONFIG_BT_CODEC_MAX_METADATA_LEN 4
#endif

#if (defined(CONFIG_BT_AUDIO_UNICAST_CLIENT) && (CONFIG_BT_AUDIO_UNICAST_CLIENT > 0))

/*! @brief Basic Audio Unicast Group count
 * This option sets the number of connected audio groups to support as the unicast client.
 *
 * Valid range 1 ~ CONFIG_BT_ISO_MAX_CIG
 */
#ifndef CONFIG_BT_AUDIO_UNICAST_CLIENT_GROUP_COUNT
    #define CONFIG_BT_AUDIO_UNICAST_CLIENT_GROUP_COUNT CONFIG_BT_ISO_MAX_CIG
#endif

#if (defined(CONFIG_BT_AUDIO_UNICAST_CLIENT_GROUP_COUNT) && (CONFIG_BT_AUDIO_UNICAST_CLIENT_GROUP_COUNT > 0))

    #if (!defined(CONFIG_BT_AUDIO_UNICAST) || !(CONFIG_BT_AUDIO_UNICAST > 0))
        #error CONFIG_BT_AUDIO_UNICAST_CLIENT_GROUP_COUNT depends on CONFIG_BT_AUDIO_UNICAST.
    #endif

#endif

/*! @brief Basic Audio Unicast Group Stream count
 * This option sets the maximum number of streams per unicast group to support.
 *
 * Valid range 1 ~ 31
 * Valid range 1 ~ BT_ISO_MAX_CHAN if BT_ISO_MAX_CHAN < 31
 */
#ifndef CONFIG_BT_AUDIO_UNICAST_CLIENT_GROUP_STREAM_COUNT
    #define CONFIG_BT_AUDIO_UNICAST_CLIENT_GROUP_STREAM_COUNT 1
#endif

#if (defined(CONFIG_BT_AUDIO_UNICAST_CLIENT_GROUP_STREAM_COUNT) && (CONFIG_BT_AUDIO_UNICAST_CLIENT_GROUP_STREAM_COUNT > 0))

    #if (!defined(CONFIG_BT_AUDIO_UNICAST_CLIENT_GROUP_COUNT) || !(CONFIG_BT_AUDIO_UNICAST_CLIENT_GROUP_COUNT > 0))
        #error CONFIG_BT_AUDIO_UNICAST_CLIENT_GROUP_STREAM_COUNT depends on CONFIG_BT_AUDIO_UNICAST_CLIENT_GROUP_COUNT > 0.
    #endif

#endif

/*! @brief Basic Audio Profile PAC count
 * This option enables caching a number of Published Audio Capabilities
 * (PAC) for Basic Audio Profile on a per connection basis.
 *
 * Valid range 0 ~ 4
 */
#ifndef CONFIG_BT_AUDIO_UNICAST_CLIENT_PAC_COUNT
    #define CONFIG_BT_AUDIO_UNICAST_CLIENT_PAC_COUNT 2
#endif

/*! @brief Basic Audio Profile ASE Sink count
 * This option enables caching a number of Audio Stream Endpoint Sink
 * instances for Basic Audio Profile on a per connection basis.
 *
 * Valid range 0 ~ 2
 */
#ifndef CONFIG_BT_AUDIO_UNICAST_CLIENT_ASE_SNK_COUNT
    #define CONFIG_BT_AUDIO_UNICAST_CLIENT_ASE_SNK_COUNT 2
#endif

/*! @brief Basic Audio Profile ASE Source count
 * This option enables caching a number of Audio Stream Endpoint Source
 * instances for Basic Audio Profile on a per connection basis.
 *
 * Valid range 0 ~ 2
 */
#ifndef CONFIG_BT_AUDIO_UNICAST_CLIENT_ASE_SRC_COUNT
    #define CONFIG_BT_AUDIO_UNICAST_CLIENT_ASE_SRC_COUNT 1
#endif

#endif /* CONFIG_BT_AUDIO_UNICAST_CLIENT */

/*! @brief Bluetooth Broadcast Source Audio Support
 * This option enables support for Bluetooth Broadcast Source Audio using
 * Isochronous channels.
 */
#ifndef CONFIG_BT_AUDIO_BROADCAST_SOURCE
    #define CONFIG_BT_AUDIO_BROADCAST_SOURCE 0
#endif

#if (defined(CONFIG_BT_AUDIO_BROADCAST_SOURCE) && (CONFIG_BT_AUDIO_BROADCAST_SOURCE > 0))

    #if (!defined(CONFIG_BT_ISO_BROADCASTER) || !(CONFIG_BT_ISO_BROADCASTER > 0))
        #error CONFIG_BT_AUDIO_BROADCAST_SOURCE depands on CONFIG_BT_ISO_BROADCASTER.
    #endif

#endif /* CONFIG_BT_AUDIO_BROADCAST_SOURCE */

#if (defined(CONFIG_BT_AUDIO_BROADCAST_SOURCE) && (CONFIG_BT_AUDIO_BROADCAST_SOURCE > 0))

#ifndef CONFIG_BT_AUDIO_BROADCAST_SRC_SUBGROUP_COUNT
    #define CONFIG_BT_AUDIO_BROADCAST_SRC_SUBGROUP_COUNT 1
#endif

/*! @brief Basic Audio Broadcaster source count
 * This option sets the number of broadcast sources to support.
 * One broadcast source can send multiple streams
 * (up to BT_AUDIO_BROADCAST_SRC_STREAM_COUNT per broadcast source).
 *
 * Valid range 0 ~ CONFIG_BT_ISO_MAX_BIG
 */
#ifndef CONFIG_BT_AUDIO_BROADCAST_SRC_COUNT
    #define CONFIG_BT_AUDIO_BROADCAST_SRC_COUNT 1
#endif

/*! @brief Basic Audio Broadcast Source Stream count
 * This option sets the maximum number of streams per broadcast source to support.
 *
 * Valid range 0 ~ CONFIG_BT_ISO_MAX_CHAN
 */
#ifndef CONFIG_BT_AUDIO_BROADCAST_SRC_STREAM_COUNT
    #define CONFIG_BT_AUDIO_BROADCAST_SRC_STREAM_COUNT 1
#endif

#endif /* CONFIG_BT_AUDIO_BROADCAST_SOURCE */

/*! @brief Bluetooth Broadcast Sink Audio Support
 * This option enables support for Bluetooth Broadcast Sink Audio using Isochronous channels.
 */
#ifndef CONFIG_BT_AUDIO_BROADCAST_SINK
    #define CONFIG_BT_AUDIO_BROADCAST_SINK 0
#endif

#if (defined(CONFIG_BT_AUDIO_BROADCAST_SINK) && (CONFIG_BT_AUDIO_BROADCAST_SINK > 0))

    #if (!defined(CONFIG_BT_ISO_SYNC_RECEIVER) || !(CONFIG_BT_ISO_SYNC_RECEIVER > 0))
        #error CONFIG_BT_AUDIO_BROADCAST_SINK depands on CONFIG_BT_ISO_SYNC_RECEIVER.
    #endif

    #if (!defined(CONFIG_BT_PERIPHERAL) || !(CONFIG_BT_PERIPHERAL > 0))
        #error CONFIG_BT_AUDIO_BROADCAST_SINK depands on CONFIG_BT_PERIPHERAL.
    #endif

    #if (!defined(CONFIG_BT_PAC_SNK) || !(CONFIG_BT_PAC_SNK > 0))
        #error CONFIG_BT_AUDIO_BROADCAST_SINK depands on CONFIG_BT_PAC_SNK.
    #endif

#endif /* CONFIG_BT_AUDIO_BROADCAST_SINK */

#if (defined(CONFIG_BT_AUDIO_BROADCAST_SINK) && (CONFIG_BT_AUDIO_BROADCAST_SINK > 0))

#ifndef CONFIG_BT_AUDIO_BROADCAST_SNK_SUBGROUP_COUNT
    #define CONFIG_BT_AUDIO_BROADCAST_SNK_SUBGROUP_COUNT 1
#endif

/*! @brief Basic Audio Broadcaster Sink count
 * This option sets the number of broadcast sinks to support.
 * One broadcast sink can receive multiple streams
 * (up to BT_AUDIO_BROADCAST_SNK_STREAM_COUNT per broadcast sink).
 *
 * Valid range 0 ~ CONFIG_BT_ISO_MAX_BIG
 */
#ifndef CONFIG_BT_AUDIO_BROADCAST_SNK_COUNT
    #define CONFIG_BT_AUDIO_BROADCAST_SNK_COUNT 1
#endif

/*! @brief Basic Audio Broadcast Sink Stream count
 * This option sets the maximum number of streams per broadcast sink to support.
 *
 * Valid range 1 ~ CONFIG_BT_ISO_MAX_CHAN
 */
#ifndef CONFIG_BT_AUDIO_BROADCAST_SNK_STREAM_COUNT
    #define CONFIG_BT_AUDIO_BROADCAST_SNK_STREAM_COUNT 1
#endif

#if (defined(CONFIG_BT_AUDIO_BROADCAST_SNK_STREAM_COUNT) && (CONFIG_BT_AUDIO_BROADCAST_SNK_STREAM_COUNT > 0))

    #if (!defined(CONFIG_BT_AUDIO_BROADCAST_SNK_COUNT) || !(CONFIG_BT_AUDIO_BROADCAST_SNK_COUNT > 0))
        #error CONFIG_BT_AUDIO_BROADCAST_SNK_STREAM_COUNT depends on CONFIG_BT_AUDIO_BROADCAST_SNK_COUNT > 0.
    #endif

#endif

#endif /* CONFIG_BT_AUDIO_BROADCAST_SINK */

/*! @brief Bluetooth Audio Stream debug
 * Use this option to enable Bluetooth Audio Stream debug logs for the
 * Bluetooth Audio functionality.
 */
#ifndef CONFIG_BT_AUDIO_DEBUG_STREAM
    #define CONFIG_BT_AUDIO_DEBUG_STREAM 0
#endif

#if (defined(CONFIG_BT_AUDIO_DEBUG_STREAM) && (CONFIG_BT_AUDIO_DEBUG_STREAM > 0))

    #if (!defined(CONFIG_BT_AUDIO_STREAM) || !(CONFIG_BT_AUDIO_STREAM > 0))
        #error CONFIG_BT_AUDIO_DEBUG_STREAM depends on CONFIG_BT_AUDIO_STREAM.
    #endif

#endif

/*! @brief Bluetooth Audio Stream data debug
 * Use this option to enable Bluetooth Audio Stream data debug logs for
 * the Bluetooth Audio functionality. This will enable debug logs for all
 * audio data received and sent.
 */
#ifndef CONFIG_BT_AUDIO_DEBUG_STREAM_DATA
    #define CONFIG_BT_AUDIO_DEBUG_STREAM_DATA 0
#endif

#if (defined(CONFIG_BT_AUDIO_DEBUG_STREAM_DATA) && (CONFIG_BT_AUDIO_DEBUG_STREAM_DATA > 0))

    #if (!defined(CONFIG_BT_AUDIO_DEBUG_STREAM) || !(CONFIG_BT_AUDIO_DEBUG_STREAM > 0))
        #error CONFIG_BT_AUDIO_DEBUG_STREAM_DATA depends on CONFIG_BT_AUDIO_DEBUG_STREAM.
    #endif

#endif

/*! @brief Bluetooth Audio Capabilities debug
 * Use this option to enable Bluetooth Audio Capabilities debug logs for
 * the Bluetooth Audio functionality.
 */
#ifndef CONFIG_BT_AUDIO_DEBUG_CAPABILITIES
    #define CONFIG_BT_AUDIO_DEBUG_CAPABILITIES 0
#endif

#if (defined(CONFIG_BT_AUDIO_DEBUG_CAPABILITIES) && (CONFIG_BT_AUDIO_DEBUG_CAPABILITIES > 0))

    #if (!defined(CONFIG_BT_AUDIO_CAPABILITY) || !(CONFIG_BT_AUDIO_CAPABILITY > 0))
        #error CONFIG_BT_AUDIO_DEBUG_CAPABILITIES depends on CONFIG_BT_AUDIO_CAPABILITY.
    #endif

#endif

/*! @brief Audio Stream Control Service debug
 * Use this option to enable Audio Stream Control Service debug logs for
 * the Bluetooth Audio functionality.
 */
#ifndef CONFIG_BT_DEBUG_ASCS
    #define CONFIG_BT_DEBUG_ASCS 0
#endif

#if (defined(CONFIG_BT_DEBUG_ASCS) && (CONFIG_BT_DEBUG_ASCS > 0))

    #if (!defined(CONFIG_BT_ASCS) || !(CONFIG_BT_ASCS > 0))
        #error CONFIG_BT_DEBUG_ASCS depends on CONFIG_BT_ASCS.
    #endif

#endif

/*! @brief Bluetooth Audio Unicast Server debug
 * Use this option to enable Bluetooth Audio Unicast Server debug logs
 * for the Bluetooth Audio functionality.
 */
#ifndef CONFIG_BT_AUDIO_DEBUG_UNICAST_SERVER
    #define CONFIG_BT_AUDIO_DEBUG_UNICAST_SERVER 0
#endif

#if (defined(CONFIG_BT_AUDIO_DEBUG_UNICAST_SERVER) && (CONFIG_BT_AUDIO_DEBUG_UNICAST_SERVER > 0))

    #if (!defined(CONFIG_BT_AUDIO_UNICAST_SERVER) || !(CONFIG_BT_AUDIO_UNICAST_SERVER > 0))
        #error CONFIG_BT_AUDIO_DEBUG_UNICAST_SERVER depends on CONFIG_BT_AUDIO_UNICAST_SERVER.
    #endif

#endif

/*! @brief Basic Audio Profile debug
 * Use this option to enable Basic Audio Profile debug logs for the
 * Bluetooth Audio functionality.
 */
#ifndef CONFIG_BT_AUDIO_DEBUG_UNICAST_CLIENT
    #define CONFIG_BT_AUDIO_DEBUG_UNICAST_CLIENT 0
#endif

#if (defined(CONFIG_BT_AUDIO_DEBUG_UNICAST_CLIENT) && (CONFIG_BT_AUDIO_DEBUG_UNICAST_CLIENT > 0))

    #if (!defined(CONFIG_BT_AUDIO_UNICAST_CLIENT) || !(CONFIG_BT_AUDIO_UNICAST_CLIENT > 0))
        #error CONFIG_BT_AUDIO_DEBUG_UNICAST_CLIENT depends on CONFIG_BT_AUDIO_UNICAST_CLIENT.
    #endif

#endif

/*! @brief Bluetooth Audio Broadcast Source debug
 * Use this option to enable Bluetooth Audio Broadcast Source debug logs
 * for the Bluetooth Audio functionality.
 */
#ifndef CONFIG_BT_AUDIO_DEBUG_BROADCAST_SOURCE
    #define CONFIG_BT_AUDIO_DEBUG_BROADCAST_SOURCE 0
#endif

#if (defined(CONFIG_BT_AUDIO_DEBUG_BROADCAST_SOURCE) && (CONFIG_BT_AUDIO_DEBUG_BROADCAST_SOURCE > 0))

    #if (!defined(CONFIG_BT_AUDIO_BROADCAST_SOURCE) || !(CONFIG_BT_AUDIO_BROADCAST_SOURCE > 0))
        #error CONFIG_BT_AUDIO_DEBUG_BROADCAST_SOURCE depends on CONFIG_BT_AUDIO_BROADCAST_SOURCE.
    #endif

#endif

/*! @brief Bluetooth Audio Broadcast Sink debug
 * Use this option to enable Bluetooth Audio Broadcast Sink debug logs
 * for the Bluetooth Audio functionality.
 */
#ifndef CONFIG_BT_AUDIO_DEBUG_BROADCAST_SINK
    #define CONFIG_BT_AUDIO_DEBUG_BROADCAST_SINK 0
#endif

#if (defined(CONFIG_BT_AUDIO_DEBUG_BROADCAST_SINK) && (CONFIG_BT_AUDIO_DEBUG_BROADCAST_SINK > 0))

    #if (!defined(CONFIG_BT_AUDIO_BROADCAST_SINK) || !(CONFIG_BT_AUDIO_BROADCAST_SINK > 0))
        #error CONFIG_BT_AUDIO_DEBUG_BROADCAST_SINK depends on CONFIG_BT_AUDIO_BROADCAST_SINK.
    #endif

#endif

#if (defined(CONFIG_BT_ASCS) && (CONFIG_BT_ASCS > 0)) || \
    (defined(CONFIG_BT_AUDIO_UNICAST_CLIENT) && (CONFIG_BT_AUDIO_UNICAST_CLIENT > 0)) || \
    (defined(CONFIG_BT_AUDIO_BROADCAST_SOURCE) && (CONFIG_BT_AUDIO_BROADCAST_SOURCE > 0)) || \
    (defined(CONFIG_BT_AUDIO_BROADCAST_SINK) && (CONFIG_BT_AUDIO_BROADCAST_SINK > 0))

    #ifndef CONFIG_BT_AUDIO_STREAM
        #define CONFIG_BT_AUDIO_STREAM 1
    #endif

#endif

#if (defined(CONFIG_BT_PACS) && (CONFIG_BT_PACS > 0))

    #ifndef CONFIG_BT_AUDIO_CAPABILITY
        #define CONFIG_BT_AUDIO_CAPABILITY 1
    #endif

#endif

#include "config_pacs.h"
#include "config_ascs.h"

#endif /* __CONFIG_BAPS_H__ */