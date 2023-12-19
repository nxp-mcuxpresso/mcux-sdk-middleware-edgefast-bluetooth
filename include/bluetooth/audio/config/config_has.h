/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __CONFIG_HAS_H__
#define __CONFIG_HAS_H__

/*! @brief Hearing Access Service support
 * This option enables support for Hearing Access Service.
 */
#if (defined(CONFIG_BT_HAS) && (CONFIG_BT_HAS > 0))

    #if (!defined(CONFIG_UTF8) || !(CONFIG_UTF8 > 0))
        #error CONFIG_BT_HAS depends on CONFIG_UTF8.
    #endif

    #if (!defined(CONFIG_BT_AUDIO_UNICAST_SERVER) || !(CONFIG_BT_AUDIO_UNICAST_SERVER > 0))
        #error CONFIG_BT_HAS depends on CONFIG_BT_AUDIO_UNICAST_SERVER.
    #endif

#endif /* CONFIG_BT_HAS */

#if (defined(CONFIG_BT_HAS) && (CONFIG_BT_HAS > 0))

/*! @brief Hearing Aid Type selection
 * Select the Hearing Aid Type to compile.
 */
#ifndef CONFIG_BT_HAS_HEARING_AID_TYPE_CHOICE
    #define CONFIG_BT_HAS_HEARING_AID_TYPE_CHOICE 0
#endif

#if (defined(CONFIG_BT_HAS_HEARING_AID_TYPE_CHOICE) && (CONFIG_BT_HAS_HEARING_AID_TYPE_CHOICE > 0))

/*! @brief Monaural Hearing Aid
 */
#ifndef CONFIG_BT_HAS_HEARING_AID_MONAURAL
    #define CONFIG_BT_HAS_HEARING_AID_MONAURAL 0
#endif

/*! @brief Binaural Hearing Aid
 */
#ifndef CONFIG_BT_HAS_HEARING_AID_BINAURAL
    #define CONFIG_BT_HAS_HEARING_AID_BINAURAL 0
#endif

#if (defined(CONFIG_BT_HAS_HEARING_AID_BINAURAL) && (CONFIG_BT_HAS_HEARING_AID_BINAURAL > 0))

    #if (!defined(CONFIG_BT_CSIS) || !(CONFIG_BT_CSIS > 0))
        #error CONFIG_BT_HAS_HEARING_AID_BINAURAL depends on CONFIG_BT_CSIS.
    #endif

#endif /* CONFIG_BT_HAS_HEARING_AID_BINAURAL */

/*! @brief Banded Hearing Aid
 */
#ifndef CONFIG_BT_HAS_HEARING_AID_BANDED
    #define CONFIG_BT_HAS_HEARING_AID_BANDED 0
#endif

#endif /* CONFIG_BT_HAS_HEARING_AID_TYPE_CHOICE */

#if (defined(CONFIG_BT_HAS_HEARING_AID_MONAURAL) && (CONFIG_BT_HAS_HEARING_AID_MONAURAL > 0)) || \
    (defined(CONFIG_BT_HAS_HEARING_AID_BINAURAL) && (CONFIG_BT_HAS_HEARING_AID_BINAURAL > 0))

/*! @brief Hearing Aid Device Location
 * Select the Hearing Aid Device location.
 */
#ifndef CONFIG_BT_HAS_HEARING_AID_LOCATION
    #define CONFIG_BT_HAS_HEARING_AID_LOCATION 0
#endif

#if (defined(CONFIG_BT_HAS_HEARING_AID_LOCATION) && (CONFIG_BT_HAS_HEARING_AID_LOCATION > 0))

/*! @brief Left Ear
 */
#ifndef CONFIG_BT_HAS_HEARING_AID_LEFT
    #define CONFIG_BT_HAS_HEARING_AID_LEFT 0
#endif

/*! @brief Right Ear
 */
#ifndef CONFIG_BT_HAS_HEARING_AID_RIGHT
    #define CONFIG_BT_HAS_HEARING_AID_RIGHT 0
#endif

#endif /* CONFIG_BT_HAS_HEARING_AID_LOCATION */

#endif /* CONFIG_BT_HAS_HEARING_AID_MONAURAL || CONFIG_BT_HAS_HEARING_AID_MONAURAL */

/*! @brief Hearing Aid Type
 *
 * Valid range 0 ~ 2
 * 	default 0 if BT_HAS_HEARING_AID_BINAURAL
 *	default 1 if BT_HAS_HEARING_AID_MONAURAL
 *	default 2 if BT_HAS_HEARING_AID_BANDED
 */
#ifndef CONFIG_BT_HAS_HEARING_AID_TYPE
    #if (defined(CONFIG_BT_HAS_HEARING_AID_BINAURAL) && (CONFIG_BT_HAS_HEARING_AID_BINAURAL > 0))
        #define CONFIG_BT_HAS_HEARING_AID_TYPE 0
    #endif
    #if (defined(CONFIG_BT_HAS_HEARING_AID_MONAURAL) && (CONFIG_BT_HAS_HEARING_AID_MONAURAL > 0))
        #define CONFIG_BT_HAS_HEARING_AID_TYPE 1
    #endif
    #if (defined(CONFIG_BT_HAS_HEARING_AID_BANDED) && (CONFIG_BT_HAS_HEARING_AID_BANDED > 0))
        #define CONFIG_BT_HAS_HEARING_AID_TYPE 2
    #endif
#endif

/*! @brief Preset record list size
 * This option sets the number of Hearing Access Service Presets
 * that can be registered. Setting this value to 0 disables Presets support.
 *
 * Valid range 0 ~ 255
 */
#ifndef CONFIG_BT_HAS_PRESET_COUNT
    #define CONFIG_BT_HAS_PRESET_COUNT 2
#endif

#if (defined(CONFIG_BT_HAS_PRESET_COUNT) && (CONFIG_BT_HAS_PRESET_COUNT > 0))
    #ifndef CONFIG_BT_HAS_PRESET_SUPPORT
        #define CONFIG_BT_HAS_PRESET_SUPPORT 1
    #endif
#endif /* CONFIG_BT_HAS_PRESET_COUNT */

#if (defined(CONFIG_BT_HAS_PRESET_SUPPORT) && (CONFIG_BT_HAS_PRESET_SUPPORT > 0))

#if (defined(CONFIG_BT_HAS_HEARING_AID_BINAURAL) && (CONFIG_BT_HAS_HEARING_AID_BINAURAL > 0))

/*! @brief Identical preset records in Binaural Hearing Aid Set
 * Set if the list of preset records is identical to the list of preset records
 * on the other member in the Binaural Hearing Aid Set.
 * This option sets Independent Presets field in Hearing Aid Features to 0b0.
 */
#ifndef CONFIG_BT_HAS_IDENTICAL_PRESET_RECORDS
    #define CONFIG_BT_HAS_IDENTICAL_PRESET_RECORDS 0
#endif

/*! @brief Preset synchronization support
 * Set if the hearing aid has support for relaying active preset changes to the other
 * member in the Binaural Hearing Aid Set.
 */
#ifndef CONFIG_BT_HAS_PRESET_SYNC_SUPPORT
    #define CONFIG_BT_HAS_PRESET_SYNC_SUPPORT 0
#endif

#if (defined(CONFIG_BT_HAS_PRESET_SYNC_SUPPORT) && (CONFIG_BT_HAS_PRESET_SYNC_SUPPORT > 0))

    #if (!defined(CONFIG_BT_HAS_IDENTICAL_PRESET_RECORDS) || !(CONFIG_BT_HAS_IDENTICAL_PRESET_RECORDS > 0))
        #error CONFIG_BT_HAS_PRESET_SYNC_SUPPORT depends on CONFIG_BT_HAS_IDENTICAL_PRESET_RECORDS.
    #endif

#endif /* CONFIG_BT_HAS_PRESET_SYNC_SUPPORT */

#endif /* CONFIG_BT_HAS_HEARING_AID_BINAURAL */

/*! @brief Allow to set preset name on runtime
 * Enabling this option allows for runtime configuration of preset name.
 */
#ifndef CONFIG_BT_HAS_PRESET_NAME_DYNAMIC
    #define CONFIG_BT_HAS_PRESET_NAME_DYNAMIC 0
#endif

#endif /* CONFIG_BT_HAS_PRESET_SUPPORT */

/*! @brief Hearing Access Service debug
 * This option enables enables Hearing Access Service debug logs.
 */
#ifndef CONFIG_BT_DEBUG_HAS
    #define CONFIG_BT_DEBUG_HAS 0
#endif

#endif /* CONFIG_BT_HAS */

/*! @brief Hearing Access Service Client support
 * This option enables support for Hearing Access Service Client.
 */
#if (defined(CONFIG_BT_HAS_CLIENT) && (CONFIG_BT_HAS_CLIENT > 0))

    #if (!defined(CONFIG_BT_GATT_CLIENT) || !(CONFIG_BT_GATT_CLIENT > 0))
        #error CONFIG_BT_HAS_CLIENT depends on CONFIG_BT_GATT_CLIENT.
    #endif

    #if (!defined(CONFIG_BT_GATT_AUTO_DISCOVER_CCC) || !(CONFIG_BT_GATT_AUTO_DISCOVER_CCC > 0))
        #error CONFIG_BT_HAS_CLIENT depends on CONFIG_BT_GATT_AUTO_DISCOVER_CCC.
    #endif

    #if (!defined(CONFIG_BT_GATT_AUTO_UPDATE_MTU) || !(CONFIG_BT_GATT_AUTO_UPDATE_MTU > 0))
        #error CONFIG_BT_HAS_CLIENT depends on CONFIG_BT_GATT_AUTO_UPDATE_MTU.
    #endif

    #if (!defined(CONFIG_UTF8) || !(CONFIG_UTF8 > 0))
        #error CONFIG_BT_HAS_CLIENT depends on CONFIG_UTF8.
    #endif

#endif /* CONFIG_BT_HAS_CLIENT */

/*! @brief Hearing Access Service Client debug
 * This option enables enables Hearing Access Service Client debug logs.
 */
#ifndef CONFIG_BT_DEBUG_HAS_CLIENT
    #define CONFIG_BT_DEBUG_HAS_CLIENT 0
#endif

#if (defined(CONFIG_BT_DEBUG_HAS_CLIENT) && (CONFIG_BT_DEBUG_HAS_CLIENT > 0))

    #if (!defined(CONFIG_BT_HAS_CLIENT) || !(CONFIG_BT_HAS_CLIENT > 0))
        #error CONFIG_BT_DEBUG_HAS_CLIENT depends on CONFIG_BT_HAS_CLIENT.
    #endif

#endif /* CONFIG_BT_DEBUG_HAS_CLIENT */

#endif /* __CONFIG_HAS_H__ */