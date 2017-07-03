#ifndef _PAFW_H
#define _PAFW_H

/******************************************************************************
 * FILE PURPOSE: PASS Firmware Image Definitions for the PA LLD 
 ******************************************************************************
 * FILE NAME:   pafw.h
 *
 * DESCRIPTION: Define PASS firmware image related constants and data structures.  
 *  The PASS contains six PDSPs wihich perform the command and packet processing. 
 *  There are three PDSP images provided by the module under the ti/drv/pa/fw directory:
 *  -Packet Classifier 1 image: classify1_bin.c for PDSP0, PDSP1 and PDSP2
 *  -Packet Classifier 2 image: classify2_bin.c for PDSP3
 *  -Packet Modifier image: pam_bin.c for PDSP4 and PDSP5
 *
 * REVISION HISTORY:
 *
 *  Copyright (c) Texas Instruments Incorporated 2009-2011
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
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
/* PDSP Packet Classifier 1 image */
extern const uint32_t c1[];
extern const int c1Size;
/* PDSP Packet Classifier 2 image */
extern const uint32_t c2[];
extern const int c2Size;
/* PDSP Packet Modifier image */
extern const uint32_t m[];
extern const int mSize;

#ifdef __cplusplus
}
#endif
  

#endif  /* _PAFW_H */
