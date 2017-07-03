
#ifndef _KEYSTONE_SRIO_DRV_H_
#define _KEYSTONE_SRIO_DRV_H_

#include "tistdtypes.h"
#include "Keystone_packet_descriptor.h"

typedef enum
{
	SRIO_PKT_TYPE_NREAD     = 0x24,
	SRIO_PKT_TYPE_NWRITE    = 0x54,
	SRIO_PKT_TYPE_NWRITE_R  = 0x55,
	SRIO_PKT_TYPE_SWRITE    = 0x60,
	SRIO_PKT_TYPE_MTN_READ  = 0x80,
	SRIO_PKT_TYPE_MTN_WRITE = 0x81,
	SRIO_PKT_TYPE9_STREAM   = 0x90,
	SRIO_PKT_TYPE_DOORBELL  = 0xA0,
	SRIO_PKT_TYPE11_MESSAGE = 0xB0
}SRIO_Packet_Type;

typedef enum
{
	SRIO_TYPE9_CPPI_PACKET  = 30,
	SRIO_TYPE11_CPPI_PACKET = 31
}SRIO_CPPI_Packet_Type;

typedef enum
{
	SRIO_SSIZE_8_BYTES   = 9,
	SRIO_SSIZE_16_BYTES  , 
	SRIO_SSIZE_32_BYTES  , 
	SRIO_SSIZE_64_BYTES  , 
	SRIO_SSIZE_128_BYTES , 
	SRIO_SSIZE_256_BYTES  
}SRIO_message_ssize;

/** @brief SRIO LSU Transfer
 * This structure is used to configure LSU module for Transfer
 */
typedef struct SRIO_LSU_Transfer 
{
    Uint32  rioAddressMSB;
    Uint32  rioAddressLSB_ConfigOffset;
    Uint32  localDspAddress;
    Uint32  bytecount; 	
    SRIO_Packet_Type   packetType;

    Uint16  dstID;
    Uint16  doorbellInfo;
    Bool    waitLsuReady; /*if BUSY or FULL is set, should we wait for them?*/

    Uint8   lsuNum; 	/*the LSU used for this transfer*/
    Uint8   doorbellValid;
    Uint8   intrRequest;
    Uint8   supGoodInt;
    Uint8   priority;
    Uint8   outPortID;
    Uint8   idSize;
    Uint8   srcIDMap;
    Uint8   hopCount;

	/*transaction ID returned to caller for completion check */
    Uint8   transactionID;
	/*transaction contextBit returned to caller for completion check */
    Uint8   contextBit;
}SRIO_LSU_Transfer;

extern Uint8 LSU_state_index_table[SRIO_MAX_LSU_NUM][SRIO_LSU0_4_MAX_SHADOW_REG_SET];

/*
the maximum size per LSU transfer block is 1MB, if transfer->byteCount 
larger than 1MB, this function only submit transfer for 1MB. After transfer
submission, the transfer->byteCount is decreased to indicate the remaining 
bytes, the transfer addresses are increased to indicate the start address
for next possible transfer. The caller can check these to take proper action.
Transcation ID and context bit are recorded in transfer->transactionID
and transfer->contextBit for caller to check completion status.
*/
extern void Keystone_SRIO_LSU_transfer(SRIO_LSU_Transfer * transfer);

/*wait LSU transfer completion, return completion code*/
extern Uint32 Keystone_SRIO_wait_LSU_completion(Uint32 lsuNum,
	Uint32 transactionID, Uint32 contextBit);

/*read completion code and context bit*/
extern Uint32 Keystone_SRIO_get_LSU_completion_context_code(Uint32 lsuNum,
	Uint32 transactionID);

//SRIO DirectIO operation
extern Int32 Keystone_SRIO_DirectIO(Uint32 uiLocalAddress, Uint32 uiRemoteAddress, 
	Uint32 uiDestID, Uint32 uiByteCount, Uint32 uiPort, Uint32 uiLSU_No, 
	SRIO_Packet_Type packetType);

//SRIO maintenance operation
extern Int32 Keystone_SRIO_Maintenance(Uint32 uiPort, Uint32 uiLSU_No,
	Uint32 uiDestID, Uint32 uiOffSet, Uint32 uiLocalAddress, 
	SRIO_Packet_Type uiPacketType);

//Send SRIO DoorBell packet
extern Int32 Keystone_SRIO_DoorBell(Uint32 uiPort, Uint32 uiLSU_No, 
	Uint32 uiDestID, Uint32 uiDoorBellInfo);

/* Make the ACK_ID of both sides match */
extern void Keystone_SRIO_match_ACK_ID(Uint32 uiLocalPort,
	Uint32 uiDestID, Uint32 uiRemotePort);

//Build SRIO type11 message descriptor
extern void Keystone_SRIO_Build_Type11_Msg_Desc(
	HostPacketDescriptor * hostDescriptor, Uint32 uiSrcID, Uint32 uiDestID, 
	Uint32 uiByteCount, Uint32 uiMailBox, Uint32 uiLetter);

//Build SRIO type9 data stream message Descriptror
extern void Keystone_SRIO_Build_Type9_Msg_Desc(
	HostPacketDescriptor * hostDescriptor, Uint32 uiSrcID, Uint32 uiDestID, 
	Uint32 uiByteCount, Uint32 uiStreamID, Uint32 uiCOS);

#endif
