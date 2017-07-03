
 
#ifndef _CSL_CHIP_H_
#define _CSL_CHIP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "csl.h"
#include "cslr_chip.h"
#include <c6x.h>        
#include "soc.h"


typedef enum  {
    /** Addressing Mode Register */
    CSL_CHIP_AMR = 0,
    /** Control Status Register */
    CSL_CHIP_CSR = 1,
    /** signed interrupt Flag Register */
    CSL_CHIP_IFR = 2,
    /** signed interrupt Set Register */
    CSL_CHIP_ISR = 2,
    /** signed interrupt Clear Register */
    CSL_CHIP_ICR = 3,
    /** signed interrupt Enable Register */
    CSL_CHIP_IER = 4,
    /** signed interrupt Service Table Pointer Register */
    CSL_CHIP_ISTP= 5,
    /** signed interrupt Return Pointer Register */
    CSL_CHIP_IRP= 6,
    /** Nonmaskable signed interrupt (NMI) Return Pointer Register */
    CSL_CHIP_NRP= 7,
    /** Exception Return Pointer Register */
    CSL_CHIP_ERP= 7,
    /** Time Stamp Counter Register - Low */
    CSL_CHIP_TSCL= 10,
    /** Time Stamp Counter Registers - High */
    CSL_CHIP_TSCH= 11,
    /** Analysis Return Pointer */
    CSL_CHIP_ARP= 12,    
    /** SPLOOP Inner Loop Count Register */
    CSL_CHIP_ILC= 13,
    /** SPLOOP Reload Inner Loop Count Register */
    CSL_CHIP_RILC= 14,
    /** Restricted Entry Point Address Register */
    CSL_CHIP_REP= 15,
    /** E1 Phase Program Counter */
    CSL_CHIP_PCE1= 16,
    /** DSP Core Number Register */
    CSL_CHIP_DNUM= 17,
    /** Saturation Status Register */
    CSL_CHIP_SSR= 21,
    /** GMPY Polynomial.A Side Register */
    CSL_CHIP_GPLYA= 22,
    /** GMPY Polynomial.B Side Register */
    CSL_CHIP_GPLYB= 23,
    /** Galois Field Polynomial Generator Function Register */
    CSL_CHIP_GFPGFR= 24,
    /** Debug interrupt enable register */
    CSL_CHIP_DIER=25,
    /** Task State Register */
    CSL_CHIP_TSR= 26,
    /** signed interrupt Task State Register */
    CSL_CHIP_ITSR= 27,
    /** NMI/Exception Task State Register */
    CSL_CHIP_NTSR= 28,
    /** Exception Flag Register */
    CSL_CHIP_EFR= 29,
    /** Exception Clear Register */
    CSL_CHIP_ECR= 29,
    /** signed internal Exception Report Register */
    CSL_CHIP_IERR= 31
} CSL_ChipReg;

/**
@}
*/

/* CHIP global function declarations   */

extern unsigned int  CSL_chipWriteReg (
    CSL_ChipReg    reg,
    CSL_Reg32      val
);

extern unsigned int  CSL_chipReadReg(
    CSL_ChipReg    reg
);

#ifdef __cplusplus
}
#endif

#endif
