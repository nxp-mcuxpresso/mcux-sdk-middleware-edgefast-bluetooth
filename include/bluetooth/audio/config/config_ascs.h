/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __CONFIG_ASCS_H__
#define __CONFIG_ASCS_H__

/*! @brief Audio Stream Control Service Support
 * This option enables support for Audio Stream Control Service.
 */
#ifndef CONFIG_BT_ASCS
    #define CONFIG_BT_ASCS 0
#endif

#if (defined(CONFIG_BT_ASCS) && (CONFIG_BT_ASCS > 0))

/*! @brief Number of Audio Stream Endpoint Sink Characteristics
 * An ASE Sink characteristic represents the state of an ASE, which is
 * coupled to a single direction of a unicast Audio Stream.
 * 
 * Valid range 0 ~ 2
 */
#ifndef CONFIG_BT_ASCS_ASE_SNK_COUNT
    #define CONFIG_BT_ASCS_ASE_SNK_COUNT 2
#endif

/*! @brief Number of Audio Stream Endpoint Source Characteristics
 * An ASE Source characteristic represents the state of an ASE, which is
 * coupled to a single direction of a unicast Audio Stream.
 * 
 * Valid range 0 ~ 2
 */
#ifndef CONFIG_BT_ASCS_ASE_SRC_COUNT
    #define CONFIG_BT_ASCS_ASE_SRC_COUNT 2
#endif

#endif /* CONFIG_BT_ASCS */

#endif /* __CONFIG_ASCS_H__ */