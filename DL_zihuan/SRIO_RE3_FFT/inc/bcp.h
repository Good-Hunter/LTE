
#ifndef _BCP_H_
#define _BCP_H_

/* BCP LLD and MMR includes */
#include "bcp_lld.h"
#include "bcp_mmr.h"

/* QMSS LLD include */
#include "qmss_drv.h"

/* CPPI LLD include */
#include "cppi_drv.h"
#include "cppi_desc.h"

/* CHIP module include */
#include  "csl_chip.h"



#ifdef __cplusplus
extern "C"
{
#endif

   
#define     BCP_MAX_NUM_INSTANCES               (CSL_BCP_PER_CNT)

/** @brief
 * The number of CPPI flows reserved for each BCP instance.
 */
#define     BCP_MAX_NUM_FLOWS                   (64)

/** @brief
 *  The maximum number of Rx objects that will be tracked
 *  by the driver globally in the system across all cores.
 */
#define     BCP_MAX_NUM_RXOBJECTS               (64)        

/** @brief  
 *
 *  When returned this, indicates the API succeeded in 
 *  its intent.
 */
#define     BCP_RETVAL_SUCCESS                  (0)

/** @brief  
 *
 *  When returned this, indicates that the handle passed
 *  to the API was invalid.
 */
#define     BCP_RETVAL_EBAD_HANDLE              (-1)

/** @brief  
 *
 *  When returned this, indicates that the input configuration
 *  passed to the API is invalid.
 */
#define     BCP_RETVAL_EINVALID_PARAMS          (-2)

/** @brief  
 *
 *  When returned this, indicates that the API's attempt to
 *  allocate memory or retrieve descriptors failed and 
 *  is out of memory/descriptors.
 */
#define     BCP_RETVAL_ENO_MEM                  (-3)

/** @brief  
 *
 *  When returned this, indicates that there is no pending
 *  BCP processed output available for the application's Rx object.
 */
#define     BCP_RETVAL_ENO_RESULT               (-4)

/** @brief  
 *
 *  When returned this, indicates that the API doesnt support
 *  the feature requested for the inputs passed.
 */
#define     BCP_RETVAL_ENOT_SUPPORTED           (-5)

/** @brief  
 *
 *  When returned this, indicates a general error.
 */
#define     BCP_RETVAL_EFAILURE                 (-6)

/** @brief  
 *
 *  BCP Driver version Id.
 *
 *  The Version ID format is as follows:
 *
 *  0xAABBCCDD -> Arch (AA); API Changes (BB); Major (CC); Minor (DD)
 */
#define     BCP_VERSION_ID                      (0x01000005)

/** @brief  
 *
 *  BCP Driver version Info in string format.
 */
#define     BCP_VERSION_STR                     "@(#) BCP Driver Revision: 01.00.00.05;"
        
/**
@}
*/

/** @addtogroup BCP_DATASTRUCT
 @{ */

/** 
 *  @brief  Bcp_DrvHandle
 *          
 *          BCP driver handle.
 */    
typedef     void*               Bcp_DrvHandle;

/** 
 *  @brief  Bcp_TxHandle
 *          
 *          BCP Tx object handle.
 */    
typedef     void*               Bcp_TxHandle;

/** 
 *  @brief  Bcp_RxHandle
 *          
 *          BCP Rx object handle.
 */    
typedef     void*               Bcp_RxHandle;

/** 
 *  @brief  Bcp_DrvBufferHandle
 *          
 *          BCP driver buffer object handle.
 */    
typedef     void*               Bcp_DrvBufferHandle;

/** 
 *  @brief  Bcp_RetVal
 *          
 *          Holds the various error codes reported by 
 *          the BCP driver.
 *
 *          Please refer to BCP_RETVAL_* definitions for 
 *          the various error codes reported by this driver.
 */    
typedef     signed int             Bcp_RetVal;

     
typedef enum   
{
    /** BCP is local to the device.  
     *
     *  BCP is present on the device (SoC) on which driver is being used
     *  currently. 
     */
    Bcp_DrvMode_LOCAL           = 0,

    /** BCP is remotely accessible to the device.  
     *
     *  BCP is not present on the device (SoC) on which driver is being used
     *  currently. It can be accessed via SRIO.
     */
    Bcp_DrvMode_REMOTE          = 1
} Bcp_DrvMode;

/** 
 *  @brief  Bcp_InitCfg
 *
 *          BCP Peripheral Initialization configuration.
 */    
typedef struct _Bcp_InitCfg
{
    /** CPDMA Number corresponding to this BCP instance. */
    Cppi_CpDma                  cpdmaNum;

    /** Tx queue number base for this BCP instance. */
    unsigned int                    baseTxQueueNum;

    /** Base address for BCP configuration registers for this instance. */
    unsigned int                    cfgRegsBaseAddress;

    /** Callback functions to BCP transport layer. */

    /** Called from Bcp_txOpen () API to initialize one endpoint of the Tx tunnel
     *  between device having BCP and remote device. Application developer should 
     *  implement the necessary logic here to perform any setup required to send 
     *  packets to BCP on the same/remote device as this device.
     */
    void*                       (*BcpTunnel_txOpen) (void* txEpCfg);

    /** Called from Bcp_txClose () API to close a BCP tunnel Tx endpoint. */
    signed int                     (*BcpTunnel_txClose) (void* hTxEpInfo);

    /** Called from Bcp_rxOpen () API to initialize one endpoint of the Rx tunnel
     *  between device having BCP and remote device. Application developer should 
     *  implement the necessary logic here to perform any setup required to receive
     *  packets from BCP on the same/remote device as this device.
     */
    void*                       (*BcpTunnel_rxOpen) (void* rxEpCfg);

    /** Called from Bcp_rxClose () API to close a BCP tunnel Rx endpoint. */
    signed int                     (*BcpTunnel_rxClose) (void* hRxEpInfo);

    /** Called from Bcp_send () API to send out a packet through the tunnel
     *  to a remote BCP device. Invoked by BCP driver only if it was initialized in 
     *  "remote" mode.
     */
    signed int                     (*BcpTunnel_send) (void* hTxEpInfo, void* pPkt, unsigned int pktSize, void* pDestnDev);

    /** Called from Bcp_recv () API to receive output from BCP using tunnel from a remote device.
     *  Invoked by BCP driver only if it was initialized in "remote" mode.
     */
    signed int                     (*BcpTunnel_recv) (void* hRxEpInfo, void** pPkt);

    /** Called from Bcp_rxFreeRecvBuffer () API to free an Rx packet obtained using Bcp_recv ()
     *  API. Invoked by BCP driver only if it was initialized in "remote" mode.
     */
    signed int                     (*BcpTunnel_freeRecvBuffer) (void* hRxEpInfo, void* pPkt, unsigned int pktSize);
} Bcp_InitCfg;


/** 
 *  @brief  Bcp_DrvCfg
 *
 *          BCP Driver configuration.
 */    
typedef struct _Bcp_DrvCfg
{
    unsigned int                    dummy;        
} Bcp_DrvCfg;

/** 
 *  @brief  Bcp_TxCfg
 *
 *          BCP Tx object configuration.
 */    
typedef struct _Bcp_TxCfg
{
    /** BCP Tx queue number to use for sending data to BCP. 
     *
     *  Valid values are 0 - BCP_MAX_NUM_TXQUEUES
     */        
    Bcp_QueueId                 txQNum;
} Bcp_TxCfg;

/** 
 *  @brief  Bcp_RxCfg
 *
 *          BCP Rx object configuration.
 */    
typedef struct _Bcp_RxCfg
{
    /** Rx queue number on which data from BCP must be
     *  received. 
     *
     *  This can be set to -1 to let the driver pick a Rx
     *  queue number.
     */        
    signed int                     rxQNum;

    /** CPPI Receive flow configuration. */            
    Cppi_RxFlowCfg              flowCfg;

    /** Corresponding BCP Traffic Manager flow configuration */
    Bcp_TmFlowEntry             tmFlowCfg;

    /** unsigned shortean flag to indicate whether if interrupt support is 
     *  required for this Rx object.
     *
     *  When set to 1, indicates that accumulation interrupts must 
     *  be used for this Rx object.
     */
    unsigned char                     bUseInterrupts;

    /*  Accumulator configuration to use for this Rx object. */
    Qmss_AccCmdCfg              accumCfg;
} Bcp_RxCfg;

/**
@}
*/

extern unsigned int Bcp_getVersionID 
( 
    void
);

extern const char* Bcp_getVersionStr 
(
    void
);

extern Bcp_RetVal Bcp_init
(
    unsigned char                     instNum, 
    Bcp_DrvMode                 mode,
    Bcp_InitCfg*                pBcpInitCfg
);

extern Bcp_RetVal Bcp_deInit 
(
    unsigned char                     instNum
);

extern unsigned char Bcp_isInitialized
(
    unsigned char                     instNum
);

extern Bcp_LldObj* Bcp_getLLDHandle 
(
    unsigned char                     instNum
);

extern Bcp_DrvHandle Bcp_open 
(
    unsigned char                     instNum,
    Bcp_DrvCfg*                 pBcpDrvCfg,
    Bcp_RetVal*                 pRetVal
);

extern Bcp_RetVal Bcp_close 
(
    Bcp_DrvHandle               hBcp
);

extern Bcp_TxHandle Bcp_txOpen
(
    Bcp_DrvHandle               hBcp,
    Bcp_TxCfg*                  pBcpTxCfg,
    void*                       pTxEndpointCfg
);

extern Bcp_RetVal Bcp_txClose 
(
    Bcp_TxHandle                hBcpTxInfo
);

extern Bcp_RxHandle Bcp_rxOpen
(
    Bcp_DrvHandle               hBcp,
    Bcp_RxCfg*                  pBcpRxCfg,
    void*                       pRxEndpointCfg
);

extern Bcp_RetVal Bcp_rxClose 
(
    Bcp_RxHandle                hBcpRxInfo
);

extern Bcp_RetVal Bcp_rxGetRxQueueNumber
(
    Bcp_RxHandle                hBcpRxInfo
);

extern Bcp_RetVal Bcp_rxGetFlowId
(
    Bcp_RxHandle                hBcpRxInfo
);

extern Bcp_RetVal Bcp_findFlowIdByQueueNumber
(
    Bcp_DrvHandle               hBcp,
    unsigned int                    rxQueueNumber
);

extern Bcp_RetVal Bcp_send 
(
    Bcp_TxHandle                hBcpTxInfo,
    Bcp_DrvBufferHandle         hDrvBuffer,
    unsigned int                    drvBufferLen,
    void*                       pDestnAddress
);

extern signed int Bcp_rxGetNumOutputEntries 
(
    Bcp_RxHandle                hBcpRxInfo
);

extern Bcp_RetVal Bcp_recv
(
    Bcp_RxHandle                hBcpRxInfo,
    Bcp_DrvBufferHandle*        phDrvBuffer,
    unsigned char**                   ppDataBuffer,
    unsigned int*                   pDataBufferLen,                   
    unsigned char**                   ppPSInfo,
    unsigned int*                   pPSInfoLen,
    unsigned char*                    pFlowId,
    unsigned char*                    pSrcId,
    uint16_t*                   pDestnTagInfo
);

extern Bcp_RetVal Bcp_rxProcessDesc 
(
    Bcp_RxHandle                hBcpRxInfo,
    Cppi_Desc*                  pCppiDesc,
    Bcp_DrvBufferHandle*        phDrvBuffer,
    unsigned char**                   ppDataBuffer,
    unsigned int*                   pDataBufferLen,                   
    unsigned char**                   ppPSInfo,
    unsigned int*                   pPSInfoLen,
    unsigned char*                    pFlowId,
    unsigned char*                    pSrcId,
    uint16_t*                   pDestnTagInfo
);

extern Bcp_RetVal Bcp_rxFreeRecvBuffer
(
    Bcp_RxHandle                hBcpRxInfo,
    Bcp_DrvBufferHandle         hDrvBuffer,
    unsigned int                    drvBufferLen
);

#ifdef __cplusplus
}
#endif

#endif  /* __BCP_H__ */

