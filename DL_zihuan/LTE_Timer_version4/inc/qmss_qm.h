
#ifndef QMSS_QM_H_
#define QMSS_QM_H_

#ifdef __cplusplus
extern "C" {
#endif

/* QM device specific include file */
//#include <ti/drv/qmss/device/qmss_device.h>
        
/* CSL RL includes */
#include "cslr_device.h"
#include "cslr_qm_config.h"
#include "cslr_qm_descriptor_region_config.h"
#include "cslr_qm_queue_management.h"
#include "cslr_qm_queue_status_config.h"
#include "cslr_pdsp.h"
#include "cslr_qm_intd.h"
#include "cslr_mcdma.h"
#include "cslr_cp_timer16.h"
#include "csl_qm_queue.h"


#define QMSS_LLD_VERSION_ID                         (0x0100000F)


#define QMSS_LLD_VERSION_STR                        "QMSS LLD Revision: 01.00.00.15"

/** signed internal Linking RAM offset */
#define QMSS_LINKING_RAM_OFFSET                     0x80000
/** signed internal Linking RAM default size */
#define QMSS_LINKING_RAM_REGION_0_DEFAULT_SIZE      0x3FFF

/** Used as input parameter when queue number is 
 * not known and not specified */
#define QMSS_PARAM_NOT_SPECIFIED                    -1

/** QMSS Low level Driver return and Error Codes */
/** QMSS successful return code */
#define QMSS_SOK                                    0
/** QMSS Error Base */       
#define QMSS_LLD_EBASE                              (-128)
/** QMSS LLD invalid parameter */
#define QMSS_INVALID_PARAM                          QMSS_LLD_EBASE-1
/** QMSS LLD not initialized */
#define QMSS_NOT_INITIALIZED                        QMSS_LLD_EBASE-2
/** QMSS LLD queue open error */
#define QMSS_QUEUE_OPEN_ERROR                       QMSS_LLD_EBASE-3
/** QMSS memory region not initialized */
#define QMSS_MEMREGION_NOT_INITIALIZED              QMSS_LLD_EBASE-4
/** QMSS memory region already initialized */
#define QMSS_MEMREGION_ALREADY_INITIALIZED          QMSS_LLD_EBASE-5
/** QMSS memory region invalid parameter */
#define QMSS_MEMREGION_INVALID_PARAM                QMSS_LLD_EBASE-6
/** QMSS maximum number of allowed descriptor are already configured */
#define QMSS_MAX_DESCRIPTORS_CONFIGURED             QMSS_LLD_EBASE-7
/** QMSS Specified memory region index is invalid or no memory regions are available */
#define QMSS_MEMREGION_INVALID_INDEX                QMSS_LLD_EBASE-8
/** QMSS memory region overlap */
#define QMSS_MEMREGION_OVERLAP                      QMSS_LLD_EBASE-9
/** QMSS PDSP firmware download failure */
#define QMSS_FIRMWARE_DOWNLOAD_FAILED               QMSS_LLD_EBASE-10

/** QMSS maximum number of memory regions */
#define QMSS_MAX_MEM_REGIONS                        20
#define QMSS_MAX_PDSP                               2


#define QMSS_DESC_PTR(desc)                         ((unsigned int)desc & 0xFFFFFFF0)


#define QMSS_DESC_SIZE(desc)                        ((((unsigned int)desc & 0x0000000F) + 1) << 4) 

/**
@}
*/


typedef enum
{
    /** Queue packet to the tail of the queue. Default behavior. */
    Qmss_Location_TAIL = 0,
    /** Queue packet to the head of the queue. */
    Qmss_Location_HEAD 
}Qmss_Location;


typedef enum
{
    /** LLD doesnot manage the descriptors. The caller should manage them. */
    Qmss_ManageDesc_UNMANAGED_DESCRIPTOR = 0,
    /** LLD manages the descriptors. The descriptors are reclaimed using 
     * the QMSS_initDescriptor() or CPPI_initDescriptor() APIs
     * */
    Qmss_ManageDesc_MANAGE_DESCRIPTOR 
}Qmss_ManageDesc;


typedef enum
{
    /** Memory region not specified. LLD allocates the next available memory region */
    Qmss_MemRegion_MEMORY_REGION_NOT_SPECIFIED = -1,
    /** Configure memory region0. */
    Qmss_MemRegion_MEMORY_REGION0 = 0,
    /** Configure memory region 1. */
    Qmss_MemRegion_MEMORY_REGION1,
    /** Configure memory region 2. */
    Qmss_MemRegion_MEMORY_REGION2,
    /** Configure memory region 3. */
    Qmss_MemRegion_MEMORY_REGION3,
    /** Configure memory region 4. */
    Qmss_MemRegion_MEMORY_REGION4,
    /** Configure memory region 5. */
    Qmss_MemRegion_MEMORY_REGION5,
    /** Configure memory region 6. */
    Qmss_MemRegion_MEMORY_REGION6,
    /** Configure memory region 7. */
    Qmss_MemRegion_MEMORY_REGION7,
    /** Configure memory region 8. */
    Qmss_MemRegion_MEMORY_REGION8,
    /** Configure memory region 9. */
    Qmss_MemRegion_MEMORY_REGION9,
    /** Configure memory region 10. */
    Qmss_MemRegion_MEMORY_REGION10,
    /** Configure memory region 11. */
    Qmss_MemRegion_MEMORY_REGION11,
    /** Configure memory region 12. */
    Qmss_MemRegion_MEMORY_REGION12,
    /** Configure memory region 13. */
    Qmss_MemRegion_MEMORY_REGION13,
    /** Configure memory region 14. */
    Qmss_MemRegion_MEMORY_REGION14,
    /** Configure memory region 15. */
    Qmss_MemRegion_MEMORY_REGION15,
    /** Configure memory region 16. */
    Qmss_MemRegion_MEMORY_REGION16,
    /** Configure memory region 17. */
    Qmss_MemRegion_MEMORY_REGION17,
    /** Configure memory region 18. */
    Qmss_MemRegion_MEMORY_REGION18,
    /** Configure memory region 19. */
    Qmss_MemRegion_MEMORY_REGION19
}Qmss_MemRegion;


typedef enum
{
    /** PDSP 1 */
    Qmss_PdspId_PDSP1 = 0,
    /** PDSP 2 */
    Qmss_PdspId_PDSP2
}Qmss_PdspId;


typedef enum
{
    /** signed interrupt generated for the high priority accumulator.
     * 32 interrupts are generated in response to events in the 32 high-priority queues.
     */
    Qmss_IntdInterruptType_HIGH = 0,
    /** signed interrupt generated for the low priority accumulator.
     * 16 interrupts are generated in response to events in the 512 low-priority queues.
     */
    Qmss_IntdInterruptType_LOW,
    /** signed interrupt generated for QMSS CDMA.
     * 2 interrupts are generated for buffer descriptor starvation event on 
     * receive SOP (start of packet) and MOP (middle of packet) for any of the receive DMA units in the CDMA.
     */
    Qmss_IntdInterruptType_CDMA
}Qmss_IntdInterruptType;


typedef struct
{
    /** Queue manager number */
    signed int qMgr;        
    /** Queue number within Queue Manager */
    signed int qNum;
}Qmss_Queue;

/** 
 * @brief Queue definition
 */
typedef struct
{
    /** Queue manager number */
    signed int     startIndex;        
    /** Queue number within Queue Manager */
    signed int     maxNum;
}Qmss_QueueNumRange;

/** 
 * @brief descriptor configuration structure
 */
typedef struct 
{
    unsigned int          memRegion;
    /** Number of descriptors that should be allocated */
    unsigned int          descNum;
    /** Queue where the descriptor is stored. If QueueNum is set to QMSS_PARAM_NOT_SPECIFIED then the next 
     * available queue of type Qmss_QueueType will be allocated */
    signed int           destQueueNum;
    /** If QueueNum is set to QMSS_PARAM_NOT_SPECIFIED then the next available queue of type 
     * Qmss_QueueType will be allocated */
    Qmss_QueueType  queueType;
}Qmss_DescCfg;

typedef struct 
{
    /** The base address of descriptor region. Note the 
     * descriptor Base address must be specified in ascending memory order
     * */
    unsigned int          *descBase;
    /** Size of each descriptor in the memory region. Must be a multiple of 16 */
    unsigned int          descSize;
    /** Number of descriptors in the memory region. 
     * Must be a minimum of 32. 
     * Must be 2^(5 or greater) 
     * Maximum supported value 2^20
     * */
    unsigned int          descNum;

    /** Memory Region corresponding to the descriptor. 
     * At init time this field must have a valid memory region 
     * index (0 to Maximum number of memory regions supported).
     *
     * At runtime this field is used to either 
     *      * set to Qmss_MemRegion_MEMORY_REGION_NOT_SPECIFIED, in this case the LLD 
     *      * will decide which memory region to use.
     *      * OR 
     *      * specify the descriptor memory region, must be a valid memory 
     *      * region index (0 to Maximum number of memory regions supported).
     */
    Qmss_MemRegion  memRegion;
    /** Flag control whether the descriptors are managed 
     * by LLD or by the caller allocating descriptor memory */
    Qmss_ManageDesc manageDescFlag;
    /** Used to leave holes by configuring dummy regions which can be later 
     * configured with actual values. Must be calculated and a correct startIndex must be 
     * specified if memRegion value is valid (0 to Maximum number of memory regions supported). */
    unsigned int          startIndex;
} Qmss_MemRegInfo;


typedef struct 
{
    /** Descriptor information for each CPDMA passed during cppi_Init */
    Qmss_MemRegInfo memRegInfo[QMSS_MAX_MEM_REGIONS];
    /** Current descriptor count. Sum of descriptors in all memory regions */
    unsigned int          currDescCnt;
} Qmss_MemRegCfg;


typedef struct
{
    /** ID of the PDSP to download this firmware to */
    Qmss_PdspId     pdspId;
    /** Pointer to the firmware image, If the firmware pointer is NULL, LLD will not 
     * download the firmware */
    void            *firmware;
    /** Size of firmware in bytes */
    unsigned int          size; 
}Qmss_PdspCfg;


typedef struct
{
    /** Base address of Linking RAM 0. LLD will configure linking RAM0 address to internal linking RAM 
     * address if a value of zero is specified. */
    unsigned int          linkingRAM0Base;
    /** Linking RAM 0 Size. LLD will configure linking RAM0 size to maximum internal linking RAM 
     * size if a value of zero is specified*/
    unsigned int          linkingRAM0Size; 
    /** Base address of Linking RAM 1. Depends on RAM 0 Size and total number of 
     * descriptors. If linkingRAM1Base is zero then linkingRAM0Size must be large 
     * enough to store all descriptors in the system */
    unsigned int          linkingRAM1Base;
    /** Maximum number of descriptors in the system. Should be equal to less than 
     * the RAM0+RAM1 size */
    unsigned int          maxDescNum;
    /** PDSP firmware to download. If the firmware pointer is NULL, LLD will not download the firmware */
    Qmss_PdspCfg    pdspFirmware[QMSS_MAX_PDSP];
}Qmss_InitCfg;


typedef struct
{
    /** Maximum number of queue Managers */
    unsigned int                                maxQueMgr;
    /** Maximum number of queues */
    unsigned int                                maxQue;

    /** Queue start index and maximum number of queues of each queue type */
    Qmss_QueueNumRange                      maxQueueNum[25];

    /** Base address for the CPDMA overlay registers */

    /** QM Global Config registers */
    CSL_Qm_configRegs                       *qmConfigReg;
    /** QM Descriptor Config registers */
    CSL_Qm_descriptor_region_configRegs     *qmDescReg;
    /** QM queue Management registers, accessed via CFG port */
    CSL_Qm_queue_managementRegs             *qmQueMgmtReg;
    /** QM queue Management Proxy registers, accessed via CFG port */
    CSL_Qm_queue_managementRegs             *qmQueMgmtProxyReg;
    /** QM queue status registers */
    CSL_Qm_queue_status_configRegs          *qmQueStatReg;
    /** QM INTD registers */
    CSL_Qm_intdRegs                         *qmQueIntdReg;
    /** QM PDSP command register */
    volatile unsigned int                       *qmPdspCmdReg[QMSS_MAX_PDSP];
    /** QM PDSP control register */
    CSL_PdspRegs                            *qmPdspCtrlReg[QMSS_MAX_PDSP];
    /** QM PDSP IRAM register */
    volatile unsigned int                       *qmPdspIRamReg[QMSS_MAX_PDSP];
    /** QM Status RAM */
    CSL_Qm_Queue_Status                     *qmStatusRAM;
    /** QM Linking RAM register */
    volatile unsigned int                       *qmLinkingRAMReg;
    /** QM McDMA register */
    CSL_McdmaRegs                           *qmMcDMAReg;
    /** QM Timer16 register */
    CSL_Cp_timer16Regs                      *qmTimer16Reg[2];
    /** QM queue Management registers, accessed via DMA port */
    CSL_Qm_queue_managementRegs             *qmQueMgmtDataReg;
    /** QM queue Management Proxy registers, accessed via DMA port */
    CSL_Qm_queue_managementRegs             *qmQueMgmtProxyDataReg;

}Qmss_GlobalConfigParams;


typedef signed int   Qmss_QueueHnd;

/** 
 * @brief QMSS return result
 */
typedef signed int   Qmss_Result;


typedef unsigned int* Qmss_QueuePushHnd;



/* Exported functions */
extern Qmss_Result Qmss_init (Qmss_InitCfg *initCfg, Qmss_GlobalConfigParams *qmssGblCfgParams);
extern Qmss_Result Qmss_start (void);
extern Qmss_Result Qmss_getMemoryRegionCfg (Qmss_MemRegCfg *memRegInfo);
extern Qmss_Result Qmss_insertMemoryRegion (Qmss_MemRegInfo *memRegCfg);
extern Qmss_QueueHnd Qmss_initDescriptor (Qmss_DescCfg *descCfg, unsigned int *numAllocated);
extern Qmss_QueueHnd Qmss_queueOpen (Qmss_QueueType queType, signed int queNum, unsigned char *isAllocated);
extern Qmss_QueueHnd Qmss_queueOpenInRange (unsigned int startQueNum, unsigned int endQueNum, unsigned char *isAllocated);
extern Qmss_Result Qmss_queueClose (Qmss_QueueHnd hnd);
extern unsigned int Qmss_getQueueThreshold (Qmss_QueueHnd hnd);
extern void Qmss_setQueueThreshold (Qmss_QueueHnd hnd, unsigned short hilo, unsigned char threshold);
extern unsigned int Qmss_getStarvationCount (Qmss_QueueHnd hnd);
extern unsigned short Qmss_getQueueThresholdStatus (Qmss_QueueHnd hnd);
extern Qmss_Queue Qmss_getQueueNumber (Qmss_QueueHnd hnd);
extern Qmss_QueueHnd Qmss_getQueueHandle (Qmss_Queue queue);
extern unsigned int Qmss_getMemRegDescSize (unsigned int memRegion);
extern Qmss_Result Qmss_downloadFirmware (Qmss_PdspId pdspId, void *image, unsigned int size);
extern Qmss_Result Qmss_setEoiVector (Qmss_IntdInterruptType type, unsigned char interruptNum);
extern Qmss_Result Qmss_ackInterrupt (unsigned char interruptNum, unsigned char value);
extern unsigned int Qmss_getVersion (void);
extern const char* Qmss_getVersionStr (void);
 
#ifdef __cplusplus
}
#endif

#endif /* QMSS_QM_H_ */

