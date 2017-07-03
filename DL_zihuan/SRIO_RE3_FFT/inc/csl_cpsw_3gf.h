/**  
 * @file  csl_cpsw_3gf.h
 *
 * @brief  
 *  Header file containing various enumerations, structure definitions and function 
 *  declarations for the Ethernet switch submodule (CPSW_3GF) of EMAC.
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

#ifndef _CSL_CPSW_3GF_H
#define _CSL_CPSW_3GF_H

#ifdef __cplusplus
extern "C" {
#endif

#include "soc.h"
#include "csl.h"
#include "cslr_cpsw_3gf.h"

/** ============================================================================ 
 *
 * @defgroup CSL_CPSW_3GF_API Ethernet switch submodule (CPSW_3GF)
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
@defgroup CSL_CPSW_3GF_SYMBOL  CPSW_3GF Symbols Defined
@ingroup CSL_CPSW_3GF_API
*/
/**
@defgroup CSL_CPSW_3GF_DATASTRUCT  CPSW_3GF Data Structures
@ingroup CSL_CPSW_3GF_API
*/
/**
@defgroup CSL_CPSW_3GF_FUNCTION  CPSW_3GF Functions
@ingroup CSL_CPSW_3GF_API
*/
/**
@defgroup CSL_CPSW_3GF_ENUM CPSW_3GF Enumerated Data Types
@ingroup CSL_CPSW_3GF_API
*/

/**
@addtogroup CSL_CPSW_3GF_SYMBOL
@{
*/

/**
@}
*/

/** @addtogroup CSL_CPSW_3GF_DATASTRUCT
 @{ */

/** Constants for passing parameters to the functions.
 */
/** @brief Pointer to the Ethernet Switch overlay registers */
#define     hCpsw3gfRegs                            ((CSL_Cpsw_3gfRegs *) (CSL_PA_SS_CFG_REGS + 0x00090800))

/** @brief Number of statistic blocks. 
 *
 *  EMAC has two sub-blocks: STATSA and STATSB.
 *
 *  STATSA holds statistics for Host/CPU port (Switch port 0).
 *  STATSB holds statistics for MAC ports (Switch ports 1, 2).
 */
#define     CSL_CPSW_3GF_NUMSTATBLOCKS              2        

/** @brief Number of hardware statistics registers */
#define     CSL_CPSW_3GF_NUMSTATS                   36        

/** @brief Maximum number of ALE entries that can be programmed */
#define     CSL_CPSW_3GF_NUMALE_ENTRIES             1024           

/** @brief ALE control register configuration definitions */       

/**  Enable Broadcast/Multicast rate limit */	
#define    CSL_CPSW_3GF_ALECONTROL_RATELIMIT_EN      (1 << 0u) 

/**  MAC auhorization mode enable */	
#define    CSL_CPSW_3GF_ALECONTROL_AUTHMODE_EN       (1 << 1u) 

/**  VLAN Aware Mode enable */	
#define    CSL_CPSW_3GF_ALECONTROL_VLANAWARE_EN      (1 << 2u) 

/**  Tx rate limit enable */	
#define    CSL_CPSW_3GF_ALECONTROL_RATELIMIT_TX_EN   (1 << 3u) 

/**  OUI deny enable */	
#define    CSL_CPSW_3GF_ALECONTROL_OUIDENY_EN        (1 << 5u) 

/**  VID0 mode enable */	
#define    CSL_CPSW_3GF_ALECONTROL_VID0MODE_EN       (1 << 6u) 

/**  Learn no VID enable */	
#define    CSL_CPSW_3GF_ALECONTROL_LEARN_NO_VID_EN   (1 << 7u) 

/**  Age out now enable */	
#define    CSL_CPSW_3GF_ALECONTROL_AGEOUT_NOW_EN     (1 << 29u) 

/**  Clear table enable */	
#define    CSL_CPSW_3GF_ALECONTROL_CLRTABLE_EN       (1 << 30u) 

/**  ALE enable */	
#define    CSL_CPSW_3GF_ALECONTROL_ALE_EN            (1 << 31u) 

/** @brief Port Mask definitions */       

/**  Port 0 Enable */	
#define    CSL_CPSW_3GF_PORTMASK_PORT0_EN           (1 << 0u) 

/**  Port 1 Enable */	
#define    CSL_CPSW_3GF_PORTMASK_PORT1_EN           (1 << 1u) 

/**  Port 2 Enable */	
#define    CSL_CPSW_3GF_PORTMASK_PORT2_EN           (1 << 2u) 
        

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
} CSL_CPSW_3GF_VERSION;        

/** @brief      
 *
 *  Holds CPSW control register contents. 
 */        
typedef struct {
	/**  FIFO loopback mode */	
	Uint32      fifoLb;

	/**  Vlan aware mode */	
	Uint32      vlanAware;

	/** Port 0 Enable */	
	Uint32      p0Enable;

	/**  Port 0 Pass Priority Tagged */	
	Uint32      p0PassPriTag;

	/**  Port 1 Pass Priority Tagged */	
	Uint32      p1PassPriTag;

	/**  Port 2 Pass Priority Tagged */	
	Uint32      p2PassPriTag;
} CSL_CPSW_3GF_CONTROL;

/** @brief      
 *
 *  Holds Port Statistics Enable register contents. 
 */        
typedef struct {
	/**  Port 0 Statistics A Enable bit */	
	Uint32      p0AStatEnable;

	/**  Port 0 Statistics B Enable bit */	
	Uint32      p0BStatEnable;

	/**  Port 1 Statistics Enable bit */	
	Uint32      p1StatEnable;

	/**  Port 2 Statistics Enable bit */	
	Uint32      p2StatEnable;
} CSL_CPSW_3GF_PORTSTAT;

/** @brief      
 *
 *  Holds Priority type register contents. 
 */        
typedef struct {
	/**  Escalate priority load value */	
	Uint32      escPriLdVal;

	/**  Port 0 Priority type escalate */	
	Uint32      p0PtypeEsc;

	/**  Port 1 Priority type escalate */	
	Uint32      p1PtypeEsc;

	/**  Port 2 Priority type escalate */	
	Uint32      p2PtypeEsc;
} CSL_CPSW_3GF_PTYPE;

/** @brief      
 *
 *  Holds flow control register contents. 
 */        
typedef struct {
	/**  Port 0 flow control enable */	
	Uint32      p0FlowEnable;

	/**  Port 1 flow control enable */	
	Uint32      p1FlowEnable;

	/**  Port 2 flow control enable */	
	Uint32      p2FlowEnable;
} CSL_CPSW_3GF_FLOWCNTL;

/** @brief      
 *
 *  Holds Port Time Sync Control register contents. 
 */        
typedef struct {
	/**  Port Time sync receive enable bit */	
	Uint32      tsRxEnable;

	/**  Port Time sync receive VLAN LTYPE 1 enable bit */	
	Uint32      tsRxVlanLType1Enable;

	/**  Port Time sync receive VLAN LTYPE 2 enable bit */	
	Uint32      tsRxVlanLType2Enable;

	/**  Port Time sync transmit enable bit */	
	Uint32      tsTxEnable;

	/**  Port Time sync transmit VLAN LTYPE 1 enable bit */	
	Uint32      tsTxVlanLType1Enable;

	/**  Port Time sync transmit VLAN LTYPE 2 enable bit */	
	Uint32      tsTxVlanLType2Enable;

	/**  Port Time sync message type enable bit */	
	Uint32      txMsgTypeEnable;
} CSL_CPSW_3GF_TSCNTL;

/** @brief      
 *  
 *  Holds the EMAC statistics.
 *
 *  The statistics structure is the used to retrieve the current count
 *  of various packet events in the system. These values represent the
 *  delta values from the last time the statistics were read.
 */
typedef struct {
    /** Good Frames Received                      */        
    Uint32      RxGoodFrames;     

    /** Good Broadcast Frames Received            */
    Uint32      RxBCastFrames;    

    /** Good Multicast Frames Received            */
    Uint32      RxMCastFrames;    

    /** PauseRx Frames Received                   */
    Uint32      RxPauseFrames;    

    /** Frames Received with CRC Errors           */
    Uint32      RxCRCErrors;      

    /** Frames Received with Alignment/Code Errors*/
    Uint32      RxAlignCodeErrors;

    /** Oversized Frames Received                 */
    Uint32      RxOversized;      

    /** Jabber Frames Received                    */
    Uint32      RxJabber;         

    /** Undersized Frames Received                */
    Uint32      RxUndersized;     

    /** Rx Frame Fragments Received               */
    Uint32      RxFragments;      

    /** Reserved       */
    Uint32      reserved;         

    /** Reserved */
    Uint32      reserved2;      

    /** Total Received Bytes in Good Frames       */
    Uint32      RxOctets;         

    /** Good Frames Sent                          */
    Uint32      TxGoodFrames;     
    
    /** Good Broadcast Frames Sent                */
    Uint32      TxBCastFrames;    

    /** Good Multicast Frames Sent                */
    Uint32      TxMCastFrames;    

    /** PauseTx Frames Sent                       */
    Uint32      TxPauseFrames;    

    /** Frames Where Transmission was Deferred    */
    Uint32      TxDeferred;       

    /** Total Frames Sent With Collision          */
    Uint32      TxCollision;      

    /** Frames Sent with Exactly One Collision    */
    Uint32      TxSingleColl;     

    /** Frames Sent with Multiple Colisions       */
    Uint32      TxMultiColl;      

    /** Tx Frames Lost Due to Excessive Collisions*/
    Uint32      TxExcessiveColl;  

    /** Tx Frames Lost Due to a Late Collision    */
    Uint32      TxLateColl;       

    /** Tx Frames Lost with Tx Underrun Error     */
    Uint32      TxUnderrun;       

    /** Tx Frames Lost Due to Carrier Sense Loss  */
    Uint32      TxCarrierSLoss;   

    /** Total Transmitted Bytes in Good Frames    */
    Uint32      TxOctets;         

    /** Total Tx&Rx with Octet Size of 64         */
    Uint32      Frame64;          

    /** Total Tx&Rx with Octet Size of 65 to 127  */
    Uint32      Frame65t127;      

    /** Total Tx&Rx with Octet Size of 128 to 255 */
    Uint32      Frame128t255;     

    /** Total Tx&Rx with Octet Size of 256 to 511 */
    Uint32      Frame256t511;     

    /** Total Tx&Rx with Octet Size of 512 to 1023*/
    Uint32      Frame512t1023;    

    /** Total Tx&Rx with Octet Size of >=1024     */
    Uint32      Frame1024tUp;     

    /** Sum of all Octets Tx or Rx on the Network */
    Uint32      NetOctets;        

    /** Total Rx Start of Frame Overruns          */
    Uint32      RxSOFOverruns;    

    /** Total Rx Middle of Frame Overruns         */
    Uint32      RxMOFOverruns;    

    /** Total Rx DMA Overruns                     */
    Uint32      RxDMAOverruns;    
} CSL_CPSW_3GF_STATS;


/** @brief      
 *
 *  Holds the ALE submodule's version info. 
 */        
typedef struct {
	/**  Minor version value */	
	Uint32      minorVer;

	/**  Major version value */	
	Uint32      majorVer;

	/**  Identification value */	
	Uint32      id;
} CSL_CPSW_3GF_ALE_VERSION;

/** @brief      
 *
 *  Defines ALE port states
 */        
typedef enum {
    ALE_PORTSTATE_DISABLED = 0,
    ALE_PORTSTATE_BLOCKED,
    ALE_PORTSTATE_LEARN,
    ALE_PORTSTATE_FORWARD
} CSL_CPSW_3GF_ALE_PORTSTATE;

/** @brief      
 *
 *  Holds the ALE Port control register info. 
 */        
typedef struct {
	/**  Port state */	
	CSL_CPSW_3GF_ALE_PORTSTATE  portState;

	/**  Drop non-VLAN tagged ingress packets?  */	
	Uint32                      dropUntaggedEnable;

	/**  VLAN ID Ingress check enable */	
	Uint32                      vidIngressCheckEnable;

	/**  No learn mode enable */	
	Uint32                      noLearnModeEnable;

	/**  Multicast packet rate limit */	
	Uint32                      mcastLimit;

	/**  Broadcast packet rate limit */	
	Uint32                      bcastLimit;
} CSL_CPSW_3GF_ALE_PORTCONTROL;

/** @brief      
 *
 *  Defines ALE Table Entry types
 */        
typedef enum {
    ALE_ENTRYTYPE_FREE = 0,
    ALE_ENTRYTYPE_ADDRESS,
    ALE_ENTRYTYPE_VLAN,
    ALE_ENTRYTYPE_VLANADDRESS
} CSL_CPSW_3GF_ALE_ENTRYTYPE;

/** @brief      
 *
 *  Defines ALE Unicast types
 */        
typedef enum {
    ALE_UCASTTYPE_UCAST_NOAGE = 0,
    ALE_UCASTTYPE_UCAST_AGENOTOUCH,
    ALE_UCASTTYPE_UCAST_OUI,
    ALE_UCASTTYPE_UCAST_AGETOUCH
} CSL_CPSW_3GF_ALE_UCASTTYPE;

/** @brief      
 *
 *  Defines ALE Address types
 */        
typedef enum {
    ALE_ADDRTYPE_UCAST = 0,
    ALE_ADDRTYPE_MCAST,
    ALE_ADDRTYPE_OUI
} CSL_CPSW_3GF_ALE_ADDRTYPE;

/** @brief      
 *
 *  Holds the ALE Multicast Address Table entry
 *  configuration.
 */        
typedef struct {
	/**  Multicast address */	
	Uint8                       macAddress [6];

	/**  Multicast forward state  */	
	Uint32                      mcastFwdState;

	/**  Supervisory bit enable? */	
	Uint32                      superEnable;

	/**  Port Mask. */	
	Uint32                      portMask;
} CSL_CPSW_3GF_ALE_MCASTADDR_ENTRY;

/** @brief      
 *
 *  Holds the ALE VLAN/Multicast Address Table entry
 *  configuration.
 */        
typedef struct {
	/**  Multicast address */	
	Uint8                       macAddress [6];

	/**  VLAN Id  */	
	Uint32                      vlanId;

	/**  Multicast forward state  */	
	Uint32                      mcastFwdState;

	/**  Supervisory bit enable? */	
	Uint32                      superEnable;

	/**  Port Mask. */	
	Uint32                      portMask;
} CSL_CPSW_3GF_ALE_VLANMCASTADDR_ENTRY;

/** @brief      
 *
 *  Holds the ALE Unicast Address Table entry
 *  configuration.
 */        
typedef struct {
	/**  Unicast address */	
	Uint8                       macAddress [6];

	/**  Unicast type  */	
	CSL_CPSW_3GF_ALE_UCASTTYPE  ucastType;

	/**  Secure bit enable?  */	
	Uint32                      secureEnable;

	/**  Block bit enable? */	
	Uint32                      blockEnable;

	/**  Port Number to forward matching packets to. */	
	Uint32                      portNumber;
} CSL_CPSW_3GF_ALE_UNICASTADDR_ENTRY;

/** @brief      
 *
 *  Holds the ALE OUI Unicast Address Table entry
 *  configuration.
 */        
typedef struct {
	/**  OUI Unicast address */	
	Uint8                       ouiAddress [3];

	/**  Unicast type  */	
	CSL_CPSW_3GF_ALE_UCASTTYPE  ucastType;
} CSL_CPSW_3GF_ALE_OUIADDR_ENTRY;

/** @brief      
 *
 *  Holds the ALE VLAN Unicast Address Table entry
 *  configuration.
 */        
typedef struct {
	/**  Unicast address */	
	Uint8                       macAddress [6];

	/**  VLAN Id  */	
	Uint32                      vlanId;

	/**  Unicast type  */	
	CSL_CPSW_3GF_ALE_UCASTTYPE  ucastType;

	/**  Secure bit enable?  */	
	Uint32                      secureEnable;

	/**  Block bit enable? */	
	Uint32                      blockEnable;

	/**  Port Number to forward matching packets to. */	
	Uint32                      portNumber;
} CSL_CPSW_3GF_ALE_VLANUNICASTADDR_ENTRY;

/** @brief      
 *
 *  Holds the ALE VLAN Table entry configuration.
 */        
typedef struct {
	/**  VLAN Id  */	
	Uint32                      vlanId;

	/**  VLAN member list */	
	Uint32                      vlanMemList;

	/**  Unregistered Multicast Flood mask  */	
	Uint32                      unRegMcastFloodMask;

	/**  Registered Multicast Flood mask  */	
	Uint32                      regMcastFloodMask;

	/**  Force Untagged Packet Egress. */	
	Uint32                      forceUntaggedEgress;
} CSL_CPSW_3GF_ALE_VLAN_ENTRY;

/**
@}
*/


/** @addtogroup CSL_CPSW_3GF_FUNCTION
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
