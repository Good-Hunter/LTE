
#ifndef _CSL_PSC_H_
#define _CSL_PSC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "soc.h"
#include "csl.h"
#include "cslr_psc.h"


#define     hPscRegs                            ((CSL_PscRegs *) (CSL_PSC_REGS))

/** @brief      
 *
 *  Possible PSC Power domain states
 */        
typedef enum {
    /** Power domain is Off */        
    PSC_PDSTATE_OFF = 0,

    /** Power domain is On */        
    PSC_PDSTATE_ON = 1
} CSL_PSC_PDSTATE;

/** @brief      
 *
 *  Possible PSC Module states
 */        
typedef enum {
    /** Module is in Reset state. Clock is off. */        
    PSC_MODSTATE_SWRSTDISABLE = 0,

    /** Module is in Sync Reset state. */        
    PSC_MODSTATE_SYNCRST = 1,

    /** Module is in disable state. */        
    PSC_MODSTATE_DISABLE = 2,

    /** Module is in enable state. */        
    PSC_MODSTATE_ENABLE = 3,

    /** Module is in Auto sleep state */        
    PSC_MODSTATE_AUTOSLP = 4,

    /** Module is in Auto wake state */        
    PSC_MODSTATE_AUTOWK = 5
} CSL_PSC_MODSTATE;

/** @brief      
 *
 *  Possible module local reset status
 */        
typedef enum {
    /** Module local reset asserted */        
    PSC_MDLRST_ASSERTED = 0,

    /** Module local reset deasserted */        
    PSC_MDLRST_DEASSERTED = 1
} CSL_PSC_MDLRST;

/** @brief      
 *
 *  Possible module reset status
 */        
typedef enum {
    /** Module reset asserted */        
    PSC_MDRST_ASSERTED = 0,

    /** Module reset deasserted */        
    PSC_MDRST_DEASSERTED = 1
} CSL_PSC_MDRST;


#ifdef __cplusplus
}
#endif

#endif  /* _CSL_PSC_H_ */

