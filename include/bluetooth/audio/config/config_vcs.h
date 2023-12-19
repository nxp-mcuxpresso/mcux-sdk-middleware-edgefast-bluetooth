/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __CONFIG_VCS_H__
#define __CONFIG_VCS_H__

//##################### Volume Control Service #####################

/*! @brief Volume Control Service Support
 * This option enables support for Volume Control Service.
 */
#ifndef CONFIG_BT_VCS
    #define CONFIG_BT_VCS 0
#endif

#if (defined(CONFIG_BT_VCS) && (CONFIG_BT_VCS > 0))

/*! @brief Volume Offset Control Service instance count
 * This option sets the number of instances of Volume Offset Control Services.
 *
 * Valid range 0 ~ CONFIG_BT_VOCS_MAX_INSTANCE_COUNT
 */
#ifndef CONFIG_BT_VCS_VOCS_INSTANCE_COUNT
    #define CONFIG_BT_VCS_VOCS_INSTANCE_COUNT 0
#endif

#if (defined(CONFIG_BT_VCS_VOCS_INSTANCE_COUNT) && (CONFIG_BT_VCS_VOCS_INSTANCE_COUNT > 0))
    #ifndef CONFIG_BT_VCS_VOCS
        #define CONFIG_BT_VCS_VOCS 1
    #endif /* CONFIG_BT_VCS_VOCS */
#endif /* CONFIG_BT_VCS_VOCS_INSTANCE_COUNT */

/*! @brief Audio Input Control Service instance count for VCS
 * This option sets the number of instances of Audio Input Control Services for VCS.
 *
 * Valid range 0 ~ CONFIG_BT_AICS_MAX_INSTANCE_COUNT
 */
#ifndef CONFIG_BT_VCS_AICS_INSTANCE_COUNT
    #define CONFIG_BT_VCS_AICS_INSTANCE_COUNT 0
#endif

#if (defined(CONFIG_BT_VCS_AICS_INSTANCE_COUNT) && (CONFIG_BT_VCS_AICS_INSTANCE_COUNT > 0))
    #ifndef CONFIG_BT_VCS_AICS
        #define CONFIG_BT_VCS_AICS 1
    #endif /* CONFIG_BT_VCS_AICS */
#endif /* CONFIG_BT_VCS_AICS_INSTANCE_COUNT */

//############# DEBUG #############

/*! @brief Volume Control Service debug
 * Use this option to enable Volume Control Service debug logs for the
 * Bluetooth Audio functionality.
 */
#ifndef CONFIG_BT_DEBUG_VCS
    #define CONFIG_BT_DEBUG_VCS 0
#endif

#endif /* CONFIG_BT_VCS */

//##################### Volume Control Profile Client #####################

/*! @brief Volume Control Profile Support
 * This option enables support for Volume Control Profile.
 */
#ifndef CONFIG_BT_VCS_CLIENT
    #define CONFIG_BT_VCS_CLIENT 0
#endif

#if (defined(CONFIG_BT_VCS_CLIENT) && (CONFIG_BT_VCS_CLIENT > 0))

    #if (!defined(CONFIG_BT_GATT_CLIENT) || !(CONFIG_BT_GATT_CLIENT > 0))
        #error CONFIG_BT_VCS_CLIENT depends on CONFIG_BT_GATT_CLIENT.
    #endif

    #if (!defined(CONFIG_BT_GATT_AUTO_DISCOVER_CCC) || !(CONFIG_BT_GATT_AUTO_DISCOVER_CCC > 0))
        #error CONFIG_BT_VCS_CLIENT depends on CONFIG_BT_GATT_AUTO_DISCOVER_CCC.
    #endif

#endif /* CONFIG_BT_VCS_CLIENT */

#if (defined(CONFIG_BT_VCS_CLIENT) && (CONFIG_BT_VCS_CLIENT > 0))

/*! @brief Maximum number of VOCS instances to setup
 * Sets the maximum number of Volume Offset Control Service (VOCS)
 * instances to setup and use.
 *
 * Valid range 0 ~ CONFIG_BT_VOCS_CLIENT_MAX_INSTANCE_COUNT
 */
#ifndef CONFIG_BT_VCS_CLIENT_MAX_VOCS_INST
    #define CONFIG_BT_VCS_CLIENT_MAX_VOCS_INST 0
#endif

#if (defined(CONFIG_BT_VCS_CLIENT_MAX_VOCS_INST) && (CONFIG_BT_VCS_CLIENT_MAX_VOCS_INST > 0))
    #ifndef CONFIG_BT_VCS_CLIENT_VOCS
        #define CONFIG_BT_VCS_CLIENT_VOCS 1
    #endif /* CONFIG_BT_VCS_CLIENT_VOCS */
#endif /* CONFIG_BT_VCS_CLIENT_MAX_VOCS_INST */

/*! @brief Maximum number of AICS instances to setup
 * Sets the maximum number of Audio Input Control Service (AICS)
 * instances to setup and use.
 *
 * Valid range 0 ~ 3
 */
#ifndef CONFIG_BT_VCS_CLIENT_MAX_AICS_INST
    #define CONFIG_BT_VCS_CLIENT_MAX_AICS_INST 0
#endif

#if (defined(CONFIG_BT_VCS_CLIENT_MAX_AICS_INST) && (CONFIG_BT_VCS_CLIENT_MAX_AICS_INST > 0))
    #ifndef CONFIG_BT_VCS_CLIENT_AICS
        #define CONFIG_BT_VCS_CLIENT_AICS 1
    #endif /* CONFIG_BT_VCS_CLIENT_AICS */
#endif /* CONFIG_BT_VCS_CLIENT_MAX_AICS_INST */

//############# DEBUG #############

/*! @brief Volume Control Profile debug
 * Use this option to enable Volume Control Profile debug logs for the
 * Bluetooth Audio functionality.
 */
#ifndef CONFIG_BT_DEBUG_VCS_CLIENT
    #define CONFIG_BT_DEBUG_VCS_CLIENT 0
#endif

#endif /* CONFIG_BT_VCS_CLIENT */

#endif /* __CONFIG_VCS_H__ */