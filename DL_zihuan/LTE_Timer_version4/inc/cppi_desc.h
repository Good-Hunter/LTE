
#ifndef CPPI_DESC_H_
#define CPPI_DESC_H_

#ifdef __cplusplus
extern "C" {
#endif

/* QMSS LLD includes */
#include "qmss_drv.h"
#include "cppi_drv.h"

/** @addtogroup CPPI_LLD_SYMBOL
@{
*/

/** Monolithic descriptor extended packet information block size */
#define CPPI_MONOLITHIC_DESC_EPIB_SIZE      20
/** Host descriptor extended packet information block size */
#define CPPI_HOST_DESC_EPIB_SIZE            16

/** 
@} 
*/

/** @addtogroup CPPI_LLD_ENUM
@{ 
*/

/** 
 * @brief CPPI descriptor types
 */
typedef enum
{
    /** Host descriptor */
    Cppi_DescType_HOST = 0,
    /** Monolithic descriptor */
    Cppi_DescType_MONOLITHIC = 2
}Cppi_DescType;

/** 
 * @brief Packet return policy
 */
typedef enum
{
    /** Return entire packet */
    Cppi_ReturnPolicy_RETURN_ENTIRE_PACKET = 0,
    /** Return one buffer at a time */
    Cppi_ReturnPolicy_RETURN_BUFFER 
}Cppi_ReturnPolicy;

/** 
 * @brief protocol specific information location 
 */
typedef enum
{
    /** protocol specific information is located in descriptor */
    Cppi_PSLoc_PS_IN_DESC = 0,
    /** protocol specific information is located in SOP buffer */
    Cppi_PSLoc_PS_IN_SOP
}Cppi_PSLoc;

/** 
 * @brief extended packet information block
 */
typedef enum
{
    /** extended packet information block is not present in descriptor */
    Cppi_EPIB_NO_EPIB_PRESENT = 0,
    /** extended packet information block is present in descriptor  */
    Cppi_EPIB_EPIB_PRESENT
}Cppi_EPIB;

/** 
 * @brief Descriptor resource management
 */
typedef enum
{
    /** LLD initializes the descriptors with specified values */
    Cppi_InitDesc_INIT_DESCRIPTOR = 0,
    /** LLD does not initialize the descriptor with specified values */    
    Cppi_InitDesc_BYPASS_INIT 
}Cppi_InitDesc;

/** 
@} 
*/

/** @addtogroup CPPI_LLD_DATASTRUCT
@{ 
*/

/** 
 * @brief CPPI host descriptor configuration structure
 */
typedef struct 
{
    /** Indicates return policy for the packet. 
     * Valid only for host descriptor */
    Cppi_ReturnPolicy       returnPolicy;
    /** Indicates protocol specific location CPPI_PS_DESC - located in descriptor, CPPI_PS_SOP - located in SOP buffer 
     * Valid only for host descriptor */
    Cppi_PSLoc              psLocation;
}Cppi_HostDescCfg;

/** 
 * @brief CPPI monolithic descriptor configuration structure
 */
typedef struct 
{
    /** Byte offset from byte 0 of monolithic descriptor to the location where the valid data begins */
    unsigned int                  dataOffset;
}Cppi_MonolithicDescCfg;

/** 
 * @brief CPPI descriptor configuration structure
 */
typedef struct 
{
    /** Memory Region corresponding to the descriptor. */
    Qmss_MemRegion          memRegion;
    /** Number of descriptors that should be configured with value below */
    unsigned int                  descNum;
    /** Queue where the descriptor is stored. If destQueueNum is set to QMSS_PARAM_NOT_SPECIFIED then the next 
     * available queue of type Qmss_QueueType will be allocated */
    signed int                   destQueueNum;
    /** If destQueueNum is set to QMSS_PARAM_NOT_SPECIFIED then the next available queue of type 
     * Qmss_QueueType will be allocated */
    Qmss_QueueType          queueType;

    /** Descriptor configuration parameters */
    /** Indicates if the descriptor should be initialized with parameters listed below */
    Cppi_InitDesc           initDesc;

    /** Type of descriptor - Host or Monolithic */
    Cppi_DescType           descType;
    /** Indicates return Queue Manager and Queue Number. If both qMgr and qNum in returnQueue is 
     * set to QMSS_PARAM_NOT_SPECIFIED then the destQueueNum is configured in returnQueue of the descriptor */
    Qmss_Queue              returnQueue;
    /** Indicates how the CPDMA returns descriptors to free queue */
    Qmss_Location           returnPushPolicy;
    /** Indicates presence of EPIB */
    Cppi_EPIB               epibPresent;

    /** Union contains configuration that should be initialized in for host or monolithic descriptor. 
     * The configuration for host or monolithic descriptor is choosen based on "descType" field. 
     * The approriate structure fields must be specified if "initDesc" field is set to CPPI_INIT_DESCRIPTOR. */
    union{
    /** Host descriptor configuration parameters */
    Cppi_HostDescCfg        host;
    /** Monolithic  descriptor configuration parameters */
    Cppi_MonolithicDescCfg  mono;
    }cfg;
}Cppi_DescCfg;

/** 
 * @brief CPPI descriptor Word 1 Tag information
 */
typedef struct {
    unsigned char srcTagHi;
    unsigned char srcTagLo;
    unsigned char destTagHi;
    unsigned char destTagLo;
}Cppi_DescTag;

/** 
 * @brief CPPI host descriptor layout
 */
typedef struct {
    /** Descriptor type, packet type, protocol specific region location, packet length */
    unsigned int          descInfo;  
    /** Source tag, Destination tag */
    unsigned int          tagInfo;
    /** EPIB present, PS valid word count, error flags, PS flags, return policy, return push policy, 
     * packet return QM number, packet return queue number */
    unsigned int          packetInfo;
    /** Number of valid data bytes in the buffer */
    unsigned int          buffLen;
    /** Byte aligned memory address of the buffer associated with this descriptor */
    unsigned int          buffPtr;
    /** 32-bit word aligned memory address of the next buffer descriptor */
    unsigned int          nextBDPtr;       
    /** Completion tag, original buffer size */
    unsigned int          origBufferLen;
    /** Original buffer pointer */
    unsigned int          origBuffPtr;
    /** Optional EPIB word0 */
    unsigned int          timeStamp;
    /** Optional EPIB word1 */
    unsigned int          softwareInfo0;
    /** Optional EPIB word2 */
    unsigned int          softwareInfo1;
    /** Optional EPIB word3 */
    unsigned int          softwareInfo2;
    /** Optional protocol specific data */
    unsigned int          psData; 
}Cppi_HostDesc;

/** 
 * @brief CPPI monolithic descriptor layout
 */
typedef struct {
    /** Descriptor type, packet type, data offset, packet length */
    unsigned int          descInfo;  
    /** Source tag, Destination tag */
    unsigned int          tagInfo;
    /** EPIB present, PS valid word count, error flags, PS flags, return push policy, 
     * packet return QM number, packet return queue number */
    unsigned int          packetInfo;
    /** NULL word to align the extended packet words to a 128 bit boundary */
    unsigned int          Reserved;
    /** Optional EPIB word0 */
    unsigned int          timeStamp;
    /** Optional EPIB word1 */
    unsigned int          softwareInfo0;
    /** Optional EPIB word2 */
    unsigned int          softwareInfo1;
    /** Optional EPIB word3 */
    unsigned int          softwareInfo2;
    /** Optional protocol specific data */
    unsigned int          psData; 
}Cppi_MonolithicDesc;

/** 
 * @brief CPPI descriptor
 */
typedef union {
    /** Host descriptor */
    Cppi_HostDesc       *ptrHostDesc;  
    /** Monolithic descriptor */
    Cppi_MonolithicDesc *ptrMonoDesc;
}Cppi_Desc;

/** 
@} 
*/


static inline void Cppi_setDescType (Cppi_Desc *descAddr, Cppi_DescType descType)
{
    Cppi_HostDesc   *descPtr = (Cppi_HostDesc *) descAddr;

    /* No validation of input parameters is done */

    CSL_FINSR (descPtr->descInfo, 31, 30, descType);
    return;
}

static inline Cppi_DescType Cppi_getDescType (Cppi_Desc *descAddr)
{
    Cppi_HostDesc   *descPtr = (Cppi_HostDesc *) descAddr;

    /* No validation of input parameters is done */
    return (Cppi_DescType) CSL_FEXTR (descPtr->descInfo, 31, 30);
}


static inline unsigned int Cppi_getDescError (Cppi_DescType descType, Cppi_Desc *descAddr)
{
    Cppi_HostDesc   *descPtr = (Cppi_HostDesc *) descAddr;

    /* No validation of input parameters is done */

    return (CSL_FEXTR (descPtr->packetInfo, 23, 20));
}


static inline void Cppi_setData (Cppi_DescType descType, Cppi_Desc *descAddr, unsigned char *buffAddr, unsigned int buffLen)
{
    Cppi_HostDesc   *hostDescPtr;

    /* Does not update packet length */
    if (descType == Cppi_DescType_HOST)
    {
        hostDescPtr = (Cppi_HostDesc *) descAddr;
        hostDescPtr->buffPtr = (unsigned int) buffAddr;
        hostDescPtr->buffLen = (unsigned int) buffLen;
    }
    else
    {
        Cppi_MonolithicDesc *monolithicDescPtr = (Cppi_MonolithicDesc *) descAddr;
        unsigned short              dataOffset;

        dataOffset = CSL_FEXTR (monolithicDescPtr->descInfo, 24, 16);
        memcpy ((void *) (((unsigned char *) monolithicDescPtr) + dataOffset), buffAddr, buffLen);        
        CSL_FINSR (monolithicDescPtr->descInfo, 15, 0, buffLen);
    }
    return;
}


static inline void Cppi_getData (Cppi_DescType descType, Cppi_Desc *descAddr, unsigned char **buffAddr, unsigned int *buffLen)
{
    /* For monolithic, start of payload is returned. Check if need to skip data offset bytes */
    Cppi_HostDesc   *hostDescPtr;

    if (descType == Cppi_DescType_HOST)
    {
        hostDescPtr = (Cppi_HostDesc *) descAddr;
        *buffAddr = (unsigned char *) hostDescPtr->buffPtr;
        *buffLen = hostDescPtr->buffLen;
    }
    else
    {
        Cppi_MonolithicDesc *monolithicDescPtr = (Cppi_MonolithicDesc *) descAddr;
        unsigned short              dataOffset;

        dataOffset = CSL_FEXTR (monolithicDescPtr->descInfo, 24, 16);

        *buffAddr = (unsigned char *) (((unsigned char *) monolithicDescPtr) + dataOffset);
        *buffLen = CSL_FEXTR (monolithicDescPtr->descInfo, 15, 0);
    }
    return;
}


static inline void Cppi_setDataLen (Cppi_DescType descType, Cppi_Desc *descAddr, unsigned int buffLen)
{
    Cppi_HostDesc   *hostDescPtr;

    /* Does not update packet length */
    if (descType == Cppi_DescType_HOST)
    {
        hostDescPtr = (Cppi_HostDesc *) descAddr;
        hostDescPtr->buffLen = (unsigned int) buffLen;
    }
    else
    {
        Cppi_MonolithicDesc *monolithicDescPtr = (Cppi_MonolithicDesc *) descAddr;
        CSL_FINSR (monolithicDescPtr->descInfo, 15, 0, buffLen);
    }
    return;
}



static inline unsigned int Cppi_getDataLen (Cppi_DescType descType, Cppi_Desc *descAddr)
{
    /* For monolithic, start of payload is returned. Check if need to skip data offset bytes */
    Cppi_HostDesc   *hostDescPtr;

    if (descType == Cppi_DescType_HOST)
    {
        hostDescPtr = (Cppi_HostDesc *) descAddr;
        return (hostDescPtr->buffLen);
    }
    else
    {
        Cppi_MonolithicDesc *monolithicDescPtr = (Cppi_MonolithicDesc *) descAddr;
        return (CSL_FEXTR (monolithicDescPtr->descInfo, 15, 0));
    }
}


static inline void Cppi_linkNextBD (Cppi_DescType descType, Cppi_Desc *descAddr, Cppi_Desc *nextBD)
{
    Cppi_HostDesc   *hostDescPtr = (Cppi_HostDesc *) descAddr;

    hostDescPtr->nextBDPtr = (unsigned int) nextBD;
    return;
}

static inline Cppi_Desc* Cppi_getNextBD (Cppi_DescType descType, Cppi_Desc *descAddr)
{
    Cppi_HostDesc   *hostDescPtr = (Cppi_HostDesc *) descAddr;

    return (Cppi_Desc *) hostDescPtr->nextBDPtr;
}


static inline void Cppi_setOriginalBufInfo (Cppi_DescType descType, Cppi_Desc *descAddr, unsigned char *buffAddr, unsigned int buffLen)
{
    Cppi_HostDesc   *hostDescPtr = (Cppi_HostDesc *) descAddr;

    hostDescPtr->origBuffPtr = (unsigned int) buffAddr;
    CSL_FINSR (hostDescPtr->origBufferLen, 21, 0, buffLen);
 
    return;
}


static inline void Cppi_getOriginalBufInfo (Cppi_DescType descType, Cppi_Desc *descAddr, unsigned char **buffAddr, unsigned int *buffLen)
{
    Cppi_HostDesc   *hostDescPtr = (Cppi_HostDesc *) descAddr;

    *buffAddr = (unsigned char *) hostDescPtr->origBuffPtr;
    *buffLen = CSL_FEXTR (hostDescPtr->origBufferLen, 21, 0);

    return;
}

static inline void Cppi_setPacketType (Cppi_DescType descType, Cppi_Desc *descAddr, unsigned char packetType)
{
    Cppi_HostDesc   *descPtr = (Cppi_HostDesc *) descAddr;

    CSL_FINSR (descPtr->descInfo, 29, 25, packetType);
    return;
}


static inline unsigned char Cppi_getPacketType (Cppi_DescType descType, Cppi_Desc *descAddr)
{
    Cppi_HostDesc   *descPtr = (Cppi_HostDesc *) descAddr;

    return (CSL_FEXTR (descPtr->descInfo, 29, 25));
}


static inline void Cppi_setTimeStamp (Cppi_DescType descType, Cppi_Desc *descAddr, unsigned int timeStamp)
{
    Cppi_HostDesc   *hostDescPtr = (Cppi_HostDesc *) descAddr;

    CSL_FINSR (hostDescPtr->packetInfo, 31, 31, (unsigned int) 1);
    if (descType == Cppi_DescType_HOST)
        hostDescPtr->timeStamp = timeStamp;
    else
    {
        Cppi_MonolithicDesc *monolithicDescPtr = (Cppi_MonolithicDesc *) descAddr;
        monolithicDescPtr->timeStamp = timeStamp;
    }
    return;
}

static inline Cppi_Result Cppi_getTimeStamp (Cppi_DescType descType, Cppi_Desc *descAddr, unsigned int *timeStamp)
{
    Cppi_HostDesc   *hostDescPtr = (Cppi_HostDesc *) descAddr;

    if (CSL_FEXTR (hostDescPtr->packetInfo, 31, 31) == 0)
    {
        return CPPI_EPIB_NOT_PRESENT;
    }
    if (descType == Cppi_DescType_HOST)
        *timeStamp = hostDescPtr->timeStamp;
    else
    {
        Cppi_MonolithicDesc *monolithicDescPtr = (Cppi_MonolithicDesc *) descAddr;
        *timeStamp = monolithicDescPtr->timeStamp;
    }
    return CPPI_SOK;
}


static inline void Cppi_setSoftwareInfo (Cppi_DescType descType, Cppi_Desc *descAddr, unsigned char *infoAddr)
{
    Cppi_HostDesc   *hostDescPtr = (Cppi_HostDesc *) descAddr;

    /* Does not update packet length */
    CSL_FINSR (hostDescPtr->packetInfo, 31, 31, (unsigned int) 1);
    
    if (descType == Cppi_DescType_HOST)
        memcpy ((void *) &hostDescPtr->softwareInfo0, infoAddr, 12);
    else
    {
        Cppi_MonolithicDesc *monolithicDescPtr = (Cppi_MonolithicDesc *) descAddr;
        memcpy ((void *) &monolithicDescPtr->softwareInfo0, infoAddr, 12);
    }
    return;
}


static inline Cppi_Result Cppi_getSoftwareInfo (Cppi_DescType descType, Cppi_Desc *descAddr, unsigned char **infoAddr)
{
    Cppi_HostDesc   *hostDescPtr = (Cppi_HostDesc *) descAddr;

    if (CSL_FEXTR (hostDescPtr->packetInfo, 31, 31) == 0)
    {
        return CPPI_EPIB_NOT_PRESENT;
    }
    if (descType == Cppi_DescType_HOST)
        *infoAddr = (unsigned char *) &hostDescPtr->softwareInfo0;
    else
    {
        Cppi_MonolithicDesc *monolithicDescPtr = (Cppi_MonolithicDesc *) descAddr;
        *infoAddr = (unsigned char *) &monolithicDescPtr->softwareInfo0;
    }
    return CPPI_SOK;
}


static inline void Cppi_setSoftwareInfo0 (Cppi_DescType descType, Cppi_Desc *descAddr, unsigned int value)
{
    Cppi_HostDesc   *hostDescPtr = (Cppi_HostDesc *) descAddr;

    /* Does not update packet length */
    CSL_FINSR (hostDescPtr->packetInfo, 31, 31, (unsigned int) 1);
    
    if (descType == Cppi_DescType_HOST)
        hostDescPtr->softwareInfo0 = value;
    else
    {
        Cppi_MonolithicDesc *monolithicDescPtr = (Cppi_MonolithicDesc *) descAddr;
        monolithicDescPtr->softwareInfo0 = value;
    }
    return;
}


static inline unsigned int Cppi_getSoftwareInfo0 (Cppi_DescType descType, Cppi_Desc *descAddr)
{
    Cppi_HostDesc   *hostDescPtr = (Cppi_HostDesc *) descAddr;

    if (descType == Cppi_DescType_HOST)
        return (hostDescPtr->softwareInfo0);
    else
    {
        Cppi_MonolithicDesc *monolithicDescPtr = (Cppi_MonolithicDesc *) descAddr;
        return (monolithicDescPtr->softwareInfo0);
    }
}



static inline void Cppi_setSoftwareInfo1 (Cppi_DescType descType, Cppi_Desc *descAddr, unsigned int value)
{
    Cppi_HostDesc   *hostDescPtr = (Cppi_HostDesc *) descAddr;

    if (descType == Cppi_DescType_HOST)
        hostDescPtr->softwareInfo1 = value;
    else
    {
        Cppi_MonolithicDesc *monolithicDescPtr = (Cppi_MonolithicDesc *) descAddr;
        monolithicDescPtr->softwareInfo1 = value;
    }
    return;
}


static inline unsigned int Cppi_getSoftwareInfo1 (Cppi_DescType descType, Cppi_Desc *descAddr)
{
    Cppi_HostDesc   *hostDescPtr = (Cppi_HostDesc *) descAddr;

    if (descType == Cppi_DescType_HOST)
        return (hostDescPtr->softwareInfo1);
    else
    {
        Cppi_MonolithicDesc *monolithicDescPtr = (Cppi_MonolithicDesc *) descAddr;
        return (monolithicDescPtr->softwareInfo1);
    }
}


static inline void Cppi_setSoftwareInfo2 (Cppi_DescType descType, Cppi_Desc *descAddr, unsigned int value)
{
    Cppi_HostDesc   *hostDescPtr = (Cppi_HostDesc *) descAddr;

    if (descType == Cppi_DescType_HOST)
        hostDescPtr->softwareInfo2 = value;
    else
    {
        Cppi_MonolithicDesc *monolithicDescPtr = (Cppi_MonolithicDesc *) descAddr;
        monolithicDescPtr->softwareInfo2 = value;
    }
    return;
}


static inline unsigned int Cppi_getSoftwareInfo2 (Cppi_DescType descType, Cppi_Desc *descAddr)
{
    Cppi_HostDesc   *hostDescPtr = (Cppi_HostDesc *) descAddr;

    if (descType == Cppi_DescType_HOST)
        return (hostDescPtr->softwareInfo2);
    else
    {
        Cppi_MonolithicDesc *monolithicDescPtr = (Cppi_MonolithicDesc *) descAddr;
        return (monolithicDescPtr->softwareInfo2);
    }
}


static inline void Cppi_setPSData (Cppi_DescType descType, Cppi_Desc *descAddr, unsigned char *dataAddr, unsigned int dataLen)
{
    Cppi_HostDesc   *hostDescPtr = (Cppi_HostDesc *) descAddr;
    unsigned char           epibPresent;
 
    /* Does not update packet length */
    CSL_FINSR (hostDescPtr->packetInfo, 29, 24, (dataLen / 4));

    epibPresent = CSL_FEXTR (hostDescPtr->packetInfo, 31, 31);

    if (descType == Cppi_DescType_HOST)
        memcpy ((void *) (((unsigned char *) &hostDescPtr->psData) - (!epibPresent * CPPI_HOST_DESC_EPIB_SIZE)), dataAddr, dataLen);
    else
    {
        Cppi_MonolithicDesc *monolithicDescPtr = (Cppi_MonolithicDesc *) descAddr;
        memcpy ((void *) (((unsigned char *) &monolithicDescPtr->psData) - (!epibPresent * CPPI_MONOLITHIC_DESC_EPIB_SIZE)), dataAddr, dataLen);
    }
    return;
}


static inline Cppi_Result Cppi_getPSData (Cppi_DescType descType, Cppi_PSLoc location, Cppi_Desc *descAddr, unsigned char **dataAddr, unsigned int *dataLen)
{
    Cppi_HostDesc   *hostDescPtr = (Cppi_HostDesc *) descAddr;
    unsigned char           epibPresent;

    if ((*dataLen = CSL_FEXTR (hostDescPtr->packetInfo, 29, 24) * 4) == 0)
    {
        return CPPI_PSDATA_NOT_PRESENT;
    }

    epibPresent = CSL_FEXTR (hostDescPtr->packetInfo, 31, 31);

    if (descType == Cppi_DescType_HOST)
    {
        if (location == Cppi_PSLoc_PS_IN_SOP)
            *dataAddr = (unsigned char *) hostDescPtr->buffPtr;
        else
            *dataAddr = (unsigned char *) (((unsigned char *) &hostDescPtr->psData) - (!epibPresent * CPPI_HOST_DESC_EPIB_SIZE));
    }
    else
    {
        Cppi_MonolithicDesc *monolithicDescPtr = (Cppi_MonolithicDesc *) descAddr;
        *dataAddr = (unsigned char *) (((unsigned char *) &monolithicDescPtr->psData) - (!epibPresent * CPPI_MONOLITHIC_DESC_EPIB_SIZE));
    }
    return CPPI_SOK;
}


static inline void Cppi_setPSLen (Cppi_DescType descType, Cppi_Desc *descAddr, unsigned int dataLen)
{
    Cppi_HostDesc   *descPtr = (Cppi_HostDesc *) descAddr;
 
    /* Does not update packet length */
    CSL_FINSR (descPtr->packetInfo, 29, 24, (dataLen / 4));
    return;
}


static inline unsigned int Cppi_getPSLen (Cppi_DescType descType, Cppi_Desc *descAddr)
{
    Cppi_HostDesc   *descPtr = (Cppi_HostDesc *) descAddr;

    return ((CSL_FEXTR (descPtr->packetInfo, 29, 24) * 4));
}


static inline void Cppi_setPacketLen (Cppi_DescType descType, Cppi_Desc *descAddr, unsigned int packetLen)
{
    Cppi_HostDesc   *descPtr = (Cppi_HostDesc *) descAddr;

    if (descType == Cppi_DescType_HOST)
        CSL_FINSR (descPtr->descInfo, 21, 0, packetLen);
    else
        CSL_FINSR (descPtr->descInfo, 15, 0, packetLen);
    return;
}


static inline unsigned int Cppi_getPacketLen (Cppi_DescType descType, Cppi_Desc *descAddr)
{
    Cppi_HostDesc   *descPtr = (Cppi_HostDesc *) descAddr;

    if (descType == Cppi_DescType_HOST)
        return (CSL_FEXTR (descPtr->descInfo, 21, 0));
    else
        return (CSL_FEXTR (descPtr->descInfo, 15, 0));
}

static inline void Cppi_setPSLocation (Cppi_DescType descType, Cppi_Desc *descAddr, Cppi_PSLoc location)
{
    Cppi_HostDesc   *hostDescPtr = (Cppi_HostDesc *) descAddr;

    CSL_FINSR (hostDescPtr->descInfo, 22, 22, location);
    return;
}


static inline Cppi_PSLoc Cppi_getPSLocation (Cppi_DescType descType, Cppi_Desc *descAddr)
{
    Cppi_HostDesc   *hostDescPtr = (Cppi_HostDesc *) descAddr;

    return ((Cppi_PSLoc) CSL_FEXTR (hostDescPtr->descInfo, 22, 22));
}


static inline void Cppi_setPSFlags (Cppi_DescType descType, Cppi_Desc *descAddr, unsigned char psFlags)
{
    Cppi_HostDesc   *descPtr = (Cppi_HostDesc *) descAddr;

    CSL_FINSR (descPtr->packetInfo, 19, 16, psFlags);
    return;
}


static inline unsigned char Cppi_getPSFlags (Cppi_DescType descType, Cppi_Desc *descAddr)
{
    Cppi_HostDesc   *descPtr = (Cppi_HostDesc *) descAddr;

    return (CSL_FEXTR (descPtr->packetInfo, 19, 16));
}


static inline void Cppi_setOrigBufferpooIndex (Cppi_DescType descType, Cppi_Desc *descAddr, unsigned char poolIndex)
{
    Cppi_HostDesc   *hostDescPtr = (Cppi_HostDesc *) descAddr;

    CSL_FINSR (hostDescPtr->origBufferLen, 31, 28, poolIndex);

    return;
}


static inline unsigned char Cppi_getOrigBufferpooIndex (Cppi_DescType descType, Cppi_Desc *descAddr)
{
    Cppi_HostDesc   *hostDescPtr = (Cppi_HostDesc *) descAddr;

    return (CSL_FEXTR (hostDescPtr->origBufferLen, 31, 28));
}


static inline void Cppi_incrementRefCount (Cppi_DescType descType, Cppi_Desc *descAddr)
{
    Cppi_HostDesc   *hostDescPtr = (Cppi_HostDesc *) descAddr;
    unsigned char        count;

    count = CSL_FEXTR (hostDescPtr->origBufferLen, 27, 22);
    CSL_FINSR (hostDescPtr->origBufferLen, 27, 22, ++count);

    return;
}


static inline void Cppi_decrementRefCount (Cppi_DescType descType, Cppi_Desc *descAddr)
{
    Cppi_HostDesc   *hostDescPtr = (Cppi_HostDesc *) descAddr;
    unsigned char        count;

    count = CSL_FEXTR (hostDescPtr->origBufferLen, 27, 22);
    CSL_FINSR (hostDescPtr->origBufferLen, 27, 22, --count);

    return;
}


static inline unsigned char Cppi_getRefCount (Cppi_DescType descType, Cppi_Desc *descAddr)
{
    Cppi_HostDesc   *hostDescPtr = (Cppi_HostDesc *) descAddr;

    return (CSL_FEXTR (hostDescPtr->origBufferLen, 27, 22));
}


static inline void Cppi_setDataOffset (Cppi_DescType descType, Cppi_Desc *descAddr, unsigned int dataOffset)
{
    Cppi_MonolithicDesc *monolithicDescPtr = (Cppi_MonolithicDesc *) descAddr;

    CSL_FINSR (monolithicDescPtr->descInfo, 24, 16, dataOffset);
    return;
}


static inline unsigned int Cppi_getDataOffset (Cppi_DescType descType, Cppi_Desc *descAddr)
{
    Cppi_MonolithicDesc *monolithicDescPtr = (Cppi_MonolithicDesc *) descAddr;

    return (CSL_FEXTR (monolithicDescPtr->descInfo, 24, 16));
}


static inline void Cppi_setReturnPolicy (Cppi_DescType descType, Cppi_Desc *descAddr, Cppi_ReturnPolicy returnPolicy)
{
    Cppi_HostDesc   *hostDescPtr = (Cppi_HostDesc *) descAddr;

    CSL_FINSR (hostDescPtr->packetInfo, 15, 15, returnPolicy);
    return;
}


static inline Cppi_ReturnPolicy Cppi_getReturnPolicy (Cppi_DescType descType, Cppi_Desc *descAddr)
{
    Cppi_HostDesc   *hostDescPtr = (Cppi_HostDesc *) descAddr;

    return ((Cppi_ReturnPolicy) CSL_FEXTR (hostDescPtr->packetInfo, 15, 15));
}


static inline void Cppi_setReturnPushPolicy (Cppi_DescType descType, Cppi_Desc *descAddr, Qmss_Location returnPushPolicy)
{
    Cppi_HostDesc   *hostDescPtr = (Cppi_HostDesc *) descAddr;

    CSL_FINSR (hostDescPtr->packetInfo, 14, 14, returnPushPolicy);
    return;
}


static inline Qmss_Location Cppi_getReturnPushPolicy (Cppi_DescType descType, Cppi_Desc *descAddr)
{
    Cppi_HostDesc   *hostDescPtr = (Cppi_HostDesc *) descAddr;

    return ((Qmss_Location) CSL_FEXTR (hostDescPtr->packetInfo, 14, 14));
}


static inline void Cppi_setReturnQueue (Cppi_DescType descType, Cppi_Desc *descAddr, Qmss_Queue queue)
{
    Cppi_HostDesc   *descPtr = (Cppi_HostDesc *) descAddr;

    CSL_FINSR (descPtr->packetInfo, 13, 12, queue.qMgr);
    CSL_FINSR (descPtr->packetInfo, 11, 0, queue.qNum);
    return;
}


static inline Qmss_Queue Cppi_getReturnQueue (Cppi_DescType descType, Cppi_Desc *descAddr)
{
    Cppi_HostDesc   *descPtr = (Cppi_HostDesc *) descAddr;
    Qmss_Queue      queue;

    queue.qMgr = CSL_FEXTR (descPtr->packetInfo, 13, 12);
    queue.qNum = CSL_FEXTR (descPtr->packetInfo, 11, 0);
    return (queue);
}


static inline void Cppi_setTag (Cppi_DescType descType, Cppi_Desc *descAddr, Cppi_DescTag *tag)
{
    Cppi_HostDesc   *descPtr = (Cppi_HostDesc *) descAddr;

    CSL_FINSR (descPtr->tagInfo, 7, 0, tag->destTagLo);
    CSL_FINSR (descPtr->tagInfo, 15, 8, tag->destTagHi);
    CSL_FINSR (descPtr->tagInfo, 23, 16, tag->srcTagLo);
    CSL_FINSR (descPtr->tagInfo, 31, 24, tag->srcTagHi);
    return;
}


static inline Cppi_DescTag Cppi_getTag (Cppi_DescType descType, Cppi_Desc *descAddr)
{
    Cppi_HostDesc   *descPtr = (Cppi_HostDesc *) descAddr;
    Cppi_DescTag    tag;

    tag.destTagLo = CSL_FEXTR (descPtr->tagInfo, 7, 0);
    tag.destTagHi = CSL_FEXTR (descPtr->tagInfo, 15, 8);
    tag.srcTagLo = CSL_FEXTR (descPtr->tagInfo, 23, 16);
    tag.srcTagHi = CSL_FEXTR (descPtr->tagInfo, 31, 24);

    return (tag);
}


extern Qmss_QueueHnd Cppi_initDescriptor (Cppi_DescCfg *descCfg, unsigned int *numAllocated);

#ifdef __cplusplus
}
#endif

#endif /* CPPI_DESC_H_ */

