/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __BLE_AUDIO_CONFIG_H__
#define __BLE_AUDIO_CONFIG_H__

/*!
 * @brief Bluetooth Audio Configuration
 * @defgroup bt_audio_config Bluetooth Audio Configuration
 * @{
 */

#if (defined(CONFIG_BT_AUDIO) && (CONFIG_BT_AUDIO > 0))

#if (defined(CONFIG_BT_AUDIO_UNICAST) && (CONFIG_BT_AUDIO_UNICAST > 0)) || \
    (defined(CONFIG_BT_AUDIO_UNICAST_SERVER) && (CONFIG_BT_AUDIO_UNICAST_SERVER > 0)) || \
    (defined(CONFIG_BT_AUDIO_UNICAST_CLIENT) && (CONFIG_BT_AUDIO_UNICAST_CLIENT > 0)) || \
    (defined(CONFIG_BT_AUDIO_BROADCAST_SOURCE) && (CONFIG_BT_AUDIO_BROADCAST_SOURCE > 0)) || \
    (defined(CONFIG_BT_AUDIO_BROADCAST_SINK) && (CONFIG_BT_AUDIO_BROADCAST_SINK > 0))
#include "config_baps.h"
#endif

#if (defined(CONFIG_BT_VOCS) && (CONFIG_BT_VOCS > 0)) || \
    (defined(CONFIG_BT_VOCS_CLIENT) && (CONFIG_BT_VOCS_CLIENT > 0))
#include "config_vocs.h"
#endif

#if (defined(CONFIG_BT_AICS) && (CONFIG_BT_AICS > 0)) || \
    (defined(CONFIG_BT_AICS_CLIENT) && (CONFIG_BT_AICS_CLIENT > 0))
#include "config_aics.h"
#endif

#if (defined(CONFIG_BT_VOCS) && (CONFIG_BT_VOCS > 0)) || \
    (defined(CONFIG_BT_VOCS_CLIENT) && (CONFIG_BT_VOCS_CLIENT > 0))
#include "config_vcs.h"
#endif

#if (defined(CONFIG_BT_MICS) && (CONFIG_BT_MICS > 0)) || \
    (defined(CONFIG_BT_MICS_CLIENT) && (CONFIG_BT_MICS_CLIENT > 0))
#include "config_mics.h"
#endif

#if (defined(CONFIG_BT_CSIS) && (CONFIG_BT_CSIS > 0)) || \
    (defined(CONFIG_BT_CSIS_CLIENT) && (CONFIG_BT_CSIS_CLIENT > 0))
#include "config_csis.h"
#endif

#if (defined(CONFIG_BT_TBS) && (CONFIG_BT_TBS > 0)) || \
    (defined(CONFIG_BT_TBS_CLIENT) && (CONFIG_BT_TBS_CLIENT > 0))
#include "config_tbs.h"
#endif

#if (defined(CONFIG_BT_MCS) && (CONFIG_BT_MCS > 0)) || \
    (defined(CONFIG_BT_MCC) && (CONFIG_BT_MCC > 0)) || \
    (defined(CONFIG_BT_MCC_OTS) && (CONFIG_BT_MCC_OTS > 0))
#include "config_mcs.h"
#endif

#if (defined(CONFIG_BT_BASS) && (CONFIG_BT_BASS > 0)) || \
    (defined(CONFIG_BT_BASS_CLIENT) && (CONFIG_BT_BASS_CLIENT > 0))
#include "config_bass.h"
#endif

#if (defined(CONFIG_BT_HAS) && (CONFIG_BT_HAS > 0)) || \
    (defined(CONFIG_BT_HAS_CLIENT) && (CONFIG_BT_HAS_CLIENT > 0))
#include "config_has.h"
#endif

#if (defined(CONFIG_BT_MPL) && (CONFIG_BT_MPL > 0))
#include "config_mpl.h"
#endif

#if (defined(CONFIG_BT_MCTL) && (CONFIG_BT_MCTL > 0))
#include "config_mctl.h"
#endif

#if (defined(CONFIG_BT_OTS) && (CONFIG_BT_OTS > 0)) || \
    (defined(CONFIG_BT_OTS_CLIENT) && (CONFIG_BT_OTS_CLIENT > 0))
#include "config_ots.h"
#endif


#endif /* CONFIG_BT_AUDIO */

/*! @}*/

#endif /* __BLE_AUDIO_CONFIG_H__ */