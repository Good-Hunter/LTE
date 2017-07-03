
#ifndef _CSLR_CPPIDMA_TX_SCHEDULER_CONFIG_H_
#define _CSLR_CPPIDMA_TX_SCHEDULER_CONFIG_H_



#include "cslr.h"


/* Minimum unit = 1 byte */

/**************************************************************************\
* Register Overlay Structure
\**************************************************************************/
typedef struct  {
    volatile unsigned int TX_CHANNEL_SCHEDULER_CONFIG_REG[129];
} CSL_Cppidma_tx_scheduler_configRegs;

/**************************************************************************\
* Field Definition Macros
\**************************************************************************/

/* TX_CHANNEL_SCHEDULER_CONFIG_REG */

#define CSL_CPPIDMA_TX_SCHEDULER_CONFIG_TX_CHANNEL_SCHEDULER_CONFIG_REG_PRIORITY_MASK (0x00000003u)
#define CSL_CPPIDMA_TX_SCHEDULER_CONFIG_TX_CHANNEL_SCHEDULER_CONFIG_REG_PRIORITY_SHIFT (0x00000000u)
#define CSL_CPPIDMA_TX_SCHEDULER_CONFIG_TX_CHANNEL_SCHEDULER_CONFIG_REG_PRIORITY_RESETVAL (0x00000000u)

#define CSL_CPPIDMA_TX_SCHEDULER_CONFIG_TX_CHANNEL_SCHEDULER_CONFIG_REG_RESETVAL (0x00000000u)

#endif
