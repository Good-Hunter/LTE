/**  
 * @file  csl_cpts.h
 *
 * @brief  
 *  Header file containing various enumerations, structure definitions and function 
 *  declarations for the Time synchronization submodule of EMAC.
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

#ifndef _CSL_CPTS_H
#define _CSL_CPTS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "soc.h"
#include "csl.h"
#include "cslr_cpts.h"

/** ============================================================================ 
 *
 * @defgroup CSL_CPTS_API Time Sync Submodule (CPTS)
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
@defgroup CSL_CPTS_SYMBOL  CPTS Symbols Defined
@ingroup CSL_CPTS_API
*/
/**
@defgroup CSL_CPTS_DATASTRUCT  CPTS Data Structures
@ingroup CSL_CPTS_API
*/
/**
@defgroup CSL_CPTS_FUNCTION  CPTS Functions
@ingroup CSL_CPTS_API
*/
/**
@defgroup CSL_CPTS_ENUM CPTS Enumerated Data Types
@ingroup CSL_CPTS_API
*/

/**
@addtogroup CSL_CPTS_SYMBOL
@{
*/

/**
@}
*/

/** @addtogroup CSL_CPTS_DATASTRUCT
 @{ */

/** Constants for passing parameters to the functions.
 */
/** @brief Pointer to the Time synchronization submodule (CPTS) overlay registers */
#define     hCptsRegs                       ((CSL_CptsRegs *) (CSL_PA_SS_CFG_REGS + 0x00090D00))

/** @brief      
 *
 *  Defines CPTS event types. 
 */        
/**  Time stamp push event */	
#define     CSL_CPTS_EVENTTYPE_TS_PUSH          0

/**  Time stamp rollover event */	
#define     CSL_CPTS_EVENTTYPE_TS_ROLLOVER      1

/**  Time stamp Half Rollover event */	
#define     CSL_CPTS_EVENTTYPE_TS_HALFROLLOVER  2

/**  Ethernet receive event */	
#define     CSL_CPTS_EVENTTYPE_ETH_RECEIVE      4

/**  Ethernet Transmit event */	
#define	    CSL_CPTS_EVENTTYPE_ETH_TRANSMIT     5
        

/** @brief      
 *
 *  Holds the Time sync submodule's version info. 
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
} CSL_CPTS_VERSION;        

/** @brief      
 *
 *  Holds Time sync event info contents. 
 */        
typedef struct {
	/**  Time stamp read from the CPTS EVENT_LOW register */	
	Uint32      timeStamp;

	/**  Sequence Id read from CPTS EVENT_HIGH register */	
	Uint32      seqId;

	/**  Message Type read from CPTS EVENT_HIGH register */	
	Uint32      msgType;

	/**  Event Type read from CPTS EVENT_HIGH register */	
	Uint32      eventType;

	/**  Port number read from CPTS EVENT_HIGH register */	
	Uint32      portNo;
} CSL_CPTS_EVENTINFO;

        
/**
@}
*/


/** @addtogroup CSL_CPTS_FUNCTION
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

