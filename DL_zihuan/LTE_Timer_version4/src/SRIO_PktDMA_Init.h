
#ifndef _SRIO_PKTDMA_INIT_H_
#define _SRIO_PKTDMA_INIT_H_

#include "KeyStone_Multicore_Navigator_drv.h"

/*size of descriptor must be multiple of 16B*/
#define SRIO_DESCRIPTOR_SIZE 		64

/*the linking RAM entry size for each descriptor region 
must be power of 2, and no less than 32.
Actual descriptor number may less than linking entry size, this may 
leave hole in linking RAM*/
#define SL2_HOST_DESCRIPTOR_NUM 		(40)
#define SL2_HOST_DESC_LINK_ENTRY_SIZE 	(64)
#define LL2_HOST_DESCRIPTOR_NUM 		(40)
#define LL2_HOST_DESC_LINK_ENTRY_SIZE 	(64)
#define DDR_HOST_DESCRIPTOR_NUM 		(80)
#define DDR_HOST_DESC_LINK_ENTRY_SIZE 	(128)

#define SL2_PACKET_BUFFER_SIZE0 	(256)
#define SL2_PACKET_BUFFER_SIZE1 	(32*1024)
#define SL2_SIZE0_PKT_BUF_NUM 		(32)
#define SL2_SIZE1_PKT_BUF_NUM 		(8)
#define SL2_SIZE1_DESC_START_IDX 	(SL2_SIZE0_PKT_BUF_NUM)

#define LL2_PACKET_BUFFER_SIZE0 	(256)
#define LL2_PACKET_BUFFER_SIZE1 	(16*1024)
#define LL2_SIZE0_PKT_BUF_NUM 		(32)
#define LL2_SIZE1_PKT_BUF_NUM 		(8)
#define LL2_SIZE1_DESC_START_IDX 	(LL2_SIZE0_PKT_BUF_NUM)

#define DDR_PACKET_BUFFER_SIZE0 	(240*1024)
#define DDR_PACKET_BUFFER_SIZE1 	(240*1024)
#define DDR_SIZE0_PKT_BUF_NUM 		(64)
#define DDR_SIZE1_PKT_BUF_NUM 		(16)
#define DDR_SIZE1_DESC_START_IDX 	(DDR_SIZE0_PKT_BUF_NUM)

#define TEST_QUEUE_BASE 			2048

#define SL2_HOST_SIZE0_FDQ          (TEST_QUEUE_BASE+0 )
#define SL2_HOST_SIZE1_FDQ          (TEST_QUEUE_BASE+1 )
#define DDR_HOST_SIZE0_FDQ          (TEST_QUEUE_BASE+4 )
#define DDR_HOST_SIZE1_FDQ          (TEST_QUEUE_BASE+5 )
#define CORE0_LL2_HOST_SIZE0_FDQ    (TEST_QUEUE_BASE+8 )
#define CORE0_LL2_HOST_SIZE1_FDQ    (TEST_QUEUE_BASE+9 )
#define CORE1_LL2_HOST_SIZE0_FDQ    (TEST_QUEUE_BASE+12)
#define CORE1_LL2_HOST_SIZE1_FDQ    (TEST_QUEUE_BASE+13)
#define CORE2_LL2_HOST_SIZE0_FDQ    (TEST_QUEUE_BASE+16)
#define CORE2_LL2_HOST_SIZE1_FDQ    (TEST_QUEUE_BASE+17)
#define CORE3_LL2_HOST_SIZE0_FDQ    (TEST_QUEUE_BASE+20)
#define CORE3_LL2_HOST_SIZE1_FDQ    (TEST_QUEUE_BASE+21)
#define EMPTY_FDQ                   (TEST_QUEUE_BASE+24)
#define EMPTY_DST_Q                 (TEST_QUEUE_BASE+25)
#define RECLAMATION_QUEUE           (TEST_QUEUE_BASE+26)
#define SRIO_RX_SIZE_ERROR_GARBAGE_Q         (TEST_QUEUE_BASE+27)
#define SRIO_RX_TIMEOUT_GARBAGE_Q            (TEST_QUEUE_BASE+28)
#define SRIO_TX_EXCESSIVE_RETRIES_GARBAGE_Q  (TEST_QUEUE_BASE+29)
#define SRIO_TX_ERROR_GARBAGE_Q              (TEST_QUEUE_BASE+30)
#define SRIO_TX_SIZE_ERROR_GARBAGE_Q         (TEST_QUEUE_BASE+31)

/*Low priority accumulation will be used for these queue,
so, their index start at 32 boundary*/
#define SRIO_RX_FLOW0_DST_Q         (TEST_QUEUE_BASE+32)

#define SRIO_RX_FLOW_DDR 			0
#define SRIO_RX_FLOW_SL2 			1
#define SRIO_RX_FLOW_CORE0_LL2 		2
#define SRIO_RX_FLOW_CORE1_LL2 		3
#define SRIO_RX_FLOW_CORE2_LL2 		4
#define SRIO_RX_FLOW_CORE3_LL2 		5

/*Descriptor accumulation buffer*/
extern Uint32 uiaDescriptorAccumulationList[16*2];

extern Uint32 uiUsedDescRegionNum;
extern Uint32 uiInitialHostFdqNum;
extern Uint32 uiInitialMonoFdqNum;
extern Uint32 uiUsedRxFlowNum;

extern Uint8 packetBuffer_SL2_Size0[SL2_SIZE0_PKT_BUF_NUM][SL2_PACKET_BUFFER_SIZE0];
extern Uint8 packetBuffer_SL2_Size1[SL2_SIZE1_PKT_BUF_NUM][SL2_PACKET_BUFFER_SIZE1];

extern Uint8 packetBuffer_LL2_Size0[LL2_SIZE0_PKT_BUF_NUM][LL2_PACKET_BUFFER_SIZE0];
extern Uint8 packetBuffer_LL2_Size1[LL2_SIZE1_PKT_BUF_NUM][LL2_PACKET_BUFFER_SIZE1];

extern Uint8 packetBuffer_DDR_Size0[DDR_SIZE0_PKT_BUF_NUM][DDR_PACKET_BUFFER_SIZE0];
extern Uint8 packetBuffer_DDR_Size1[DDR_SIZE1_PKT_BUF_NUM][DDR_PACKET_BUFFER_SIZE1];

/*must be called after SRIO PSC enabled*/
extern void SRIO_PktDM_init();
extern void QMSS_Accumulation_config();

/*check the descriptors in the queue, for debug purpose*/
extern void SRIO_Check_Free_Queues();
extern void SRIO_Check_TxRx_Queues();
extern void SRIO_Check_Queue(Uint32 uiQIdx, Uint32 uiExpectEntryCount);

#endif
