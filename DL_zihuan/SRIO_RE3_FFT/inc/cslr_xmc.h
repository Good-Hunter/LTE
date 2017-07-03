
#ifndef _CSLR_XMC_H_
#define _CSLR_XMC_H_

#include "cslr.h"


/* Minimum unit = 1 byte */

/**************************************************************************\
* Register Overlay Structure for XMPAX
\**************************************************************************/
typedef struct  {
    volatile unsigned int XMPAXL;
    volatile unsigned int XMPAXH;
} CSL_XmcXmpaxRegs;


/**************************************************************************\
* Register Overlay Structure
\**************************************************************************/
typedef struct  {
    CSL_XmcXmpaxRegs XMPAX[16];
    volatile unsigned char RSVD0[384];
    volatile unsigned int XMPFAR;
    volatile unsigned int XMPFSR;
    volatile unsigned int XMPFCR;
    volatile unsigned char RSVD1[116];
    volatile unsigned int MDMAARBX;
    volatile unsigned char RSVD2[124];
    volatile unsigned int XPFCMD;
    volatile unsigned int XPFACS;
    volatile unsigned char RSVD3[8];
    volatile unsigned int XPFAC0;
    volatile unsigned int XPFAC1;
    volatile unsigned int XPFAC2;
    volatile unsigned int XPFAC3;
    volatile unsigned char RSVD4[224];
    volatile unsigned int XPFADDR[8];
} CSL_XmcRegs;

/**************************************************************************\
* Field Definition Macros
\**************************************************************************/

/* XMPAXL */

#define CSL_XMC_XMPAXL_RADDR_MASK        (0xFFFFFF00u)
#define CSL_XMC_XMPAXL_RADDR_SHIFT       (0x00000008u)
#define CSL_XMC_XMPAXL_RADDR_RESETVAL    (0x00000000u)



#define CSL_XMC_XMPAXL_SR_MASK           (0x00000020u)
#define CSL_XMC_XMPAXL_SR_SHIFT          (0x00000005u)
#define CSL_XMC_XMPAXL_SR_RESETVAL       (0x00000000u)

#define CSL_XMC_XMPAXL_SW_MASK           (0x00000010u)
#define CSL_XMC_XMPAXL_SW_SHIFT          (0x00000004u)
#define CSL_XMC_XMPAXL_SW_RESETVAL       (0x00000000u)

#define CSL_XMC_XMPAXL_SX_MASK           (0x00000008u)
#define CSL_XMC_XMPAXL_SX_SHIFT          (0x00000003u)
#define CSL_XMC_XMPAXL_SX_RESETVAL       (0x00000000u)

#define CSL_XMC_XMPAXL_UR_MASK           (0x00000004u)
#define CSL_XMC_XMPAXL_UR_SHIFT          (0x00000002u)
#define CSL_XMC_XMPAXL_UR_RESETVAL       (0x00000000u)

#define CSL_XMC_XMPAXL_UW_MASK           (0x00000002u)
#define CSL_XMC_XMPAXL_UW_SHIFT          (0x00000001u)
#define CSL_XMC_XMPAXL_UW_RESETVAL       (0x00000000u)

#define CSL_XMC_XMPAXL_UX_MASK           (0x00000001u)
#define CSL_XMC_XMPAXL_UX_SHIFT          (0x00000000u)
#define CSL_XMC_XMPAXL_UX_RESETVAL       (0x00000000u)

#define CSL_XMC_XMPAXL_RESETVAL          (0x00000000u)

/* XMPAXH */

#define CSL_XMC_XMPAXH_BADDR_MASK        (0xFFFFF000u)
#define CSL_XMC_XMPAXH_BADDR_SHIFT       (0x0000000Cu)
#define CSL_XMC_XMPAXH_BADDR_RESETVAL    (0x00000000u)


#define CSL_XMC_XMPAXH_SEGSZ_MASK        (0x0000001Fu)
#define CSL_XMC_XMPAXH_SEGSZ_SHIFT       (0x00000000u)
#define CSL_XMC_XMPAXH_SEGSZ_RESETVAL    (0x00000000u)

#define CSL_XMC_XMPAXH_RESETVAL          (0x00000000u)

/* XMPAXL0 */

#define CSL_XMC_XMPAXL0_RADDR_MASK       (0xFFFFFF00u)
#define CSL_XMC_XMPAXL0_RADDR_SHIFT      (0x00000008u)
#define CSL_XMC_XMPAXL0_RADDR_RESETVAL   (0x00000000u)



#define CSL_XMC_XMPAXL0_SR_MASK          (0x00000020u)
#define CSL_XMC_XMPAXL0_SR_SHIFT         (0x00000005u)
#define CSL_XMC_XMPAXL0_SR_RESETVAL      (0x00000001u)

#define CSL_XMC_XMPAXL0_SW_MASK          (0x00000010u)
#define CSL_XMC_XMPAXL0_SW_SHIFT         (0x00000004u)
#define CSL_XMC_XMPAXL0_SW_RESETVAL      (0x00000001u)

#define CSL_XMC_XMPAXL0_SX_MASK          (0x00000008u)
#define CSL_XMC_XMPAXL0_SX_SHIFT         (0x00000003u)
#define CSL_XMC_XMPAXL0_SX_RESETVAL      (0x00000001u)

#define CSL_XMC_XMPAXL0_UR_MASK          (0x00000004u)
#define CSL_XMC_XMPAXL0_UR_SHIFT         (0x00000002u)
#define CSL_XMC_XMPAXL0_UR_RESETVAL      (0x00000001u)

#define CSL_XMC_XMPAXL0_UW_MASK          (0x00000002u)
#define CSL_XMC_XMPAXL0_UW_SHIFT         (0x00000001u)
#define CSL_XMC_XMPAXL0_UW_RESETVAL      (0x00000001u)

#define CSL_XMC_XMPAXL0_UX_MASK          (0x00000001u)
#define CSL_XMC_XMPAXL0_UX_SHIFT         (0x00000000u)
#define CSL_XMC_XMPAXL0_UX_RESETVAL      (0x00000001u)

#define CSL_XMC_XMPAXL0_RESETVAL         (0x000000FFu)

/* XMPAXH0 */

#define CSL_XMC_XMPAXH0_BADDR_MASK       (0xFFFFF000u)
#define CSL_XMC_XMPAXH0_BADDR_SHIFT      (0x0000000Cu)
#define CSL_XMC_XMPAXH0_BADDR_RESETVAL   (0x00000000u)


#define CSL_XMC_XMPAXH0_SEGSZ_MASK       (0x0000001Fu)
#define CSL_XMC_XMPAXH0_SEGSZ_SHIFT      (0x00000000u)
#define CSL_XMC_XMPAXH0_SEGSZ_RESETVAL   (0x0000001Eu)

#define CSL_XMC_XMPAXH0_RESETVAL         (0x0000001Eu)

/* XMPAXL1 */

#define CSL_XMC_XMPAXL1_RADDR_MASK       (0xFFFFFF00u)
#define CSL_XMC_XMPAXL1_RADDR_SHIFT      (0x00000008u)
#define CSL_XMC_XMPAXL1_RADDR_RESETVAL   (0x00800000u)



#define CSL_XMC_XMPAXL1_SR_MASK          (0x00000020u)
#define CSL_XMC_XMPAXL1_SR_SHIFT         (0x00000005u)
#define CSL_XMC_XMPAXL1_SR_RESETVAL      (0x00000001u)

#define CSL_XMC_XMPAXL1_SW_MASK          (0x00000010u)
#define CSL_XMC_XMPAXL1_SW_SHIFT         (0x00000004u)
#define CSL_XMC_XMPAXL1_SW_RESETVAL      (0x00000001u)

#define CSL_XMC_XMPAXL1_SX_MASK          (0x00000008u)
#define CSL_XMC_XMPAXL1_SX_SHIFT         (0x00000003u)
#define CSL_XMC_XMPAXL1_SX_RESETVAL      (0x00000001u)

#define CSL_XMC_XMPAXL1_UR_MASK          (0x00000004u)
#define CSL_XMC_XMPAXL1_UR_SHIFT         (0x00000002u)
#define CSL_XMC_XMPAXL1_UR_RESETVAL      (0x00000001u)

#define CSL_XMC_XMPAXL1_UW_MASK          (0x00000002u)
#define CSL_XMC_XMPAXL1_UW_SHIFT         (0x00000001u)
#define CSL_XMC_XMPAXL1_UW_RESETVAL      (0x00000001u)

#define CSL_XMC_XMPAXL1_UX_MASK          (0x00000001u)
#define CSL_XMC_XMPAXL1_UX_SHIFT         (0x00000000u)
#define CSL_XMC_XMPAXL1_UX_RESETVAL      (0x00000001u)

#define CSL_XMC_XMPAXL1_RESETVAL         (0x800000FFu)

/* XMPAXH1 */

#define CSL_XMC_XMPAXH1_BADDR_MASK       (0xFFFFF000u)
#define CSL_XMC_XMPAXH1_BADDR_SHIFT      (0x0000000Cu)
#define CSL_XMC_XMPAXH1_BADDR_RESETVAL   (0x00080000u)


#define CSL_XMC_XMPAXH1_SEGSZ_MASK       (0x0000001Fu)
#define CSL_XMC_XMPAXH1_SEGSZ_SHIFT      (0x00000000u)
#define CSL_XMC_XMPAXH1_SEGSZ_RESETVAL   (0x0000001Eu)

#define CSL_XMC_XMPAXH1_RESETVAL         (0x8000001Eu)

/* XMPFAR */

#define CSL_XMC_XMPFAR_FADDR_MASK        (0xFFFFFFFFu)
#define CSL_XMC_XMPFAR_FADDR_SHIFT       (0x00000000u)
#define CSL_XMC_XMPFAR_FADDR_RESETVAL    (0x00000000u)

#define CSL_XMC_XMPFAR_RESETVAL          (0x00000000u)

/* XMPFSR */


#define CSL_XMC_XMPFSR_LOCAL_MASK        (0x00000100u)
#define CSL_XMC_XMPFSR_LOCAL_SHIFT       (0x00000008u)
#define CSL_XMC_XMPFSR_LOCAL_RESETVAL    (0x00000000u)



#define CSL_XMC_XMPFSR_SR_MASK           (0x00000020u)
#define CSL_XMC_XMPFSR_SR_SHIFT          (0x00000005u)
#define CSL_XMC_XMPFSR_SR_RESETVAL       (0x00000000u)

#define CSL_XMC_XMPFSR_SW_MASK           (0x00000010u)
#define CSL_XMC_XMPFSR_SW_SHIFT          (0x00000004u)
#define CSL_XMC_XMPFSR_SW_RESETVAL       (0x00000000u)

#define CSL_XMC_XMPFSR_SX_MASK           (0x00000008u)
#define CSL_XMC_XMPFSR_SX_SHIFT          (0x00000003u)
#define CSL_XMC_XMPFSR_SX_RESETVAL       (0x00000000u)

#define CSL_XMC_XMPFSR_UR_MASK           (0x00000004u)
#define CSL_XMC_XMPFSR_UR_SHIFT          (0x00000002u)
#define CSL_XMC_XMPFSR_UR_RESETVAL       (0x00000000u)

#define CSL_XMC_XMPFSR_UW_MASK           (0x00000002u)
#define CSL_XMC_XMPFSR_UW_SHIFT          (0x00000001u)
#define CSL_XMC_XMPFSR_UW_RESETVAL       (0x00000000u)

#define CSL_XMC_XMPFSR_UX_MASK           (0x00000001u)
#define CSL_XMC_XMPFSR_UX_SHIFT          (0x00000000u)
#define CSL_XMC_XMPFSR_UX_RESETVAL       (0x00000000u)

#define CSL_XMC_XMPFSR_RESETVAL          (0x00000000u)

/* XMPFCR */


#define CSL_XMC_XMPFCR_MPFCLR_MASK       (0x00000001u)
#define CSL_XMC_XMPFCR_MPFCLR_SHIFT      (0x00000000u)
#define CSL_XMC_XMPFCR_MPFCLR_RESETVAL   (0x00000000u)

#define CSL_XMC_XMPFCR_RESETVAL          (0x00000000u)

/* MDMAARBX */


#define CSL_XMC_MDMAARBX_PRI_MASK        (0x00070000u)
#define CSL_XMC_MDMAARBX_PRI_SHIFT       (0x00000010u)
#define CSL_XMC_MDMAARBX_PRI_RESETVAL    (0x00000007u)


#define CSL_XMC_MDMAARBX_RESETVAL        (0x00070000u)

/* XPFCMD */


#define CSL_XMC_XPFCMD_ACRST_MASK        (0x00000010u)
#define CSL_XMC_XPFCMD_ACRST_SHIFT       (0x00000004u)
#define CSL_XMC_XPFCMD_ACRST_RESETVAL    (0x00000000u)

#define CSL_XMC_XPFCMD_ACEN_MASK         (0x0000000Cu)
#define CSL_XMC_XPFCMD_ACEN_SHIFT        (0x00000002u)
#define CSL_XMC_XPFCMD_ACEN_RESETVAL     (0x00000000u)

#define CSL_XMC_XPFCMD_ACENL_MASK        (0x00000002u)
#define CSL_XMC_XPFCMD_ACENL_SHIFT       (0x00000001u)
#define CSL_XMC_XPFCMD_ACENL_RESETVAL    (0x00000000u)

#define CSL_XMC_XPFCMD_INV_MASK          (0x00000001u)
#define CSL_XMC_XPFCMD_INV_SHIFT         (0x00000000u)
#define CSL_XMC_XPFCMD_INV_RESETVAL      (0x00000000u)

#define CSL_XMC_XPFCMD_RESETVAL          (0x00000000u)

/* XPFACS */


#define CSL_XMC_XPFACS_ACEN_MASK         (0x0000000Cu)
#define CSL_XMC_XPFACS_ACEN_SHIFT        (0x00000002u)
#define CSL_XMC_XPFACS_ACEN_RESETVAL     (0x00000000u)


#define CSL_XMC_XPFACS_RESETVAL          (0x00000000u)

/* XPFAC0 */


#define CSL_XMC_XPFAC0_SENT_MASK         (0x000FFFFFu)
#define CSL_XMC_XPFAC0_SENT_SHIFT        (0x00000000u)
#define CSL_XMC_XPFAC0_SENT_RESETVAL     (0x00000000u)

#define CSL_XMC_XPFAC0_RESETVAL          (0x00000000u)

/* XPFAC1 */


#define CSL_XMC_XPFAC1_CANCELED_MASK     (0x000FFFFFu)
#define CSL_XMC_XPFAC1_CANCELED_SHIFT    (0x00000000u)
#define CSL_XMC_XPFAC1_CANCELED_RESETVAL (0x00000000u)

#define CSL_XMC_XPFAC1_RESETVAL          (0x00000000u)

/* XPFAC2 */


#define CSL_XMC_XPFAC2_HIT_MASK          (0x000FFFFFu)
#define CSL_XMC_XPFAC2_HIT_SHIFT         (0x00000000u)
#define CSL_XMC_XPFAC2_HIT_RESETVAL      (0x00000000u)

#define CSL_XMC_XPFAC2_RESETVAL          (0x00000000u)

/* XPFAC3 */


#define CSL_XMC_XPFAC3_MISS_MASK         (0x000FFFFFu)
#define CSL_XMC_XPFAC3_MISS_SHIFT        (0x00000000u)
#define CSL_XMC_XPFAC3_MISS_RESETVAL     (0x00000000u)

#define CSL_XMC_XPFAC3_RESETVAL          (0x00000000u)

/* XPFADDR */

#define CSL_XMC_XPFADDR_ADDR_MASK        (0xFFFFFF80u)
#define CSL_XMC_XPFADDR_ADDR_SHIFT       (0x00000007u)
#define CSL_XMC_XPFADDR_ADDR_RESETVAL    (0x00000000u)

#define CSL_XMC_XPFADDR_DIR_MASK         (0x00000040u)
#define CSL_XMC_XPFADDR_DIR_SHIFT        (0x00000006u)
#define CSL_XMC_XPFADDR_DIR_RESETVAL     (0x00000000u)

#define CSL_XMC_XPFADDR_DPH_MASK         (0x00000020u)
#define CSL_XMC_XPFADDR_DPH_SHIFT        (0x00000005u)
#define CSL_XMC_XPFADDR_DPH_RESETVAL     (0x00000000u)

#define CSL_XMC_XPFADDR_DVH_MASK         (0x00000010u)
#define CSL_XMC_XPFADDR_DVH_SHIFT        (0x00000004u)
#define CSL_XMC_XPFADDR_DVH_RESETVAL     (0x00000000u)

#define CSL_XMC_XPFADDR_AVH_MASK         (0x00000008u)
#define CSL_XMC_XPFADDR_AVH_SHIFT        (0x00000003u)
#define CSL_XMC_XPFADDR_AVH_RESETVAL     (0x00000000u)

#define CSL_XMC_XPFADDR_DPL_MASK         (0x00000004u)
#define CSL_XMC_XPFADDR_DPL_SHIFT        (0x00000002u)
#define CSL_XMC_XPFADDR_DPL_RESETVAL     (0x00000000u)

#define CSL_XMC_XPFADDR_DVL_MASK         (0x00000002u)
#define CSL_XMC_XPFADDR_DVL_SHIFT        (0x00000001u)
#define CSL_XMC_XPFADDR_DVL_RESETVAL     (0x00000000u)

#define CSL_XMC_XPFADDR_AVL_MASK         (0x00000001u)
#define CSL_XMC_XPFADDR_AVL_SHIFT        (0x00000000u)
#define CSL_XMC_XPFADDR_AVL_RESETVAL     (0x00000000u)

#define CSL_XMC_XPFADDR_RESETVAL         (0x00000000u)

#endif
