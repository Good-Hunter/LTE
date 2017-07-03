

#ifndef QMSS_MGMT_H_
#define QMSS_MGMT_H_

#ifdef __cplusplus
extern "C" {
#endif

/* QMSS LLD includes */
#include "qmss_qm.h"
#include "qmss_pvt.h"
        
/* QMSS OSAL layer */
#include "qmss_osal.h"


/**********************************************************************
 ************************** Externs *********************************
 **********************************************************************/
        
/* QMSS Local object */
extern Qmss_GlobalConfigParams  qmssLObj;


static inline void Qmss_queuePush (Qmss_QueueHnd hnd, void *descAddr, unsigned int packetSize, unsigned int descSize, Qmss_Location location)
{
    unsigned int            regc = 0, regd = 0;
    unsigned long long            dWord = 0;
    volatile unsigned long long   *regCregDPtr;
    void                *key;
 
    CSL_FINS (regc, QM_QUEUE_MANAGEMENT_QUEUE_REG_C_HEAD_TAIL, location);
    
    CSL_FINS (regc, QM_QUEUE_MANAGEMENT_QUEUE_REG_C_PACKET_SIZE, packetSize);
   
    regd = ((unsigned int) descAddr | ((descSize >> 4) - 1));

#ifdef _BIG_ENDIAN
    dWord = _itoll (regc, regd);
#else
    dWord = _itoll (regd, regc);
#endif

    /* Begin Critical Section before accessing shared resources. */
    key = Qmss_osalMtCsEnter ();

    regCregDPtr = (volatile unsigned long long *) (& qmssLObj.qmQueMgmtDataReg->QUEUE_MGMT_GROUP[hnd].QUEUE_REG_C);
    *regCregDPtr = dWord;
    
    /* End Critical Section */   
    Qmss_osalMtCsExit (key);
    return;
}


static inline void Qmss_queuePushDesc (Qmss_QueueHnd hnd, void *descAddr)
{
    qmssLObj.qmQueMgmtDataReg->QUEUE_MGMT_GROUP[hnd].QUEUE_REG_D = (unsigned int) descAddr;
    return;
}


static inline void Qmss_queuePushDescSize (Qmss_QueueHnd hnd, void *descAddr, unsigned int descSize)
{
    qmssLObj.qmQueMgmtDataReg->QUEUE_MGMT_GROUP[hnd].QUEUE_REG_D = ((unsigned int) descAddr | ((descSize >> 4) - 1));
    return;
}


static inline void* Qmss_queuePop (Qmss_QueueHnd hnd)
{
    return (void *) qmssLObj.qmQueMgmtReg->QUEUE_MGMT_GROUP[hnd].QUEUE_REG_D;
}


static inline void Qmss_queuePopDescSize (Qmss_QueueHnd hnd, void **descAddr, unsigned int *packetSize)
{
    *packetSize = CSL_FEXT (qmssLObj.qmQueStatReg->QUEUE_STATUS_CONFIG_GROUP[hnd].QUEUE_STATUS_CONFIG_REG_C, 
                    QM_QUEUE_STATUS_CONFIG_QUEUE_STATUS_CONFIG_REG_C_PACKET_SIZE); 
    *descAddr = (void *) qmssLObj.qmQueMgmtReg->QUEUE_MGMT_GROUP[hnd].QUEUE_REG_D;

    return;
}


static inline Qmss_QueuePushHnd Qmss_getQueuePushHandle (Qmss_QueueHnd hnd)
{
    return ((Qmss_QueuePushHnd) &(qmssLObj.qmQueMgmtDataReg->QUEUE_MGMT_GROUP[hnd].QUEUE_REG_D));
}


static inline void Qmss_queueDivert (Qmss_QueueHnd srcQnum, Qmss_QueueHnd dstQnum, Qmss_Location location)
{
    unsigned int  temp = 0;
    
    CSL_FINS (temp, QM_CONFIG_QUEUE_DIVERSION_REG_SOURCE_QNUM, srcQnum);
    CSL_FINS (temp, QM_CONFIG_QUEUE_DIVERSION_REG_DEST_QNUM, dstQnum);
    CSL_FINS (temp, QM_CONFIG_QUEUE_DIVERSION_REG_HEAD_TAIL, location);

    qmssLObj.qmConfigReg->QUEUE_DIVERSION_REG = temp;
    return; 
}


static inline void Qmss_queueEmpty (Qmss_QueueHnd hnd)
{
    qmssLObj.qmQueMgmtReg->QUEUE_MGMT_GROUP[hnd].QUEUE_REG_D = (unsigned int) 0x0;
    return;
}


static inline unsigned int Qmss_getQueueEntryCount (Qmss_QueueHnd hnd)
{
    return (unsigned int) CSL_FEXT (qmssLObj.qmQueStatReg->QUEUE_STATUS_CONFIG_GROUP[hnd].QUEUE_STATUS_CONFIG_REG_A, 
                    QM_QUEUE_STATUS_CONFIG_QUEUE_STATUS_CONFIG_REG_A_QUEUE_ENTRY_COUNT); 
}


static inline unsigned int Qmss_getQueueByteCount (Qmss_QueueHnd hnd)
{
    return (unsigned int) CSL_FEXT (qmssLObj.qmQueStatReg->QUEUE_STATUS_CONFIG_GROUP[hnd].QUEUE_STATUS_CONFIG_REG_B, 
                    QM_QUEUE_STATUS_CONFIG_QUEUE_STATUS_CONFIG_REG_B_QUEUE_BYTE_COUNT); 
}


static inline unsigned int Qmss_getQueuePacketSize (Qmss_QueueHnd hnd)
{
    return (unsigned int) CSL_FEXT (qmssLObj.qmQueStatReg->QUEUE_STATUS_CONFIG_GROUP[hnd].QUEUE_STATUS_CONFIG_REG_C, 
                    QM_QUEUE_STATUS_CONFIG_QUEUE_STATUS_CONFIG_REG_C_PACKET_SIZE); 
}

/** 
@} 
*/


#ifdef __cplusplus
}
#endif

#endif /* QMSS_MGMT_H_ */

