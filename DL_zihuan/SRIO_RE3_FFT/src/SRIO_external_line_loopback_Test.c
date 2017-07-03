
#include <stdio.h>
#include "SRIO_PktDMA_Init.h"
#include "SRIO_test.h"
#include "SRIO_debug.h"
#include "SRIO_loopback_test.h"

SRIO_1x2x4x_Path_Control external_path[]=
{
	SRIO_PATH_CTL_1xLaneA_1xLaneB_1xLaneC_1xLaneD, 
	SRIO_PATH_CTL_2xLaneAB_2xLaneCD
};

/*Please note, Turbo Nyquist EVM only connects port 2 and 3 between 2 DSPs.
and packet forwording does not support message*/
SRIO_Multiple_Test_Config external_multi_port_test_cfg[]=
{
    {SRIO_PATH_CTL_1xLaneA_1xLaneB_1xLaneC_1xLaneD, /*multiple_port_path*/
        /*packet_type               source                                                   dest                                                size*/
        {{0, 						0, 														0, 													0},   /*port0 is not availible for this case*/
        {0, 						0, 														0, 													0},   /*port1 is not availible for this case*/
        {SRIO_PKT_TYPE_SWRITE,     ((Uint32)&packetBuffer_LL2_Size1[2][0])+0x12000000,     ((Uint32)&packetBuffer_LL2_Size1[2][0])+0x13000000, LL2_PACKET_BUFFER_SIZE1},   /*port2*/
        {SRIO_PKT_TYPE_NREAD,      ((Uint32)&packetBuffer_LL2_Size1[3][0])+0x12000000,     ((Uint32)&packetBuffer_LL2_Size1[3][0])+0x13000000, LL2_PACKET_BUFFER_SIZE1}}    /*port3*/
    },
    {SRIO_PATH_CTL_1xLaneA_1xLaneB_1xLaneC_1xLaneD,               /*multiple_port_path*/
        /*packet_type               source                                                  dest                                                 size*/
        {{0, 						0, 														0, 													0},   /*port0 is not availible for this case*/
        {0, 						0, 														0, 													0},   /*port1 is not availible for this case*/
        {SRIO_PKT_TYPE9_STREAM,    	CORE0_LL2_HOST_SIZE1_FDQ,                               DSP0_SRIO_BASE_ID+SRIO_RX_FLOW_CORE1_LL2,            4096},   /*port2*/
        {SRIO_PKT_TYPE11_MESSAGE,   SL2_HOST_SIZE1_FDQ,                                     DSP0_SRIO_BASE_ID+SRIO_RX_FLOW_CORE1_LL2,            4096}}    /*port3*/
    }
};

/*test SRIO transfer through single port*/
void srio_signle_port_external_line_loopback_test(SRIO_Loopback_Mode loopback_mode)
{
	int j;

	printf("SRIO link speed is %.3fGbps\n", SRIO_DEFAULT_TEST_SPEED);
	serdesLinkSetup.linkSpeed_GHz= SRIO_DEFAULT_TEST_SPEED;

	//test 1x 2x and 4x path
	for(j= 0; j< sizeof(external_path)/sizeof(SRIO_1x2x4x_Path_Control); j++)
	{
		srio_cfg.srio_1x2x4x_path_control= external_path[j];                
		srio_identify_used_ports_lanes(srio_cfg.srio_1x2x4x_path_control);

		/*On Turbo Nyquist EVM only lane 2 and 3 are connected between 2 DSPs*/
		srio_cfg.blockEn.bLogic_Port_EN[0]= FALSE;
		srio_cfg.blockEn.bLogic_Port_EN[1]= FALSE;
	
		Keystone_SRIO_Init(&srio_cfg);
	   	SRIO_PktDM_init();
		
		srio_signle_port_loopback_test(2, loopback_mode);
	}
}

/*test SRIO transfer through multiple ports in parallel*/
void srio_multiple_ports_external_line_loopback_test()
{
	int i;
	Uint32 uiNumTestCases;

	SRIO_Multiple_Test_Config * test_cfg;

	printf("SRIO mulitple ports test with link speed %.3fGbps\n", SRIO_DEFAULT_TEST_SPEED);
	serdesLinkSetup.linkSpeed_GHz= SRIO_DEFAULT_TEST_SPEED;

	//test different cases
	uiNumTestCases = 
		sizeof(external_multi_port_test_cfg)/sizeof(SRIO_Multiple_Test_Config);
	for(i= 0; i< uiNumTestCases; i++)
	{
		test_cfg= &external_multi_port_test_cfg[i];
   		srio_cfg.srio_1x2x4x_path_control= test_cfg->multiple_port_path;            
		srio_identify_used_ports_lanes(srio_cfg.srio_1x2x4x_path_control);

		/*On Turbo Nyquist EVM only lane 2 and 3 are connected between 2 DSPs*/
		srio_cfg.blockEn.bLogic_Port_EN[0]= FALSE;
		srio_cfg.blockEn.bLogic_Port_EN[1]= FALSE;
	
		Keystone_SRIO_Init(&srio_cfg);
	   	SRIO_PktDM_init();
		
		srio_multiple_port_transfer(test_cfg);
		//print_SRIO_status_error(&srio_cfg.blockEn);

		puts("");
	}
}

