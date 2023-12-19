/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __CONFIG_CAP_H__
#define __CONFIG_CAP_H__

/*! @brief Common Audio Profile Acceptor Role Support
 * Enabling this will enable the CAP Acceptor role. This instantiates the
 * common audio service (CAS).
 */
#ifndef CONFIG_BT_CAP_ACCEPTOR
    #define CONFIG_BT_CAP_ACCEPTOR 0
#endif

#if (defined(CONFIG_BT_CAP_ACCEPTOR) && (CONFIG_BT_CAP_ACCEPTOR > 0))

    //depends on BT_AUDIO_UNICAST_SERVER || (BT_AUDIO_BROADCAST_SINK && BT_BASS)
    #if !(  (defined(CONFIG_BT_CAP_ACCEPTOR) && (CONFIG_BT_CAP_ACCEPTOR > 0)) || 
            (
                (defined(CONFIG_BT_CAP_ACCEPTOR) && (CONFIG_BT_CAP_ACCEPTOR > 0)) && \
                (defined(CONFIG_BT_CAP_ACCEPTOR) && (CONFIG_BT_CAP_ACCEPTOR > 0))
            )
        )
        #error CONFIG_BT_CAP_ACCEPTOR depends on BT_AUDIO_UNICAST_SERVER || (BT_AUDIO_BROADCAST_SINK && BT_BASS).
    #endif

    #ifndef CONFIG_BT_CAP
        #define CONFIG_BT_CAP 1
    #endif

#endif /* CONFIG_BT_CAP_ACCEPTOR */

//Hidden config to enable cross-role CAP capabilities.
#ifndef CONFIG_BT_CAP
    #define CONFIG_BT_CAP 0
#endif

/*! @brief Common Audio Profile Acceptor Role Set Member support
 * Enabling this will allow a CAP acceptor to be a set member.
 * Enabling this will require a manual register of the CAS service.
 * Enabling will take one of the allocated CSIS instances (BT_CSIS_MAX_INSTANCE_COUNT).
 *
 */
#ifndef CONFIG_BT_CAP_ACCEPTOR_SET_MEMBER
    #define CONFIG_BT_CAP_ACCEPTOR_SET_MEMBER 0
#endif

#if (defined(CONFIG_BT_CAP_ACCEPTOR_SET_MEMBER) && (CONFIG_BT_CAP_ACCEPTOR_SET_MEMBER > 0))

    #if !(defined(CONFIG_BT_CAP_ACCEPTOR) && (CONFIG_BT_CAP_ACCEPTOR > 0))
        #error CONFIG_BT_CAP_ACCEPTOR_SET_MEMBER depends on CONFIG_BT_CAP_ACCEPTOR.
    #endif

    #if !(defined(CONFIG_BT_CSIS) && (CONFIG_BT_CSIS > 0))
        #error CONFIG_BT_CAP_ACCEPTOR_SET_MEMBER depends on CONFIG_BT_CSIS.
    #endif

#endif /* CONFIG_BT_CAP_ACCEPTOR_SET_MEMBER */

//############### DEBUG ###############

/*! @brief Common Audio Profile debug
 * Use this option to enable CAP debug logs for the Bluetooth Audio functionality.
 */
#ifndef CONFIG_BT_DEBUG_CAP
    #define CONFIG_BT_DEBUG_CAP 0
#endif

#if (defined(CONFIG_BT_DEBUG_CAP) && (CONFIG_BT_DEBUG_CAP > 0))

    #if !(defined(CONFIG_BT_CAP) && (CONFIG_BT_CAP > 0))
        #error CONFIG_BT_DEBUG_CAP depends on CONFIG_BT_CAP.
    #endif

#endif /* CONFIG_BT_DEBUG_CAP */

#endif /* __CONFIG_CAP_H__ */