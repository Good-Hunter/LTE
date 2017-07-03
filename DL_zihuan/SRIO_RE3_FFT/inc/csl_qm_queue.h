
#ifndef CSL_QMSS_QUEUE_H_
#define CSL_QMSS_QUEUE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "csl.h"

/** 
 * @brief Queue start number and maximum number of each type of queue supported.
 */
#define QMSS_LOW_PRIORITY_QUEUE_BASE       	    0
#define QMSS_MAX_LOW_PRIORITY_QUEUE         	512
#define QMSS_AIF_QUEUE_BASE                     512         
#define QMSS_MAX_AIF_QUEUE                  	128    
#define QMSS_PASS_QUEUE_BASE                    640
#define QMSS_MAX_PASS_QUEUE                 	12
#define QMSS_INTC_QUEUE_BASE                    662
#define QMSS_MAX_INTC_QUEUE                 	10
#define QMSS_SRIO_QUEUE_BASE                    672
#define QMSS_MAX_SRIO_QUEUE                 	16
#define QMSS_FFTC_A_QUEUE_BASE                  688
#define QMSS_MAX_FFTC_A_QUEUE               	4
#define QMSS_FFTC_B_QUEUE_BASE                  692
#define QMSS_MAX_FFTC_B_QUEUE               	4
#define QMSS_FFTC_C_QUEUE_BASE                  864
#define QMSS_MAX_FFTC_C_QUEUE               	4
#define QMSS_BCP_QUEUE_BASE                     868
#define QMSS_MAX_BCP_QUEUE               	    8
#define QMSS_HIGH_PRIORITY_QUEUE_BASE           704
#define QMSS_MAX_HIGH_PRIORITY_QUEUE        	32
#define QMSS_STARVATION_COUNTER_QUEUE_BASE  	736
#define QMSS_MAX_STARVATION_COUNTER_QUEUE   	64
#define QMSS_INFRASTRUCTURE_QUEUE_BASE      	800
#define QMSS_MAX_INFRASTRUCTURE_QUEUE       	32
#define QMSS_TRAFFIC_SHAPING_QUEUE_BASE     	832
#define QMSS_MAX_TRAFFIC_SHAPING_QUEUE      	32
#define QMSS_GENERAL_PURPOSE_QUEUE_BASE     	876
#define QMSS_MAX_GENERAL_PURPOSE_QUEUE      	7316

/** 
 * @brief Queue Type. Specifies different queue classifications
 */
typedef enum
{
    /** Low priority queue */
    Qmss_QueueType_LOW_PRIORITY_QUEUE = 0,
    /** AIF queue */
    Qmss_QueueType_AIF_QUEUE,
    /** PASS queue */
    Qmss_QueueType_PASS_QUEUE,
    /** INTC pending queue */
    Qmss_QueueType_INTC_QUEUE,
    /** SRIO queue */
    Qmss_QueueType_SRIO_QUEUE,
    /** FFTC queue A */
    Qmss_QueueType_FFTC_A_QUEUE,
    /** FFTC queue B */
    Qmss_QueueType_FFTC_B_QUEUE,
    /** FFTC queue C */
    Qmss_QueueType_FFTC_C_QUEUE,
    /** BCP queue */
    Qmss_QueueType_BCP_QUEUE,
    /** High priority queue */
    Qmss_QueueType_HIGH_PRIORITY_QUEUE,
    /** starvation counter queue */
    Qmss_QueueType_STARVATION_COUNTER_QUEUE,
    /** Infrastructure queue */
    Qmss_QueueType_INFRASTRUCTURE_QUEUE,
    /** Traffic shaping queue */
    Qmss_QueueType_TRAFFIC_SHAPING_QUEUE,
    /** General purpose queue */
    Qmss_QueueType_GENERAL_PURPOSE_QUEUE
}Qmss_QueueType;


#ifdef __cplusplus
}
#endif

#endif /* CSL_QMSS_QUEUE_H_ */
