/********************************************************************
* Copyright (C) 2003-2008 Texas Instruments Incorporated.
 * 
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions 
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the   
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
*/
#ifndef _CSLR_CPTS_H_
#define _CSLR_CPTS_H_

/* Modified by CSL team for:
 *      (1) Header files are included to be RTSC compliant
 */
#include "cslr.h"
#include "tistdtypes.h"


/* Minimum unit = 1 byte */

/**************************************************************************\
* Register Overlay Structure
\**************************************************************************/
typedef struct  {
    volatile Uint32 IDVER_REG;
    volatile Uint32 CONTROL_REG;
    volatile Uint32 RFTCLK_SEL_REG;
    volatile Uint32 TS_PUSH_REG;
    volatile Uint32 TS_LOAD_VAL_REG;
    volatile Uint32 TS_LOAD_EN_REG;
    volatile Uint8 RSVD0[8];
    volatile Uint32 INTSTAT_RAW_REG;
    volatile Uint32 INTSTAT_MASKED_REG;
    volatile Uint32 INT_ENABLE_REG;
    volatile Uint8 RSVD1[4];
    volatile Uint32 EVENT_POP_REG;
    volatile Uint32 EVENT_LOW_REG;
    volatile Uint32 EVENT_HIGH_REG;
} CSL_CptsRegs;

/**************************************************************************\
* Field Definition Macros
\**************************************************************************/

/* idver_reg */

#define CSL_CPTS_IDVER_REG_MINOR_VER_MASK (0x000000FFu)
#define CSL_CPTS_IDVER_REG_MINOR_VER_SHIFT (0x00000000u)
#define CSL_CPTS_IDVER_REG_MINOR_VER_RESETVAL (0x00000002u)

#define CSL_CPTS_IDVER_REG_MAJOR_VER_MASK (0x00000700u)
#define CSL_CPTS_IDVER_REG_MAJOR_VER_SHIFT (0x00000008u)
#define CSL_CPTS_IDVER_REG_MAJOR_VER_RESETVAL (0x00000000u)

#define CSL_CPTS_IDVER_REG_RTL_VER_MASK  (0x0000F800u)
#define CSL_CPTS_IDVER_REG_RTL_VER_SHIFT (0x0000000Bu)
#define CSL_CPTS_IDVER_REG_RTL_VER_RESETVAL (0x00000000u)

#define CSL_CPTS_IDVER_REG_TX_IDENT_MASK (0xFFFF0000u)
#define CSL_CPTS_IDVER_REG_TX_IDENT_SHIFT (0x00000010u)
#define CSL_CPTS_IDVER_REG_TX_IDENT_RESETVAL (0x00004E8Au)

#define CSL_CPTS_IDVER_REG_RESETVAL      (0x4E8A0002u)

/* control_reg */

#define CSL_CPTS_CONTROL_REG_CPTS_EN_MASK (0x00000001u)
#define CSL_CPTS_CONTROL_REG_CPTS_EN_SHIFT (0x00000000u)
#define CSL_CPTS_CONTROL_REG_CPTS_EN_RESETVAL (0x00000000u)

#define CSL_CPTS_CONTROL_REG_INT_TEST_MASK (0x00000002u)
#define CSL_CPTS_CONTROL_REG_INT_TEST_SHIFT (0x00000001u)
#define CSL_CPTS_CONTROL_REG_INT_TEST_RESETVAL (0x00000000u)

#define CSL_CPTS_CONTROL_REG_RESETVAL    (0x00000000u)

/* rftclk_sel_reg */

#define CSL_CPTS_RFTCLK_SEL_REG_RFTCLK_SEL_MASK (0x0000001Fu)
#define CSL_CPTS_RFTCLK_SEL_REG_RFTCLK_SEL_SHIFT (0x00000000u)
#define CSL_CPTS_RFTCLK_SEL_REG_RFTCLK_SEL_RESETVAL (0x00000000u)

#define CSL_CPTS_RFTCLK_SEL_REG_RESETVAL (0x00000000u)

/* ts_push_reg */

#define CSL_CPTS_TS_PUSH_REG_TS_PUSH_MASK (0x00000001u)
#define CSL_CPTS_TS_PUSH_REG_TS_PUSH_SHIFT (0x00000000u)
#define CSL_CPTS_TS_PUSH_REG_TS_PUSH_RESETVAL (0x00000000u)

#define CSL_CPTS_TS_PUSH_REG_RESETVAL    (0x00000000u)

/* ts_load_val_reg */

#define CSL_CPTS_TS_LOAD_VAL_REG_TS_LOAD_VAL_MASK (0xFFFFFFFFu)
#define CSL_CPTS_TS_LOAD_VAL_REG_TS_LOAD_VAL_SHIFT (0x00000000u)
#define CSL_CPTS_TS_LOAD_VAL_REG_TS_LOAD_VAL_RESETVAL (0x00000000u)

#define CSL_CPTS_TS_LOAD_VAL_REG_RESETVAL (0x00000000u)

/* ts_load_en_reg */

#define CSL_CPTS_TS_LOAD_EN_REG_TS_LOAD_EN_MASK (0x00000001u)
#define CSL_CPTS_TS_LOAD_EN_REG_TS_LOAD_EN_SHIFT (0x00000000u)
#define CSL_CPTS_TS_LOAD_EN_REG_TS_LOAD_EN_RESETVAL (0x00000000u)

#define CSL_CPTS_TS_LOAD_EN_REG_RESETVAL (0x00000000u)

/* intstat_raw_reg */

#define CSL_CPTS_INTSTAT_RAW_REG_TS_PEND_RAW_MASK (0x00000001u)
#define CSL_CPTS_INTSTAT_RAW_REG_TS_PEND_RAW_SHIFT (0x00000000u)
#define CSL_CPTS_INTSTAT_RAW_REG_TS_PEND_RAW_RESETVAL (0x00000000u)

#define CSL_CPTS_INTSTAT_RAW_REG_RESETVAL (0x00000000u)

/* intstat_masked_reg */

#define CSL_CPTS_INTSTAT_MASKED_REG_TS_PEND_MASK (0x00000001u)
#define CSL_CPTS_INTSTAT_MASKED_REG_TS_PEND_SHIFT (0x00000000u)
#define CSL_CPTS_INTSTAT_MASKED_REG_TS_PEND_RESETVAL (0x00000000u)

#define CSL_CPTS_INTSTAT_MASKED_REG_RESETVAL (0x00000000u)

/* int_enable_reg */

#define CSL_CPTS_INT_ENABLE_REG_TS_PEND_EN_MASK (0x00000001u)
#define CSL_CPTS_INT_ENABLE_REG_TS_PEND_EN_SHIFT (0x00000000u)
#define CSL_CPTS_INT_ENABLE_REG_TS_PEND_EN_RESETVAL (0x00000000u)

#define CSL_CPTS_INT_ENABLE_REG_RESETVAL (0x00000000u)

/* event_pop_reg */

#define CSL_CPTS_EVENT_POP_REG_EVENT_POP_MASK (0x00000001u)
#define CSL_CPTS_EVENT_POP_REG_EVENT_POP_SHIFT (0x00000000u)
#define CSL_CPTS_EVENT_POP_REG_EVENT_POP_RESETVAL (0x00000000u)

#define CSL_CPTS_EVENT_POP_REG_RESETVAL  (0x00000000u)

/* event_low_reg */

#define CSL_CPTS_EVENT_LOW_REG_TIME_STAMP_MASK (0xFFFFFFFFu)
#define CSL_CPTS_EVENT_LOW_REG_TIME_STAMP_SHIFT (0x00000000u)
#define CSL_CPTS_EVENT_LOW_REG_TIME_STAMP_RESETVAL (0x00000000u)

#define CSL_CPTS_EVENT_LOW_REG_RESETVAL  (0x00000000u)

/* event_high_reg */

#define CSL_CPTS_EVENT_HIGH_REG_SEQUENCE_ID_MASK (0x0000FFFFu)
#define CSL_CPTS_EVENT_HIGH_REG_SEQUENCE_ID_SHIFT (0x00000000u)
#define CSL_CPTS_EVENT_HIGH_REG_SEQUENCE_ID_RESETVAL (0x00000000u)

#define CSL_CPTS_EVENT_HIGH_REG_MESSAGE_TYPE_MASK (0x000F0000u)
#define CSL_CPTS_EVENT_HIGH_REG_MESSAGE_TYPE_SHIFT (0x00000010u)
#define CSL_CPTS_EVENT_HIGH_REG_MESSAGE_TYPE_RESETVAL (0x00000000u)

#define CSL_CPTS_EVENT_HIGH_REG_EVENT_TYPE_MASK (0x00F00000u)
#define CSL_CPTS_EVENT_HIGH_REG_EVENT_TYPE_SHIFT (0x00000014u)
#define CSL_CPTS_EVENT_HIGH_REG_EVENT_TYPE_RESETVAL (0x00000000u)

#define CSL_CPTS_EVENT_HIGH_REG_PORT_NUMBER_MASK (0x1F000000u)
#define CSL_CPTS_EVENT_HIGH_REG_PORT_NUMBER_SHIFT (0x00000018u)
#define CSL_CPTS_EVENT_HIGH_REG_PORT_NUMBER_RESETVAL (0x00000000u)

#define CSL_CPTS_EVENT_HIGH_REG_RESETVAL (0x00000000u)

#endif
