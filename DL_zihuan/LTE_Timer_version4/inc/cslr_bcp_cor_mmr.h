
#ifndef _CSLR_COR_MMR_H_
#define _CSLR_COR_MMR_H_

#include "cslr.h"



/* Minimum unit = 1 byte */

/**************************************************************************\
* Register Overlay Structure
\**************************************************************************/
typedef struct  {
    volatile unsigned int M[13];
    volatile unsigned int CONTROL;
} CSL_Cor_mmrRegs;

/**************************************************************************\
* Field Definition Macros
\**************************************************************************/

/* M0 */
#define CSL_COR_MMR_M0_MOD_EN_MASK       (0x000FFFFFu)
#define CSL_COR_MMR_M0_MOD_EN_SHIFT      (0x00000000u)
#define CSL_COR_MMR_M0_MOD_EN_RESETVAL   (0x000FFFFFu)

#define CSL_COR_MMR_M0_RESETVAL          (0x000FFFFFu)

/* M1 */
#define CSL_COR_MMR_M1_MOD_EN_MASK       (0x000FFFFFu)
#define CSL_COR_MMR_M1_MOD_EN_SHIFT      (0x00000000u)
#define CSL_COR_MMR_M1_MOD_EN_RESETVAL   (0x0005A933u)

#define CSL_COR_MMR_M1_RESETVAL          (0x0005A933u)

/* M2 */
#define CSL_COR_MMR_M2_MOD_EN_MASK       (0x000FFFFFu)
#define CSL_COR_MMR_M2_MOD_EN_SHIFT      (0x00000000u)
#define CSL_COR_MMR_M2_MOD_EN_RESETVAL   (0x00010E5Au)

#define CSL_COR_MMR_M2_RESETVAL          (0x00010E5Au)

/* M3 */
#define CSL_COR_MMR_M3_MOD_EN_MASK       (0x000FFFFFu)
#define CSL_COR_MMR_M3_MOD_EN_SHIFT      (0x00000000u)
#define CSL_COR_MMR_M3_MOD_EN_RESETVAL   (0x0006339Cu)

#define CSL_COR_MMR_M3_RESETVAL          (0x0006339Cu)

/* M4 */
#define CSL_COR_MMR_M4_MOD_EN_MASK       (0x000FFFFFu)
#define CSL_COR_MMR_M4_MOD_EN_SHIFT      (0x00000000u)
#define CSL_COR_MMR_M4_MOD_EN_RESETVAL   (0x0007C3E0u)

#define CSL_COR_MMR_M4_RESETVAL          (0x0007C3E0u)

/* M5 */
#define CSL_COR_MMR_M5_MOD_EN_MASK       (0x000FFFFFu)
#define CSL_COR_MMR_M5_MOD_EN_SHIFT      (0x00000000u)
#define CSL_COR_MMR_M5_MOD_EN_RESETVAL   (0x000FFC00u)

#define CSL_COR_MMR_M5_RESETVAL          (0x000FFC00u)

/* M6 */
#define CSL_COR_MMR_M6_MOD_EN_MASK       (0x000FFFFFu)
#define CSL_COR_MMR_M6_MOD_EN_SHIFT      (0x00000000u)
#define CSL_COR_MMR_M6_MOD_EN_RESETVAL   (0x000D8E64u)

#define CSL_COR_MMR_M6_RESETVAL          (0x000D8E64u)

/* M7 */
#define CSL_COR_MMR_M7_MOD_EN_MASK       (0x000FFFFFu)
#define CSL_COR_MMR_M7_MOD_EN_SHIFT      (0x00000000u)
#define CSL_COR_MMR_M7_MOD_EN_RESETVAL   (0x0004F5B0u)

#define CSL_COR_MMR_M7_RESETVAL          (0x0004F5B0u)

/* M8 */
#define CSL_COR_MMR_M8_MOD_EN_MASK       (0x000FFFFFu)
#define CSL_COR_MMR_M8_MOD_EN_SHIFT      (0x00000000u)
#define CSL_COR_MMR_M8_MOD_EN_RESETVAL   (0x000218ECu)

#define CSL_COR_MMR_M8_RESETVAL          (0x000218ECu)

/* M9 */
#define CSL_COR_MMR_M9_MOD_EN_MASK       (0x000FFFFFu)
#define CSL_COR_MMR_M9_MOD_EN_SHIFT      (0x00000000u)
#define CSL_COR_MMR_M9_MOD_EN_RESETVAL   (0x0001B746u)

#define CSL_COR_MMR_M9_RESETVAL          (0x0001B746u)

/* M10 */
#define CSL_COR_MMR_M10_MOD_EN_MASK      (0x000FFFFFu)
#define CSL_COR_MMR_M10_MOD_EN_SHIFT     (0x00000000u)
#define CSL_COR_MMR_M10_MOD_EN_RESETVAL  (0x0000FFFFu)

#define CSL_COR_MMR_M10_RESETVAL         (0x0000FFFFu)

/* M11 */
#define CSL_COR_MMR_M11_MOD_EN_MASK      (0x000FFFFFu)
#define CSL_COR_MMR_M11_MOD_EN_SHIFT     (0x00000000u)
#define CSL_COR_MMR_M11_MOD_EN_RESETVAL  (0x00033FFFu)

#define CSL_COR_MMR_M11_RESETVAL         (0x00033FFFu)

/* M12 */
#define CSL_COR_MMR_M12_MOD_EN_MASK      (0x000FFFFFu)
#define CSL_COR_MMR_M12_MOD_EN_SHIFT     (0x00000000u)
#define CSL_COR_MMR_M12_MOD_EN_RESETVAL  (0x0003FFFCu)

#define CSL_COR_MMR_M12_RESETVAL         (0x0003FFFCu)

/* control */
#define CSL_COR_MMR_CONTROL_QPSK_MAP_11_MASK (0x00000003u)
#define CSL_COR_MMR_CONTROL_QPSK_MAP_11_SHIFT (0x00000000u)
#define CSL_COR_MMR_CONTROL_QPSK_MAP_11_RESETVAL (0x00000000u)

#define CSL_COR_MMR_CONTROL_QPSK_MAP_10_MASK (0x0000000Cu)
#define CSL_COR_MMR_CONTROL_QPSK_MAP_10_SHIFT (0x00000002u)
#define CSL_COR_MMR_CONTROL_QPSK_MAP_10_RESETVAL (0x00000000u)

#define CSL_COR_MMR_CONTROL_QPSK_MAP_01_MASK (0x00000030u)
#define CSL_COR_MMR_CONTROL_QPSK_MAP_01_SHIFT (0x00000004u)
#define CSL_COR_MMR_CONTROL_QPSK_MAP_01_RESETVAL (0x00000000u)

#define CSL_COR_MMR_CONTROL_QPSK_MAP_00_MASK (0x000000C0u)
#define CSL_COR_MMR_CONTROL_QPSK_MAP_00_SHIFT (0x00000006u)
#define CSL_COR_MMR_CONTROL_QPSK_MAP_00_RESETVAL (0x00000000u)

#define CSL_COR_MMR_CONTROL_RESETVAL     (0x00000000u)

#endif
