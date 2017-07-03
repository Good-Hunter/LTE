/**  
 * @file  csl_cpgmac_sl.h
 *
 * @brief  
 *  Header file containing various enumerations, structure definitions and function 
 *  declarations for the CP GMAC Sliver submodule (CPGMAC_SL) of EMAC.
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

#ifndef _CSL_CPGMAC_SL_H
#define _CSL_CPGMAC_SL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "soc.h"
#include "csl.h"
#include "cslr_cpgmac_sl.h"

/** ============================================================================ 
 *
 * @defgroup CSL_CPGMAC_SL_API GMAC Sliver submodule (CPGMAC_SL)
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
@defgroup CSL_CPGMAC_SL_SYMBOL  CPGMAC_SL Symbols Defined
@ingroup CSL_CPGMAC_SL_API
*/
/**
@defgroup CSL_CPGMAC_SL_DATASTRUCT  CPGMAC_SL Data Structures
@ingroup CSL_CPGMAC_SL_API
*/
/**
@defgroup CSL_CPGMAC_SL_FUNCTION  CPGMAC_SL Functions
@ingroup CSL_CPGMAC_SL_API
*/
/**
@defgroup CSL_CPGMAC_SL_ENUM CPGMAC_SL Enumerated Data Types
@ingroup CSL_CPGMAC_SL_API
*/

/**
@addtogroup CSL_CPGMAC_SL_SYMBOL
@{
*/

/**
@}
*/

/** @addtogroup CSL_CPGMAC_SL_DATASTRUCT
 @{ */

/** Constants for passing parameters to the functions.
 */
/** @brief Pointer to the EMAC overlay registers */
#define     hCpgmacSlRegs                               ((CSL_Cpgmac_slPortRegs *) (CSL_PA_SS_CFG_REGS + 0x00090900))

/** @brief MAC control register configuration definitions */       

/** Enable full duplex mode */
#define    CSL_CPGMAC_SL_MACCONTROL_FULLDUPLEX_EN       (1 << 0u) 

/** Enable loopback mode */
#define    CSL_CPGMAC_SL_MACCONTROL_LOOPBACK_EN         (1 << 1u) 

/** Enable Rx flow control mode */
#define    CSL_CPGMAC_SL_MACCONTROL_RX_FLOW_EN          (1 << 3u) 

/** Enable Tx flow control mode */
#define    CSL_CPGMAC_SL_MACCONTROL_TX_FLOW_EN          (1 << 4u) 

/** Enable GMII */
#define    CSL_CPGMAC_SL_MACCONTROL_GMII_EN             (1 << 5u) 

/** Enable Tx pacing */
#define    CSL_CPGMAC_SL_MACCONTROL_TX_PACE_EN          (1 << 6u) 

/** Enable Gigabit mode */
#define    CSL_CPGMAC_SL_MACCONTROL_GIG_EN              (1 << 7u) 

/** Enable Tx short gap */
#define    CSL_CPGMAC_SL_MACCONTROL_TX_SHORT_GAP_EN     (1 << 10u) 

/** Enable idle mode */
#define    CSL_CPGMAC_SL_MACCONTROL_CMD_IDLE_EN         (1 << 11u) 

/** Set IFCTL_A bit to 1 */
#define    CSL_CPGMAC_SL_MACCONTROL_IFCTL_A_EN          (1 << 15u) 

/** Set IFCTL_B bit to 1 */
#define    CSL_CPGMAC_SL_MACCONTROL_IFCTL_B_EN          (1 << 16u) 

/** Enable forced Gigabit mode */
#define    CSL_CPGMAC_SL_MACCONTROL_GIG_FORCE_EN        (1 << 17u) 

/** Enable external control mode */
#define    CSL_CPGMAC_SL_MACCONTROL_EXT_EN              (1 << 18u) 

/** Enable Rx copy error frames mode */
#define    CSL_CPGMAC_SL_MACCONTROL_RX_CEF_EN           (1 << 22u) 

/** Enable Rx copy short frames mode */
#define    CSL_CPGMAC_SL_MACCONTROL_RX_CSF_EN           (1 << 23u) 

/** Enable Rx copy MAC control frames mode */
#define    CSL_CPGMAC_SL_MACCONTROL_RX_CMF_EN           (1 << 24u) 

/** @brief      
 *
 *  Holds the Sliver submodule's version info. 
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
} CSL_CPGMAC_SL_VERSION;        

/** @brief      
 *
 *  Holds MAC status register contents. 
 */        
typedef struct {
	/** Transmit flow control active? */	
	Uint32      txFlowActive;

	/** Receive flow control active? */	
	Uint32      rxFlowActive;

	/**  External full duplex input set? */	
	Uint32      extFullDuplexEnabled;

	/**  External gigabit input set? */	
	Uint32      extGigabitEnabled;

	/**  Is MAC idle? */	
	Uint32      idleEnabled;
} CSL_CPGMAC_SL_MACSTATUS;


/**
@}
*/


/** @addtogroup CSL_CPGMAC_SL_FUNCTION
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

