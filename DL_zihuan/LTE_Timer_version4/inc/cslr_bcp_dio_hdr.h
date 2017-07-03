
#ifndef _CSLR_DIO_HDR_H_
#define _CSLR_DIO_HDR_H_

#include "cslr.h"


/* Minimum unit = 1 byte */

/**************************************************************************\
* Register Overlay Structure
\**************************************************************************/

/**************************************************************************\
* Field Definition Macros
\**************************************************************************/

/* Word 0 */
#define CSL_DIO_HDR_WORD0_LOCAL_HDR_LEN_MASK (0x000000FFu)
#define CSL_DIO_HDR_WORD0_LOCAL_HDR_LEN_SHIFT (0x00000000u)
#define CSL_DIO_HDR_WORD0_LOCAL_HDR_LEN_RESETVAL (0x00000000u)

#define CSL_DIO_HDR_WORD0_MOD_ID_MASK    (0x00000F00u)
#define CSL_DIO_HDR_WORD0_MOD_ID_SHIFT   (0x00000008u)
#define CSL_DIO_HDR_WORD0_MOD_ID_RESETVAL (0x00000000u)

#define CSL_DIO_HDR_WORD0_DIO_ENDIAN_MASK  (0x00180000u)
#define CSL_DIO_HDR_WORD0_DIO_ENDIAN_SHIFT (0x00000013u)
#define CSL_DIO_HDR_WORD0_DIO_ENDIAN_RESETVAL (0x00000000u)

#define CSL_DIO_HDR_WORD0_DIO_RD_WR_MASK  (0x01000000u)
#define CSL_DIO_HDR_WORD0_DIO_RD_WR_SHIFT (0x00000018u)
#define CSL_DIO_HDR_WORD0_DIO_RD_WR_RESETVAL (0x00000000u)

#define CSL_DIO_HDR_WORD0_DIO_BLK_CNT_MASK  (0x0E000000u)
#define CSL_DIO_HDR_WORD0_DIO_BLK_CNT_SHIFT (0x00000019u)
#define CSL_DIO_HDR_WORD0_DIO_BLK_CNT_RESETVAL (0x00000000u)

#define CSL_DIO_HDR_WORD0_RESETVAL       (0x00000000u)

/* Word 1,3,5,7,9,11 (addresses 0..5) */
#define CSL_DIO_HDR_WORDX_ADDRESS_X_MASK (0xFFFFFFFFu)
#define CSL_DIO_HDR_WORDX_ADDRESS_X_SHIFT (0x00000000u)
#define CSL_DIO_HDR_WORDX_ADDRESS_X_RESETVAL (0x00000000u)

/* Word 2,4,6,8,10,12 (counts 0..5) */
#define CSL_DIO_HDR_WORDX_COUNT_X_MASK (0x01FFFFFFu)
#define CSL_DIO_HDR_WORDX_COUNT_X_SHIFT (0x00000000u)
#define CSL_DIO_HDR_WORDX_COUNT_X_RESETVAL (0x00000000u)

#define CSL_DIO_HDR_WORDX_RESETVAL       (0x00000000u)

#endif
