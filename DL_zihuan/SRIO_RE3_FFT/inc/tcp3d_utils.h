#ifndef _TCP3D_UTILS_H_
#define _TCP3D_UTILS_H_

/* c99 types includes */
#include <stdint.h>
#include <stdlib.h>

/* ========================================================================= */
/**
 * @brief  Macro for providing the address with the alignment requested
 */
#define ALIGN(Addr, Algnmt) ((Addr+(Algnmt)-1)&(0xFFFF-(Algnmt)+1))

/**
 * @brief  Macro for computing minimum of the two values
 */
#define MIN(x,y)            ((x) < (y) ? (x):(y))

/**
 * @brief  Macro for computing maximum of the two values
 */
#define MAX(x,y)            ((x) > (y) ? (x):(y))

/**
 * @brief   Macro for computing hard decisions size in words 
 */
#define COMPUTE_HD_WORD_SIZE(x) (((x)+31)>>5)

/**
 * @brief   Macro for computing hard decisions size in bytes
 */
#define COMPUTE_HD_BYTE_SIZE(x) (COMPUTE_HD_WORD_SIZE(x)*4)

/**
 * @brief   Macro for computing Kext value for WCDMA using the formula
 *              Kext=4*Ceil(K/4)
 */
#define COMPUTE_KEXT(a)         (4*((a+3)>>2))

/**
 * @brief   Macro for computing Kout value for WCDMA using the formula
 *              Kout = (2*KEXT-K).
 * @ref     COMPUTE_KEXT macro
 */
#define COMPUTE_KOUT(x)         ((2*COMPUTE_KEXT(x))-(x))

/**
 * @brief   Macro for computing Kt value for WCDMA using the formula
 *              Kt = 3 - (Kext-K)
 * @ref     COMPUTE_KOUT macro
 */
#define COMPUTE_KT(x)           (3 -(COMPUTE_KEXT(x)-(x)))


unsigned int Tcp3d_glbMap (unsigned char coreID, unsigned int addr);


unsigned int Tcp3d_div32by16(unsigned int num, unsigned short den);



#endif /* _TCP3D_UTILS_H_ */
