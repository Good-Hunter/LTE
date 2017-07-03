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
#ifndef _CSLR_CPSW_3GFSS_S_H_
#define _CSLR_CPSW_3GFSS_S_H_

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
} CSL_Cpsw_3gfss_sRegs;

/**************************************************************************\
* Field Definition Macros
\**************************************************************************/

/* idver_reg */

#define CSL_CPSW_3GFSS_S_IDVER_REG_MINOR_VER_MASK (0x000000FFu)
#define CSL_CPSW_3GFSS_S_IDVER_REG_MINOR_VER_SHIFT (0x00000000u)
#define CSL_CPSW_3GFSS_S_IDVER_REG_MINOR_VER_RESETVAL (0x00000007u)

#define CSL_CPSW_3GFSS_S_IDVER_REG_MAJOR_VER_MASK (0x00000700u)
#define CSL_CPSW_3GFSS_S_IDVER_REG_MAJOR_VER_SHIFT (0x00000008u)
#define CSL_CPSW_3GFSS_S_IDVER_REG_MAJOR_VER_RESETVAL (0x00000000u)

#define CSL_CPSW_3GFSS_S_IDVER_REG_RTL_VER_MASK (0x0000F800u)
#define CSL_CPSW_3GFSS_S_IDVER_REG_RTL_VER_SHIFT (0x0000000Bu)
#define CSL_CPSW_3GFSS_S_IDVER_REG_RTL_VER_RESETVAL (0x00000000u)

#define CSL_CPSW_3GFSS_S_IDVER_REG_IDENT_MASK (0xFFFF0000u)
#define CSL_CPSW_3GFSS_S_IDVER_REG_IDENT_SHIFT (0x00000010u)
#define CSL_CPSW_3GFSS_S_IDVER_REG_IDENT_RESETVAL (0x00004ED9u)

#define CSL_CPSW_3GFSS_S_IDVER_REG_RESETVAL (0x4ED90007u)

#endif
