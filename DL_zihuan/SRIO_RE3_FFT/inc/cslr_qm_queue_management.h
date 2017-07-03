
#ifndef _CSLR_QM_QUEUE_MANAGEMENT_H_
#define _CSLR_QM_QUEUE_MANAGEMENT_H_


#include "cslr.h"


/* Minimum unit = 1 byte */

/**************************************************************************\
* Register Overlay Structure for QUEUE_MGMT_GROUP
\**************************************************************************/
typedef struct  {
    volatile unsigned int QUEUE_REG_A;
    volatile unsigned int QUEUE_REG_B;
    volatile unsigned int QUEUE_REG_C;
    volatile unsigned int QUEUE_REG_D;
} CSL_Qm_queue_managementQueue_mgmt_groupRegs;

/**************************************************************************\
* Register Overlay Structure
\**************************************************************************/
typedef struct  {
    CSL_Qm_queue_managementQueue_mgmt_groupRegs QUEUE_MGMT_GROUP[8192];
} CSL_Qm_queue_managementRegs;

/**************************************************************************\
* Field Definition Macros
\**************************************************************************/

/* QUEUE_REG_A */

#define CSL_QM_QUEUE_MANAGEMENT_QUEUE_REG_A_QUEUE_ENTRY_COUNT_MASK (0x0007FFFFu)
#define CSL_QM_QUEUE_MANAGEMENT_QUEUE_REG_A_QUEUE_ENTRY_COUNT_SHIFT (0x00000000u)
#define CSL_QM_QUEUE_MANAGEMENT_QUEUE_REG_A_QUEUE_ENTRY_COUNT_RESETVAL (0x00000000u)

#define CSL_QM_QUEUE_MANAGEMENT_QUEUE_REG_A_RESETVAL (0x00000000u)

/* QUEUE_REG_B */

#define CSL_QM_QUEUE_MANAGEMENT_QUEUE_REG_B_QUEUE_BYTE_COUNT_MASK (0xFFFFFFFFu)
#define CSL_QM_QUEUE_MANAGEMENT_QUEUE_REG_B_QUEUE_BYTE_COUNT_SHIFT (0x00000000u)
#define CSL_QM_QUEUE_MANAGEMENT_QUEUE_REG_B_QUEUE_BYTE_COUNT_RESETVAL (0x00000000u)

#define CSL_QM_QUEUE_MANAGEMENT_QUEUE_REG_B_RESETVAL (0x00000000u)

/* QUEUE_REG_C */

#define CSL_QM_QUEUE_MANAGEMENT_QUEUE_REG_C_HEAD_TAIL_MASK (0x80000000u)
#define CSL_QM_QUEUE_MANAGEMENT_QUEUE_REG_C_HEAD_TAIL_SHIFT (0x0000001Fu)
#define CSL_QM_QUEUE_MANAGEMENT_QUEUE_REG_C_HEAD_TAIL_RESETVAL (0x00000000u)

#define CSL_QM_QUEUE_MANAGEMENT_QUEUE_REG_C_PACKET_SIZE_MASK (0x0001FFFFu)
#define CSL_QM_QUEUE_MANAGEMENT_QUEUE_REG_C_PACKET_SIZE_SHIFT (0x00000000u)
#define CSL_QM_QUEUE_MANAGEMENT_QUEUE_REG_C_PACKET_SIZE_RESETVAL (0x00000000u)

#define CSL_QM_QUEUE_MANAGEMENT_QUEUE_REG_C_RESETVAL (0x00000000u)

/* QUEUE_REG_D */

#define CSL_QM_QUEUE_MANAGEMENT_QUEUE_REG_D_DESC_PTR_MASK (0xFFFFFFF0u)
#define CSL_QM_QUEUE_MANAGEMENT_QUEUE_REG_D_DESC_PTR_SHIFT (0x00000004u)
#define CSL_QM_QUEUE_MANAGEMENT_QUEUE_REG_D_DESC_PTR_RESETVAL (0x00000000u)

#define CSL_QM_QUEUE_MANAGEMENT_QUEUE_REG_D_DESC_INFO_MASK (0x0000000Fu)
#define CSL_QM_QUEUE_MANAGEMENT_QUEUE_REG_D_DESC_INFO_SHIFT (0x00000000u)
#define CSL_QM_QUEUE_MANAGEMENT_QUEUE_REG_D_DESC_INFO_RESETVAL (0x00000000u)

#define CSL_QM_QUEUE_MANAGEMENT_QUEUE_REG_D_RESETVAL (0x00000000u)

#endif
