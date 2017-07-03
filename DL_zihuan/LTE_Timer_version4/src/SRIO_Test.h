/*  ============================================================================
 *   Copyright (c) Texas Instruments Inc 2011
 *
 *   Use of this software is controlled by the terms and conditions found in the
 *   license agreement under which this software has been supplied.
* ============================================================================
 Example to show the usage and performance of SRIO on Keystone DSP
* =============================================================================
 *  Revision History
 *  ===============
 *  June 13, 2011 Brighton Feng   File Created
 * ============================================================================
 */
#ifndef _SRIO_TEST_H_
#define _SRIO_TEST_H_

#include "Keystone_SRIO_init.h"
#include "Keystone_SRIO_drv.h"

#define DSP0_SRIO_BASE_ID	0xA0
#define DSP1_SRIO_BASE_ID	0xB0

#define SRIO_DEFAULT_TEST_SPEED	5.0
#define INVALID_DOORBELL  11
#define CONFIG_DOORBELL 10
/*define the message convey by doorbell for this test*/
typedef enum
{
	SRIO_DOORBELL_MSG_LATENCY_TEST= 0,
	SRIO_DOORBELL_MSG_NREAD_DONE    ,
	SRIO_DOORBELL_MSG_NWRITE_DONE   ,
	SRIO_DOORBELL_MSG_NWRITE_R_DONE ,
	SRIO_DOORBELL_MSG_SWRITE_DONE   
}SRIO_Doorbell_Message;


typedef struct 
{
	SRIO_Packet_Type packet_type;

	/*for directIO test, this is source address in memory;
	for message test, this is source queue number*/
	Uint32 source; 	

	/*for directIO test, this is destination address in memory;
	for message test, this is dest device ID, different dest ID map to
	different flow to different memory in this test*/
	Uint32 dest;

	Uint32 byteCount;
	
}SRIO_Transfer_Param;

typedef struct 
{
	SRIO_1x2x4x_Path_Control multiple_port_path;
	/*test parameters for 4 ports, set 0 for unused port*/
	SRIO_Transfer_Param transfer_param[4]; 	
}SRIO_Multiple_Test_Config;
typedef struct
{
	SRIO_1x2x4x_Path_Control multiple_port_path;
	/*test parameters for 4 ports, set 0 for unused port*/
	SRIO_Transfer_Param transfer_param;
}SRIO_Multiple_Config;
typedef struct
{
	SRIO_1x2x4x_Path_Control multiple_port_path;
	/*test parameters for 4 ports, set 0 for unused port*/
	SRIO_Transfer_Param transfer_param;
	Uint16              doorbell_info;
	Uint16              pad;
}SRIO_Tx_Config;
extern SRIO_Multiple_Test_Config test_2DSP_cfg;
extern SerdesSetup_4links serdes_cfg;
extern SerdesLinkSetup serdesLinkSetup;
extern SRIO_Config srio_cfg;
extern Uint32 uiDoorbell_TSC; 	//timestamp when doorbell interrupt happens

/* identify used ports lanes according to 1x 2x 4x path configuration*/
extern void srio_identify_used_ports_lanes(
	SRIO_1x2x4x_Path_Control srio_1x2x4x_path_control);

extern char * get_packet_type_string(SRIO_Packet_Type packetType);

/*fill data before test*/
extern void InitDataBuffer(Uint32 * uipSrc, Uint32 * uipDst, Uint32 uiByteCount);

/*verify data after test*/
extern void VerifyData(Uint32 * uipSrc, Uint32 * uipDst, Uint32 uiByteCount);

extern void InitLsuTransfer(SRIO_LSU_Transfer * lsuTransfer, 
	SRIO_Transfer_Param * transferParam, Uint32 uiLsuNum, 
	Uint32 uiDstID, Uint32 uiSrcIDMap);
extern void fxx_srio_send(unsigned char slot_index,int port_num);
#endif
