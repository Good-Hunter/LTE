
#ifndef _CSLR_CPPIDMA_RX_CHANNEL_CONFIG_H_
#define _CSLR_CPPIDMA_RX_CHANNEL_CONFIG_H_



#include "cslr.h"


/* Minimum unit = 1 byte */

/**************************************************************************\
* Register Overlay Structure for RX_CHANNEL_GLOBAL_CONFIG
\**************************************************************************/
typedef struct  {
    volatile unsigned int RX_CHANNEL_GLOBAL_CONFIG_REG;
    volatile unsigned char RSVD0[28];
} CSL_Cppidma_rx_channel_configRx_channel_global_configRegs;

/**************************************************************************\
* Register Overlay Structure
\**************************************************************************/
typedef struct  {
    CSL_Cppidma_rx_channel_configRx_channel_global_configRegs RX_CHANNEL_GLOBAL_CONFIG[129];
} CSL_Cppidma_rx_channel_configRegs;

/**************************************************************************\
* Field Definition Macros
\**************************************************************************/

/* RX_CHANNEL_GLOBAL_CONFIG_REG */

#define CSL_CPPIDMA_RX_CHANNEL_CONFIG_RX_CHANNEL_GLOBAL_CONFIG_REG_RX_PAUSE_MASK (0x20000000u)
#define CSL_CPPIDMA_RX_CHANNEL_CONFIG_RX_CHANNEL_GLOBAL_CONFIG_REG_RX_PAUSE_SHIFT (0x0000001Du)
#define CSL_CPPIDMA_RX_CHANNEL_CONFIG_RX_CHANNEL_GLOBAL_CONFIG_REG_RX_PAUSE_RESETVAL (0x00000000u)

#define CSL_CPPIDMA_RX_CHANNEL_CONFIG_RX_CHANNEL_GLOBAL_CONFIG_REG_RX_TEARDOWN_MASK (0x40000000u)
#define CSL_CPPIDMA_RX_CHANNEL_CONFIG_RX_CHANNEL_GLOBAL_CONFIG_REG_RX_TEARDOWN_SHIFT (0x0000001Eu)
#define CSL_CPPIDMA_RX_CHANNEL_CONFIG_RX_CHANNEL_GLOBAL_CONFIG_REG_RX_TEARDOWN_RESETVAL (0x00000000u)

#define CSL_CPPIDMA_RX_CHANNEL_CONFIG_RX_CHANNEL_GLOBAL_CONFIG_REG_RX_ENABLE_MASK (0x80000000u)
#define CSL_CPPIDMA_RX_CHANNEL_CONFIG_RX_CHANNEL_GLOBAL_CONFIG_REG_RX_ENABLE_SHIFT (0x0000001Fu)
#define CSL_CPPIDMA_RX_CHANNEL_CONFIG_RX_CHANNEL_GLOBAL_CONFIG_REG_RX_ENABLE_RESETVAL (0x00000000u)

#define CSL_CPPIDMA_RX_CHANNEL_CONFIG_RX_CHANNEL_GLOBAL_CONFIG_REG_RESETVAL (0x00000000u)

#endif
