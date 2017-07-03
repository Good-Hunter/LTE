
#include <stdio.h>
#include "csl_qm_queue.h"
#include "SRIO_PktDMA_Init.h"
#include "SRIO_test.h"
#include "TSC.h"
#include "SRIO_debug.h"

SRIO_Packet_Type packet_type[]=
{
	SRIO_PKT_TYPE_SWRITE   ,
	//SRIO_PKT_TYPE_NWRITE   ,
    //SRIO_PKT_TYPE_NWRITE_R ,
	//SRIO_PKT_TYPE_NREAD    ,
	SRIO_PKT_TYPE9_STREAM  , 
	//SRIO_PKT_TYPE11_MESSAGE
};

Uint32 directIO_src_dest[][2]=
{
	{((Uint32)&packetBuffer_LL2_Size1[0][0])+0x10000000,	((Uint32)&packetBuffer_LL2_Size1[1][0])+0x10000000}, 	/*LL2->LL2*/
	{((Uint32)&packetBuffer_LL2_Size1[0][0])+0x10000000,	((Uint32)&packetBuffer_SL2_Size1[1][0])}, 				/*LL2->SL2*/
	{((Uint32)&packetBuffer_LL2_Size1[0][0])+0x10000000,	((Uint32)&packetBuffer_DDR_Size1[1][0])}, 				/*LL2->DDR*/
	{((Uint32)&packetBuffer_SL2_Size1[0][0]),				((Uint32)&packetBuffer_LL2_Size1[1][0])+0x10000000}, 	/*SL2->LL2*/
	{((Uint32)&packetBuffer_SL2_Size1[0][0]),				((Uint32)&packetBuffer_SL2_Size1[1][0])}, 				/*SL2->SL2*/
	{((Uint32)&packetBuffer_SL2_Size1[0][0]),				((Uint32)&packetBuffer_DDR_Size1[1][0])}, 				/*SL2->DDR*/
	{((Uint32)&packetBuffer_DDR_Size1[0][0]),				((Uint32)&packetBuffer_LL2_Size1[1][0])+0x10000000}, 	/*DDR->LL2*/
	{((Uint32)&packetBuffer_DDR_Size1[0][0]),				((Uint32)&packetBuffer_SL2_Size1[1][0])}, 				/*DDR->SL2*/
	{((Uint32)&packetBuffer_DDR_Size1[0][0]),				((Uint32)&packetBuffer_DDR_Size1[1][0])}, 				/*DDR->DDR*/
};

Uint32 message_src_dest[][2]=
{
	{CORE0_LL2_HOST_SIZE1_FDQ,	DSP0_SRIO_BASE_ID+SRIO_RX_FLOW_CORE1_LL2}, 	/*LL2->LL2*/
	{CORE0_LL2_HOST_SIZE1_FDQ,	DSP0_SRIO_BASE_ID+SRIO_RX_FLOW_SL2      }, 	/*LL2->SL2*/
	{CORE0_LL2_HOST_SIZE1_FDQ,	DSP0_SRIO_BASE_ID+SRIO_RX_FLOW_DDR      }, 	/*LL2->DDR*/
	{SL2_HOST_SIZE1_FDQ,		DSP0_SRIO_BASE_ID+SRIO_RX_FLOW_CORE1_LL2},    /*SL2->LL2*/
	{SL2_HOST_SIZE1_FDQ,		DSP0_SRIO_BASE_ID+SRIO_RX_FLOW_SL2      }, 	/*SL2->SL2*/
	{SL2_HOST_SIZE1_FDQ,		DSP0_SRIO_BASE_ID+SRIO_RX_FLOW_DDR      }, 	/*SL2->DDR*/
	{DDR_HOST_SIZE1_FDQ,		DSP0_SRIO_BASE_ID+SRIO_RX_FLOW_CORE1_LL2},   /*DDR->LL2*/
	{DDR_HOST_SIZE1_FDQ,		DSP0_SRIO_BASE_ID+SRIO_RX_FLOW_SL2      }, 	/*DDR->SL2*/
	{DDR_HOST_SIZE1_FDQ,		DSP0_SRIO_BASE_ID+SRIO_RX_FLOW_DDR      }, 	/*DDR->DDR*/
};

/*transfer through single SRIO port*/
void srio_signle_port_transfer(SRIO_Transfer_Param * transferParam,
	Uint32 uiPortNum)
{
	SRIO_LSU_Transfer lsuTransfer;
	Uint32 uiCompletionCode;
	Uint32 cycles;
	Uint32 uiQueueNum;
	volatile Uint32 * uipSrc, * uipDst;
	Uint32 uiByteCount, uiWordIndex;
	HostPacketDescriptor * hostDescriptor;
	SRIO_Type9_Message_RX_Descriptor * type9MsgRxDesc;
	SRIO_Type11_Message_RX_Descriptor * type11MsgRxDesc;
	Uint32 uiPortSpeed_Mbps= (Uint32)(serdesLinkSetup.linkSpeed_GHz*1000)*
		(srio_cfg.srio_1x2x4x_path_control>>CSL_SRIO_RIO_PLM_SP_PATH_CTL_PATH_CONFIGURATION_SHIFT);

	uiByteCount = transferParam->byteCount;

	if(SRIO_PKT_TYPE9_STREAM==transferParam->packet_type)
	{
		if(uiByteCount>64*1024)	//maximum TYPE9 transfer size
			return; 

	}
	else if(SRIO_PKT_TYPE11_MESSAGE==transferParam->packet_type)
	{
		if(uiByteCount>4*1024)	//maximum TYPE11 transfer size
			return; 
	}
	else
	{
		if(uiByteCount>1024*1024)	//maximum LSU transfer size
			return; 
	}

	if(transferParam->packet_type<0x90) 	//directIO
	{
		uipSrc = (volatile Uint32 *)transferParam->source;
		uipDst= (volatile Uint32 *)transferParam->dest;

		/*initialize buffer*/
		InitDataBuffer((Uint32 *)uipSrc, (Uint32 *)uipDst, uiByteCount);

		InitLsuTransfer(&lsuTransfer, transferParam, uiPortNum, 
			DSP0_SRIO_BASE_ID+uiPortNum, uiPortNum);
		
		cycles= TSCL;
		Keystone_SRIO_LSU_transfer(&lsuTransfer);

		uiCompletionCode= Keystone_SRIO_wait_LSU_completion(uiPortNum,
			lsuTransfer.transactionID, lsuTransfer.contextBit);

		if(0==uiCompletionCode)	//complete without error
		{
			/*wait for theoretical transfer time*/
			while(TSC_delay_cycle(cycles)<(uiByteCount*10*1000/uiPortSpeed_Mbps)+300);

			/*Since NWRITE/SWRITE complete without response, 
			we need waiting for last data landing*/
			uiWordIndex= uiByteCount/4-1;
			while(uipSrc[uiWordIndex]!=uipDst[uiWordIndex])
			{	
				Invalid((void *)&uipDst[uiWordIndex], 4);
				if(TSC_delay_cycle(cycles)>
					(uiByteCount*10*1000/uiPortSpeed_Mbps)*4+4000) 	//timeout
				{
					break;
				}
			};
		}
		cycles= TSC_delay_cycle(cycles);

	}
	else 	//message
	{
		hostDescriptor= (HostPacketDescriptor *)
			Keystone_queuePop(transferParam->source);
		if(NULL==hostDescriptor)
		{
			printf("Source queue %d descriptor is NULL\n", transferParam->source);
			SRIO_Check_Free_Queues(); 	//for debug
			return;
		}
	
		/*invalid cache before read descriptor RAM*/
		Invalid((void *)hostDescriptor, 64);

		/*initialize the source buffer*/
		uipSrc= (volatile Uint32 *)hostDescriptor->buffer_ptr;
		InitDataBuffer((Uint32 *)uipSrc, NULL, uiByteCount);
	
		if(SRIO_PKT_TYPE9_STREAM==transferParam->packet_type)
		{
			Keystone_SRIO_Build_Type9_Msg_Desc(hostDescriptor,
				DSP0_SRIO_BASE_ID, transferParam->dest, uiByteCount, 0xAAAA, 0x55);
		}
		else if(SRIO_PKT_TYPE11_MESSAGE==transferParam->packet_type)
		{
			Keystone_SRIO_Build_Type11_Msg_Desc(hostDescriptor,
				DSP0_SRIO_BASE_ID, transferParam->dest, uiByteCount, 0, 0);
		}
		else
		{
			printf("Invalid packet type %d\n", transferParam->packet_type);
			return;
		}
		/*the SRC_TAG_LO field in the Tx descriptor is used as RX flow ID*/
		hostDescriptor->src_tag_lo= 
			(transferParam->dest- DSP0_SRIO_BASE_ID);//for PktDMA loopback test

		/*write back data from cache to descriptor RAM*/
		Writeback((void *)hostDescriptor, 64);

		cycles= TSCL;
		/*push the packet descriptor to Packet DMA TX queue*/
		Keystone_queuePush(QMSS_SRIO_QUEUE_BASE+uiPortNum, 
			(Uint32)hostDescriptor|FETCH_SIZE_64);
		asm(" MFENCE"); 	//force all memory operation complete
		
		/*dest flow = destID - BASE_ID*/
		uiQueueNum= SRIO_RX_FLOW0_DST_Q+ (transferParam->dest- DSP0_SRIO_BASE_ID);
		/*poll the packet descriptor in destination queue*/	
		while(0==Keystone_GetQueueEntryCount(uiQueueNum))
		{
			if(TSC_delay_cycle(cycles)>0x3FFFFFFF) 	//timeout
			{
				printf("timeout for packet at 0x%x\n", (Uint32)uipSrc);
				SRIO_Check_TxRx_Queues(); 	//for debug
				SRIO_Check_Free_Queues(); 	//for debug
				return;
			}
		}
		cycles= TSC_delay_cycle(cycles);

		/*pop descriptor from destination queue*/
		hostDescriptor= (HostPacketDescriptor *)
			Keystone_queuePop(uiQueueNum);

		/*invalid cache before read descriptor RAM*/
		Invalid((void *)hostDescriptor, 64);

		/*get the destination buffer*/
		uipDst= (volatile Uint32 *)hostDescriptor->buffer_ptr;

		if(hostDescriptor->packet_length != uiByteCount)
		{
			printf("received byte count %d != sent byte count %d\n",
				hostDescriptor->packet_length, uiByteCount);
		}
		/*get completion code*/
		if(SRIO_PKT_TYPE9_STREAM==transferParam->packet_type)
		{
			type9MsgRxDesc= (SRIO_Type9_Message_RX_Descriptor *)
				(((Uint32)hostDescriptor)+32);
			uiCompletionCode= type9MsgRxDesc->CC;
			if(type9MsgRxDesc->Dest_ID!= transferParam->dest)
			{
				printf("received destination ID 0x%x != 0x%x\n",
					type9MsgRxDesc->Dest_ID, transferParam->dest);
			}
		}
		else if(SRIO_PKT_TYPE11_MESSAGE==transferParam->packet_type)
		{
			type11MsgRxDesc= (SRIO_Type11_Message_RX_Descriptor *)
				(((Uint32)hostDescriptor)+32);
			uiCompletionCode= type11MsgRxDesc->CC;
		}

		/*descriptor Reclamation*/
		uiQueueNum= (hostDescriptor->pkt_return_qmgr<<12)|
			hostDescriptor->pkt_return_qnum;

		
		Keystone_queuePush(uiQueueNum, (Uint32)hostDescriptor|FETCH_SIZE_64);
		//queueManageRegs[uiQueueNum].REG_D_Descriptor= (Uint32)hostDescriptor|FETCH_SIZE_64;
		
		//SRIO_Check_TxRx_Queues(); 	//for debug
		//SRIO_Check_Free_Queues(); 	//for debug
	}

	printf("%s from 0x%8x to 0x%8x, %6d bytes, %6d cycles, %5lld Mbps, completion code = %d\n", 
		get_packet_type_string(transferParam->packet_type),
		uipSrc, uipDst, uiByteCount, cycles,
		(unsigned long long)uiByteCount*8*CPU_CLK_KHZ/cycles/1000, 
		uiCompletionCode);

	VerifyData((Uint32 *)uipSrc, (Uint32 *)uipDst, uiByteCount);

}

/*test SRIO transfer through single port*/
void srio_signle_port_loopback_test(Uint32 uiPortNum, 
	SRIO_Loopback_Mode loopback_mode)
{
	int k, m, n;
	SRIO_Transfer_Param testParam;
	Uint32 uiSrcDstComb;

	//test different packet types
	for(k=0; k< sizeof(packet_type)/sizeof(SRIO_Packet_Type); k++)
	{
		testParam.packet_type= packet_type[k];

		//test different source/destination combinations
		if(testParam.packet_type<0x90) 	//directIO
		{
			uiSrcDstComb= sizeof(directIO_src_dest)/8;
		}
		else 	//message
		{
			if(SRIO_EXTERNAL_FORWARD_BACK==loopback_mode)
				continue; 	//forwarding does not support message
				
			uiSrcDstComb= sizeof(message_src_dest)/8;
		}
		for(m=0; m< uiSrcDstComb; m++)
		{
			if(testParam.packet_type<0x90) 	//directIO
			{
				testParam.source= directIO_src_dest[m][0];
				testParam.dest= directIO_src_dest[m][1];
			}
			else 	//message
			{
				testParam.source= message_src_dest[m][0];
				testParam.dest= message_src_dest[m][1];
			}
			
			//test different packet size
			for(n=8; n<= LL2_PACKET_BUFFER_SIZE1; n*=2)
			{
				    testParam.byteCount = n;                            
				srio_signle_port_transfer(&testParam, uiPortNum);
			}
		}
	}
	//print_SRIO_status_error(&srio_cfg.blockEn);
	puts("");
}

/*transfer through multiple SRIO ports in parallel*/
void srio_multiple_port_transfer(SRIO_Multiple_Test_Config * test_cfg)
{
	int j;
	SRIO_Transfer_Param * transferParam;
	SRIO_LSU_Transfer lsuTransfer[4];
	Int32 uiCompletionCode[4]= {-1, -1, -1, -1}, uiCompletion_context_code;
	Uint32 cycles[4];
	Uint32 uiQueueNum, uiWordIndex;
	Uint32 * uipSrc[4], * uipDst[4], uiByteCount[4], uiDestID[4];
	SRIO_Packet_Type packet_type[4];
	HostPacketDescriptor * hostDescriptor[4];
	SRIO_Type9_Message_RX_Descriptor * type9MsgRxDesc;
	SRIO_Type11_Message_RX_Descriptor * type11MsgRxDesc;

	//prepare all transfer requests
	for(j= 0; j< 4; j++)
	{
		transferParam= &test_cfg->transfer_param[j];
		uiByteCount[j]= transferParam->byteCount;
		if(0==uiByteCount[j])
		{
			uiCompletionCode[j]= 0;
			continue;
		}

		packet_type[j]= transferParam->packet_type;
		if(packet_type[j]<0x90) 	//directIO
		{
			uipSrc[j] = (Uint32 *)transferParam->source;
			uipDst[j]= (Uint32 *)transferParam->dest;

			/*initialize buffer*/
			InitDataBuffer(uipSrc[j], uipDst[j], uiByteCount[j]);

			InitLsuTransfer(&lsuTransfer[j], transferParam, j,
				DSP0_SRIO_BASE_ID+j, j);
        }
		else 	//message
		{
			hostDescriptor[j]= (HostPacketDescriptor *)
				Keystone_queuePop(transferParam->source);
			if(NULL==hostDescriptor[j])
			{
				printf("Source queue descriptor %d is NULL\n", 
					transferParam->source);
				SRIO_Check_Free_Queues(); 	//for debug
				return;
			}
		
			/*invalid cache before read descriptor RAM*/
			Invalid((void *)hostDescriptor[j], 64);

			/*initialize the source buffer*/
			uipSrc[j]= (Uint32 *)hostDescriptor[j]->buffer_ptr;
			InitDataBuffer(uipSrc[j], NULL, uiByteCount[j]);

			uiDestID[j]= transferParam->dest;
			if(SRIO_PKT_TYPE9_STREAM==packet_type[j])
			{
				Keystone_SRIO_Build_Type9_Msg_Desc(hostDescriptor[j],
					DSP0_SRIO_BASE_ID, uiDestID[j], uiByteCount[j], 0xAAAA, 0x55);
			}
			else if(SRIO_PKT_TYPE11_MESSAGE==packet_type[j])
			{
				Keystone_SRIO_Build_Type11_Msg_Desc(hostDescriptor[j],
					DSP0_SRIO_BASE_ID, uiDestID[j], uiByteCount[j], 0, 0);
			}
			else
			{
				printf("Invalid packet type %d\n", packet_type[j]);
				return;
			}

			/*write back data from cache to descriptor RAM*/
			Writeback((void *)hostDescriptor[j], 64);
		}
        
    }   

    //submit all transfers
	for(j= 0; j< 4; j++)
	{
		transferParam= &test_cfg->transfer_param[j];
		if(0==uiByteCount[j])
			continue;
		if(packet_type[j]<0x90) 	//directIO
		{

			cycles[j]= TSCL;
			Keystone_SRIO_LSU_transfer(&lsuTransfer[j]);

		}
		else 	//message
		{
			cycles[j]= TSCL;
			/*push the packet descriptor to Packet DMA TX queue*/
			Keystone_queuePush(QMSS_SRIO_QUEUE_BASE+j, 
				(Uint32)hostDescriptor[j]|FETCH_SIZE_64);
			asm(" MFENCE"); 	//force all memory operation complete
			
		}
	}

	//wait all transfers complete
	while((-1==uiCompletionCode[0])||(-1==uiCompletionCode[1])||
		(-1==uiCompletionCode[2])||(-1==uiCompletionCode[3]))
	{
		for(j= 0; j< 4; j++)
		{
			if(-1!=uiCompletionCode[j])
				continue;

			if(packet_type[j]<0x90) 	//DirectIO
			{

				uiCompletion_context_code= 
					Keystone_SRIO_get_LSU_completion_context_code(j,
					lsuTransfer[j].transactionID);

				uiWordIndex= uiByteCount[j]/4-1;
				Invalid((void *)&uipSrc[j][uiWordIndex], 4);
				Invalid((void *)&uipDst[j][uiWordIndex], 4);

				if((uipSrc[j][uiWordIndex]==uipDst[j][uiWordIndex])&&
					((uiCompletion_context_code&1)==lsuTransfer[j].contextBit))
				{
					cycles[j]= TSC_delay_cycle(cycles[j]);
                    uiCompletionCode[j]= uiCompletion_context_code>>1;
				}
			}
			else 	//message
			{
				/*dest flow = destID - BASE_ID*/
				uiQueueNum= SRIO_RX_FLOW0_DST_Q+ 
					(uiDestID[j]- DSP0_SRIO_BASE_ID);
				/*poll the packet descriptor in destination queue*/	
				if(0!=Keystone_GetQueueEntryCount(uiQueueNum))
				{
					cycles[j]= TSC_delay_cycle(cycles[j]);

					/*pop descriptor from destination queue*/
					hostDescriptor[j]= (HostPacketDescriptor *)
						Keystone_queuePop(uiQueueNum);

					/*invalid cache before read descriptor RAM*/
					Invalid((void *)hostDescriptor[j], 64);

					/*get the destination buffer*/
					uipDst[j]= (Uint32 *)hostDescriptor[j]->buffer_ptr;

					/*get completion code*/
					if(SRIO_PKT_TYPE9_STREAM==packet_type[j])
					{
						type9MsgRxDesc= (SRIO_Type9_Message_RX_Descriptor *)
							(((Uint32)hostDescriptor[j])+32);
						uiCompletionCode[j]= type9MsgRxDesc->CC;
					}
					else if(SRIO_PKT_TYPE11_MESSAGE==packet_type[j])
					{
						type11MsgRxDesc= (SRIO_Type11_Message_RX_Descriptor *)
							(((Uint32)hostDescriptor[j])+32);
						uiCompletionCode[j]= type11MsgRxDesc->CC;
					}

					/*descriptor Reclamation*/
					uiQueueNum= (hostDescriptor[j]->pkt_return_qmgr<<12)|
						hostDescriptor[j]->pkt_return_qnum;
					Keystone_queuePush(uiQueueNum, 
						(Uint32)hostDescriptor[j]|FETCH_SIZE_64);
				}
			}
		}
	}
	//SRIO_Check_TxRx_Queues(); 	//for debug
	//SRIO_Check_Free_Queues(); 	//for debug
	
	//check and print result
	for(j= 0; j< 4; j++)
	{
		if(0==uiByteCount[j])
			continue;

		printf("%s from 0x%8x to 0x%8x through port %d, %6d bytes, %6d cycles, %5lld Mbps, completion code = %d\n", 
			get_packet_type_string(packet_type[j]),
			uipSrc[j], uipDst[j], j, uiByteCount[j], cycles[j],
			(unsigned long long)uiByteCount[j]*8*CPU_CLK_KHZ/cycles[j]/1000, 
			uiCompletionCode[j]);

		VerifyData(uipSrc[j], uipDst[j], uiByteCount[j]);
	}

}

