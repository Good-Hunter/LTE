


#ifndef QMSS_QOS_H_
#define QMSS_QOS_H_

#ifdef __cplusplus
extern "C" {
#endif

/* QMSS LLD includes */
#include "qmss_drv.h"


#define QMSS_QOS_PDSP_NUM                           1
#define QMSS_QOS_MAX_CLUSTERS                       8
#define QMSS_QOS_MAX_QUEUES                         32
#define QMSS_QOS_MAX_QUE_PER_CLUSTER_COUNT          9

/** QMSS QoS commands */
#define QMSS_QCMD_GET_QUEUE_BASE                    0x80
#define QMSS_QCMD_SET_QUEUE_BASE                    0x81
#define QMSS_QCMD_TIMER_CONFIG                      0x82
#define QMSS_QCMD_ENABLE_CLUSTER                    0x83

/** QMSS QoS return and Error Codes */
/** QMSS QoS successful return code */
#define QCMD_RETCODE_SUCCESS                        1
/** QMSS QoS invalid command return code */
#define QMSS_QCMD_INVALID_COMMAND                   2
/** QMSS QoS invalid index return code */
#define QMSS_QCMD_INVALID_INDEX                     3
/** QMSS QoS invalid option return code */
#define QMSS_QCMD_INVALID_OPTION                    4

/**
@}
*/

/** @addtogroup QMSS_LLD_DATASTRUCT
@{ 
*/


typedef struct
{
    /** Queue manger and Queue index of the forwarding queue */
	unsigned short              egressQueNum;
    /** The amount of forwarding byte credit that the queue receives every 25us */
    unsigned short              iterationCredit;
    /** The maximum amount of forwarding byte credit that the queue is allowed to 
     * hold at the end of the timer iteration. Any credit over the maximum limit 
     * is added to a global pool */
    unsigned int              maxCredit;
    /** The size in bytes at which point the QOS queue is considered to be congested */
    unsigned int              congestionThreshold;
} Qmss_QosQueueCfg;


typedef struct
{
    /** The maximum amount of global credit allowed to carry over to the next queue. 
     * Excess global credit is discarded */       
    unsigned int              maxGlobalCredit;
    /** The number of QOS queues in this cluster. Valid range is 1 to QMSS_QOS_MAX_QUE_PER_CLUSTER_COUNT */
    unsigned char               qosQueCnt;
    /** The queue index (0 to 31) of each QOS queue in the cluster listed 
     * in priority order. These queue indices are relative to the configured QOS 
     * queue base index */
    unsigned char               qosQueNum[QMSS_QOS_MAX_QUE_PER_CLUSTER_COUNT];
    /** This 9-bit mask contains 1 bit for each QOS queue in the cluster. 
     * When this bit is set for its corresponding QOS queue, iteration credit is treated 
     * as "real time" scheduling and does not scale when the egress queue become congested */
    unsigned short              qosQueRTFlags;
    /** The total number of egress queues sampled to obtain the egress queue congestion estimation. 
     * Valid range is 1 to QMSS_QOS_MAX_QUE_PER_CLUSTER_COUNT */
    unsigned char               egressQueCnt;
    /** The Queue manger and Queue index of every egress queue enumerated in Egress Queue Count. 
     * These queue indices are absolute index values */
    Qmss_Queue          egressQueNum[QMSS_QOS_MAX_QUE_PER_CLUSTER_COUNT];
    /** Each QOS cluster is configured with four egress congestion threshold values. 
     * Iteration credit is assigned to each queue in the cluster depending on the egress 
     * congestion, and the value of these four congestion thresholds. 
     *
     * It is implemented as shown below:
     *
     * Egress Queue Congestion (Backlog) Level  |   QOS Queue Credit Assigned
     * From no backlog to Threshold 1	        |   Double credit
     * Between Threshold 1 and Threshold 2	    |   Normal credit
     * Between Threshold 2 and Threshold 3	    |   Half credit
     * Between Threshold 3 and Threshold 4	    |   Quarter credit
     * Over Threshold 4	                        |   No credit
     * 
     */ 
    /** Egress Congestion Threshold point 1 */ 
    unsigned int              egressCongestionThreshold1;
    /** Egress Congestion Threshold point 2 */ 
    unsigned int              egressCongestionThreshold2;
    /** Egress Congestion Threshold point 3 */ 
    unsigned int              egressCongestionThreshold3;
    /** Egress Congestion Threshold point 4 */ 
    unsigned int              egressCongestionThreshold4;
} Qmss_QosClusterCfg;


extern Qmss_Result Qmss_setQosQueueBase (unsigned int queueNum);
extern Qmss_Result Qmss_getQosQueueBase (unsigned int *queueNum);
extern Qmss_Result Qmss_configureQosTimer (unsigned int timerConstant);
extern Qmss_Result Qmss_enableQosCluster (unsigned int clusterIndex);
extern Qmss_Result Qmss_disableQosCluster (unsigned int clusterIndex);
extern Qmss_Result Qmss_configureQosQueue (unsigned int queIndex, Qmss_QosQueueCfg *cfg);
extern Qmss_Result Qmss_configureQosCluster (unsigned int clusterIndex, Qmss_QosClusterCfg *cfg);
extern Qmss_Result Qmss_getQosQueueForwardPktStats (unsigned int queueIndex);
extern Qmss_Result Qmss_getQosQueueDroppedPktStats (unsigned int queueIndex);
extern Qmss_Result Qmss_resetQosQueueStats (unsigned int queueIndex);

#ifdef __cplusplus
}
#endif

#endif /* QMSS_QOS_H_ */

