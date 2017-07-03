
#ifndef _CSLR_TM_HDR_H_
#define _CSLR_TM_HDR_H_

#include "cslr.h"


/* Minimum unit = 1 byte */

/**************************************************************************\
* Register Overlay Structure
\**************************************************************************/

/**************************************************************************\
* Field Definition Macros
\**************************************************************************/

/* Word 0 */
#define CSL_TM_HDR_WORD0_LOCAL_HDR_LEN_MASK (0x000000FFu)
#define CSL_TM_HDR_WORD0_LOCAL_HDR_LEN_SHIFT (0x00000000u)
#define CSL_TM_HDR_WORD0_LOCAL_HDR_LEN_RESETVAL (0x00000000u)

#define CSL_TM_HDR_WORD0_MOD_ID_MASK     (0x00000F00u)
#define CSL_TM_HDR_WORD0_MOD_ID_SHIFT    (0x00000008u)
#define CSL_TM_HDR_WORD0_MOD_ID_RESETVAL (0x00000000u)

#define CSL_TM_HDR_WORD0_RESETVAL        (0x00000000u)

/* Word 1 */
#define CSL_TM_HDR_WORD1_PS_DATA_SIZE_MASK (0x0000000Fu)
#define CSL_TM_HDR_WORD1_PS_DATA_SIZE_SHIFT (0x00000000u)
#define CSL_TM_HDR_WORD1_PS_DATA_SIZE_RESETVAL (0x00000000u)

#define CSL_TM_HDR_WORD1_INFO_DATA_SIZE_MASK (0x000001f0u)
#define CSL_TM_HDR_WORD1_INFO_DATA_SIZE_SHIFT (0x00000004u)
#define CSL_TM_HDR_WORD1_INFO_DATA_SIZE_RESETVAL (0x00000000u)

#define CSL_TM_HDR_WORD1_RESETVAL        (0x00000000u)
#endif
