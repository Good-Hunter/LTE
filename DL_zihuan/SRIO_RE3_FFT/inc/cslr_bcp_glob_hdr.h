
#ifndef _CSLR_GLOB_HDR_H_
#define _CSLR_GLOB_HDR_H_

#include "cslr.h"



/* Minimum unit = 1 byte */

/**************************************************************************\
* Register Overlay Structure
\**************************************************************************/

/**************************************************************************\
* Field Definition Macros
\**************************************************************************/

/* Word 0 */
#define CSL_GLOB_HDR_WORD0_FLOW_ID_MASK  (0x3F000000u)
#define CSL_GLOB_HDR_WORD0_FLOW_ID_SHIFT (0x00000018u)
#define CSL_GLOB_HDR_WORD0_FLOW_ID_RESETVAL (0x00000000u)

#define CSL_GLOB_HDR_WORD0_HDR_END_PTR_MASK (0x00FF0000u)
#define CSL_GLOB_HDR_WORD0_HDR_END_PTR_SHIFT (0x00000010u)
#define CSL_GLOB_HDR_WORD0_HDR_END_PTR_RESETVAL (0x00000000u)

#define CSL_GLOB_HDR_WORD0_RADIO_STANDARD_MASK (0x00000700u)
#define CSL_GLOB_HDR_WORD0_RADIO_STANDARD_SHIFT (0x00000008u)
#define CSL_GLOB_HDR_WORD0_RADIO_STANDARD_RESETVAL (0x00000000u)

#define CSL_GLOB_HDR_WORD0_HALT_MASK     (0x00000020u)
#define CSL_GLOB_HDR_WORD0_HALT_SHIFT    (0x00000005u)
#define CSL_GLOB_HDR_WORD0_HALT_RESETVAL (0x00000000u)

#define CSL_GLOB_HDR_WORD0_DROP_MASK     (0x00000010u)
#define CSL_GLOB_HDR_WORD0_DROP_SHIFT    (0x00000004u)
#define CSL_GLOB_HDR_WORD0_DROP_RESETVAL (0x00000000u)

#define CSL_GLOB_HDR_WORD0_FLUSH_MASK    (0x00000008u)
#define CSL_GLOB_HDR_WORD0_FLUSH_SHIFT   (0x00000003u)
#define CSL_GLOB_HDR_WORD0_FLUSH_RESETVAL (0x00000000u)

#define CSL_GLOB_HDR_WORD0_PKT_TYPE_MASK (0x00000003u)
#define CSL_GLOB_HDR_WORD0_PKT_TYPE_SHIFT (0x00000000u)
#define CSL_GLOB_HDR_WORD0_PKT_TYPE_RESETVAL (0x00000000u)

#define CSL_GLOB_HDR_WORD0_RESETVAL      (0x00000000u)

/* Word 1 */
#define CSL_GLOB_HDR_WORD1_TAG_MASK      (0xFFFFFFFFu)
#define CSL_GLOB_HDR_WORD1_TAG_SHIFT     (0x00000000u)
#define CSL_GLOB_HDR_WORD1_TAG_RESETVAL  (0x00000000u)

#define CSL_GLOB_HDR_WORD1_RESETVAL      (0x00000000u)

#endif
