

#ifndef QMSS_PVT_H_
#define QMSS_PVT_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "qmss_qm.h"

/* CSL RL includes */
#include "cslr_device.h"
#include "cslr_qm_config.h"
#include "cslr_qm_descriptor_region_config.h"
#include "cslr_qm_queue_management.h"
#include "cslr_qm_queue_status_config.h"
#include "cslr_qm_intd.h"
#include "cslr_pdsp.h"

/* QMSS Global object definition. */
typedef struct 
{
    /** Store the configuration structure passed during Qmss_init */
    Qmss_GlobalConfigParams             qmssGblCfgParams;
    /** Store the intialization structure passed during Qmss_init */
    Qmss_InitCfg                        initCfg;
    /** Current Memory regions configuration */
    Qmss_MemRegInfo                     memRegInfo[QMSS_MAX_MEM_REGIONS];
    /** General purpose source queue handles */
    signed int                             descQueue[QMSS_MAX_MEM_REGIONS];
    /** Current descriptor count */
    unsigned int                            currDescCnt;
}Qmss_GlobalObj;

extern signed int Qmss_getMemRegQueueHandle(unsigned int memRegion);
extern unsigned int Qmss_getMemRegDescSize (unsigned int memRegion);

#ifdef __cplusplus
}
#endif

#endif /* QMSS_PVT_H_ */

