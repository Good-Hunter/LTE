
 
#ifndef _CSL_CACHE_H_
#define _CSL_CACHE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "csl.h"
#include "cslr_cgem.h"
#include "soc.h"


/** L2 Line Size */
#define CACHE_L2_LINESIZE    128 
/** L1D Line Size */
#define CACHE_L1D_LINESIZE    64
/** L1P Line Size */
#define CACHE_L1P_LINESIZE    32

/** Handle to the CGEM Register Layer */
#define hCache      ((CSL_CgemRegs *)CSL_CGEM0_5_REG_BASE_ADDRESS_REGS)

/** Cache Round to Line size */        
#define CACHE_ROUND_TO_LINESIZE(CACHE,ELCNT,ELSIZE)\
( ( ( ( (ELCNT) * (ELSIZE)\
        + CACHE_##CACHE##_LINESIZE - 1\
      ) / CACHE_##CACHE##_LINESIZE\
      * CACHE_##CACHE##_LINESIZE\
    ) + (ELSIZE) - 1\
  ) / (ELSIZE)\
)
/**
@}
*/


typedef enum 
{
    /** No blocking, the call exits after programmation of the 
     *  control registers 
     */
    CACHE_NOWAIT = 0,
                         
    /** Blocking Call, the call exits after the relevant cache 
     *  status registers indicate completion. For block coherence
     *  this waits on the Word count register to be come 0.
     */
    CACHE_WAIT   = 1,

    /** Blocking Call, For block coherence this uses the MFENCE to 
     * wait for completion
     */
    CACHE_FENCE_WAIT  = 2    
}CACHE_Wait;

/** @brief Enumeration for L1 (P or D) Sizes */

typedef enum { 
    /** No Cache    */                 
    CACHE_L1_0KCACHE  = 0,
    /** 4KB Cache   */                               
    CACHE_L1_4KCACHE  = 1,
    /** 8KB Cache   */                 
    CACHE_L1_8KCACHE  = 2,
    /** 16KB Cache  */                                 
    CACHE_L1_16KCACHE = 3,
    /** 32KB Cache  */                
    CACHE_L1_32KCACHE = 4,
    /** MAX Cache Size */                
    CACHE_L1_MAXIM1   = 5,
    /** MAX Cache Size */                
    CACHE_L1_MAXIM2   = 6,      
    /** MAX Cache Size */                
    CACHE_L1_MAXIM3   = 7          
} CACHE_L1Size;

/** @brief Enumeration for L2 Sizes */

    /** For devices that have maximum caches less 
     * than the defined values, setting a higher value 
     * will map to maximum cache 
     */
typedef enum { 
    /** No Cache     */
    CACHE_0KCACHE   = 0,  
    /** 32KB Cache   */
    CACHE_32KCACHE  = 1, 
    /** 64KB Cache   */
    CACHE_64KCACHE  = 2, 
    /** 128KB Cache  */
    CACHE_128KCACHE = 3,
    /** 256KB Cache  */
    CACHE_256KCACHE = 4,
    /** 512KB Cache  */
    CACHE_512KCACHE = 5,
    /* 1024KB Cache  */
    CACHE_1024KCACHE = 6 
} CACHE_L2Size; 

/**
@}
*/

#ifdef __cplusplus
}
#endif

#endif /*_CSL_CACHE_H_*/
