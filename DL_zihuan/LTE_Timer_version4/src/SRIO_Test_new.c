
#include <stdio.h>
#include <string.h>
#include "Keystone_SRIO_init.h"
#include "Keystone_DDR_init.h"
#include "SRIO_test.h"
//#include "SRIO_Interrupts.h"   CS 14/11/05
#include "SRIO_PktDMA_init.h"
#include "SRIO_2DSP_test.h"
#include "TSC.h"
#include "system_init.h"


SRIO_Loopback_Mode loopback_mode= SRIO_NO_LOOPBACK;//SRIO_NO_LOOPBACK;

/*up to 16 deviceID can be setup here*/
SRIO_Device_ID_Routing_Config dsp0_device_ID_routing_config[]=
{
     /*idPattern 	idMatchMask 	routeMaintenance*/
	{DSP0_SRIO_BASE_ID+0, 	0xFFFF, 	1},
	{DSP0_SRIO_BASE_ID+1, 	0xFFFF, 	1},
	{DSP0_SRIO_BASE_ID+2, 	0xFFFF, 	1},
	{DSP0_SRIO_BASE_ID+3, 	0xFFFF, 	1},
	{DSP0_SRIO_BASE_ID+4, 	0xFFFF, 	1},
	{DSP0_SRIO_BASE_ID+5, 	0xFFFF, 	1},
	{DSP0_SRIO_BASE_ID+6, 	0xFFFF, 	1},
	{DSP0_SRIO_BASE_ID+7, 	0xFFFF, 	1},
};

/*up to 16 deviceID can be setup here*/
SRIO_Device_ID_Routing_Config dsp1_device_ID_routing_config[]=
{
     /*idPattern 	idMatchMask 	routeMaintenance*/
	{DSP1_SRIO_BASE_ID+0, 	0xFFFF, 	1},
	{DSP1_SRIO_BASE_ID+1, 	0xFFFF, 	1},
	{DSP1_SRIO_BASE_ID+2, 	0xFFFF, 	1},
	{DSP1_SRIO_BASE_ID+3, 	0xFFFF, 	1},
	{DSP1_SRIO_BASE_ID+4, 	0xFFFF, 	1},
	{DSP1_SRIO_BASE_ID+5, 	0xFFFF, 	1},
	{DSP1_SRIO_BASE_ID+6, 	0xFFFF, 	1},
	{DSP1_SRIO_BASE_ID+7, 	0xFFFF, 	1},
};

/*map RX message to flow or queue according to specific fields in the message.
In this test, we only use destination device ID to determine the RX flow ID, 
destination queue is configured in flow conifgurations*/
/*up to 64 map entries can be setup here*/
SRIO_RX_Message_Map DSP0_message_map[]=
{
    /*flowId*/              /*destQuID*/    /*dstId*/                               /*dstProm*/ /*srcId*/ /*srcProm*/ /*tt*/ /*mbx*/ /*mbxMask*/ /*ltr*/ /*ltrMask*/ /*segMap*/ /*cos*/ /*cosMask*/ /*streamId*/ /*streamMask*/         
    {SRIO_RX_FLOW_CORE0_LL2,0x1FFF,         DSP0_SRIO_BASE_ID+SRIO_RX_FLOW_CORE0_LL2,   0,          0,      1,          0,      0,      0,          0,      0,          0,          0,      0,          0,          0}, /*single segement*/
    {SRIO_RX_FLOW_CORE0_LL2,0x1FFF,         DSP0_SRIO_BASE_ID+SRIO_RX_FLOW_CORE0_LL2,   0,          0,      1,          0,      0,      0,          0,      0,          1,          0,      0,          0,          0}, /*multiple segements*/
    {SRIO_RX_FLOW_CORE1_LL2,0x1FFF,         DSP0_SRIO_BASE_ID+SRIO_RX_FLOW_CORE1_LL2,   0,          0,      1,          0,      0,      0,          0,      0,          0,          0,      0,          0,          0}, /*single segement*/
    {SRIO_RX_FLOW_CORE1_LL2,0x1FFF,         DSP0_SRIO_BASE_ID+SRIO_RX_FLOW_CORE1_LL2,   0,          0,      1,          0,      0,      0,          0,      0,          1,          0,      0,          0,          0}, /*multiple segements*/
    {SRIO_RX_FLOW_CORE2_LL2,0x1FFF,         DSP0_SRIO_BASE_ID+SRIO_RX_FLOW_CORE2_LL2,   0,          0,      1,          0,      0,      0,          0,      0,          0,          0,      0,          0,          0}, /*single segement*/
    {SRIO_RX_FLOW_CORE2_LL2,0x1FFF,         DSP0_SRIO_BASE_ID+SRIO_RX_FLOW_CORE2_LL2,   0,          0,      1,          0,      0,      0,          0,      0,          1,          0,      0,          0,          0}, /*multiple segements*/
    {SRIO_RX_FLOW_CORE3_LL2,0x1FFF,         DSP0_SRIO_BASE_ID+SRIO_RX_FLOW_CORE3_LL2,   0,          0,      1,          0,      0,      0,          0,      0,          0,          0,      0,          0,          0}, /*single segement*/
    {SRIO_RX_FLOW_CORE3_LL2,0x1FFF,         DSP0_SRIO_BASE_ID+SRIO_RX_FLOW_CORE3_LL2,   0,          0,      1,          0,      0,      0,          0,      0,          1,          0,      0,          0,          0}, /*multiple segements*/
    {SRIO_RX_FLOW_SL2,      0x1FFF,         DSP0_SRIO_BASE_ID+SRIO_RX_FLOW_SL2,         0,          0,      1,          0,      0,      0,          0,      0,          0,          0,      0,          0,          0}, /*single segement*/
    {SRIO_RX_FLOW_SL2,      0x1FFF,         DSP0_SRIO_BASE_ID+SRIO_RX_FLOW_SL2,         0,          0,      1,          0,      0,      0,          0,      0,          1,          0,      0,          0,          0}, /*multiple segements*/
    {SRIO_RX_FLOW_DDR,      0x1FFF,         DSP0_SRIO_BASE_ID+SRIO_RX_FLOW_DDR,         0,          0,      1,          0,      0,      0,          0,      0,          0,          0,      0,          0,          0}, /*single segement*/
    {SRIO_RX_FLOW_DDR,      0x1FFF,         DSP0_SRIO_BASE_ID+SRIO_RX_FLOW_DDR,         0,          0,      1,          0,      0,      0,          0,      0,          1,          0,      0,          0,          0}  /*multiple segements*/
};

/*up to 64 map entries can be setup here*/
SRIO_RX_Message_Map DSP1_message_map[]=
{
    /*flowId*/              /*destQuID*/    /*dstId*/                               /*dstProm*/ /*srcId*/ /*srcProm*/ /*tt*/ /*mbx*/ /*mbxMask*/ /*ltr*/ /*ltrMask*/ /*segMap*/ /*cos*/ /*cosMask*/ /*streamId*/ /*streamMask*/         
    {SRIO_RX_FLOW_CORE0_LL2,0x1FFF,         DSP1_SRIO_BASE_ID+SRIO_RX_FLOW_CORE0_LL2,   0,          0,      1,          0,      0,      0,          0,      0,          0,          0,      0,          0,          0}, /*single segement*/
    {SRIO_RX_FLOW_CORE0_LL2,0x1FFF,         DSP1_SRIO_BASE_ID+SRIO_RX_FLOW_CORE0_LL2,   0,          0,      1,          0,      0,      0,          0,      0,          1,          0,      0,          0,          0}, /*multiple segements*/
    {SRIO_RX_FLOW_CORE1_LL2,0x1FFF,         DSP1_SRIO_BASE_ID+SRIO_RX_FLOW_CORE1_LL2,   0,          0,      1,          0,      0,      0,          0,      0,          0,          0,      0,          0,          0}, /*single segement*/
    {SRIO_RX_FLOW_CORE1_LL2,0x1FFF,         DSP1_SRIO_BASE_ID+SRIO_RX_FLOW_CORE1_LL2,   0,          0,      1,          0,      0,      0,          0,      0,          1,          0,      0,          0,          0}, /*multiple segements*/
    {SRIO_RX_FLOW_CORE2_LL2,0x1FFF,         DSP1_SRIO_BASE_ID+SRIO_RX_FLOW_CORE2_LL2,   0,          0,      1,          0,      0,      0,          0,      0,          0,          0,      0,          0,          0}, /*single segement*/
    {SRIO_RX_FLOW_CORE2_LL2,0x1FFF,         DSP1_SRIO_BASE_ID+SRIO_RX_FLOW_CORE2_LL2,   0,          0,      1,          0,      0,      0,          0,      0,          1,          0,      0,          0,          0}, /*multiple segements*/
    {SRIO_RX_FLOW_CORE3_LL2,0x1FFF,         DSP1_SRIO_BASE_ID+SRIO_RX_FLOW_CORE3_LL2,   0,          0,      1,          0,      0,      0,          0,      0,          0,          0,      0,          0,          0}, /*single segement*/
    {SRIO_RX_FLOW_CORE3_LL2,0x1FFF,         DSP1_SRIO_BASE_ID+SRIO_RX_FLOW_CORE3_LL2,   0,          0,      1,          0,      0,      0,          0,      0,          1,          0,      0,          0,          0}, /*multiple segements*/
    {SRIO_RX_FLOW_SL2,      0x1FFF,         DSP1_SRIO_BASE_ID+SRIO_RX_FLOW_SL2,         0,          0,      1,          0,      0,      0,          0,      0,          0,          0,      0,          0,          0}, /*single segement*/
    {SRIO_RX_FLOW_SL2,      0x1FFF,         DSP1_SRIO_BASE_ID+SRIO_RX_FLOW_SL2,         0,          0,      1,          0,      0,      0,          0,      0,          1,          0,      0,          0,          0}, /*multiple segements*/
    {SRIO_RX_FLOW_DDR,      0x1FFF,         DSP1_SRIO_BASE_ID+SRIO_RX_FLOW_DDR,         0,          0,      1,          0,      0,      0,          0,      0,          0,          0,      0,          0,          0}, /*single segement*/
    {SRIO_RX_FLOW_DDR,      0x1FFF,         DSP1_SRIO_BASE_ID+SRIO_RX_FLOW_DDR,         0,          0,      1,          0,      0,      0,          0,      0,          1,          0,      0,          0,          0}  /*multiple segements*/
};

/*up to 16 TX queues can be setup here*/
SRIO_TX_Queue_Sch_Info TX_Queue_Sch_Info[]=
{
	/*outputPort*/ 	/*priority*/ 	/*CRF*/
	{0, 			0, 				0},
	{1, 			0, 				0},
	{2, 			0, 				0},
	{3, 			0, 				0},
	{0, 			0, 				1},
	{1, 			0, 				1},
	{2, 			0, 				1},
	{3, 			0, 				1},
	{0, 			1, 				0},
	{1, 			1, 				0},
	{2, 			1, 				0},
	{3, 			1, 				0},
	{0, 			1, 				1},
	{1, 			1, 				1},
	{2, 			1, 				1},
	{3, 			1, 				1}
};

/*For SRIO_EXTERNAL_FORWARD_BACK test, DSP1 is setup to forwarding back all
packets with ID of DSP0*/
/*up to 8 entries can be setup here*/
SRIO_PktForwarding_Cfg DSP1_PktForwarding_Cfg[]=
{
    /*forwarding ID 8 up*/  /* forwarding ID 8 lo */   /*forwarding ID 16 up*/  /*forwarding ID 16 lo*/    /*outport */
    {DSP0_SRIO_BASE_ID+0,   DSP0_SRIO_BASE_ID+1,         DSP0_SRIO_BASE_ID+0,     DSP0_SRIO_BASE_ID+1,         2},
    {DSP0_SRIO_BASE_ID+2,   DSP0_SRIO_BASE_ID+2,         DSP0_SRIO_BASE_ID+2,     DSP0_SRIO_BASE_ID+2,         2},
    {DSP0_SRIO_BASE_ID+3,   DSP0_SRIO_BASE_ID+3,         DSP0_SRIO_BASE_ID+3,     DSP0_SRIO_BASE_ID+3,         3},
    {DSP0_SRIO_BASE_ID+4,   DSP0_SRIO_BASE_ID+7,         DSP0_SRIO_BASE_ID+4,     DSP0_SRIO_BASE_ID+7,         3}
};

SRIO_RX_Mode rxMode;

SerdesSetup_4links serdes_cfg;
SerdesLinkSetup serdesLinkSetup;
SRIO_Message_Cfg msg_cfg;
SRIO_Config srio_cfg;

/*path configuration strings*/
char * path_str[]=
{
	"1xLaneA                        ", 
	"1xLaneA_1xLaneB                ",
	"2xLaneAB                       ",
	"1xLaneA_1xLaneB_1xLaneC_1xLaneD",  
	"2xLaneAB_1xLaneC_1xLaneD       ",
	"1xLaneA_1xLaneB_2xLaneCD       ",
	"2xLaneAB_2xLaneCD              ",
	"4xLaneABCD                     "
};
char * get_path_string(SRIO_1x2x4x_Path_Control srio_1x2x4x_path_control)
{
	if(SRIO_PATH_CTL_1xLaneA                             == srio_1x2x4x_path_control) return path_str[0];
	else if(SRIO_PATH_CTL_1xLaneA_1xLaneB                == srio_1x2x4x_path_control) return path_str[1];
	else if(SRIO_PATH_CTL_2xLaneAB                       == srio_1x2x4x_path_control) return path_str[2];
	else if(SRIO_PATH_CTL_1xLaneA_1xLaneB_1xLaneC_1xLaneD== srio_1x2x4x_path_control) return path_str[3]; 
	else if(SRIO_PATH_CTL_2xLaneAB_1xLaneC_1xLaneD       == srio_1x2x4x_path_control) return path_str[4];
	else if(SRIO_PATH_CTL_1xLaneA_1xLaneB_2xLaneCD       == srio_1x2x4x_path_control) return path_str[5];
	else if(SRIO_PATH_CTL_2xLaneAB_2xLaneCD              == srio_1x2x4x_path_control) return path_str[6];
	else if(SRIO_PATH_CTL_4xLaneABCD                     == srio_1x2x4x_path_control) return path_str[7];
	else return NULL;
}

/* identify used ports lanes according to 1x 2x 4x path configuration*/
void srio_identify_used_ports_lanes(
	SRIO_1x2x4x_Path_Control srio_1x2x4x_path_control)
{
	int i;
	unsigned int uiPathConfig, uiPathMode;
	unsigned int uiLaneEnableMask=0, uiLogicPortEnableMask=0;

	/* SRIO 1x 2x 4x path mode configuration:
	In a configuration 1, the following operating mode is available:
	 Mode 0: One independent 1x port in lane A

	In Configuration 2, a maximum of 2 active ports and 2 lanes per path are supported as follows:
	 Mode 0: Two independent 1x ports in lanes A and B
	 Mode 1: One independent 2x port in lanes {A, B}

	In Configuration 4, a maximum of 4 active ports and 4 lanes per path are supported as follows:
	 Mode 0: Four independent 1x ports in lanes A, B, C, and D
	 Mode 1: One independent 2x port in lanes {A,B}, and two independent 1x ports in lanes C and D
	 Mode 2: Two independent 1x ports in lanes A and B, and one independent 2x port in lanes {C,D}
	 Mode 3: Two independent 2x ports, occupying lanes {A,B} and {C,D} respectively
	 Mode 4: One independent 4x port in lanes {A,B,C,D}*/
	uiPathConfig= (srio_1x2x4x_path_control&
		CSL_SRIO_RIO_PLM_SP_PATH_CTL_PATH_CONFIGURATION_MASK)>>
		CSL_SRIO_RIO_PLM_SP_PATH_CTL_PATH_CONFIGURATION_SHIFT;

	uiPathMode= (srio_1x2x4x_path_control&
		CSL_SRIO_RIO_PLM_SP_PATH_CTL_PATH_MODE_MASK)>>
		CSL_SRIO_RIO_PLM_SP_PATH_CTL_PATH_MODE_SHIFT;

	if(1==uiPathConfig)
	{
		uiLaneEnableMask= 0x1; 	/*0001*/

		uiLogicPortEnableMask= 0x1; 	/*0001*/
	}
	else if(2==uiPathConfig)
	{
		uiLaneEnableMask= 0x3; 	/*0011*/

		if(0==uiPathMode)
			uiLogicPortEnableMask= 0x3; 	/*0011*/
		else if(1==uiPathMode)
			uiLogicPortEnableMask= 0x1; 	/*0001*/
	}
	else if(4==uiPathConfig)
	{
		uiLaneEnableMask= 0xF; 	/*1111*/

		if(0==uiPathMode)
			uiLogicPortEnableMask= 0xF; 	/*1111*/
		else if(1==uiPathMode)
			uiLogicPortEnableMask= 0xD; 	/*1101*/
		else if(2==uiPathMode)
			uiLogicPortEnableMask= 0x7; 	/*0111*/
		else if(3==uiPathMode)
			uiLogicPortEnableMask= 0x5; 	/*0101*/
		else if(4==uiPathMode)
			uiLogicPortEnableMask= 0x1; 	/*0001*/
	}

	/*enable ports data path according to 1x 2x 4x path configuration*/
	srio_cfg.blockEn.bBLK5_8_Port_Datapath_EN[0]= uiLaneEnableMask&1;
	srio_cfg.blockEn.bBLK5_8_Port_Datapath_EN[1]= (uiLaneEnableMask>>1)&1;
	srio_cfg.blockEn.bBLK5_8_Port_Datapath_EN[2]= (uiLaneEnableMask>>2)&1;
	srio_cfg.blockEn.bBLK5_8_Port_Datapath_EN[3]= (uiLaneEnableMask>>3)&1;

	/*disable Serdes according to 1x 2x 4x path configuration*/
	for(i= 0; i<4; i++ )
	{
		if(uiLaneEnableMask&(1<<i))
			srio_cfg.serdes_cfg->linkSetup[i]= &serdesLinkSetup;
		else
			srio_cfg.serdes_cfg->linkSetup[i]= NULL;
	}

	/*enable loggical ports according to 1x 2x 4x path configuration*/
	srio_cfg.blockEn.bLogic_Port_EN[0]= uiLogicPortEnableMask&1;
	srio_cfg.blockEn.bLogic_Port_EN[1]= (uiLogicPortEnableMask>>1)&1;
	srio_cfg.blockEn.bLogic_Port_EN[2]= (uiLogicPortEnableMask>>2)&1;
	srio_cfg.blockEn.bLogic_Port_EN[3]= (uiLogicPortEnableMask>>3)&1;

	printf("SRIO path configuration %s\n", 
		get_path_string(srio_1x2x4x_path_control));

}

/*packet type strings*/
char * type_str[]=
{
	"NREAD   " ,
	"NWRITE  " ,
	"NWRITE_R" ,
	"SWRITE  " ,
	"STREAM  " ,
	"MESSAGE "
};
char * get_packet_type_string(SRIO_Packet_Type packetType)
{
	if(SRIO_PKT_TYPE_NREAD         == packetType) return type_str[0];
	else if(SRIO_PKT_TYPE_NWRITE   == packetType) return type_str[1];
	else if(SRIO_PKT_TYPE_NWRITE_R == packetType) return type_str[2];
	else if(SRIO_PKT_TYPE_SWRITE   == packetType) return type_str[3];
	else if(SRIO_PKT_TYPE9_STREAM  == packetType) return type_str[4];
	else if(SRIO_PKT_TYPE11_MESSAGE== packetType) return type_str[5];
	else return NULL;
}

unsigned int uiTestID= 1;
/*fill data before test*/
void InitDataBuffer(unsigned int * uipSrc, unsigned int * uipDst, unsigned int uiByteCount)
{
	int i;
	
	if(uipSrc)
	{
		for(i= 0; i< uiByteCount/4; i++)
		{
			uipSrc[i] = uiTestID;
		}

        WritebackAll((void *)uipSrc);
        
		uiTestID++;
	}

	if(uipDst)
	{
		for(i= 0; i< uiByteCount/4; i++)
		{
			uipDst[i] = 0xFFFFFFFF;
		}
        WritebackAll((void *)uipDst);
	}
}

/*verify data after test*/
void VerifyData(unsigned int * uipSrc, unsigned int * uipDst, unsigned int uiByteCount)
{
	int i;

    WritebackInvalidAll((void *)uipSrc);
    WritebackInvalidAll((void *)uipDst);
    
	InvalidPrefetchBuffer();

	for(i= 0; i< uiByteCount/4; i++)
	{
		if(uipSrc[i]!= uipDst[i])
		{
			printf("data mismatch at unit %d, 0x%x (at 0x%x) != 0x%x (at 0x%x)\n", 
				i, uipSrc[i], uipSrc, uipDst[i], uipDst);
			return;
		}
	}
}

void InitLsuTransfer(SRIO_LSU_Transfer * lsuTransfer, 
	SRIO_Transfer_Param * transferParam, unsigned int uiLsuNum, 
	unsigned int uiDstID, unsigned int uiSrcIDMap)
{
    lsuTransfer->rioAddressMSB=0;
	/*swap source/dest for READ*/
	if(SRIO_PKT_TYPE_NREAD==transferParam->packet_type)
	{
	    lsuTransfer->rioAddressLSB_ConfigOffset= transferParam->source;
	    lsuTransfer->localDspAddress= transferParam->dest;
	}
	else
	{
	    lsuTransfer->rioAddressLSB_ConfigOffset= transferParam->dest;
	    lsuTransfer->localDspAddress= transferParam->source;
	}
    lsuTransfer->bytecount= transferParam->byteCount; 	
    lsuTransfer->packetType= transferParam->packet_type;
    lsuTransfer->dstID= uiDstID;
    lsuTransfer->doorbellInfo= 0;
    lsuTransfer->waitLsuReady= TRUE;
    lsuTransfer->lsuNum= uiLsuNum;
    lsuTransfer->doorbellValid = 0;
    lsuTransfer->intrRequest = 0;
    lsuTransfer->supGoodInt = 0;
    lsuTransfer->priority = 0;
    lsuTransfer->outPortID = uiLsuNum;
    lsuTransfer->idSize = 0;
    lsuTransfer->srcIDMap = uiSrcIDMap;
    lsuTransfer->hopCount = 0;

}

unsigned int uiDoorbell_TSC; 	//timestamp when doorbell interrupt happens
void srio_doorbell_latency_test(unsigned int uiPortNum)
{
	unsigned int cycles;

	cycles= TSCL;	
	Keystone_SRIO_DoorBell(uiPortNum, uiPortNum, 
		DSP0_SRIO_BASE_ID+uiPortNum, SRIO_DOORBELL_MSG_LATENCY_TEST);

	//asm(" IDLE"); //waiting for doorbell interupt
	cycles= ((unsigned int)((0xFFFFFFFFl+uiDoorbell_TSC)- 
		(unsigned long long)cycles)+ 1);

	printf("doorbell latency is %4d cycles\n\n", cycles);
}

/*test mode strings*/
char * test_mode_str[]=
{
	"SRIO test between two DSPs",
	"SRIO_DIGITAL_LOOPBACK test",
	"SRIO_SERDES_LOOPBACK test",
	"SRIO_EXTERNAL_LINE_LOOPBACK test",
	"SRIO_EXTERNAL_FORWARD_BACK test"
};
SRIO_Tx_Config gxx_srio_tx_cfg;
void fxx_srio_init(unsigned int uiDspNum)
{
	/*clear configuration structrue to make sure unused field is 0*/
	memset(&srio_cfg, 0, sizeof(srio_cfg)); 
	
	srio_cfg.blockEn.bBLK1_LSU_EN= 1;
	srio_cfg.blockEn.bBLK2_MAU_EN= 1;
	srio_cfg.blockEn.bBLK3_TXU_EN= 1;
	srio_cfg.blockEn.bBLK4_RXU_EN= 1;

	srio_cfg.loopback_mode= loopback_mode;
	
	srio_cfg.device_ID_routing_config= dsp0_device_ID_routing_config;
	srio_cfg.uiNumDeviceId= 
		sizeof(dsp0_device_ID_routing_config)/sizeof(SRIO_Device_ID_Routing_Config);

	serdes_cfg.commonSetup.loopBandwidth= SERDES_PLL_LOOP_BAND_MID;
	srio_cfg.serdes_cfg= &serdes_cfg;

	serdesLinkSetup.txOutputSwing    = 10; /*0~15 represents between 100 and 850 mVdfpp  */
	serdesLinkSetup.testPattern      = SERDES_TEST_DISABLED; 
	serdesLinkSetup.rxAlign          = SERDES_RX_COMMA_ALIGNMENT_ENABLE; 
	serdesLinkSetup.rxInvertPolarity = SERDES_RX_NORMAL_POLARITY; 
	serdesLinkSetup.rxTermination    = SERDES_RX_TERM_COMMON_POINT_AC_COUPLE; 
	serdesLinkSetup.rxEqualizerConfig= SERDES_RX_EQ_ADAPTIVE; 
    serdesLinkSetup.rxCDR            = SERDES_RX_CDR_1;
	serdesLinkSetup.txInvertPolarity = SERDES_TX_PAIR_NORMAL_POLARITY; 

	msg_cfg.message_map = DSP0_message_map;
	msg_cfg.uiNumMessageMap= 
		sizeof(DSP0_message_map)/sizeof(SRIO_RX_Message_Map);
	msg_cfg.TX_Queue_Sch_Info= TX_Queue_Sch_Info;
	msg_cfg.uiNumTxQueue = 
		sizeof(TX_Queue_Sch_Info)/sizeof(SRIO_TX_Queue_Sch_Info);
	msg_cfg.rx_size_error_garbage_Q        = SRIO_RX_SIZE_ERROR_GARBAGE_Q       ;
	msg_cfg.rx_timeout_garbage_Q           = SRIO_RX_TIMEOUT_GARBAGE_Q          ;
	msg_cfg.tx_excessive_retries_garbage_Q = SRIO_TX_EXCESSIVE_RETRIES_GARBAGE_Q;
	msg_cfg.tx_error_garbage_Q             = SRIO_TX_ERROR_GARBAGE_Q            ;
	msg_cfg.tx_size_error_garbage_Q        = SRIO_TX_SIZE_ERROR_GARBAGE_Q       ;
	msg_cfg.datastreaming_cfg= NULL; 	/*use default values*/
	srio_cfg.msg_cfg= &msg_cfg;

	//SRIO_Interrupts_Init();    //CS 14/11/05
	loopback_mode = SRIO_NO_LOOPBACK;
	/*DSP0 is the master for test, DSP0 DirectIO or send message to DSP1.
	DSP1 is the slave for test, it monitor the doorbell and message
	interrupt to detect the received data*/
	/**************************init************************************/
	/**************************TX_config************************************/
	if(0==uiDspNum)
	{
		        serdesLinkSetup.linkSpeed_GHz= SRIO_DEFAULT_TEST_SPEED;
				srio_cfg.srio_1x2x4x_path_control= test_2DSP_cfg.multiple_port_path;
				srio_identify_used_ports_lanes(srio_cfg.srio_1x2x4x_path_control);

				/*On Turbo Nyquist EVM only lane 2 and 3 are connected between 2 DSPs*/
				srio_cfg.blockEn.bLogic_Port_EN[0]= FALSE;
				srio_cfg.blockEn.bLogic_Port_EN[1]= FALSE;

				Keystone_SRIO_Init(&srio_cfg);
			   	SRIO_PktDM_init();
#if 0
			   	if(srio_cfg.blockEn.bLogic_Port_EN[0])
			   		Keystone_SRIO_match_ACK_ID(0, DSP1_SRIO_BASE_ID, 0);
			   	if(srio_cfg.blockEn.bLogic_Port_EN[1])
			   		Keystone_SRIO_match_ACK_ID(1, DSP1_SRIO_BASE_ID, 1);
				if(srio_cfg.blockEn.bLogic_Port_EN[2])
					Keystone_SRIO_match_ACK_ID(2, DSP1_SRIO_BASE_ID, 2);
				if(srio_cfg.blockEn.bLogic_Port_EN[3])
					Keystone_SRIO_match_ACK_ID(3, DSP1_SRIO_BASE_ID, 3);
#endif
	}
	else
	{
		srio_cfg.device_ID_routing_config= dsp1_device_ID_routing_config;
		srio_cfg.uiNumDeviceId=
			sizeof(dsp1_device_ID_routing_config)/
			sizeof(SRIO_Device_ID_Routing_Config);

		msg_cfg.message_map = DSP1_message_map;
		msg_cfg.uiNumMessageMap=
			sizeof(DSP1_message_map)/sizeof(SRIO_RX_Message_Map);
		srio_cfg.msg_cfg= &msg_cfg;

		serdesLinkSetup.linkSpeed_GHz= SRIO_DEFAULT_TEST_SPEED;
   		srio_cfg.srio_1x2x4x_path_control= SRIO_PATH_CTL_1xLaneA_1xLaneB_1xLaneC_1xLaneD;
		srio_identify_used_ports_lanes(srio_cfg.srio_1x2x4x_path_control);

		/*On Turbo Nyquist EVM only lane 2 and 3 are connected between 2 DSPs*/
		srio_cfg.blockEn.bLogic_Port_EN[0]= FALSE;
		srio_cfg.blockEn.bLogic_Port_EN[1]= FALSE;

		//Keystone_SRIO_Init(&srio_cfg);
		SRIO_PktDM_init();

		/*accumulation is only used for the 2nd DSP for two DSPs test*/
		QMSS_Accumulation_config();

		puts("DSP1 ready as slave for test");
		while(1);
	}
}
unsigned short gxx_half_sfn = 0;
unsigned short gxx_subfn = 0;
unsigned int gxx_time_ant_data[10][30720*2];
#pragma DATA_ALIGN (gxx_time_ant_data, 32)
#pragma DATA_SECTION(gxx_time_ant_data,"PacketData.buffer_DDR")
unsigned int gtest_cycle_record[100];
void srio_send(unsigned int slot_index,unsigned int uiDspNum,char flag,int port_num)
{
	int i;

	TDSP_Type dspType;
    unsigned int cycles,nn,m;
	//printf("%s start............................................\n", test_mode_str[loopback_mode]);
    if(0 == uiDspNum)
    {
    	/*BC模块，生成下行一子帧数据***********
    	*.....
    	*.....
    	*.....
    	********完成**************************/
    	/*调用SRIO发送***********/
        if(3 == flag)//表示发送数据
    	    fxx_srio_send(slot_index,port_num);
        else if(2 == flag)//表示更新小区参数
        	fxx_srio_send_config(slot_index);

    }
	//puts("SRIO test complete.\n");
}


