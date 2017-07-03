
#include <stdio.h>
#include "SRIO_PktDMA_Init.h"
#include "SRIO_test.h"
#include "SRIO_debug.h"
#include "SRIO_loopback_test.h"

float internal_speed[]= {5, 3.125, 2.5, 1.25};

SRIO_1x2x4x_Path_Control internal_path[]=
{
	SRIO_PATH_CTL_1xLaneA, 
	SRIO_PATH_CTL_2xLaneAB,
	SRIO_PATH_CTL_4xLaneABCD
};

SRIO_Multiple_Test_Config internal_multi_port_test_cfg[]=
{
    {SRIO_PATH_CTL_1xLaneA_1xLaneB_1xLaneC_1xLaneD, /*multiple_port_path*/
        /*packet_type               source                                                   dest                                                size*/
        {{SRIO_PKT_TYPE_NREAD,      ((Uint32)&packetBuffer_LL2_Size1[0][0])+0x12000000,     ((Uint32)&packetBuffer_LL2_Size1[0][0])+0x13000000, LL2_PACKET_BUFFER_SIZE1},   /*port0*/
        {SRIO_PKT_TYPE_NWRITE,      ((Uint32)&packetBuffer_LL2_Size1[1][0])+0x12000000,     ((Uint32)&packetBuffer_LL2_Size1[1][0])+0x13000000, LL2_PACKET_BUFFER_SIZE1},   /*port1*/
        {SRIO_PKT_TYPE_NWRITE_R,    ((Uint32)&packetBuffer_LL2_Size1[2][0])+0x12000000,     ((Uint32)&packetBuffer_LL2_Size1[2][0])+0x13000000, LL2_PACKET_BUFFER_SIZE1},   /*port2*/
        {SRIO_PKT_TYPE_SWRITE,      ((Uint32)&packetBuffer_LL2_Size1[3][0])+0x12000000,     ((Uint32)&packetBuffer_LL2_Size1[3][0])+0x13000000, LL2_PACKET_BUFFER_SIZE1}}    /*port3*/
    },
    {SRIO_PATH_CTL_2xLaneAB_2xLaneCD,               /*multiple_port_path*/
        /*packet_type               source                                                  dest                                                 size*/
        {{SRIO_PKT_TYPE9_STREAM,    CORE0_LL2_HOST_SIZE1_FDQ,                               DSP0_SRIO_BASE_ID+SRIO_RX_FLOW_DDR,                  4096},   /*port0*/
        {0, 						0, 														0, 													 0},   /*port1 is not availible for this case*/
        {SRIO_PKT_TYPE11_MESSAGE,   SL2_HOST_SIZE1_FDQ,                                     DSP0_SRIO_BASE_ID+SRIO_RX_FLOW_CORE1_LL2,            4096},   /*port2*/
        {0, 						0, 														0, 													 0}}   /*port3 is not availible for this case*/
    },    
    {SRIO_PATH_CTL_2xLaneAB_1xLaneC_1xLaneD,		/*multiple_port_path*/
        /*packet_type               source                                                  dest                                                 size*/
        {{SRIO_PKT_TYPE_SWRITE,     ((Uint32)&packetBuffer_LL2_Size1[0][0])+0x12000000,     ((Uint32)&packetBuffer_LL2_Size1[0][0])+0x13000000, LL2_PACKET_BUFFER_SIZE1},   /*port0*/
        {0, 						0, 														0, 													 0},   /*port1 is not availible for this case*/
        {SRIO_PKT_TYPE9_STREAM,    	CORE0_LL2_HOST_SIZE1_FDQ,                               DSP0_SRIO_BASE_ID+SRIO_RX_FLOW_CORE1_LL2,            4096},   /*port2*/
        {SRIO_PKT_TYPE11_MESSAGE,   SL2_HOST_SIZE1_FDQ,                                     DSP0_SRIO_BASE_ID+SRIO_RX_FLOW_CORE1_LL2,            4096}}    /*port3*/
    }
};

/*test SRIO transfer through single port*/
void srio_signle_port_internal_loopback_test(SRIO_Loopback_Mode loopback_mode)
{
	int i, j;

	//test different speed
	for(i= 0; i< sizeof(internal_speed)/sizeof(float); i++ )
	{
		serdesLinkSetup.linkSpeed_GHz=internal_speed[i];

		//test 1x 2x and 4x path
		for(j= 0; j< sizeof(internal_path)/sizeof(SRIO_1x2x4x_Path_Control); j++)
		{
	        printf("SRIO link speed is %.3fGbps\n", internal_speed[i]);
   			srio_cfg.srio_1x2x4x_path_control= internal_path[j];                

			srio_identify_used_ports_lanes(srio_cfg.srio_1x2x4x_path_control);
			Keystone_SRIO_Init(&srio_cfg);
		   	SRIO_PktDM_init();
			
			srio_signle_port_loopback_test(0, loopback_mode);
		}
	}
}

/*test SRIO transfer through multiple ports in parallel*/
void srio_multiple_ports_internal_loopback_test()
{
	int i;
	Uint32 uiNumTestCases;

	SRIO_Multiple_Test_Config * test_cfg;

	printf("SRIO mulitple ports test with link speed %.3fGbps\n", SRIO_DEFAULT_TEST_SPEED);
	serdesLinkSetup.linkSpeed_GHz= SRIO_DEFAULT_TEST_SPEED;

	//test different cases
	uiNumTestCases = 
		sizeof(internal_multi_port_test_cfg)/sizeof(SRIO_Multiple_Test_Config);
	for(i= 0; i< uiNumTestCases; i++)
	{
		test_cfg= &internal_multi_port_test_cfg[i];
   		srio_cfg.srio_1x2x4x_path_control= test_cfg->multiple_port_path;            
		srio_identify_used_ports_lanes(srio_cfg.srio_1x2x4x_path_control);
		Keystone_SRIO_Init(&srio_cfg);
	   	SRIO_PktDM_init();
		
		srio_multiple_port_transfer(test_cfg);
		//print_SRIO_status_error(&srio_cfg.blockEn);

		puts("");
	}
}

