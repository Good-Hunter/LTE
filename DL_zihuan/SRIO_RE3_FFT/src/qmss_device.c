/**
 *   @file  qmss_device.c
 *
 *   @brief   
 *      This file contains the device specific configuration and initialization routines
 *      for QMSS Low Level Driver.
 *
 *  \par
 *  ============================================================================
 *  @n   (C) Copyright 2009, Texas Instruments, Inc.
 *  @n   Use of this software is controlled by the terms and conditions found 
 *  @n   in the license agreement under which this software has been supplied.
 *  ============================================================================ 
 *  \par
 */

/* QMSS Types includes */
#include <qmss_types.h>

/* QMSS includes */
#include "qmss_qm.h"

/* CSL RL includes */
#include "cslr_device.h"
#include "cslr_qm_config.h"
#include "cslr_qm_descriptor_region_config.h"
#include "cslr_qm_queue_management.h"
#include "cslr_qm_queue_status_config.h"
#include "cslr_qm_intd.h"
#include "cslr_pdsp.h"
#include "csl_qm_queue.h"

/** @addtogroup QMSS_LLD_DATASTRUCT
@{ 
*/
/** @brief QMSS LLD initialization parameters */
Qmss_GlobalConfigParams qmssGblCfgParams[] =
{
    /** Maximum number of queue Managers */
    2u,
    /** Maximum number of queues */
    8192u,
    
    {
    /** Base queue number and Maximum supported low priority queues */
    {QMSS_LOW_PRIORITY_QUEUE_BASE, QMSS_MAX_LOW_PRIORITY_QUEUE},
    /** Base queue number and Maximum supported AIF queues */
    {QMSS_AIF_QUEUE_BASE, QMSS_MAX_AIF_QUEUE},
    /** Base queue number and Maximum supported PASS queues */
    {QMSS_PASS_QUEUE_BASE, QMSS_MAX_PASS_QUEUE},
    /** Base queue number and Maximum supported Intc Pend queues */
    {QMSS_INTC_QUEUE_BASE, QMSS_MAX_INTC_QUEUE},
    /** Base queue number and Maximum supported SRIO queues */
    {QMSS_SRIO_QUEUE_BASE, QMSS_MAX_SRIO_QUEUE},
    /** Base queue number and Maximum supported FFTC A queues */
    {QMSS_FFTC_A_QUEUE_BASE, QMSS_MAX_FFTC_A_QUEUE},
    /** Base queue number and Maximum supported FFTC B queues */
    {QMSS_FFTC_B_QUEUE_BASE, QMSS_MAX_FFTC_B_QUEUE},
    /** Base queue number and Maximum supported FFTC C queues */
    {QMSS_FFTC_C_QUEUE_BASE, QMSS_MAX_FFTC_C_QUEUE},
    /** Base queue number and Maximum supported BCP queues */
    {QMSS_BCP_QUEUE_BASE, QMSS_MAX_BCP_QUEUE},
    /** Base queue number and Maximum supported high priority queues */
    {QMSS_HIGH_PRIORITY_QUEUE_BASE, QMSS_MAX_HIGH_PRIORITY_QUEUE},
    /** Base queue number and Maximum supported starvation counter queues */
    {QMSS_STARVATION_COUNTER_QUEUE_BASE, QMSS_MAX_STARVATION_COUNTER_QUEUE},
    /** Base queue number and Maximum supported infrastructure queues */
    {QMSS_INFRASTRUCTURE_QUEUE_BASE, QMSS_MAX_INFRASTRUCTURE_QUEUE},
    /** Base queue number and Maximum supported traffic shaping queues */
    {QMSS_TRAFFIC_SHAPING_QUEUE_BASE, QMSS_MAX_TRAFFIC_SHAPING_QUEUE},
    /** Base queue number and Maximum supported general purpose queues */
    {QMSS_GENERAL_PURPOSE_QUEUE_BASE, QMSS_MAX_GENERAL_PURPOSE_QUEUE},

    /* Unused */
    {0u, 0u},
    {0u, 0u},
    {0u, 0u},
    {0u, 0u},
    {0u, 0u},
    {0u, 0u},
    {0u, 0u},
    {0u, 0u},
    {0u, 0u},
    {0u, 0u},
    {0u, 0u},
    },
    /** Base address for the CPDMA overlay registers */

    /** QM Global Config registers */
    (void *) CSL_QM_SS_CFG_CONFIG_STARVATION_COUNTER_REGS,
    /** QM Descriptor Config registers */
    (void *) CSL_QM_SS_CFG_DESCRIPTION_REGS, 
    /** QM queue Management registers */
    (void *) CSL_QM_SS_CFG_QM_QUEUE_DEQUEUE_REGS,
    /** QM queue Management Proxy registers */
    (void *) CSL_QM_SS_CFG_PROXY_QUEUE_DEQUEUE_REGS,
    /** QM queue status registers */
    (void *) CSL_QM_SS_CFG_QUE_PEEK_REGS,
    /** QM INTD registers */
    (void *) CSL_QM_SS_CFG_INTD_REGS,
    /** QM PDSP 1 command register */
    {
        (void *) CSL_QM_SS_CFG_SCRACH_RAM1_REGS,
        /** QM PDSP 2 command register */
        (void *) CSL_QM_SS_CFG_SCRACH_RAM2_REGS,
    },
    /** QM PDSP 1 control register */
    {
        (void *) CSL_QM_SS_CFG_ADSP1_REGS,
        /** QM PDSP 2 control register */
        (void *) CSL_QM_SS_CFG_ADSP2_REGS,
    },
    /** QM PDSP 1 IRAM register */
    {
        (void *) CSL_QM_SS_CFG_APDSP1_RAM_REGS,
        /** QM PDSP 2 IRAM register */
        (void *) CSL_QM_SS_CFG_APDSP2_RAM_REGS,
    },
    /** QM Status RAM */
    (void *) CSL_QM_SS_CFG_QM_STATUS_RAM_REGS,
    /** QM Linking RAM register */
    (void *) CSL_QM_SS_CFG_LINKING_RAM_REGS,
    /** QM McDMA register */
    (void *) CSL_QM_SS_CFG_MCDMA_REGS,
    /** QM Timer16 register */
    {
        (void *) CSL_QM_SS_CFG_TIMER1_REGS,
        (void *) CSL_QM_SS_CFG_TIMER2_REGS,
    },
    /** QM queue Management registers, accessed via DMA port */
    (void *) CSL_QM_SS_DATA_QM_QUEUE_DEQUEUE_REGS,
    /** QM queue Management Proxy registers, accessed via DMA port */
    (void *) CSL_QM_SS_DATA_PROXY_QUEUE_DEQUEUE_REGS,

};

/**
@}
*/

