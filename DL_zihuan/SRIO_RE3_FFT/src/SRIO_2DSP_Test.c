
#include <stdio.h>
#include "csl_qm_queue.h"
#include "SRIO_test.h"
#include "SRIO_PktDMA_Init.h"
#include "SRIO_debug.h"
#include "TSC.h"

/*Please note, Turbo Nyquist EVM only connects port 2 and 3 between 2 DSPs.*/
SRIO_Multiple_Test_Config test_2DSP_cfg_xx=
{
    SRIO_PATH_CTL_1xLaneA_1xLaneB_1xLaneC_1xLaneD, /*multiple_port_path*/
    /*packet_type               source                                                   dest                                                size*/
    {{0,                        0,                                                      0,                                                   0},   /*port0 is not availible for this case*/
    {0,                         0,                                                      0,                                                   0},   /*port1 is not availible for this case*/
    {SRIO_PKT_TYPE9_STREAM,     CORE0_LL2_HOST_SIZE1_FDQ,                               DSP1_SRIO_BASE_ID+SRIO_RX_FLOW_CORE1_LL2,            4096},   /*port2*/
    {SRIO_PKT_TYPE_SWRITE,      ((unsigned int)&packetBuffer_LL2_Size1[0][0])+0x10000000,     ((unsigned int)&packetBuffer_LL2_Size1[1][0])+0x10000000, LL2_PACKET_BUFFER_SIZE1}}    /*port3*/
};

void InitLsuDoorbell(SRIO_LSU_Transfer * lsuTransfer, 
	SRIO_Transfer_Param * transferParam)
{
	lsuTransfer->doorbellValid= 1;
	if(SRIO_PKT_TYPE_NWRITE==transferParam->packet_type)
		lsuTransfer->doorbellInfo= SRIO_DOORBELL_MSG_NREAD_DONE;
	else if(SRIO_PKT_TYPE_NWRITE==transferParam->packet_type)
		lsuTransfer->doorbellInfo= SRIO_DOORBELL_MSG_NWRITE_DONE;
	else if(SRIO_PKT_TYPE_NWRITE_R==transferParam->packet_type)
		lsuTransfer->doorbellInfo= SRIO_DOORBELL_MSG_NWRITE_R_DONE;
	else if(SRIO_PKT_TYPE_SWRITE==transferParam->packet_type)
		lsuTransfer->doorbellInfo= SRIO_DOORBELL_MSG_SWRITE_DONE;
	else
		lsuTransfer->doorbellValid= 0;
}

void fxx_srio_tx_cfg()
{
	unsigned int cycles;
	unsigned int uiFreeQueueEntryCount;
	HostPacketDescriptor * hostDescriptor;

	serdesLinkSetup.linkSpeed_GHz= SRIO_DEFAULT_TEST_SPEED;
	srio_cfg.srio_1x2x4x_path_control= SRIO_PATH_CTL_1xLaneA_1xLaneB_1xLaneC_1xLaneD;
	srio_identify_used_ports_lanes(srio_cfg.srio_1x2x4x_path_control);

	/*On Turbo Nyquist EVM only lane 2 and 3 are connected between 2 DSPs*/
	srio_cfg.blockEn.bLogic_Port_EN[0]= FALSE;
	srio_cfg.blockEn.bLogic_Port_EN[1]= FALSE;
			
	Keystone_SRIO_Init(&srio_cfg);
   	SRIO_PktDM_init();

	if(srio_cfg.blockEn.bLogic_Port_EN[2])
		Keystone_SRIO_match_ACK_ID(2, DSP1_SRIO_BASE_ID, 2);
	if(srio_cfg.blockEn.bLogic_Port_EN[3])
		Keystone_SRIO_match_ACK_ID(3, DSP1_SRIO_BASE_ID, 3);
}
void fxx_srio_send(SRIO_Tx_Config *tx_cfg)
{
	SRIO_Transfer_Param * transferParam;
	SRIO_LSU_Transfer lsuTransfer;
	unsigned int * uipSrc, * uipDst, uiByteCount, uiDestID;
	SRIO_Packet_Type packet_type;
	signed int uiCompletionCode;

	transferParam= &tx_cfg->transfer_param;
	uiByteCount= transferParam->byteCount;
	if(0==uiByteCount)
		return;

	packet_type= transferParam->packet_type;
	uipSrc = (unsigned int *)transferParam->source;
	uipDst= (unsigned int *)transferParam->dest;

	/*initialize buffer*/
	//InitDataBuffer(uipSrc, uipDst, uiByteCount);

	InitLsuTransfer(&lsuTransfer, transferParam, 3,
		DSP1_SRIO_BASE_ID, 3);

	/*setup a doorbell after each DirectIO operation
	to notify the other side of SRIO*/
	lsuTransfer.doorbellValid= 1;
    lsuTransfer.doorbellInfo= tx_cfg->doorbell_info;

	Keystone_SRIO_LSU_transfer(&lsuTransfer);
	uiCompletionCode= Keystone_SRIO_wait_LSU_completion(3,
		lsuTransfer.transactionID, lsuTransfer.contextBit);
	printf("%s from 0x%8x to 0x%8x, %6d bytes, completion code = %d\n",
		get_packet_type_string(transferParam->packet_type),
		uipSrc, uipDst, uiByteCount,
		uiCompletionCode);
}
