
#ifndef _CSLR_DIO_MMR_H_
#define _CSLR_DIO_MMR_H_

#include "cslr.h"


/* Minimum unit = 1 byte */

/**************************************************************************\
* Register Overlay Structure
\**************************************************************************/
typedef struct  {
    volatile unsigned int VBUSM_PRIORITY;
} CSL_Dio_mmrRegs;

/**************************************************************************\
* Field Definition Macros
\**************************************************************************/

/* VBUSM_PRIORITY */

#define CSL_DIO_MMR_VBUSM_PRIORITY_MASK       (0x00000007u)
#define CSL_DIO_MMR_VBUSM_PRIORITY_SHIFT      (0x00000000u)
#define CSL_DIO_MMR_VBUSM_PRIORITY_RESETVAL   (0x00000000u)

#define CSL_DIO_MMR_VBUSM_PRIORITY_RESETVAL   (0x00000000u)

#endif
