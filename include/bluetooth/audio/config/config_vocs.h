/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __CONFIG_VOCS_H__
#define __CONFIG_VOCS_H__

/*! @brief Volume Offset Control Service max instance count
 * This option sets the maximum number of instances of Volume Offset Control Services.
 *
 *  Valid range 0 ~ 15
 */
#ifndef CONFIG_BT_VOCS_MAX_INSTANCE_COUNT
    #define CONFIG_BT_VOCS_MAX_INSTANCE_COUNT 0
#endif

#if (defined(CONFIG_BT_VOCS_MAX_INSTANCE_COUNT) && (CONFIG_BT_VOCS_MAX_INSTANCE_COUNT > 0))
    #ifndef CONFIG_BT_VOCS
        #define CONFIG_BT_VOCS 1
    #endif
#endif

#if (defined(CONFIG_BT_VOCS) && (CONFIG_BT_VOCS > 0))

/*! @brief Volume Offset Control Service max output description size
 * This option sets the maximum output description size in octets.
 *
 *  Valid range 0 ~ 512
 */
#ifndef CONFIG_BT_VOCS_MAX_OUTPUT_DESCRIPTION_SIZE
    #define CONFIG_BT_VOCS_MAX_OUTPUT_DESCRIPTION_SIZE 32
#endif

/*! @brief Volume Offset Control Service debug
 * Use this option to enable Volume Offset Control Service debug logs for
 * the Bluetooth Audio functionality.
 */
#ifndef CONFIG_BT_DEBUG_VOCS
    #define CONFIG_BT_DEBUG_VOCS 0
#endif

#endif /* CONFIG_BT_VOCS */

/*! @brief Volume Offset Control Service client max instance count
 * This option sets the maximum number of instances of Volume Offset
 * Control Service clients.
 *
 *  Valid range 0 ~ 15
 */
#ifndef CONFIG_BT_VOCS_CLIENT_MAX_INSTANCE_COUNT
    #define CONFIG_BT_VOCS_CLIENT_MAX_INSTANCE_COUNT 0
#endif

#if (defined(CONFIG_BT_VOCS_CLIENT_MAX_INSTANCE_COUNT) && (CONFIG_BT_VOCS_CLIENT_MAX_INSTANCE_COUNT > 0))
    #ifndef CONFIG_BT_VOCS_CLIENT
        #define CONFIG_BT_VOCS_CLIENT 1
    #endif
#endif

#if (defined(CONFIG_BT_VOCS_CLIENT) && (CONFIG_BT_VOCS_CLIENT > 0))

/*! @brief Volume Offset Control Service client debug
 * Use this option to enable Volume Offset Control Service client debug
 * logs for the Bluetooth Audio functionality.
 */
#ifndef CONFIG_BT_DEBUG_VOCS_CLIENT
    #define CONFIG_BT_DEBUG_VOCS_CLIENT 0
#endif

#endif /* CONFIG_BT_VOCS_CLIENT */

#endif /* __CONFIG_VOCS_H__ */