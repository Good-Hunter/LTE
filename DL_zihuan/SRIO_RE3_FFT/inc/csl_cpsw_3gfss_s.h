/**  
 * @file  csl_cpsw_3gfss_s.h
 *
 * @brief  
 *  Header file containing various enumerations, structure definitions and function 
 *  declarations for the Ethernet switch subsystem module of EMAC.
 *  \par
 *  ============================================================================
 *  @n   (C) Copyright 2009, Texas Instruments, Inc.
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

#ifndef _CSL_CPSW_3GFSS_S_H
#define _CSL_CPSW_3GFSS_S_H

#ifdef __cplusplus
extern "C" {
#endif

#include "soc.h"
#include "csl.h"
#include "cslr_cpsw_3gfss_s.h"

/** ============================================================================ 
 *
 * @defgroup CSL_CPSW_3GFSS_S_API Ethernet Switch Subsystem Module (CPSW_3GFSS_S)
 * @ingroup CSL_CPSW_API
 *
 * @section Introduction
 *
 * @subsection xxx Overview
 *
 * @subsection References
 *    -# Ethernet Media Access Controller User Guide
 * ============================================================================
 */         
/**
@defgroup CSL_CPSW_3GFSS_S_SYMBOL  CPSW_3GFSS_S Symbols Defined
@ingroup CSL_CPSW_3GFSS_S_API
*/
/**
@defgroup CSL_CPSW_3GFSS_S_DATASTRUCT  CPSW_3GFSS_S Data Structures
@ingroup CSL_CPSW_3GFSS_S_API
*/
/**
@defgroup CSL_CPSW_3GFSS_S_FUNCTION  CPSW_3GFSS_S Functions
@ingroup CSL_CPSW_3GFSS_S_API
*/
/**
@defgroup CSL_CPSW_3GFSS_S_ENUM CPSW_3GFSS_S Enumerated Data Types
@ingroup CSL_CPSW_3GFSS_S_API
*/

/**
@addtogroup CSL_CPSW_3GFSS_S_SYMBOL
@{
*/

/**
@}
*/

/** @addtogroup CSL_CPSW_3GFSS_S_DATASTRUCT
 @{ */

/** Constants for passing parameters to the functions.
 */
/** @brief Pointer to the Ethernet Switch Subsystem overlay registers */
#define     hCpsw3gfssRegs                  ((CSL_Cpsw_3gfss_sRegs *) (CSL_PA_SS_CFG_REGS + 0x00090000))

/** @brief      
 *
 *  Holds the Ethernet switch subsystem's version info. 
 */        
typedef struct {
	/**  Minor version value */	
	Uint32      minorVer;

	/**  Major version value */	
	Uint32      majorVer;

	/**  RTL version value */	
	Uint32      rtlVer;

	/**  Identification value */	
	Uint32      id;
} CSL_CPSW_3GFSS_VERSION;        
        
/**
@}
*/


/** @addtogroup CSL_CPSW_3GFSS_S_FUNCTION
 @{ */

/**
@}
*/

#ifdef __cplusplus
}
#endif

#endif

/**
@}
*/
