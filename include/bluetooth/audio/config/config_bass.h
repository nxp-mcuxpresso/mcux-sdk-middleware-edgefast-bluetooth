/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __CONFIG_BASS_H__
#define __CONFIG_BASS_H__

#if (defined(CONFIG_BT_AUDIO) && (CONFIG_BT_AUDIO > 0))

//#####################  Broadcast Audio Scan Service #####################

/*! @brief Broadcast Audio Scan Service
 * This option enables support for the Broadcast Audio Scan Service.
 */
#if (defined(CONFIG_BT_BASS) && (CONFIG_BT_BASS > 0))

    #if (!defined(CONFIG_BT_OBSERVER) || !(CONFIG_BT_OBSERVER > 0))
        #error CONFIG_BT_BASS depends on CONFIG_BT_OBSERVER.
    #endif

    #if (!defined(CONFIG_BT_EXT_ADV) || !(CONFIG_BT_EXT_ADV > 0))
        #error CONFIG_BT_BASS depends on CONFIG_BT_EXT_ADV.
    #endif

    #if (!defined(CONFIG_BT_PER_ADV_SYNC) || !(CONFIG_BT_PER_ADV_SYNC > 0))
        #error CONFIG_BT_BASS depends on CONFIG_BT_PER_ADV_SYNC.
    #endif

    #if (!defined(CONFIG_BT_ISO_SYNC_RECEIVER) || !(CONFIG_BT_ISO_SYNC_RECEIVER > 0))
        #error CONFIG_BT_BASS depends on CONFIG_BT_ISO_SYNC_RECEIVER.
    #endif

#endif /* CONFIG_BT_BASS */

#if (defined(CONFIG_BT_BASS) && (CONFIG_BT_BASS > 0))

/*! @brief Broadcast Audio Scan Service Receive State Count
 * Sets the number of receive state characteristics present on the
 * server. Each characteristic may hold information to sync to a
 * periodic advertise or a broadcast isochronous stream.
 *
 *  Valid range 1 ~ 3
 */
#ifndef CONFIG_BT_BASS_RECV_STATE_COUNT
    #define CONFIG_BT_BASS_RECV_STATE_COUNT 1
#endif

/*! @brief Broadcast Audio Scan Service Maximum Metadata Length
 * The maximum metadata length support by the BASS server.
 *
 *  Valid range 0 ~ 255
 */
#ifndef CONFIG_BT_BASS_MAX_METADATA_LEN
    #define CONFIG_BT_BASS_MAX_METADATA_LEN 32
#endif

/*! @brief Broadcast Audio Scan Service Maximum Metadata Length
 * The maximum metadata length support by the BASS server.
 *
 *  Valid range 0 ~ 255
 */
#ifndef CONFIG_BT_BASS_MAX_METADATA_LEN
    #define CONFIG_BT_BASS_MAX_METADATA_LEN 32
#endif

/*! @brief Broadcast Audio Scan Service Maximum Number of Subgroups support
 * The maximum number of BIS subgroups supported.
 *
 *  Valid range 0 ~ 31
 */
#ifndef CONFIG_BT_BASS_MAX_SUBGROUPS
    #define CONFIG_BT_BASS_MAX_SUBGROUPS 1
#endif

//############### DEBUG ###############

/*! @brief Broadcast Audio Scan Service debug
 * Use this option to enable Broadcast Audio Scan Service debug logs for
 * the Bluetooth Audio functionality.
 */
#ifndef CONFIG_BT_DEBUG_BASS
    #define CONFIG_BT_DEBUG_BASS 0
#endif

/*! @brief Broadcast Audio Scan Service debug
 * Use this option to enable Broadcast Audio Scan Service debug logs for
 * the Bluetooth Audio functionality.
 */
#ifndef CONFIG_BT_DEBUG_BASS
    #define CONFIG_BT_DEBUG_BASS 0
#endif

#endif /* CONFIG_BT_BASS */

//##################### Broadcast Audio Scan Client #####################

/*! @brief Broadcast Audio Scan Service
 * This option enables support for the Broadcast Audio Scan Service client.
 */
#if (defined(CONFIG_BT_BASS_CLIENT) && (CONFIG_BT_BASS_CLIENT > 0))

    #if (!defined(CONFIG_BT_OBSERVER) || !(CONFIG_BT_OBSERVER > 0))
        #error CONFIG_BT_BASS_CLIENT depends on CONFIG_BT_OBSERVER.
    #endif

    #if (!defined(CONFIG_BT_EXT_ADV) || !(CONFIG_BT_EXT_ADV > 0))
        #error CONFIG_BT_BASS_CLIENT depends on CONFIG_BT_EXT_ADV.
    #endif

    #if (!defined(CONFIG_BT_PER_ADV_SYNC) || !(CONFIG_BT_PER_ADV_SYNC > 0))
        #error CONFIG_BT_BASS_CLIENT depends on CONFIG_BT_PER_ADV_SYNC.
    #endif

    #if (!defined(CONFIG_BT_ISO_SYNC_RECEIVER) || !(CONFIG_BT_ISO_SYNC_RECEIVER > 0))
        #error CONFIG_BT_BASS_CLIENT depends on CONFIG_BT_ISO_SYNC_RECEIVER.
    #endif

    #if (!defined(CONFIG_BT_GATT_CLIENT) || !(CONFIG_BT_GATT_CLIENT > 0))
        #error CONFIG_BT_BASS_CLIENT depends on CONFIG_BT_GATT_CLIENT.
    #endif

    #if (!defined(CONFIG_BT_GATT_AUTO_DISCOVER_CCC) || !(CONFIG_BT_GATT_AUTO_DISCOVER_CCC > 0))
        #error CONFIG_BT_BASS_CLIENT depends on CONFIG_BT_GATT_AUTO_DISCOVER_CCC.
    #endif

    #if (!defined(CONFIG_BT_GATT_AUTO_UPDATE_MTU) || !(CONFIG_BT_GATT_AUTO_UPDATE_MTU > 0))
        #error CONFIG_BT_BASS_CLIENT depends on CONFIG_BT_GATT_AUTO_UPDATE_MTU.
    #endif

#endif /* CONFIG_BT_BASS_CLIENT */

#if (defined(CONFIG_BT_BASS_CLIENT) && (CONFIG_BT_BASS_CLIENT > 0))

/*! @brief Broadcast Audio Scan Service Client Max Receive State Count
 * Sets the number of maximum receive stat characteristics that will be
 * discovered and ready to use. Each characteristic may hold information
 * to sync to a periodic advertise or a broadcast isochronous stream.
 *
 * Valid range 1 ~ 255
 */
#ifndef CONFIG_BT_BASS_CLIENT_RECV_STATE_COUNT
    #define CONFIG_BT_BASS_CLIENT_RECV_STATE_COUNT 1
#endif

//############### DEBUG ###############

/*! @brief Broadcast Audio Scan Service client debug
 * Use this option to enable Broadcast Audio Scan Service client
 * debug logs for the Bluetooth Audio functionality.
 */
#ifndef CONFIG_BT_DEBUG_BASS_CLIENT
    #define CONFIG_BT_DEBUG_BASS_CLIENT 0
#endif

#endif /* CONFIG_BT_BASS_CLIENT */

#endif /* CONFIG_BT_AUDIO */

#endif /* __CONFIG_BASS_H__ */