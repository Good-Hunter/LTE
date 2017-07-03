
#ifndef _CSLR_MCDMA_H_
#define _CSLR_MCDMA_H_



#include "cslr.h"


/* Minimum unit = 1 byte */

/**************************************************************************\
* Register Overlay Structure for MCDMA_CHANNEL_CONFIG_GROUP
\**************************************************************************/
typedef struct  {
    volatile unsigned int SRC_ADDR_REG;
    volatile unsigned int DST_ADDR_REG;
    volatile unsigned int XFER_CTRL_REG;
    volatile unsigned char RSVD0[52];
} CSL_McdmaMcdma_channel_config_groupRegs;

/**************************************************************************\
* Register Overlay Structure
\**************************************************************************/
typedef struct  {
    CSL_McdmaMcdma_channel_config_groupRegs MCDMA_CHANNEL_CONFIG_GROUP[4];
} CSL_McdmaRegs;

/**************************************************************************\
* Field Definition Macros
\**************************************************************************/

/* SRC_ADDR_REG */

#define CSL_MCDMA_SRC_ADDR_REG_SRC_ADDRESS_MASK (0xFFFFFFFFu)
#define CSL_MCDMA_SRC_ADDR_REG_SRC_ADDRESS_SHIFT (0x00000000u)
#define CSL_MCDMA_SRC_ADDR_REG_SRC_ADDRESS_RESETVAL (0x00000000u)

#define CSL_MCDMA_SRC_ADDR_REG_RESETVAL  (0x00000000u)

/* DST_ADDR_REG */

#define CSL_MCDMA_DST_ADDR_REG_DST_ADDRESS_MASK (0xFFFFFFFFu)
#define CSL_MCDMA_DST_ADDR_REG_DST_ADDRESS_SHIFT (0x00000000u)
#define CSL_MCDMA_DST_ADDR_REG_DST_ADDRESS_RESETVAL (0x00000000u)

#define CSL_MCDMA_DST_ADDR_REG_RESETVAL  (0x00000000u)

/* XFER_CTRL_REG */

#define CSL_MCDMA_XFER_CTRL_REG_TRANSFER_LENGTH_MASK (0x0003FFFFu)
#define CSL_MCDMA_XFER_CTRL_REG_TRANSFER_LENGTH_SHIFT (0x00000000u)
#define CSL_MCDMA_XFER_CTRL_REG_TRANSFER_LENGTH_RESETVAL (0x00000000u)

#define CSL_MCDMA_XFER_CTRL_REG_BURST_MODE_MASK (0x00300000u)
#define CSL_MCDMA_XFER_CTRL_REG_BURST_MODE_SHIFT (0x00000014u)
#define CSL_MCDMA_XFER_CTRL_REG_BURST_MODE_RESETVAL (0x00000000u)

#define CSL_MCDMA_XFER_CTRL_REG_SRC_ADDR_MODE_MASK (0x00C00000u)
#define CSL_MCDMA_XFER_CTRL_REG_SRC_ADDR_MODE_SHIFT (0x00000016u)
#define CSL_MCDMA_XFER_CTRL_REG_SRC_ADDR_MODE_RESETVAL (0x00000000u)

#define CSL_MCDMA_XFER_CTRL_REG_DST_ADDR_MODE_MASK (0x03000000u)
#define CSL_MCDMA_XFER_CTRL_REG_DST_ADDR_MODE_SHIFT (0x00000018u)
#define CSL_MCDMA_XFER_CTRL_REG_DST_ADDR_MODE_RESETVAL (0x00000000u)

#define CSL_MCDMA_XFER_CTRL_REG_PRIORITY_MASK (0x1C000000u)
#define CSL_MCDMA_XFER_CTRL_REG_PRIORITY_SHIFT (0x0000001Au)
#define CSL_MCDMA_XFER_CTRL_REG_PRIORITY_RESETVAL (0x00000007u)

#define CSL_MCDMA_XFER_CTRL_REG_ACTIVE_MASK (0x40000000u)
#define CSL_MCDMA_XFER_CTRL_REG_ACTIVE_SHIFT (0x0000001Eu)
#define CSL_MCDMA_XFER_CTRL_REG_ACTIVE_RESETVAL (0x00000000u)

#define CSL_MCDMA_XFER_CTRL_REG_START_STOP_MASK (0x80000000u)
#define CSL_MCDMA_XFER_CTRL_REG_START_STOP_SHIFT (0x0000001Fu)
#define CSL_MCDMA_XFER_CTRL_REG_START_STOP_RESETVAL (0x00000000u)

#define CSL_MCDMA_XFER_CTRL_REG_RESETVAL (0x1C000000u)

#endif
