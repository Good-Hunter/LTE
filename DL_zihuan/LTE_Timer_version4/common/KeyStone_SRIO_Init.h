
#ifndef _KEYSTONE_SRIO_INIT_H_
#define _KEYSTONE_SRIO_INIT_H_

#include "tistdtypes.h"
#include "cslr_srio.h"
#include "KeyStone_common.h"
#include "Keystone_Serdes_init.h"

#define SRIO_MAX_LSU_NUM 					8
#define SRIO_MAX_PORT_NUM 					4
#define SRIO_LSU0_4_MAX_SHADOW_REG_SET 		9
#define SRIO_LSU1_5_MAX_SHADOW_REG_SET 		6
#define SRIO_LSU2_6_MAX_SHADOW_REG_SET 		5
#define SRIO_LSU3_7_MAX_SHADOW_REG_SET 		4
#define SRIO_MAX_DEVICEID_NUM 				16
#define SRIO_MAX_MULTICAST_ID_NUM 			8
#define SRIO_MAX_MSG_MAP_ENTRY_NUM 			64
#define SRIO_MAX_FORWARDING_ENTRY_NUM 		64

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
typedef enum
{
	SRIO_PATH_CTL_1xLaneA = 		(1<<CSL_SRIO_RIO_PLM_SP_PATH_CTL_PATH_CONFIGURATION_SHIFT)|
		(0<<CSL_SRIO_RIO_PLM_SP_PATH_CTL_PATH_MODE_SHIFT),
		
	SRIO_PATH_CTL_1xLaneA_1xLaneB = 		(2<<CSL_SRIO_RIO_PLM_SP_PATH_CTL_PATH_CONFIGURATION_SHIFT)|
		(0<<CSL_SRIO_RIO_PLM_SP_PATH_CTL_PATH_MODE_SHIFT),

	SRIO_PATH_CTL_2xLaneAB = 		(2<<CSL_SRIO_RIO_PLM_SP_PATH_CTL_PATH_CONFIGURATION_SHIFT)|
		(1<<CSL_SRIO_RIO_PLM_SP_PATH_CTL_PATH_MODE_SHIFT),

	SRIO_PATH_CTL_1xLaneA_1xLaneB_1xLaneC_1xLaneD = 		(4<<CSL_SRIO_RIO_PLM_SP_PATH_CTL_PATH_CONFIGURATION_SHIFT)|
		(0<<CSL_SRIO_RIO_PLM_SP_PATH_CTL_PATH_MODE_SHIFT),

	SRIO_PATH_CTL_2xLaneAB_1xLaneC_1xLaneD = 		(4<<CSL_SRIO_RIO_PLM_SP_PATH_CTL_PATH_CONFIGURATION_SHIFT)|
		(1<<CSL_SRIO_RIO_PLM_SP_PATH_CTL_PATH_MODE_SHIFT),

	SRIO_PATH_CTL_1xLaneA_1xLaneB_2xLaneCD = 		(4<<CSL_SRIO_RIO_PLM_SP_PATH_CTL_PATH_CONFIGURATION_SHIFT)|
		(2<<CSL_SRIO_RIO_PLM_SP_PATH_CTL_PATH_MODE_SHIFT),

	SRIO_PATH_CTL_2xLaneAB_2xLaneCD = 		(4<<CSL_SRIO_RIO_PLM_SP_PATH_CTL_PATH_CONFIGURATION_SHIFT)|
		(3<<CSL_SRIO_RIO_PLM_SP_PATH_CTL_PATH_MODE_SHIFT),

	SRIO_PATH_CTL_4xLaneABCD = 		(4<<CSL_SRIO_RIO_PLM_SP_PATH_CTL_PATH_CONFIGURATION_SHIFT)|
		(4<<CSL_SRIO_RIO_PLM_SP_PATH_CTL_PATH_MODE_SHIFT)
}SRIO_1x2x4x_Path_Control;

#define SRIO_1x2x4x_PATH_CONTROL_MASK \
	(CSL_SRIO_RIO_PLM_SP_PATH_CTL_PATH_CONFIGURATION_MASK|\
	CSL_SRIO_RIO_PLM_SP_PATH_CTL_PATH_MODE_MASK)

typedef enum
{
	SRIO_LSU_SHADOW_REGS_SETUP_4_4_4_4,
	SRIO_LSU_SHADOW_REGS_SETUP_5_5_5_1,
	SRIO_LSU_SHADOW_REGS_SETUP_5_5_4_2,
	SRIO_LSU_SHADOW_REGS_SETUP_5_5_3_3,
	SRIO_LSU_SHADOW_REGS_SETUP_5_4_4_3,
	SRIO_LSU_SHADOW_REGS_SETUP_6_6_3_1,
	SRIO_LSU_SHADOW_REGS_SETUP_6_6_2_2,
	SRIO_LSU_SHADOW_REGS_SETUP_6_5_4_1,
	SRIO_LSU_SHADOW_REGS_SETUP_6_5_3_2,
	SRIO_LSU_SHADOW_REGS_SETUP_6_4_4_2,
	SRIO_LSU_SHADOW_REGS_SETUP_6_4_3_3,
	SRIO_LSU_SHADOW_REGS_SETUP_7_6_2_1,
	SRIO_LSU_SHADOW_REGS_SETUP_7_5_3_1,
	SRIO_LSU_SHADOW_REGS_SETUP_7_5_2_2,
	SRIO_LSU_SHADOW_REGS_SETUP_7_4_4_1,
	SRIO_LSU_SHADOW_REGS_SETUP_7_4_3_2,
	SRIO_LSU_SHADOW_REGS_SETUP_7_3_3_3,
	SRIO_LSU_SHADOW_REGS_SETUP_8_6_1_1,
	SRIO_LSU_SHADOW_REGS_SETUP_8_5_2_1,
	SRIO_LSU_SHADOW_REGS_SETUP_8_4_3_1,
	SRIO_LSU_SHADOW_REGS_SETUP_8_4_2_2,
	SRIO_LSU_SHADOW_REGS_SETUP_8_3_3_2,
	SRIO_LSU_SHADOW_REGS_SETUP_9_5_1_1,
	SRIO_LSU_SHADOW_REGS_SETUP_9_4_2_1,
	SRIO_LSU_SHADOW_REGS_SETUP_9_3_3_1,
	SRIO_LSU_SHADOW_REGS_SETUP_9_3_2_2
}SRIO_LSU_Shadow_Registers_Setup;

typedef enum {
  LSU_INT_DRIVE_BY_SRCID = 0,
  LSU_INT_DRIVE_BY_LSU_NUM
}SRIO_LSU_Interrupt_setup;

typedef struct
{
    SRIO_LSU_Shadow_Registers_Setup lsuGrp0ShadowRegsSetup;
    SRIO_LSU_Shadow_Registers_Setup lsuGrp1ShadowRegsSetup;
    SRIO_LSU_Interrupt_setup lsuIntSetup[8];
}SRIO_LSU_Cfg;

typedef enum
{
	SRIO_LITTLE_ENDIAN_SWAP_BYTE = 0,
	SRIO_LITTLE_ENDIAN_SWAP_HALF_WORD,
	SRIO_LITTLE_ENDIAN_SWAP_WORD,
	SRIO_LITTLE_ENDIAN_SWAP_DOUBLE_WORD
}SRIO_Little_Endian_Swap_Mode;

typedef enum
{
	SRIO_NO_LOOPBACK = 0,
	SRIO_DIGITAL_LOOPBACK,
	SRIO_SERDES_LOOPBACK,
	SRIO_EXTERNAL_LINE_LOOPBACK,
	SRIO_EXTERNAL_FORWARD_BACK
}SRIO_Loopback_Mode;

typedef enum
{
	DOORBELL0_ICRR1    = (0x00/4),
	DOORBELL0_ICRR2    = (0x04/4),
	DOORBELL1_ICRR1    = (0x0C/4),
	DOORBELL1_ICRR2    = (0x10/4),
	DOORBELL2_ICRR1    = (0x18/4),
	DOORBELL2_ICRR2    = (0x1C/4),
	DOORBELL3_ICRR1    = (0x24/4),
	DOORBELL3_ICRR2    = (0x28/4),
	LSU_SRCID_ICRR1    = (0x30/4),
	LSU_SRCID_ICRR2    = (0x34/4),
	LSU_SRCID_ICRR3    = (0x38/4),
	LSU_SRCID_ICRR4    = (0x3C/4),
	LSU_ICRR1          = (0x40/4),
	ERR_RST_EVNT_ICRR1 = (0x50/4),
	ERR_RST_EVNT_ICRR2 = (0x54/4),
	ERR_RST_EVNT_ICRR3 = (0x58/4)
}SRIO_ICRR_Index;

typedef enum
{
    /* SRIO interrupt source constant, 
    high 16 bits is the ICRR register index, 
    lower 16 bits is the offset of the field in the register*/
    DOORBELL0_0_INT  = ((DOORBELL0_ICRR1 << 16) | 0x0000),
    DOORBELL0_1_INT  = ((DOORBELL0_ICRR1 << 16) | 0x0004),
    DOORBELL0_2_INT  = ((DOORBELL0_ICRR1 << 16) | 0x0008),
    DOORBELL0_3_INT  = ((DOORBELL0_ICRR1 << 16) | 0x000C),
    DOORBELL0_4_INT  = ((DOORBELL0_ICRR1 << 16) | 0x0010),
    DOORBELL0_5_INT  = ((DOORBELL0_ICRR1 << 16) | 0x0014),
    DOORBELL0_6_INT  = ((DOORBELL0_ICRR1 << 16) | 0x0018),
    DOORBELL0_7_INT  = ((DOORBELL0_ICRR1 << 16) | 0x001C),
    DOORBELL0_8_INT  = ((DOORBELL0_ICRR2 << 16) | 0x0000),
    DOORBELL0_9_INT  = ((DOORBELL0_ICRR2 << 16) | 0x0004),
    DOORBELL0_10_INT = ((DOORBELL0_ICRR2 << 16) | 0x0008),
    DOORBELL0_11_INT = ((DOORBELL0_ICRR2 << 16) | 0x000C),
    DOORBELL0_12_INT = ((DOORBELL0_ICRR2 << 16) | 0x0010),
    DOORBELL0_13_INT = ((DOORBELL0_ICRR2 << 16) | 0x0014),
    DOORBELL0_14_INT = ((DOORBELL0_ICRR2 << 16) | 0x0018),
    DOORBELL0_15_INT = ((DOORBELL0_ICRR2 << 16) | 0x001C),  

    DOORBELL1_0_INT  = ((DOORBELL1_ICRR1 << 16) | 0x0000),
    DOORBELL1_1_INT  = ((DOORBELL1_ICRR1 << 16) | 0x0004),
    DOORBELL1_2_INT  = ((DOORBELL1_ICRR1 << 16) | 0x0008),
    DOORBELL1_3_INT  = ((DOORBELL1_ICRR1 << 16) | 0x000C),
    DOORBELL1_4_INT  = ((DOORBELL1_ICRR1 << 16) | 0x0010),
    DOORBELL1_5_INT  = ((DOORBELL1_ICRR1 << 16) | 0x0014),
    DOORBELL1_6_INT  = ((DOORBELL1_ICRR1 << 16) | 0x0018),
    DOORBELL1_7_INT  = ((DOORBELL1_ICRR1 << 16) | 0x001C),
    DOORBELL1_8_INT  = ((DOORBELL1_ICRR2 << 16) | 0x0000),
    DOORBELL1_9_INT  = ((DOORBELL1_ICRR2 << 16) | 0x0004),
    DOORBELL1_10_INT = ((DOORBELL1_ICRR2 << 16) | 0x0008),
    DOORBELL1_11_INT = ((DOORBELL1_ICRR2 << 16) | 0x000C),
    DOORBELL1_12_INT = ((DOORBELL1_ICRR2 << 16) | 0x0010),
    DOORBELL1_13_INT = ((DOORBELL1_ICRR2 << 16) | 0x0014),
    DOORBELL1_14_INT = ((DOORBELL1_ICRR2 << 16) | 0x0018),
    DOORBELL1_15_INT = ((DOORBELL1_ICRR2 << 16) | 0x001C),  

    DOORBELL2_0_INT  = ((DOORBELL2_ICRR1 << 16) | 0x0000),
    DOORBELL2_1_INT  = ((DOORBELL2_ICRR1 << 16) | 0x0004),
    DOORBELL2_2_INT  = ((DOORBELL2_ICRR1 << 16) | 0x0008),
    DOORBELL2_3_INT  = ((DOORBELL2_ICRR1 << 16) | 0x000C),
    DOORBELL2_4_INT  = ((DOORBELL2_ICRR1 << 16) | 0x0010),
    DOORBELL2_5_INT  = ((DOORBELL2_ICRR1 << 16) | 0x0014),
    DOORBELL2_6_INT  = ((DOORBELL2_ICRR1 << 16) | 0x0018),
    DOORBELL2_7_INT  = ((DOORBELL2_ICRR1 << 16) | 0x001C),
    DOORBELL2_8_INT  = ((DOORBELL2_ICRR2 << 16) | 0x0000),
    DOORBELL2_9_INT  = ((DOORBELL2_ICRR2 << 16) | 0x0004),
    DOORBELL2_10_INT = ((DOORBELL2_ICRR2 << 16) | 0x0008),
    DOORBELL2_11_INT = ((DOORBELL2_ICRR2 << 16) | 0x000C),
    DOORBELL2_12_INT = ((DOORBELL2_ICRR2 << 16) | 0x0010),
    DOORBELL2_13_INT = ((DOORBELL2_ICRR2 << 16) | 0x0014),
    DOORBELL2_14_INT = ((DOORBELL2_ICRR2 << 16) | 0x0018),
    DOORBELL2_15_INT = ((DOORBELL2_ICRR2 << 16) | 0x001C),  

    DOORBELL3_0_INT  = ((DOORBELL3_ICRR1 << 16) | 0x0000),
    DOORBELL3_1_INT  = ((DOORBELL3_ICRR1 << 16) | 0x0004),
    DOORBELL3_2_INT  = ((DOORBELL3_ICRR1 << 16) | 0x0008),
    DOORBELL3_3_INT  = ((DOORBELL3_ICRR1 << 16) | 0x000C),
    DOORBELL3_4_INT  = ((DOORBELL3_ICRR1 << 16) | 0x0010),
    DOORBELL3_5_INT  = ((DOORBELL3_ICRR1 << 16) | 0x0014),
    DOORBELL3_6_INT  = ((DOORBELL3_ICRR1 << 16) | 0x0018),
    DOORBELL3_7_INT  = ((DOORBELL3_ICRR1 << 16) | 0x001C),
    DOORBELL3_8_INT  = ((DOORBELL3_ICRR2 << 16) | 0x0000),
    DOORBELL3_9_INT  = ((DOORBELL3_ICRR2 << 16) | 0x0004),
    DOORBELL3_10_INT = ((DOORBELL3_ICRR2 << 16) | 0x0008),
    DOORBELL3_11_INT = ((DOORBELL3_ICRR2 << 16) | 0x000C),
    DOORBELL3_12_INT = ((DOORBELL3_ICRR2 << 16) | 0x0010),
    DOORBELL3_13_INT = ((DOORBELL3_ICRR2 << 16) | 0x0014),
    DOORBELL3_14_INT = ((DOORBELL3_ICRR2 << 16) | 0x0018),
    DOORBELL3_15_INT = ((DOORBELL3_ICRR2 << 16) | 0x001C),  

    SRCID0_Transaction_Complete_OK = ((LSU_SRCID_ICRR1 << 16) | 0x0000),
    SRCID1_Transaction_Complete_OK = ((LSU_SRCID_ICRR1 << 16) | 0x0004),
    SRCID2_Transaction_Complete_OK = ((LSU_SRCID_ICRR1 << 16) | 0x0008),
    SRCID3_Transaction_Complete_OK = ((LSU_SRCID_ICRR1 << 16) | 0x000C),
    SRCID4_Transaction_Complete_OK = ((LSU_SRCID_ICRR1 << 16) | 0x0010),
    SRCID5_Transaction_Complete_OK = ((LSU_SRCID_ICRR1 << 16) | 0x0014),
    SRCID6_Transaction_Complete_OK = ((LSU_SRCID_ICRR1 << 16) | 0x0018),
    SRCID7_Transaction_Complete_OK = ((LSU_SRCID_ICRR1 << 16) | 0x001C),

    SRCID8_Transaction_Complete_OK =  ((LSU_SRCID_ICRR2 << 16) | 0x0000),
    SRCID9_Transaction_Complete_OK =  ((LSU_SRCID_ICRR2 << 16) | 0x0004),
    SRCID10_Transaction_Complete_OK = ((LSU_SRCID_ICRR2 << 16) | 0x0008),
    SRCID11_Transaction_Complete_OK = ((LSU_SRCID_ICRR2 << 16) | 0x000C),
    SRCID12_Transaction_Complete_OK = ((LSU_SRCID_ICRR2 << 16) | 0x0010),
    SRCID13_Transaction_Complete_OK = ((LSU_SRCID_ICRR2 << 16) | 0x0014),
    SRCID14_Transaction_Complete_OK = ((LSU_SRCID_ICRR2 << 16) | 0x0018),
    SRCID15_Transaction_Complete_OK = ((LSU_SRCID_ICRR2 << 16) | 0x001C),

    SRCID0_Transaction_Complete_ERR = ((LSU_SRCID_ICRR3 << 16) | 0x0000),
    SRCID1_Transaction_Complete_ERR = ((LSU_SRCID_ICRR3 << 16) | 0x0004),
    SRCID2_Transaction_Complete_ERR = ((LSU_SRCID_ICRR3 << 16) | 0x0008),
    SRCID3_Transaction_Complete_ERR = ((LSU_SRCID_ICRR3 << 16) | 0x000C),
    SRCID4_Transaction_Complete_ERR = ((LSU_SRCID_ICRR3 << 16) | 0x0010),
    SRCID5_Transaction_Complete_ERR = ((LSU_SRCID_ICRR3 << 16) | 0x0014),
    SRCID6_Transaction_Complete_ERR = ((LSU_SRCID_ICRR3 << 16) | 0x0018),
    SRCID7_Transaction_Complete_ERR = ((LSU_SRCID_ICRR3 << 16) | 0x001C),

    SRCID8_Transaction_Complete_ERR =  ((LSU_SRCID_ICRR4 << 16) | 0x0000),
    SRCID9_Transaction_Complete_ERR =  ((LSU_SRCID_ICRR4 << 16) | 0x0004),
    SRCID10_Transaction_Complete_ERR = ((LSU_SRCID_ICRR4 << 16) | 0x0008),
    SRCID11_Transaction_Complete_ERR = ((LSU_SRCID_ICRR4 << 16) | 0x000C),
    SRCID12_Transaction_Complete_ERR = ((LSU_SRCID_ICRR4 << 16) | 0x0010),
    SRCID13_Transaction_Complete_ERR = ((LSU_SRCID_ICRR4 << 16) | 0x0014),
    SRCID14_Transaction_Complete_ERR = ((LSU_SRCID_ICRR4 << 16) | 0x0018),
    SRCID15_Transaction_Complete_ERR = ((LSU_SRCID_ICRR4 << 16) | 0x001C),   

    LSU0_Transaction_Complete_OK = ((LSU_ICRR1 << 16) | 0x0000),
    LSU1_Transaction_Complete_OK = ((LSU_ICRR1 << 16) | 0x0004),
    LSU2_Transaction_Complete_OK = ((LSU_ICRR1 << 16) | 0x0008),
    LSU3_Transaction_Complete_OK = ((LSU_ICRR1 << 16) | 0x000C),
    LSU4_Transaction_Complete_OK = ((LSU_ICRR1 << 16) | 0x0010),
    LSU5_Transaction_Complete_OK = ((LSU_ICRR1 << 16) | 0x0014),
    LSU6_Transaction_Complete_OK = ((LSU_ICRR1 << 16) | 0x0018),
    LSU7_Transaction_Complete_OK = ((LSU_ICRR1 << 16) | 0x001C),

	Multicast_event        = ((ERR_RST_EVNT_ICRR1<<16)|0),
	Port_write_In_received = ((ERR_RST_EVNT_ICRR1<<16)|4),
	Logical_Layer_Error    = ((ERR_RST_EVNT_ICRR1<<16)|8),
	Port0_Error            = ((ERR_RST_EVNT_ICRR2<<16)|0),
	Port1_Error            = ((ERR_RST_EVNT_ICRR2<<16)|4),
	Port2_Error            = ((ERR_RST_EVNT_ICRR2<<16)|8),
	Port3_Error            = ((ERR_RST_EVNT_ICRR2<<16)|12),
	Device_Reset           = ((ERR_RST_EVNT_ICRR3<<16)|0)
}SRIO_Interrupt_Source;


typedef enum
{
    /* SRIO interrupt destiniation */
    INTDST_0 = 0,
    INTDST_1 = 1,
    INTDST_2 = 2,
    INTDST_3 = 3,
    INTDST_4 = 4,
    INTDST_5 = 5,
    INTDST_6 = 6,
    INTDST_7 = 7,
    INTDST_8 = 8,
    INTDST_9 = 9,
    INTDST_10 = 10,
    INTDST_11 = 11,
    INTDST_12 = 12,
    INTDST_13 = 13,
    INTDST_14 = 14,
    INTDST_15 = 15,
    /* doorbell only */
    INTDST_16 = 0,
    INTDST_17 = 1,
    INTDST_18 = 2,
    INTDST_19 = 3,
    INTDST_20 = 4,
    INTDST_21 = 5,
    INTDST_22 = 6,
    INTDST_23 = 7
}SRIO_Interrupt_Dest;

typedef struct
{
    SRIO_Interrupt_Source interrupt_event;
    SRIO_Interrupt_Dest INTDST_number;
}SRIO_Interrupt_Map;

typedef struct
{
    SRIO_Interrupt_Dest INTDST_number;
    Uint32 interrupt_rate_counter;
}SRIO_Interrupt_Rate;

typedef enum
{
	SRIO_DOORBELL_ROUTE_TO_DEDICATE_INT=0,
	SRIO_DOORBELL_ROUTE_TO_GENERAL_INT
}SRIO_Doorbell_Route_Coutrol;

typedef struct
{
    SRIO_Interrupt_Map * interrupt_map;
    Uint32 uiNumInterruptMap;
    SRIO_Interrupt_Rate * interrupt_rate;
    Uint32 uiNumInterruptRateCfg; 	/*number of INTDST with rate conifguration*/
    SRIO_Doorbell_Route_Coutrol doorbell_route_ctl;
}SRIO_Interrupt_Cfg;

typedef struct  { 
    Uint16 idPattern; 	/*PATTERN provides the 16 bits that are compared one-for-one against the inbound destID.*/
    Uint16 idMatchMask; /*MATCH indicates which of the 16 bits of the inbound destID are compared against PATTERN.*/
    Uint8 routeMaintenance; /*maintenance request/reserved packets with destIDs which match this BRR are routed to the LLM*/
} SRIO_Device_ID_Routing_Config;

typedef struct  { 
	Bool accept_maintenance_with_any_ID; 	

	/*if accept_data_with_any_ID, no packet will be forwarding*/
	Bool support_multicast_forwarding; 
}SRIO_Port_RX_Mode;

typedef struct  { 
	/*if accept_data_with_any_ID, no packet will be forwarding*/
	Bool accept_data_with_any_ID;
	SRIO_Port_RX_Mode port_rx_mode[4];
}SRIO_RX_Mode;

typedef struct  { 
	Bool bBLK1_LSU_EN  ;
	Bool bBLK2_MAU_EN  ;
	Bool bBLK3_TXU_EN  ;
	Bool bBLK4_RXU_EN  ;
	Bool bBLK5_8_Port_Datapath_EN[4];
	Bool bLogic_Port_EN[4];
}SRIO_Block_Enable;

/*
 * This structure is used to determine the 
 * receive flow and queue where the packet is pushed to.
 */
typedef struct
{
	/*map matched message to flowID and optional destQuID*/
    Uint8   flowId;
	Uint16  destQuID;

	/*common fields matching*/
    Uint16  dstId;
    Uint8   dstProm;
    Uint16  srcId;
    Uint8   srcProm;
    Uint8   tt;

	/*type 11 message fields matching*/
    Uint8   mbx;
    Uint8   mbxMask;
    Uint8   ltr;
    Uint8   ltrMask;
    Uint8   segMap;

	/*type 9 message fields matching*/
    Uint8   cos;
    Uint8   cosMask;
    Uint16  streamId;
    Uint16  streamMask;

}SRIO_RX_Message_Map;

typedef struct
{
	Uint8 outputPort;
	Uint8 priority;
	Uint8 CRF;
}SRIO_TX_Queue_Sch_Info;

typedef struct
{
    /* Maximum PDU */
    Uint16 MaxPDU;
    /* The number of segmentation contexts supported by the destination endpoint */
    Uint16 SegSupport;
    /* Maximum Transmission Unit */
    Uint8  MTU;
	/*COS is a part of the segmentation context for the RXU*/
    Bool COS_EN; 
}SRIO_Datastreaming_Cfg;

typedef struct
{
	SRIO_RX_Message_Map * message_map;
	Uint32 uiNumMessageMap;
	SRIO_TX_Queue_Sch_Info * TX_Queue_Sch_Info;
	Uint32 uiNumTxQueue;
	Uint16 rx_size_error_garbage_Q;
	Uint16 rx_timeout_garbage_Q;
	Uint16 tx_excessive_retries_garbage_Q;
	Uint16 tx_error_garbage_Q;
	Uint16 tx_size_error_garbage_Q;
    SRIO_Datastreaming_Cfg * datastreaming_cfg;
}SRIO_Message_Cfg;

typedef struct
{
    Uint16 forwardingID_up_8;
    Uint16 forwardingID_lo_8;

    Uint16 forwardingID_up_16;
    Uint16 forwardingID_lo_16;

    Uint32 outport;
    
}SRIO_PktForwarding_Cfg;

typedef struct
{
    Uint16 multicast_16b_ID;
    Uint16 multicast_8b_ID;
}SRIO_Multicast_ID_Cfg;

typedef struct
{
    Uint16 flow_control_ID;
    Uint16 tt; 	/*ID length*/
}SRIO_Flow_Control_ID;

typedef struct
{
    Uint32 uiVbusPriority;

    /*when a host access DSP with DirectIO mode, the memory access 
    permission is set as supervisor if its device ID match below ID*/
    Uint16 supervisorHostID_8b; 	
    Uint16 supervisorHostID_16b;
}SRIO_priority_permission;

typedef struct  { 
	SRIO_Block_Enable blockEn;
	
	SRIO_1x2x4x_Path_Control srio_1x2x4x_path_control;
	
	SRIO_Device_ID_Routing_Config * device_ID_routing_config;
	Uint32 uiNumDeviceId;

	SRIO_Loopback_Mode loopback_mode;

	SerdesSetup_4links * serdes_cfg;

	SRIO_LSU_Cfg * lsu_cfg;

	SRIO_Message_Cfg * msg_cfg;

    SRIO_Interrupt_Cfg * interrupt_cfg;
    
    SRIO_RX_Mode * rxMode;
        
    SRIO_PktForwarding_Cfg * PktForwardingEntry_cfg;
    Uint32 uiNumPktForwardingEntry;

    SRIO_Multicast_ID_Cfg * multicastID;
    Uint32 uiNumMulticastID;

    SRIO_Flow_Control_ID * flowControlID;
    Uint32 uiNumFlowControlID;

    SRIO_priority_permission * priority_permission;

} SRIO_Config;

extern SerdesRegs * srioSerdesRegs;

/*enable globally used blocks including MMR block in SRIO*/
extern void Keystone_SRIO_GlobalEnable(void);

/*Enable SRIO blocks*/
extern void Keystone_SRIO_enable_blocks(
	SRIO_Block_Enable * blockEn);

/*Disable all SRIO blocks*/
extern void Keystone_SRIO_disable_all_blocks();

/*soft shutdown and reset SRIO*/
extern void Keystone_SRIO_soft_reset();

/*configure SRIO 1x 2x or 4x path mode*/
extern void Keystone_SRIO_set_1x2x4x_Path(
	SRIO_1x2x4x_Path_Control srio_1x2x4x_path_control);

/*configure SRIO device ID*/
extern void Keystone_SRIO_set_device_ID(
	SRIO_Device_ID_Routing_Config * device_id_routing_config,
	Uint32 uiDeviceIdNum);

/*configure the map between message and PacketDMA flow and queue*/
extern void Keystone_map_SRIO_RX_message(SRIO_RX_Message_Map * srio_message_map,
	Uint32 uiNumMessageMap);

/*configure the datastreaming parameters */
extern void Keystone_map_SRIO_Datastreaming_init(SRIO_Datastreaming_Cfg * datastreaming_cfg);

/*configure SRIO basic functions */
extern void Keystone_SRIO_set_function(SRIO_Device_ID_Routing_Config * device_id_routing_config);

/* configure SRIO packet forwarding */
extern void Keystone_SRIO_packet_forwarding(
	SRIO_PktForwarding_Cfg * PktForwardingEntry_cfg,
	Uint32 pktForwardingEntryNum);

/* interrupt map */
extern void Keystone_SRIO_Interrupt_init(
	SRIO_Interrupt_Cfg * interrupt_cfg);

extern void Keystone_SRIO_little_endian_swap(
	SRIO_Little_Endian_Swap_Mode leSwapMode);


extern void Keystone_SRIO_Init(SRIO_Config * srio_cfg);

#endif
