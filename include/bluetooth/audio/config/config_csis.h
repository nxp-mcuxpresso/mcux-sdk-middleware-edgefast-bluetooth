/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __CONFIG_CSIS_H__
#define __CONFIG_CSIS_H__

//#################### Coordinated Set Identification Service ####################

/*! @brief Coordinated Set Identification Service Support
 * This option enables support for Coordinated Set Identification Service.
 */
#ifndef CONFIG_BT_CSIS
    #define CONFIG_BT_CSIS 0
#endif

#if (defined(CONFIG_BT_CSIS) && (CONFIG_BT_CSIS > 0))

    #if (!defined(CONFIG_BT_EXT_ADV) || !(CONFIG_BT_EXT_ADV > 0))
        #error CONFIG_BT_CSIS depends on CONFIG_BT_EXT_ADV.
    #endif

#endif /* CONFIG_BT_CSIS */

#if (defined(CONFIG_BT_CSIS) && (CONFIG_BT_CSIS > 0))

/*! @brief Coordinated Set Identification Service Test Sample Data
 * Enable the use of the sample data defined by the CSIS spec SIRK.
 * This will use the sample SIRK, prand and LTK.   
 * 
 * WARNING: This option enables anyone to track and decrypt the SIRK
 * (if encrypted) using public sample data.
 * Should not be used for production builds.
 */
#ifndef CONFIG_BT_CSIS_TEST_SAMPLE_DATA
    #define CONFIG_BT_CSIS_TEST_SAMPLE_DATA 0
#endif

/*! @brief Support for encrypted SIRK
 * Enables support encrypting the SIRK.
 */
#ifndef CONFIG_BT_CSIS_ENC_SIRK_SUPPORT
    #define CONFIG_BT_CSIS_ENC_SIRK_SUPPORT 1
#endif

/*! @brief Coordinated Set Identification Service max instance count
 * This option sets the maximum number of instances of Coordinated Set
 * Identification Services. If the service is declared as primary service
 * then only a single instance is possible.
 *
 * Valid range 1 ~ 255
 */
#ifndef CONFIG_BT_CSIS_MAX_INSTANCE_COUNT
    #define CONFIG_BT_CSIS_MAX_INSTANCE_COUNT 1
#endif

//############### DEBUG ###############

/*! @brief Coordinated Set Identification Service debug
 * Use this option to enable Coordinated Set Identification Service debug
 * logs for the Bluetooth Audio functionality.
 */
#ifndef CONFIG_BT_DEBUG_CSIS
    #define CONFIG_BT_DEBUG_CSIS 0
#endif

#endif /* CONFIG_BT_CSIS */

//#################### Coordinated Set Identification Client ####################

/*! @brief Coordinated Set Identification Profile Support
 * This option enables support for Coordinated Set Identification Profile.
 */
#ifndef CONFIG_BT_CSIS_CLIENT
    #define CONFIG_BT_CSIS_CLIENT 0
#endif

#if (defined(CONFIG_BT_CSIS_CLIENT) && (CONFIG_BT_CSIS_CLIENT > 0))

    #if (!defined(CONFIG_BT_GATT_CLIENT) || !(CONFIG_BT_GATT_CLIENT > 0))
        #error CONFIG_BT_CSIS_CLIENT depends on CONFIG_BT_GATT_CLIENT.
    #endif

    #if (!defined(CONFIG_BT_GATT_AUTO_DISCOVER_CCC) || !(CONFIG_BT_GATT_AUTO_DISCOVER_CCC > 0))
        #error CONFIG_BT_CSIS_CLIENT depends on CONFIG_BT_GATT_AUTO_DISCOVER_CCC.
    #endif

#endif /* CONFIG_BT_CSIS_CLIENT */

#if (defined(CONFIG_BT_CSIS_CLIENT) && (CONFIG_BT_CSIS_CLIENT > 0))

/*! @brief Coordinated Set Identification Service Test Sample Data
 * Enable the use of the sample data defined by the CSIS spec.
 * This will use the sample SIRK, prand and LTK.
 * 
 * WARNING: This option enables the client to use the sample data
 * to locate members and decrypt the SIRK, and thus won't work
 * with set members that are not using the sample data.
 * Should not be used for production builds.
 */
#ifndef CONFIG_BT_CSIS_CLIENT_TEST_SAMPLE_DATA
    #define CONFIG_BT_CSIS_CLIENT_TEST_SAMPLE_DATA 0
#endif

/*! @brief Coordinated Set Identification Service Count
 * Sets the number of service instances, which corresponds to the number
 * of different sets the peer device may be in.
 *
 * Valid range 1 ~ 3
 */
#ifndef CONFIG_BT_CSIS_CLIENT_MAX_CSIS_INSTANCES
    #define CONFIG_BT_CSIS_CLIENT_MAX_CSIS_INSTANCES 1
#endif

/*! @brief Support for encrypted SIRK
 * Enables support encrypting the SIRK.
 */
#ifndef CONFIG_BT_CSIS_CLIENT_ENC_SIRK_SUPPORT
    #define CONFIG_BT_CSIS_CLIENT_ENC_SIRK_SUPPORT 1
#endif

//############### DEBUG ###############

/*! @brief Coordinated Set Identification Profile debug
 * Use this option to enable Coordinated Set Identification Profile debug
 * logs for the Bluetooth Audio functionality.
 */
#ifndef CONFIG_BT_DEBUG_CSIS_CLIENT
    #define CONFIG_BT_DEBUG_CSIS_CLIENT 0
#endif

#endif /* CONFIG_BT_CSIS_CLIENT */

/*! @brief Coordinated Set Identification Service crypto functions debug
 * Use this option to enable Coordinated Set Identification Service
 * crypto functions debug logs for the Bluetooth Audio functionality.
 */
#ifndef CONFIG_BT_DEBUG_CSIS_CRYPTO
    #define CONFIG_BT_DEBUG_CSIS_CRYPTO 0
#endif

#if (defined(CONFIG_BT_DEBUG_CSIS_CRYPTO) && (CONFIG_BT_DEBUG_CSIS_CRYPTO > 0))

    #if ( (!defined(CONFIG_BT_CSIS_CLIENT) || !(CONFIG_BT_CSIS_CLIENT > 0)) || \
          (!defined(CONFIG_BT_CSIS) || !(CONFIG_BT_CSIS > 0)) )
        #error CONFIG_BT_DEBUG_CSIS_CRYPTO depends on CONFIG_BT_CSIS_CLIENT | CONFIG_BT_CSIS.
    #endif

#endif /* CONFIG_BT_DEBUG_CSIS_CRYPTO */

#endif /* __CONFIG_CSIS_H__ */