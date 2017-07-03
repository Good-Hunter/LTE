
#ifndef _CSL_EDMA3_H_
#define _CSL_EDMA3_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "csl.h"
#include "soc.h"
#include "cslr_tpcc.h"

typedef CSL_TpccRegs* CSL_Edma3ccRegsOvly;



/**
@addtogroup CSL_EDMA3_SYMBOL
@{
*/
/* EDMA Symbols Defined */

/** EDMA Regions: Global and 8 SHADOW Regions. */
#define CSL_EDMA3_REGION_GLOBAL             -1
#define CSL_EDMA3_REGION_0                  0
#define CSL_EDMA3_REGION_1                  1
#define CSL_EDMA3_REGION_2                  2
#define CSL_EDMA3_REGION_3                  3
#define CSL_EDMA3_REGION_4                  4
#define CSL_EDMA3_REGION_5                  5
#define CSL_EDMA3_REGION_6                  6
#define CSL_EDMA3_REGION_7                  7

/** Link to a Null Param set */
#define CSL_EDMA3_LINK_NULL                     0xFFFF 
/** Link to a Null Param set */
#define CSL_EDMA3_LINK_DEFAULT                  0xFFFF
/** A synchronized transfer  */
#define CSL_EDMA3_SYNC_A                        0
/** AB synchronized transfer */
#define CSL_EDMA3_SYNC_AB                       1
/** Normal Completion */
#define CSL_EDMA3_TCC_NORMAL                    0
/** Early  Completion */
#define CSL_EDMA3_TCC_EARLY                     1
/** Only for ease  */
#define CSL_EDMA3_FIFOWIDTH_NONE                0
/** 8 bit FIFO Width */
#define CSL_EDMA3_FIFOWIDTH_8BIT                0    
/** 16 bit FIFO Width */
#define CSL_EDMA3_FIFOWIDTH_16BIT               1    
/** 32 bit FIFO Width */
#define CSL_EDMA3_FIFOWIDTH_32BIT               2    
/** 64 bit FIFO Width */
#define CSL_EDMA3_FIFOWIDTH_64BIT               3    
/** 128 bit FIFO Width */
#define CSL_EDMA3_FIFOWIDTH_128BIT              4   
/** 256 bit FIFO Width */
#define CSL_EDMA3_FIFOWIDTH_256BIT              5    
/** Address Mode is incremental */
#define CSL_EDMA3_ADDRMODE_INCR                 0
/** Address Mode is such it wraps around after reaching FIFO width */
#define CSL_EDMA3_ADDRMODE_CONST                1



/* Bitwise OR of the below symbols are used for setting the Memory attributes 
   These are defined only if the Memory Protection feature exists */

/** User Execute permission */
#define CSL_EDMA3_MEMACCESS_UX              0x0001  
/** User Write permission */  
#define CSL_EDMA3_MEMACCESS_UW              0x0002  
/** User Read permission */
#define CSL_EDMA3_MEMACCESS_UR              0x0004  
/** Supervisor Execute permission */  
#define CSL_EDMA3_MEMACCESS_SX              0x0008  
/** Supervisor Write permission */
#define CSL_EDMA3_MEMACCESS_SW              0x0010  
/** Supervisor Read permission */
#define CSL_EDMA3_MEMACCESS_SR              0x0020  
/** External Allowed ID. Requests with PrivID >= '6' are permitted 
 * if access type is allowed
 */
#define CSL_EDMA3_MEMACCESS_EXT            0x0200 
/** Allowed ID '0' */ 
#define CSL_EDMA3_MEMACCESS_AID0           0x0400  
/** Allowed ID '1' */
#define CSL_EDMA3_MEMACCESS_AID1           0x0800  
/** Allowed ID '2' */
#define CSL_EDMA3_MEMACCESS_AID2           0x1000  
/** Allowed ID '3' */
#define CSL_EDMA3_MEMACCESS_AID3           0x2000  
/** Allowed ID '4' */
#define CSL_EDMA3_MEMACCESS_AID4           0x4000  
/** Allowed ID '5' */
#define CSL_EDMA3_MEMACCESS_AID5           0x8000  

/** Intermediate transfer completion interrupt enable */  
#define CSL_EDMA3_ITCINT_EN             1
/** Intermediate transfer completion interrupt disable */ 
#define CSL_EDMA3_ITCINT_DIS            0
/** Intermediate transfer completion chaining enable */ 
#define CSL_EDMA3_ITCCH_EN              1
/** Intermediate transfer completion chaining disable */    
#define CSL_EDMA3_ITCCH_DIS             0
/** Transfer completion interrupt enable */  
#define CSL_EDMA3_TCINT_EN              1
/** Transfer completion interrupt disable */ 
#define CSL_EDMA3_TCINT_DIS             0
/** Transfer completion chaining enable */   
#define CSL_EDMA3_TCCH_EN               1
/** Transfer completion chaining disable */
#define CSL_EDMA3_TCCH_DIS              0
/** Enable Static */
#define CSL_EDMA3_STATIC_EN             1
/** Disable Static */
#define CSL_EDMA3_STATIC_DIS            0
/** Last trigger word in a QDMA parameter set */
#define CSL_EDMA3_TRIGWORD_DEFAULT        7
/** Trigger word option field */
#define CSL_EDMA3_TRIGWORD_OPT            0
/** Trigger word source  */
#define CSL_EDMA3_TRIGWORD_SRC            1
/** Trigger word AB count */
#define CSL_EDMA3_TRIGWORD_A_B_CNT        2
/** Trigger word destination */
#define CSL_EDMA3_TRIGWORD_DST            3
/** Trigger word src and dst B index */
#define CSL_EDMA3_TRIGWORD_SRC_DST_BIDX   4
/** Trigger word B count reload */
#define CSL_EDMA3_TRIGWORD_LINK_BCNTRLD   5
/** Trigger word src and dst C index */
#define CSL_EDMA3_TRIGWORD_SRC_DST_CIDX   6
/** Trigger word C count */
#define CSL_EDMA3_TRIGWORD_CCNT           7


/** Used for creating the options entry in the parameter ram */
#define CSL_EDMA3_OPT_MAKE(itcchEn, tcchEn, itcintEn, tcintEn, tcc, tccMode, \
                           fwid, stat, syncDim, dam, sam) \
(unsigned signed int)(\
     CSL_FMKR(23,23,itcchEn) \
    |CSL_FMKR(22,22,tcchEn) \
    |CSL_FMKR(21,21,itcintEn) \
    |CSL_FMKR(20,20,tcintEn) \
    |CSL_FMKR(17,12,tcc) \
    |CSL_FMKR(11,11,tccMode) \
    |CSL_FMKR(10,8,fwid) \
    |CSL_FMKR(3,3,stat) \
    |CSL_FMKR(2,2,syncDim) \
    |CSL_FMKR(1,1,dam) \
    |CSL_FMKR(0,0,sam)) 

/** Used for creating the A,B Count entry in the parameter ram */
#define CSL_EDMA3_CNT_MAKE(aCnt,bCnt) \
(unsigned signed int)(\
     CSL_FMKR(15, 0,  aCnt) \
    |CSL_FMKR(31, 16, bCnt) \
    )

/** Used for creating the link and B count reload entry in the parameter ram */
#define CSL_EDMA3_LINKBCNTRLD_MAKE(link,bCntRld) \
(unsigned signed int)(\
     CSL_FMKR(15, 0 , (unsigned signed int)link) \
    |CSL_FMKR(31, 16, bCntRld)\
    )

/** Used for creating the B index entry in the parameter ram */
#define CSL_EDMA3_BIDX_MAKE(src,dst) \
(unsigned signed int)(\
     CSL_FMKR(31, 16, (unsigned signed int)dst) \
    |CSL_FMKR(15, 0 , (unsigned signed int)src)\
    )

/** Used for creating the C index entry in the parameter ram */
#define CSL_EDMA3_CIDX_MAKE(src,dst) \
(unsigned signed int)(\
     CSL_FMKR(31, 16, (unsigned signed int)dst) \
    |CSL_FMKR(15, 0,  (unsigned signed int)src)\
    )

/**
@}
*/

/** @addtogroup CSL_EDMA3_ENUM
 @{ */

/** @brief Enumeration for System priorities
 *
 * This is used for Setting up the Que Priority level
 */
typedef enum {
    /** System priority level 0 */
    CSL_EDMA3_QUE_PRI_0 = 0,
    /** System priority level 1 */
    CSL_EDMA3_QUE_PRI_1 = 1,
    /** System priority level 2 */         
    CSL_EDMA3_QUE_PRI_2 = 2,
    /** System priority level 3 */
    CSL_EDMA3_QUE_PRI_3 = 3,
    /** System priority level 4 */     
    CSL_EDMA3_QUE_PRI_4 = 4,   
    /** System priority level 5 */
    CSL_EDMA3_QUE_PRI_5 = 5,   
    /** System priority level 6 */
    CSL_EDMA3_QUE_PRI_6 = 6,   
    /** System priority level 7 */
    CSL_EDMA3_QUE_PRI_7 = 7    
}CSL_Edma3QuePri;

/** @brief Enumeration for EDMA Event Queues
 *
 * These are a list of all the event queues.
 */

/* Enumerations for EDMA Event Queues */
typedef enum {
    /** Default Event Queue */
    CSL_EDMA3_QUE_DEFAULT = 0,
    /** Event Queue 0 */
    CSL_EDMA3_QUE_0       = 0,
    /** Event Queue 1 */
    CSL_EDMA3_QUE_1       = 1,
    /** Event Queue 2 */
    CSL_EDMA3_QUE_2       = 2,
    /** Event Queue 3 */
    CSL_EDMA3_QUE_3       = 3,
    /** Event Queue 4 */
    CSL_EDMA3_QUE_4       = 4,
    /** Event Queue 5 */
    CSL_EDMA3_QUE_5       = 5,
    /** Event Queue 6 */
    CSL_EDMA3_QUE_6       = 6,
    /** Event Queue 7 */
    CSL_EDMA3_QUE_7       = 7
} CSL_Edma3Que;

/** @brief Enumeration for EDMA Que Thresholds
 *
 * This is used for Setting up the Que thresholds
 */
typedef enum {
    /** EDMA Que Threshold 0 */
    CSL_EDMA3_QUE_THR_0 = 0,
    /** EDMA Que Threshold 1 */        
    CSL_EDMA3_QUE_THR_1 = 1,
    /** EDMA Que Threshold 2 */        
    CSL_EDMA3_QUE_THR_2 = 2,
    /** EDMA Que Threshold 3 */        
    CSL_EDMA3_QUE_THR_3 = 3, 
    /** EDMA Que Threshold 4 */       
    CSL_EDMA3_QUE_THR_4 = 4,
    /** EDMA Que Threshold 5 */
    CSL_EDMA3_QUE_THR_5 = 5,
    /** EDMA Que Threshold 6 */   
    CSL_EDMA3_QUE_THR_6 = 6, 
    /** EDMA Que Threshold 7 */
    CSL_EDMA3_QUE_THR_7 = 7,
    /** EDMA Que Threshold 8 */
    CSL_EDMA3_QUE_THR_8 = 8,
    /** EDMA Que Threshold 9 */
    CSL_EDMA3_QUE_THR_9 = 9,
    /** EDMA Que Threshold 10 */
    CSL_EDMA3_QUE_THR_10 = 10,
    /** EDMA Que Threshold 11 */
    CSL_EDMA3_QUE_THR_11 = 11,
    /** EDMA Que Threshold 12 */
    CSL_EDMA3_QUE_THR_12 = 12, 
    /** EDMA Que Threshold 13 */
    CSL_EDMA3_QUE_THR_13 = 13,
    /** EDMA Que Threshold 14 */
    CSL_EDMA3_QUE_THR_14 = 14,
    /** EDMA Que Threshold 15 */
    CSL_EDMA3_QUE_THR_15 = 15, 
    /** EDMA Que Threshold 16 */
    CSL_EDMA3_QUE_THR_16 = 16, 
    /* EDMA Que Threshold Disable Errors */
    CSL_EDMA3_QUE_THR_DISABLE = 17  
}CSL_Edma3QueThr;

/** MODULE Level Commands */
typedef enum {
        /**
         * @brief   Programmation of MPPAG,MPPA[0-7] attributes
         *
         * @param   (CSL_Edma3CmdRegion *)
         */
    CSL_EDMA3_CMD_MEMPROTECT_SET,
        /**
         * @brief   Clear Memory Fault
         *
         * @param   (None)
         */
    CSL_EDMA3_CMD_MEMFAULT_CLEAR,
        /**
         * @brief   Enables bits as specified in the argument passed in 
         *          DRAE/DRAEH. Please note:If bits are already set in 
         *          DRAE/DRAEH this Control command will cause additional bits
         *          (as specified by the bitmask) to be set and does   
         * @param   (CSL_Edma3CmdDrae *)
         */
    CSL_EDMA3_CMD_DMAREGION_ENABLE,
        /**
         * @brief   Disables bits as specified in the argument passed in 
         *          DRAE/DRAEH   
         * @param   (CSL_Edma3CmdDrae *)
         */     
    CSL_EDMA3_CMD_DMAREGION_DISABLE,
        /**
         * @brief   Enables bits as specified in the argument 
         *          passed in QRAE.Pleasenote:If bits are already set in 
         *          QRAE/QRAEH this Control command will cause additional bits 
         *          (as specified by the bitmask) to be set and does  
         * @param   (CSL_Edma3CmdQrae *)
         */
    CSL_EDMA3_CMD_QDMAREGION_ENABLE,
        /**
         * @brief   Disables bits as specified in the argument passed in QRAE
         *          DRAE/DRAEH   
         * @param   (CSL_Edma3CmdQrae *)
         */    
    CSL_EDMA3_CMD_QDMAREGION_DISABLE,
        /**
         * @brief   Programmation of QUEPRI register with the specified priority
         *          DRAE/DRAEH   
         * @param   (CSL_Edma3CmdQuePri *)
         */ 
    CSL_EDMA3_CMD_QUEPRIORITY_SET,
        /**
         * @brief   Programmation of QUE Threshold levels
         *
         * @param   (CSL_Edma3CmdQueThr *)
         */      
    CSL_EDMA3_CMD_QUETHRESHOLD_SET,
        /**
         * @brief   Sets the EVAL bit in the EEVAL register
         *
         * @param   (None)
         */
    CSL_EDMA3_CMD_ERROR_EVAL,
        /**
         * @brief   Clears specified (Bitmask)pending interrupt at Module/Region
         *          Level
         * @param   (CSL_Edma3CmdIntr *)
         */         
    CSL_EDMA3_CMD_INTRPEND_CLEAR,
        /**
         * @brief   Enables specified interrupts(BitMask) at Module/Region Level
         *          
         * @param   (CSL_Edma3CmdIntr *)
         */  
    CSL_EDMA3_CMD_INTR_ENABLE, 
        /**
         * @brief   Disables specified interrupts(BitMask) at Module/Region 
         *          Level
         * @param   (CSL_Edma3CmdIntr *)
         */
    CSL_EDMA3_CMD_INTR_DISABLE,
        /**
         * @brief   Interrupt Evaluation asserted for the Module/Region
         *
         * @param   (signed int *)
         */    
    CSL_EDMA3_CMD_INTR_EVAL, 
        /**
         * @brief   Clear the EDMA Controller Erorr
         *
         * @param   (CSL_Edma3CtrlErrStat *)
         */
    CSL_EDMA3_CMD_CTRLERROR_CLEAR , 
        /**
         * @brief   Pointer to an array of 3 elements, where element0 refers to 
         *          the EMR register to be cleared, element1 refers to the EMRH 
         *          register to be cleared, element2 refers to the QEMR register
         *          to be cleared.
         * @param   (CSL_BitMask32 *)
         */
    CSL_EDMA3_CMD_EVENTMISSED_CLEAR     
} CSL_Edma3HwControlCmd;

/** @brief MODULE Level Queries */
typedef enum {
        /**
         * @brief   Return the Memory fault details
         *
         * @param   (CSL_Edma3MemFaultStat *)
         */    
    CSL_EDMA3_QUERY_MEMFAULT,
        /**
         * @brief   Return memory attribute of the specified region
         *
         * @param   (CSL_Edma3CmdRegion *)
         */             
    CSL_EDMA3_QUERY_MEMPROTECT,           
        /**
         * @brief   Return Controller Error
         *
         * @param   (CSL_Edma3CtrlErrStat *)
         */ 
    CSL_EDMA3_QUERY_CTRLERROR,
        /**
         * @brief   Return pend status of specified interrupt
         *
         * @param   (CSL_Edma3CmdIntr *)
         */             
    CSL_EDMA3_QUERY_INTRPEND,
        /**
         * @brief   Returns Miss Status of all Channels
         *          Pointer to an array of 3 elements, where element0 refers to 
         *          the EMR  registr, element1 refers to the EMRH register,  
         *          element2 refers to the QEMR register
         * @param   (CSL_BitMask32 *)
         */             
    CSL_EDMA3_QUERY_EVENTMISSED,
        /**
         * @brief   Returns the Que status
         *
         * @param   (CSL_Edma3QueStat *)
         */          
    CSL_EDMA3_QUERY_QUESTATUS,            
        /**
         * @brief   Returns the Channel Controller Active Status 
         *
         * @param   (CSL_Edma3ActivityStat *)
         */
    CSL_EDMA3_QUERY_ACTIVITY, 
        /**
         * @brief   Returns the Channel Controller Information viz. 
         *          Configuration, Revision Id  
         * @param   (CSL_Edma3QueryInfo *)
         */            
    CSL_EDMA3_QUERY_INFO                 
} CSL_Edma3HwStatusQuery;

/** @brief CHANNEL Commands */
typedef enum {
        /**
         * @brief   Enables specified Channel
         *
         * @param   (None)
         */ 
    CSL_EDMA3_CMD_CHANNEL_ENABLE,
        /**
         * @brief   Disables specified Channel
         *
         * @param   (None)
         */
    CSL_EDMA3_CMD_CHANNEL_DISABLE,
        /**
         * @brief   Manually sets the Channel Event,writes into ESR/ESRH 
         *          and not ER.NA for QDMA
         * @param   (None)
         */       
    CSL_EDMA3_CMD_CHANNEL_SET,
        /**
         * @brief   Manually clears the Channel Event, does not write into
         *          ESR/ESRH or ER/ERH but the ECR/ECRH. NA for QDMA
         * @param   (None)
         */            
    CSL_EDMA3_CMD_CHANNEL_CLEAR,
        /**
         * @brief   In case of DMA channels clears SER/SERH(by writing into 
         *          SECR/SECRH if "secEvt" and "missed" are both TRUE) and 
         *          EMR/EMRH(by writing into EMCR/EMCRH if "missed" is TRUE).
         *          In case of QDMA channels clears QSER(by writing into QSECR 
         *          if "ser" and "missed" are both TRUE) and QEMR(by writing 
         *          into QEMCR if "missed" is TRUE)
         * @param   (CSL_Edma3ChannelErr *)
         */ 
    CSL_EDMA3_CMD_CHANNEL_CLEARERR       
} CSL_Edma3HwChannelControlCmd;

/** @brief CHANNEL Queries */  
typedef enum  {
        /**
         * @brief   In case of DMA channels returns TRUE if ER/ERH is set,
         *          In case of QDMA channels returns TRUE if QER is set
         * @param   (unsigned short *)
         */ 
    CSL_EDMA3_QUERY_CHANNEL_STATUS,
        /**
         * @brief   In case of DMA channels,'missed' is set
         *          to TRUE if EMR/EMRH is set, 'secEvt' is set to TRUE if 
         *          SER/SERH is set.In case of QDMA channels,'missed' is set to 
         *          TRUE if QEMR is set, 'secEvt' is set to TRUE if QSER is set.
         *          It should be noted that if secEvt ONLY is set to TRUE it 
         *          may not be a valid error condition 
         * @param   (CSL_Edma3ChannelErr *)
         */  
    CSL_EDMA3_QUERY_CHANNEL_ERR      
} CSL_Edma3HwChannelStatusQuery;

/**
@}
*/

/** @addtogroup CSL_EDMA3_DATASTRUCT
 @{ */
/** @brief Module specific context information.
 *  This is a dummy handle. 
 */
typedef void *CSL_Edma3Context;      

/** @brief Module Attributes specific information.
 *  This is a dummy handle. 
 */
typedef void *CSL_Edma3ModuleAttr;

/** @brief EDMA3 Configuration Information 
 * This describes the configuration information for each EDMA instance.
 * This is populated by the SOC layer for each instance.
 */ 
typedef struct CSL_Edma3CfgInfo
{
    /** This is the number of DMA channels supported by this instance */
    unsigned char               numDMAChannel;
    /** This is the number of QDMA channels supported by this instance */
    unsigned char               numQDMAChannel;
    /** This is the number of Interrupt Channels supported by this instance */
    unsigned char               numINTChannel;        
    /** This is the number of PARAM Sets supported by this instance */
    unsigned short              numParamsets;
    /** This is the number of Event Queues supported by this instance */
    unsigned char               numEvque;
    /** This is the number of regions supported by this instance */
    unsigned char               numRegions;    
    /** This indicates if the instance supports Channel Mapping. */
    unsigned short                IsChannelMapping;
    /** This indicates if the instance supports Memory Protection. */
    unsigned short                IsMemoryProtection;
}CSL_Edma3CfgInfo;

/** @brief This object contains the reference to the instance of Edma Module
 * opened using the @a CSL_edma3Open().
 *
 *  A pointer to this object is passed to all Edma Module level CSL APIs.
 */ 
typedef struct CSL_Edma3Obj 
{
    /** This is a pointer to the Edma CC registers */
    CSL_Edma3ccRegsOvly regs;
    /** This is the instance of module number i.e CSL_EDMA3 */
    CSL_InstNum         instNum;
    /** This is the configuration information for the CSL EDMA3 instance. */
    CSL_Edma3CfgInfo    cfgInfo;
} CSL_Edma3Obj;

/** @brief EDMA handle */
typedef struct CSL_Edma3Obj *CSL_Edma3Handle;

/** CSL Parameter Set Handle */
typedef volatile CSL_TPCC_ParamsetRegs *CSL_Edma3ParamHandle;


typedef struct CSL_Edma3ParamSetup {
    /** Options */
    unsigned int          option;
    /** Specifies the source address */               
    unsigned int          srcAddr;
    /** Lower 16 bits are A Count Upper 16 bits are B Count*/             
    unsigned int          aCntbCnt;
    /** Specifies the destination address */                                          
    unsigned int          dstAddr;
    /** Lower 16 bits are source b index Upper 16 bits are
     * destination b index 
     */             
    unsigned int          srcDstBidx;
    /** Lower 16 bits are link of the next param entry Upper 16 bits are 
     * b count reload 
     */
    unsigned int          linkBcntrld;
    /** Lower 16 bits are source c index Upper 16 bits are destination 
     * c index 
     */             
    unsigned int          srcDstCidx;
    /** C count */                                           
    unsigned int          cCnt;
} CSL_Edma3ParamSetup;


/** @brief Edma Object Structure
 *
 *  An object of this type is allocated by the user and
 *  its address is passed as a parameter to the CSL_edma3ChannelOpen()
 *  The CSL_edma3ChannelOpen() updates all the members of the data structure
 *  and returns the objects address as a @a #CSL_Edma3ChannelHandle. The
 *  @a #CSL_Edma3ChannelHandle is used in all subsequent function calls.
 */
 
typedef struct CSL_Edma3ChannelObj {
    /** Pointer to the Edma Channel Controller module register 
     * Overlay structure 
     */
    CSL_Edma3ccRegsOvly     regs;               
    /** Region number to which the channel belongs to */ 
    signed int                     region;              
    /** EDMA instance whose channel is being requested */
    signed int                     edmaNum;             
    /** Channel Number being requested */
    unsigned char                   chaNum;
    /** This is the configuration information for the CSL EDMA3 instance. */
    CSL_Edma3CfgInfo        cfgInfo;    
} CSL_Edma3ChannelObj;

/** CSL Channel Handle
 *  All channel level API calls must be made with this handle. 
 */
typedef struct CSL_Edma3ChannelObj *CSL_Edma3ChannelHandle;

/** @brief Edma Memory Protection Fault Error Status
 *
 *  An object of this type is allocated by the user and
 *  its address is passed as a parameter to the CSL_edma3GetMemoryFaultError()
 *  / CSL_edma3GetHwStatus() with the relevant command. This is relevant only is 
 *  MPEXIST is present for a given device.
 */
typedef struct CSL_Edma3MemFaultStat {
    /** Memory Protection Fault Address */
    unsigned int           addr;
    /** Bit Mask of the Errors */               
    CSL_BitMask16    error;
    /** Faulted ID */              
    unsigned short           fid;                
} CSL_Edma3MemFaultStat;

/** @brief Edma Controller Error Status. 
 *
 *  An object of this type is allocated by the user and
 *  its address is passed as a parameter to the CSL_edma3GetControllerError()
 *  /CSL_edma3GetHwStatus().
 */
typedef struct CSL_Edma3CtrlErrStat {
    /** Bit Mask of the Que Threshold Errors */
    CSL_BitMask16    error;
    /** Whether number of permissible outstanding Tcc's is exceeded */
    unsigned short             exceedTcc;          
                                                  
} CSL_Edma3CtrlErrStat;
/** @brief Edma Controller Information
 *
 *  An object of this type is allocated by the user and
 *  its address is passed as a parameter to the CSL_edma3GetInfo()
 *  /CSL_edma3GetHwStatus().
 */
typedef struct CSL_Edma3QueryInfo{
    /** Revision/Periperhal id of the EDMA3 Channel Controller */
    unsigned int revision;
    /** Channel Controller Configuration obtained from the CCCFG register */
    unsigned int config;
} CSL_Edma3QueryInfo;

/** @brief Edma Channel Controller Activity Status
 *
 *  An object of this type is allocated by the user and
 *  its address is passed as a parameter to the CSL_edma3GetActivityStatus()
 *  /CSL_edma3GetHwStatus().
 */
typedef struct CSL_Edma3ActivityStat {
    /** Number of outstanding completion requests */
    unsigned short outstandingTcc;
    /** BitMask of the que active in the Channel Controller */   
    CSL_BitMask16 queActive; 
    /** Indicates if the Channel Controller is active at all */
    unsigned short active; 
    /** Indicates whether any QDMA events are active */
    unsigned short qevtActive;
    /** Indicates whether any EDMA events are active */ 
    unsigned short evtActive;
    /** Indicates whether the TR processing/submission logic is active*/  
    unsigned short trActive;   
} CSL_Edma3ActivityStat;

/** @brief Edma Controller Que Status. 
 *
 *  An object of this type is allocated by the user and
 *  its address is passed as a parameter to the CSL_edma3GetQueStatus()
 *  /CSL_edma3GetHwStatus().
 */
typedef struct CSL_Edma3QueStat {
    /** Input field: Event Que. This needs to be specified by the user 
     *  before invocation of the above API 
     */
    CSL_Edma3Que que;                        
    /** Output field: The number of valid entries in a queue has exceeded the 
     * threshold specified in QWMTHRA has been exceeded 
     */    
    unsigned short             exceed;                 
    /** Output field: The most entries that have been in Que since reset/last 
     * time the watermark was cleared
     */
    unsigned char            waterMark;              
    /** Output field: Number of valid entries in Que N*/                                                
    unsigned char            numVal;                 
    /** Output field: Start pointer/Head of the queue */
    unsigned char            startPtr;               
} CSL_Edma3QueStat;

/** @brief Edma Control/Query Command Structure for querying region specific 
 *  attributes. 
 *
 *  An object of this type is allocated by the user and
 *  its address is passed as a parameter to the 
 *   CSL_edma3GetHwStatus/CSL_edma3HwControl with the relevant command.   
 */
typedef struct CSL_Edma3CmdRegion {
    /** Input field:- this field needs to be initialized by the user before
     * issuing the query/command 
     */
    signed int              region;                 
    /** Input/Output field:-this needs to be filled by the user in case 
     * of issuing a COMMAND or it will be filled in by the CSL when
     * used with a QUERY 
     */                                                
    CSL_BitMask32    regionVal;                                                            
} CSL_Edma3CmdRegion;

/** @brief Edma Control/Query Command Structure for querying qdma region access  
 *  enable attributes. 
 *
 *  An object of this type is allocated by the user and
 *  its address is passed as a parameter to the 
 *  CSL_edma3GetHwStatus/CSL_edma3HwControl with the relevant command.   
 */


typedef struct CSL_Edma3CmdQrae {
    /** this field needs to be initialized by the user before issuing 
     * the query/command
     */
    signed int              region;             
    /** this needs to be filled by the user in case of issuing a 
     * COMMAND or it will be filled in by the CSL when  used with a QUERY 
     */
    CSL_BitMask32    qrae;               
                                                  
                                                 
} CSL_Edma3CmdQrae;

/** @brief Edma Control/Query Control Command structure for issuing commands 
 *  for Interrupt related APIs
 *  An object of this type is allocated by the user and
 *  its address is passed to the Control API. 
 */
typedef struct CSL_Edma3CmdIntr{
    /** Input field:- this field needs to be initialized by the user before
     * issuing the query/command 
     */
    signed int              region;             
    /** Input/Output field:- this needs to be filled by the user in case 
     * of issuing a COMMAND or it will be filled in by the CSL when used with 
     * a QUERY 
     */                
    CSL_BitMask32    intr;               
    /** Input/Output:- this needs to be filled by the user in case of issuing a                                               
     * COMMAND or it will be filled in by the CSL when  used with a QUERY 
     */                                             
    CSL_BitMask32    intrh;              
                                                  
                                                  
} CSL_Edma3CmdIntr;
/** @brief Edma Command Structure for setting region specific 
 *  attributes.
 *
 *  An object of this type is allocated by the user and
 *  its address is passed as a parameter to the CSL_edma3GetHwStatus
 *  when 
 */


typedef struct CSL_Edma3CmdDrae {
    /** this field needs to be initialiazed by the user before issuing 
     * the command specifying the region for which attributes need to be set 
     */
    signed int   region;                
    /** DRAE Setting for the region  */
    CSL_BitMask32     drae;
    /** DRAEH Setting for the region */
    CSL_BitMask32     draeh;     
} CSL_Edma3CmdDrae;


/** @brief Edma Command Structure used for setting Event Que priority level
 *
 *  An object of this type is allocated by the user and
 *  its address is passed as a parameter to the CSL_edma3HwControl API.
 */


typedef struct CSL_Edma3CmdQuePri {
    /** Specifies the Que that needs a priority change */
    CSL_Edma3Que         que;
    /** Que priority */              
    CSL_Edma3QuePri      pri;              
} CSL_Edma3CmdQuePri;    

/** @brief Edma Command Structure used for setting Event Que threshold level
 *
 *  An object of this type is allocated by the user and
 *  its address is passed as a parameter to the CSL_edma3HwControl API.
 */
typedef struct CSL_Edma3CmdQueThr {
    /** Specifies the Que that needs a change in the threshold setting */
    CSL_Edma3Que         que;
    /** Que threshold setting */              
    CSL_Edma3QueThr      threshold;        
} CSL_Edma3CmdQueThr;    

/** @brief This will have the base-address information for the module
 *  instance
 */
typedef struct {
    /** Base-address of the peripheral registers */
    CSL_Edma3ccRegsOvly regs;                   
} CSL_Edma3ModuleBaseAddress;

/** @brief Edma Channel parameter structure used for opening a channel
 */
typedef struct { 
     /** Region Number */   
    signed int regionNum;
    /** Channel number */                             
    signed int chaNum;                                 
} CSL_Edma3ChannelAttr;

/** @brief Edma Channel Error . 
 *
 *  An object of this type is allocated by the user and
 *  its address is passed as a parameter to the CSL_edma3GetChannelError()
 *  /CSL_edma3GetHwStatus()/ CSL_edma3ChannelErrorClear()
 *  /CSL_edma3HwChannelControl().
 */
typedef struct CSL_Edma3ChannelErr {
    /** a TRUE indicates an event is missed on this channel.  */
    unsigned short    missed;      
    /** a TRUE indicates an event that no events on this channel will be
     * prioritized till this is cleared. This being TRUE does NOT necessarily
     * mean it is an error. ONLY if both missed and ser are set, this kind of 
     * error need  to be cleared.
     */
    unsigned short    secEvt;      
} CSL_Edma3ChannelErr;

/** @brief QDMA Edma Channel Setup
 *
 *  An array of such objects are allocated by the user and
 *  address initialized in the CSL_Edma3HwSetup structure which is passed
 *  CSL_edma3HwSetup()
 */

typedef struct CSL_Edma3HwQdmaChannelSetup {
    /** Que number for the channel */
    CSL_Edma3Que que;                            
    /** Parameter set mapping for the channel. */             
    unsigned short  paramNum;                            
    /** Trigger word for the QDMA channels. */
    unsigned char   triggerWord;                         
} CSL_Edma3HwQdmaChannelSetup;

/** @brief QDMA Edma Channel Setup
 *
 *  An array of such objects are allocated by the user and
 *  address initialized in the CSL_Edma3HwSetup structure which is passed
 *  CSL_edma3HwSetup()
 */

typedef struct CSL_Edma3HwDmaChannelSetup {
    /** Que number for the channel */
    CSL_Edma3Que que;                     

    /** Parameter set mapping for the channel. */
    unsigned short  paramNum;                     
} CSL_Edma3HwDmaChannelSetup;

/** @brief Edma Hw Setup Structure 
 */
typedef struct {
    /** Edma Hw Channel setup */
    CSL_Edma3HwDmaChannelSetup *dmaChaSetup;
    /** QEdma Hw Channel setup */
    CSL_Edma3HwQdmaChannelSetup *qdmaChaSetup;
} CSL_Edma3HwSetup;
/**
@}
*/

/** @addtogroup CSL_EDMA3_FUNCTION
 @{ */
/**************************************************************************\
* EDMA global function declarations
\**************************************************************************/

extern CSL_Status CSL_edma3Init (
    CSL_Edma3Context    *pContext
);

extern CSL_Edma3Handle  CSL_edma3Open (
    CSL_Edma3Obj             *edmaObj,
    CSL_InstNum              edmaNum,
    CSL_Edma3ModuleAttr      *attr,
    CSL_Status               *status
);

extern CSL_Status CSL_edma3Close (
    CSL_Edma3Handle       hEdma
);


extern CSL_Status  CSL_edma3HwSetup (
    CSL_Edma3Handle           hMod,
    CSL_Edma3HwSetup          *setup
);

extern CSL_Status  CSL_edma3GetHwSetup (
    CSL_Edma3Handle      hMod,
    CSL_Edma3HwSetup    *setup
);

extern CSL_Status  CSL_edma3HwControl (
    CSL_Edma3Handle           hMod,
    CSL_Edma3HwControlCmd     cmd,
    void                      *cmdArg
);

extern CSL_Status CSL_edma3ccGetModuleBaseAddr (
    CSL_InstNum                 edmaNum,
    CSL_Edma3ModuleAttr*        pAttr,
    CSL_Edma3ModuleBaseAddress* pBaseAddress,
    CSL_Edma3CfgInfo*           pCfgInfo
);

extern CSL_Status  CSL_edma3GetHwStatus (
    CSL_Edma3Handle              hMod,
    CSL_Edma3HwStatusQuery       myQuery,
    void                         *response
);

extern CSL_Edma3ChannelHandle  CSL_edma3ChannelOpen (
    CSL_Edma3ChannelObj        *edmaObj,
    CSL_InstNum                edmaNum,
    CSL_Edma3ChannelAttr       *chAttr,
    CSL_Status                 *status
);

extern CSL_Status  CSL_edma3ChannelClose (
    CSL_Edma3ChannelHandle      hEdma
);

extern CSL_Status  CSL_edma3HwChannelSetupParam (
    CSL_Edma3ChannelHandle      hEdma,
    unsigned short                      paramNum 
);

extern CSL_Status  CSL_edma3HwChannelSetupTriggerWord (
    CSL_Edma3ChannelHandle      hEdma,
    unsigned char                       triggerWord   
);

extern CSL_Status  CSL_edma3HwChannelSetupQue (
    CSL_Edma3ChannelHandle       hEdma,
    CSL_Edma3Que                 que    
);

extern CSL_Status  CSL_edma3GetHwChannelSetupParam (
    CSL_Edma3ChannelHandle       hEdma,
    unsigned short                      *paramNum    
);

extern CSL_Status  CSL_edma3GetHwChannelSetupTriggerWord (
    CSL_Edma3ChannelHandle      hEdma,
    unsigned char                       *triggerWord  
);

extern CSL_Status  CSL_edma3GetHwChannelSetupQue (
    CSL_Edma3ChannelHandle      hEdma,
    CSL_Edma3Que                *que   
);

extern CSL_Status  CSL_edma3HwChannelControl (
    CSL_Edma3ChannelHandle         hCh,
    CSL_Edma3HwChannelControlCmd   cmd,
    void                           *cmdArg
);

extern CSL_Status  CSL_edma3GetHwChannelStatus (
    CSL_Edma3ChannelHandle          hCh,
    CSL_Edma3HwChannelStatusQuery   myQuery,
    void                            *response
);

extern CSL_Edma3ParamHandle  CSL_edma3GetParamHandle (
    CSL_Edma3ChannelHandle       hEdma,
    signed short                        paramNum,
    CSL_Status                   *status       
);

extern CSL_Status  CSL_edma3ParamSetup (
    CSL_Edma3ParamHandle        hParam,
    CSL_Edma3ParamSetup         *pSetup  
);

extern CSL_Status  CSL_edma3ParamWriteWord (
    CSL_Edma3ParamHandle       hParamHndl,
    unsigned short                     wordOffset,
    unsigned int                     word
);

/**
@}
*/

#ifdef __cplusplus
}
#endif

#endif

