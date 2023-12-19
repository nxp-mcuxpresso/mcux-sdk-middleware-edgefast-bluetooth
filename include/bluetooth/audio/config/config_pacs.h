/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __CONFIG_PACS_H__
#define __CONFIG_PACS_H__

/*! @brief Sink PAC Characteristic Support
 * This option enables support for Sink Published Audio Capabilities.
 */
#ifndef CONFIG_BT_PAC_SNK
    #define CONFIG_BT_PAC_SNK 0
#endif

#if (defined(CONFIG_BT_PAC_SNK) && (CONFIG_BT_PAC_SNK > 0))

/*! @brief Supported Sink Contexts
 * The Supported Sink Contexts exposes the server’s support for
 *   reception of audio data associated with specific Context Types:
 *   0x0001: Unspecified
 *   0x0002: Conversational
 *   0x0004: Media
 *   0x0008: Instructional
 *   0x0010: AttentionSeeking
 *   0x0020: ImmediateAlert
 *   0x0040: ManMachine
 *   0x0080: EmergencyAlert
 *   0x0100: Ringtone
 *   0x0200: TV
 * 
 * Valid range 0x0000 ~ 0x03ff
 */
#ifndef CONFIG_BT_PACS_SNK_CONTEXT
    #define CONFIG_BT_PACS_SNK_CONTEXT 0x03ff
#endif /* CONFIG_BT_PACS_SNK_CONTEXT */

/*! @brief Sink PAC Location Support
 * This option enables support for Sink PAC Location Characteristic.
 */
#ifndef CONFIG_BT_PAC_SNK_LOC
    #define CONFIG_BT_PAC_SNK_LOC 1
#endif

/*! @brief Sink PAC Location Writable Support
 * This option enables support for clients to write to the Sink PAC
 * Location Characteristic.
 */
#ifndef CONFIG_BT_PAC_SNK_LOC_WRITEABLE
    #define CONFIG_BT_PAC_SNK_LOC_WRITEABLE 1
#endif

#if (defined(CONFIG_BT_PAC_SNK_LOC_WRITEABLE) && (CONFIG_BT_PAC_SNK_LOC_WRITEABLE > 0))

    #if (!defined(CONFIG_BT_PAC_SNK_LOC) || !(CONFIG_BT_PAC_SNK_LOC > 0))
        #error CONFIG_BT_PAC_SNK_LOC_WRITEABLE depends on CONFIG_BT_PAC_SNK_LOC.
    #endif

#endif

#endif /* CONFIG_BT_PAC_SNK */

/*! @brief Source PAC Characteristic Support
 * This option enables support for Source PAC Characteristic.
 *
 * 	default y if BT_ASCS
 *  default y
 */
#ifndef CONFIG_BT_PAC_SRC
    #define CONFIG_BT_PAC_SRC 0
#endif

#if (defined(CONFIG_BT_PAC_SRC) && (CONFIG_BT_PAC_SRC > 0))

/*! @brief Supported Source Contexts
 * The Supported Source Contexts exposes the server’s support for
 *   transmission of audio data associated with specific Context Types:
 *   0x0001: Unspecified
 *   0x0002: Conversational
 *   0x0004: Media
 *   0x0008: Instructional
 *   0x0010: AttentionSeeking
 *   0x0020: ImmediateAlert
 *   0x0040: ManMachine
 *   0x0080: EmergencyAlert
 *   0x0100: Ringtone
 *   0x0200: TV
 * 
 * Valid range 0x0000 ~ 0x03ff
 */
#ifndef CONFIG_BT_PACS_SRC_CONTEXT
    #define CONFIG_BT_PACS_SRC_CONTEXT 0x03ff
#endif

/*! @brief Source PAC Location Support
 * This option enables support for Source PAC Locations Characteristic.
 */
#ifndef CONFIG_BT_PAC_SRC_LOC
    #define CONFIG_BT_PAC_SRC_LOC 1
#endif

/*! @brief Source PAC Location Writable Support
 * This option enables support for clients to write to the Source PAC
 * Location Characteristic.
 */
#ifndef CONFIG_BT_PAC_SRC_LOC_WRITEABLE
    #define CONFIG_BT_PAC_SRC_LOC_WRITEABLE 1
#endif

#if (defined(CONFIG_BT_PAC_SRC_LOC_WRITEABLE) && (CONFIG_BT_PAC_SRC_LOC_WRITEABLE > 0))

    #if (!defined(CONFIG_BT_PAC_SRC_LOC) || !(CONFIG_BT_PAC_SRC_LOC > 0))
        #error CONFIG_BT_PAC_SRC_LOC_WRITEABLE depends on CONFIG_BT_PAC_SRC_LOC.
    #endif

#endif

#endif /* CONFIG_BT_PAC_SRC */

#if (defined(CONFIG_BT_PAC_SNK) && (CONFIG_BT_PAC_SNK > 0)) || \
    (defined(CONFIG_BT_PAC_SRC) && (CONFIG_BT_PAC_SRC > 0))

    #ifndef CONFIG_BT_PACS
        #define CONFIG_BT_PACS 1
    #endif

#endif

/*! @brief Published Audio Capabilities Service debug
 * Use this option to enable Published Audio Capabilities Service debug
 * logs for the Bluetooth Audio functionality.
 */
#ifndef CONFIG_BT_DEBUG_PACS
    #define CONFIG_BT_DEBUG_PACS 0
#endif

#if (defined(CONFIG_BT_DEBUG_PACS) && (CONFIG_BT_DEBUG_PACS > 0))

    #if (!defined(CONFIG_BT_PACS) || !(CONFIG_BT_PACS > 0))
        #error CONFIG_BT_DEBUG_PACS depends on CONFIG_BT_PACS.
    #endif

#endif

#endif /* __CONFIG_PACS_H__ */