
#include <stdio.h>
#include "csl_qm_queue.h"
#include "SRIO_test.h"
#include "SRIO_PktDMA_Init.h"
#include "SRIO_debug.h"
#include "TSC.h"
#include "pl_comm.h"
#include "system_init.h"
#include <SRIO_drv.h>   //cs 14/11/06
/*Please note, Turbo Nyquist EVM only connects port 2 and 3 between 2 DSPs.*/
#pragma DATA_SECTION(output_symbol, ".shareddata")
volatile int output_symbol[2048+2+160+4];
#pragma DATA_SECTION(output_totalsymbol, ".shareddata")
unsigned int output_totalsymbol[14][2048+160+4+2];
extern SRIO_Trans_Config transParameter[];    //cs 14/11/06



SRIO_Multiple_Test_Config test_2DSP_cfg=
{
    SRIO_PATH_CTL_1xLaneA_1xLaneB_1xLaneC_1xLaneD, /*multiple_port_path*/
    /*packet_type               source                                                   dest                                                size*/
    {{SRIO_PKT_TYPE_SWRITE,     ((Uint32)&packetBuffer_LL2_Size1[0][0])+0x10000000,     ((Uint32)&packetBuffer_LL2_Size1[1][0])+0x10000000, LL2_PACKET_BUFFER_SIZE1},   /*port0 is not availible for this case*/
    {SRIO_PKT_TYPE_SWRITE,     ((Uint32)&packetBuffer_DDR_Size1[0][0]),     ((Uint32)&packetBuffer_DDR_Size1[1][0]), DDR_PACKET_BUFFER_SIZE1},   /*port1 is not availible for this case*/
    {SRIO_PKT_TYPE_SWRITE,     ((Uint32)vxx_layermapping_fft_data.fft_output_data), 0x81ac4800, 4 * (2048+160)},   /*port2*/
    {SRIO_PKT_TYPE_SWRITE,      ((Uint32)&packetBuffer_DDR_Size1[2][0]),     ((Uint32)&packetBuffer_DDR_Size1[3][0]), DDR_PACKET_BUFFER_SIZE1}}    /*port3*/
};
SRIO_Multiple_Test_Config test_2DSP_cell_cfg=
{
    SRIO_PATH_CTL_1xLaneA_1xLaneB_1xLaneC_1xLaneD, /*multiple_port_path*/
    /*packet_type               source                                                   dest                                                size*/
    {{SRIO_PKT_TYPE_SWRITE,     ((Uint32)&packetBuffer_LL2_Size1[0][0])+0x10000000,     ((Uint32)&packetBuffer_LL2_Size1[1][0])+0x10000000, LL2_PACKET_BUFFER_SIZE1},   /*port0 is not availible for this case*/
    {SRIO_PKT_TYPE_SWRITE,     ((Uint32)&packetBuffer_DDR_Size1[0][0]),     ((Uint32)&packetBuffer_DDR_Size1[1][0]), DDR_PACKET_BUFFER_SIZE1},   /*port1 is not availible for this case*/
    {SRIO_PKT_TYPE_SWRITE,     ((Uint32)&vxx_config_para_init)+0x10000000, 0x1082fd50, 64},   /*小于1024的时候必须是2的幂*/
    {SRIO_PKT_TYPE_SWRITE,      ((Uint32)&packetBuffer_DDR_Size1[2][0]),     ((Uint32)&packetBuffer_DDR_Size1[3][0]), DDR_PACKET_BUFFER_SIZE1}}    /*port3*/
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
#if 1
#endif
void fxx_srio_send(unsigned char slot_index,int port_num)
{
	    int j;
		SRIO_Transfer_Param * transferParam;
		SRIO_LSU_Transfer lsuTransfer[4];
		Int32 uiCompletionCode[4];
		Uint32 cycles[4],cycles1;
		Uint32 uiFreeQueueEntryCount;
		Uint32 * uipSrc[4], * uipDst[4], uiByteCount[4], uiDestID[4];
		SRIO_Packet_Type packet_type[4];
		HostPacketDescriptor * hostDescriptor;




		for(j= 2; j< 3; j++)
		{
			transferParam= &test_2DSP_cfg.transfer_param[j];
			uiByteCount[j]= transferParam->byteCount;
			if(0==uiByteCount[j])
				continue;

			packet_type[j]= transferParam->packet_type;
			if(packet_type[j]<0x90) 	//directIO
			{


				uipSrc[j] = (Uint32 *)transferParam->source;
				uipDst[j]= (Uint32 *)transferParam->dest;

				/*initialize buffer*/
				//InitDataBuffer(uipSrc[j], uipDst[j], uiByteCount[j]);
				InitLsuTransfer(&lsuTransfer[j], transferParam, j,
								DSP1_SRIO_BASE_ID + j, j);



				/*setup a doorbell after each DirectIO operation
				to notify the other side of SRIO*/
				if(port_num==1)
					lsuTransfer[j].doorbellValid= 1;
				else
					lsuTransfer[j].doorbellValid= 0;

				if(j == 2)
				lsuTransfer[j].doorbellInfo =slot_index >> 1;
				else
				lsuTransfer[j].doorbellInfo = INVALID_DOORBELL;

				Keystone_SRIO_LSU_transfer(&lsuTransfer[j]);

	            cycles[j]= TSCL;
				uiCompletionCode[j]= Keystone_SRIO_wait_LSU_completion(j,
					lsuTransfer[j].transactionID, lsuTransfer[j].contextBit);
				cycles[j]= TSC_delay_cycle(cycles[j]);



	        }

		}
#if 0
		for(j= 2; j< 4; j++)
		{
			transferParam= &test_2DSP_cfg.transfer_param[j];
			uiByteCount[j]= transferParam->byteCount;
			if(0==uiByteCount[j])
				continue;

			packet_type[j]= transferParam->packet_type;
			if(packet_type[j]<0x90) 	//directIO
			{



				  Keystone_SRIO_LSU_transfer(&lsuTransfer[j]);




	        }

		}
		    for(j= 2; j< 4; j++)
			{
				transferParam= &test_2DSP_cfg.transfer_param[j];

				packet_type[j]= transferParam->packet_type;
				if(packet_type[j]<0x90) 	//directIO
				{


					            cycles[j]= TSCL;
								uiCompletionCode[j]= Keystone_SRIO_wait_LSU_completion(j,
									lsuTransfer[j].transactionID, lsuTransfer[j].contextBit);
								cycles[j]= TSC_delay_cycle(cycles[j]);
								//printf("%s from 0x%8x to 0x%8x, %6d bytes, %6d cycles,%5lld Mbps,completion code = %d\n",
								//	get_packet_type_string(transferParam->packet_type),
								//	uipSrc[j], uipDst[j], uiByteCount[j],cycles[j],(unsigned long long)uiByteCount[j]*8*CPU_CLK_KHZ/cycles[j]/1000,
								//	uiCompletionCode[j]);



		        }

			}
#endif

}
void fxx_srio_send_config(unsigned char slot_index)
{
	    int j;
		SRIO_Transfer_Param * transferParam;
		SRIO_LSU_Transfer lsuTransfer[4];
		Int32 uiCompletionCode[4];
		Uint32 cycles[4],cycles1;
		Uint32 uiFreeQueueEntryCount;
		Uint32 * uipSrc[4], * uipDst[4], uiByteCount[4], uiDestID[4];
		SRIO_Packet_Type packet_type[4];
		HostPacketDescriptor * hostDescriptor;




		for(j= 2; j< 4; j++)
		{
			transferParam= &test_2DSP_cell_cfg.transfer_param[j];
			uiByteCount[j]= transferParam->byteCount;
			if(0==uiByteCount[j])
				continue;

			packet_type[j]= transferParam->packet_type;
			if(packet_type[j]<0x90) 	//directIO
			{


				uipSrc[j] = (Uint32 *)transferParam->source;
				uipDst[j]= (Uint32 *)transferParam->dest;

				/*initialize buffer*/
				//InitDataBuffer(uipSrc[j], uipDst[j], uiByteCount[j]);
				InitLsuTransfer(&lsuTransfer[j], transferParam, j,
								DSP1_SRIO_BASE_ID + j, j);



				/*setup a doorbell after each DirectIO operation
				to notify the other side of SRIO*/
				lsuTransfer[j].doorbellValid= 1;
				if(j == 2)
				lsuTransfer[j].doorbellInfo = CONFIG_DOORBELL;
				else
				lsuTransfer[j].doorbellInfo = INVALID_DOORBELL;




	        }

		}
		for(j= 2; j< 4; j++)
		{
			transferParam= &test_2DSP_cell_cfg.transfer_param[j];
			uiByteCount[j]= transferParam->byteCount;
			if(0==uiByteCount[j])
				continue;

			packet_type[j]= transferParam->packet_type;
			if(packet_type[j]<0x90) 	//directIO
			{



				  Keystone_SRIO_LSU_transfer(&lsuTransfer[j]);




	        }

		}
		    for(j= 2; j< 4; j++)
			{
				transferParam= &test_2DSP_cell_cfg.transfer_param[j];

				packet_type[j]= transferParam->packet_type;
				if(packet_type[j]<0x90) 	//directIO
				{


					            cycles[j]= TSCL;
								uiCompletionCode[j]= Keystone_SRIO_wait_LSU_completion(j,
									lsuTransfer[j].transactionID, lsuTransfer[j].contextBit);
								cycles[j]= TSC_delay_cycle(cycles[j]);
								//printf("%s from 0x%8x to 0x%8x, %6d bytes, %6d cycles,%5lld Mbps,completion code = %d\n",
								//	get_packet_type_string(transferParam->packet_type),
								//	uipSrc[j], uipDst[j], uiByteCount[j],cycles[j],(unsigned long long)uiByteCount[j]*8*CPU_CLK_KHZ/cycles[j]/1000,
								//	uiCompletionCode[j]);



		        }

			}

}

	void cfg_change(int ii,int port_num,int slot)//mjt 2014.10.17 修改每次发送的源地址与目标地址
	{

		//transParameter[].local_addr
		//transParameter[].remote_addrL
		//transParameter[].byte_count
		if(port_num==0)//端口号为0,下一次发送切换到端口号为1的数据
		{
			transParameter[0].local_addr=transParameter[0].local_addr+0x1E000;//0x2290;//每次发送在接收端空出偏移位
			transParameter[0].remote_addrL = transParameter[0].remote_addrL+0x1E000;//0x2290;//每次发送在接收端空出偏移位
			if(ii%7==0)
				transParameter[0].byte_count = 4*(2048+160);
			else
				transParameter[0].byte_count = 4*(2048+144);
		}
		else if(port_num==1)//端口号为1,下一次切换到端口号为0的下一个符号的据发送
		{
			if(ii%7==0)
			{
				transParameter[0].local_addr=transParameter[0].local_addr-0x1E000+0x2280;//0x2290;//每次发送在接收端空出偏移位
				transParameter[0].remote_addrL=transParameter[0].remote_addrL-0x1E000+0x2280;//0x2290;//每次发送在接收端空出偏移位
				transParameter[0].byte_count = 4*(2048+144);
			}
			else if(ii!=13)
			{
				transParameter[0].local_addr=transParameter[0].local_addr-0x1E000+0x2240;//0x2290;//每次发送在接收端空出偏移位
				transParameter[0].remote_addrL=transParameter[0].remote_addrL-0x1E000+0x2240;//0x2290;//每次发送在接收端空出偏移位
				if(ii!=6)
					transParameter[0].byte_count = 4*(2048+144);
				else
					transParameter[0].byte_count = 4*(2048+160);
			}
			else//发送符号为13,下一次发送要从头开始
			{
				//transParameter[0].local_addr=(Uint32)vxx_layermapping_fft_data.fft_output_data;//0x2290;//每次发送在接收端空出偏移位
				transParameter[0].remote_addrL=transParameter[0].remote_addrL+0x2240;//0x2290;//每次发送在接收端空出偏移位
				transParameter[0].byte_count = 4*(2048+160);
			}
			if(slot==2||slot==12)//特殊子帧传输10次后偏移
			{
				if(ii==9)
				{
					//transParameter[0].local_addr=(Uint32)vxx_layermapping_fft_data.fft_output_data;//0x2290;//每次发送在接收端空出偏移位
					transParameter[0].remote_addrL=transParameter[0].remote_addrL+0x2240*4+0x1E000;//0x2290;//每次发送在接收端空出偏移位
					transParameter[0].byte_count = 4*(2048+160);
				}
			}


		}


	}
