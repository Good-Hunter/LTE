
#include <stdio.h>
#include "csl_srioAux.h"
#include "Keystone_common.h"
#include "Keystone_SRIO_init.h"
#include "Keystone_SRIO_drv.h"

/*
the maximum size per LSU transfer block is 1MB, if transfer->byteCount 
larger than 1MB, this function only submit transfer for 1MB. After transfer
submission, the transfer->byteCount is decreased to indicate the remaining 
bytes, the transfer addresses are increased to indicate the start address
for next possible transfer. The caller can check these to take proper action.
Transcation ID and context bit are recorded in transfer->transactionID
and transfer->contextBit for caller to check completion status.
*/
void Keystone_SRIO_LSU_transfer(SRIO_LSU_Transfer * transfer)
{
	unsigned int   lsuNum= transfer->lsuNum;
	unsigned int   uiByteCount;
	unsigned int   uiReg6;
	unsigned long long ullRioAddress;

	/*check if LSU is busy or full*/
	uiReg6 = srioRegs->LSU_CMD[lsuNum].RIO_LSU_REG6;
	while(uiReg6&(CSL_SRIO_RIO_LSU_REG6_BUSY_MASK
		|CSL_SRIO_RIO_LSU_REG6_FULL_MASK))
	{
		if(FALSE==transfer->waitLsuReady) /*should we wait?*/
			return;

		uiReg6 = srioRegs->LSU_CMD[lsuNum].RIO_LSU_REG6;
	}

	/*record the transcation ID and context bit*/
	transfer->transactionID= uiReg6&
		CSL_SRIO_RIO_LSU_REG6_LTID_MASK;
	transfer->contextBit= (uiReg6&CSL_SRIO_RIO_LSU_REG6_LCB_MASK)>>
		CSL_SRIO_RIO_LSU_REG6_LCB_SHIFT;

	/*the maximum size per LSU transfer block is 1MB, if transfer->byteCount 
	larger than 1MB, this function only submit transfer for 1MB*/
	if(transfer->bytecount>=1024*1024)
		uiByteCount= 1024*1024;
	else
		uiByteCount= transfer->bytecount;

	/*submit transfer*/
	srioRegs->LSU_CMD[lsuNum].RIO_LSU_REG0= transfer->rioAddressMSB;
	srioRegs->LSU_CMD[lsuNum].RIO_LSU_REG1= transfer->rioAddressLSB_ConfigOffset;
	srioRegs->LSU_CMD[lsuNum].RIO_LSU_REG2= transfer->localDspAddress;

	srioRegs->LSU_CMD[lsuNum].RIO_LSU_REG3= 
    	((uiByteCount&0xFFFFF)<<CSL_SRIO_RIO_LSU_REG3_BYTE_COUNT_SHIFT)|
    	(transfer->doorbellValid<<CSL_SRIO_RIO_LSU_REG3_DRBLL_VALUE_SHIFT);

	srioRegs->LSU_CMD[lsuNum].RIO_LSU_REG4= 
    	(transfer->intrRequest<<CSL_SRIO_RIO_LSU_REG4_INT_REQ_SHIFT)|
    	(transfer->srcIDMap<<CSL_SRIO_RIO_LSU_REG4_SRCID_MAP_SHIFT)|
    	(transfer->supGoodInt<<CSL_SRIO_RIO_LSU_REG4_SUP_GINT_SHIFT)|
    	(transfer->dstID<<CSL_SRIO_RIO_LSU_REG4_DESTID_SHIFT)|
    	(transfer->idSize<<CSL_SRIO_RIO_LSU_REG4_ID_SIZE_SHIFT)|
    	(0<<CSL_SRIO_RIO_LSU_REG4_XAMBS_SHIFT)|
    	(transfer->priority<<CSL_SRIO_RIO_LSU_REG4_PRIORITY_SHIFT)|
    	(transfer->outPortID<<CSL_SRIO_RIO_LSU_REG4_OUTPORTID_SHIFT);

	srioRegs->LSU_CMD[lsuNum].RIO_LSU_REG5= transfer->packetType|
		(transfer->hopCount<<CSL_SRIO_RIO_LSU_REG5_HOP_COUNT_SHIFT)|
		(transfer->doorbellInfo<<CSL_SRIO_RIO_LSU_REG5_DRBLL_INFO_SHIFT);

	/*update the byte count and address after submit the transfer*/
	transfer->bytecount -= uiByteCount;
	transfer->localDspAddress += uiByteCount;
	ullRioAddress= _itoll(transfer->rioAddressMSB, 
		transfer->rioAddressLSB_ConfigOffset)+ uiByteCount;
	transfer->rioAddressLSB_ConfigOffset= _loll(ullRioAddress);
	transfer->rioAddressMSB= _hill(ullRioAddress);

}

/* LSU states are in 6 LSU state registers as below:
--------------------------------------------------------------------------------------------------------------
               |31      28|         24|         20|         16|         12|          8|          4|          0|
---------------|----------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|
LSU_STAT_REG0  |Lsu0_Stat7| Lsu0_Stat6| Lsu0_Stat5| Lsu0_Stat4| Lsu0_Stat3| Lsu0_Stat2| Lsu0_Stat1| Lsu0_Stat0|
LSU_STAT_REG1  |Lsu2_Stat0| Lsu1_Stat5| Lsu1_Stat4| Lsu1_Stat3| Lsu1_Stat2| Lsu1_Stat1| Lsu1_Stat0| Lsu0_Stat8|
LSU_STAT_REG2  |Lsu3_Stat3| Lsu3_Stat2| Lsu3_Stat1| Lsu3_Stat0| Lsu2_Stat4| Lsu2_Stat3| Lsu2_Stat2| Lsu2_Stat1|
LSU_STAT_REG3  |Lsu4_Stat7| Lsu4_Stat6| Lsu4_Stat5| Lsu4_Stat4| Lsu4_Stat3| Lsu4_Stat2| Lsu4_Stat1| Lsu4_Stat0|
LSU_STAT_REG4  |Lsu6_Stat0| Lsu5_Stat5| Lsu5_Stat4| Lsu5_Stat3| Lsu5_Stat2| Lsu5_Stat1| Lsu5_Stat0| Lsu4_Stat8|
LSU_STAT_REG5  |Lsu7_Stat3| Lsu7_Stat2| Lsu7_Stat1| Lsu7_Stat0| Lsu6_Stat4| Lsu6_Stat3| Lsu6_Stat2| Lsu6_Stat1|
---------------------------------------------------------------------------------------------------------------
below is a table to indicate the index of state for a transaction of a LSU 
in the LSU state registers
*/
unsigned char LSU_state_index_table[SRIO_MAX_LSU_NUM][SRIO_LSU0_4_MAX_SHADOW_REG_SET]=
{
	/*LSU0*/ {0, 1, 2, 3, 4, 5, 6, 7, 8},
	/*LSU1*/ {9, 10, 11, 12, 13, 14},
	/*LSU2*/ {15, 16, 17, 18, 19},
	/*LSU3*/ {20, 21, 22, 23},
	/*LSU4*/ {24, 25, 26, 27, 28, 29, 30, 31, 32},
	/*LSU5*/ {33, 34, 35, 36, 37, 38},
	/*LSU6*/ {39, 40, 41, 42, 43},
	/*LSU7*/ {44, 45, 46, 47}
};
/*wait LSU transfer completion, return completion code*/
unsigned int Keystone_SRIO_wait_LSU_completion(unsigned int lsuNum,
	unsigned int transactionID, unsigned int contextBit)
{
	unsigned int uiStateIndex= LSU_state_index_table[lsuNum][transactionID]; 
	unsigned int uiCompletionCode;

	do
	{
		uiCompletionCode=(srioRegs->LSU_STAT_REG[uiStateIndex/8]>>
			((uiStateIndex&7)*4))&0xF;
	}
	while((uiCompletionCode&1) != contextBit);

	return (uiCompletionCode>>1);
}

/*read completion code and context bit*/
unsigned int Keystone_SRIO_get_LSU_completion_context_code(unsigned int lsuNum,
	unsigned int transactionID)
{
	unsigned int uiStateIndex= LSU_state_index_table[lsuNum][transactionID]; 

	return ((srioRegs->LSU_STAT_REG[uiStateIndex/8]>>((uiStateIndex&7)*4))&0xF);
}

//SRIO DirectIO operation
signed int Keystone_SRIO_DirectIO(unsigned int uiLocalAddress, unsigned int uiRemoteAddress, 
	unsigned int uiDestID, unsigned int uiByteCount, unsigned int uiPort, unsigned int uiLSU_No, 
	SRIO_Packet_Type packetType)
{
	SRIO_LSU_Transfer lsuTransfer;

    lsuTransfer.rioAddressMSB=0;
    lsuTransfer.rioAddressLSB_ConfigOffset= uiRemoteAddress;
    lsuTransfer.localDspAddress= uiLocalAddress;
    lsuTransfer.bytecount= uiByteCount; 	
    lsuTransfer.packetType= packetType;
    lsuTransfer.dstID= uiDestID;
    lsuTransfer.doorbellInfo= 0;
    lsuTransfer.waitLsuReady= 1;
    lsuTransfer.lsuNum= uiLSU_No;
    lsuTransfer.doorbellValid = 0;
    lsuTransfer.intrRequest = 0;
    lsuTransfer.supGoodInt = 0;
    lsuTransfer.priority = 0;
    lsuTransfer.outPortID = uiPort;
    lsuTransfer.idSize = 0;
    lsuTransfer.srcIDMap = 0;
    lsuTransfer.hopCount = 0;

	Keystone_SRIO_LSU_transfer(&lsuTransfer);

	return Keystone_SRIO_wait_LSU_completion(uiLSU_No,
		lsuTransfer.transactionID, lsuTransfer.contextBit);

}

//SRIO maintenance operation
signed int Keystone_SRIO_Maintenance(unsigned int uiPort, unsigned int uiLSU_No,
	unsigned int uiDestID, unsigned int uiOffSet, unsigned int uiLocalAddress, 
	SRIO_Packet_Type packetType)
{
	SRIO_LSU_Transfer lsuTransfer;
	unsigned int uiCompletionCode;
#ifdef _LITTLE_ENDIAN
	unsigned int * uipData = (unsigned int *)uiLocalAddress;

	//swap maintenance value for little endian
	*uipData= _swap4(_packlh2(*uipData, *uipData));	
#endif

    lsuTransfer.rioAddressMSB=0;
    lsuTransfer.rioAddressLSB_ConfigOffset= uiOffSet;
    lsuTransfer.localDspAddress= uiLocalAddress;
    lsuTransfer.bytecount= 4; 	
    lsuTransfer.packetType= packetType;
    lsuTransfer.dstID= uiDestID;
    lsuTransfer.doorbellInfo= 0;
    lsuTransfer.waitLsuReady= 1;
    lsuTransfer.lsuNum= uiLSU_No;
    lsuTransfer.doorbellValid = 0;
    lsuTransfer.intrRequest = 0;
    lsuTransfer.supGoodInt = 0;
    lsuTransfer.priority = 0;
    lsuTransfer.outPortID = uiPort;
    lsuTransfer.idSize = 0;
    lsuTransfer.srcIDMap = 0;
    lsuTransfer.hopCount = 1;

	Keystone_SRIO_LSU_transfer(&lsuTransfer);

	uiCompletionCode= Keystone_SRIO_wait_LSU_completion(uiLSU_No,
		lsuTransfer.transactionID, lsuTransfer.contextBit);

#ifdef _LITTLE_ENDIAN
	//swap maintenance value for little endian
	*uipData= _swap4(_packlh2(*uipData, *uipData));	
#endif

	return uiCompletionCode;

}

//Send SRIO DoorBell packet
signed int Keystone_SRIO_DoorBell(unsigned int uiPort, unsigned int uiLSU_No,
	unsigned int uiDestID, unsigned int uiDoorBellInfo)
{
	SRIO_LSU_Transfer lsuTransfer;

    lsuTransfer.rioAddressMSB=0;
    lsuTransfer.rioAddressLSB_ConfigOffset= 0;
    lsuTransfer.localDspAddress= 0;
    lsuTransfer.bytecount= 4; 	
    lsuTransfer.packetType= SRIO_PKT_TYPE_DOORBELL;
    lsuTransfer.dstID= uiDestID;
    lsuTransfer.doorbellInfo= uiDoorBellInfo;
    lsuTransfer.waitLsuReady= 1;
    lsuTransfer.lsuNum= uiLSU_No;
    lsuTransfer.doorbellValid = 0;
    lsuTransfer.intrRequest = 0;
    lsuTransfer.supGoodInt = 0;
    lsuTransfer.priority = 0;
    lsuTransfer.outPortID = uiPort;
    lsuTransfer.idSize = 0;
    lsuTransfer.srcIDMap = 0;
    lsuTransfer.hopCount = 0;

	Keystone_SRIO_LSU_transfer(&lsuTransfer);

	return Keystone_SRIO_wait_LSU_completion(uiLSU_No,
		lsuTransfer.transactionID, lsuTransfer.contextBit);

}

/* Make the ACK_ID of both sides match */
void Keystone_SRIO_match_ACK_ID(unsigned int uiLocalPort,
	unsigned int uiDestID, unsigned int uiRemotePort)
{
	unsigned int uiMaintenanceValue, uiResult;
	unsigned int uiLocal_In_ACK_ID, uiRemote_In_ACK_ID, uiRemote_out_ACK_ID;

	//send a "restart-from-error" commond, request the ACK_ID of the other side
	srioRegs->RIO_SP[uiLocalPort].RIO_SP_LM_REQ=4; 	

	//wait for link response
	while(0==(srioRegs->RIO_SP[uiLocalPort].RIO_SP_LM_RESP>>
		CSL_SRIO_RIO_SP_LM_RESP_RESP_VALID_SHIFT))
		asm(" nop 5");

	uiRemote_In_ACK_ID= (srioRegs->RIO_SP[uiLocalPort].RIO_SP_LM_RESP&
		CSL_SRIO_RIO_SP_LM_RESP_ACK_ID_STAT_MASK)>>
		CSL_SRIO_RIO_SP_LM_RESP_ACK_ID_STAT_SHIFT;

	//Set the local OUTBOUND_ACKID to be same as the responsed ACKID 
	srioRegs->RIO_SP[uiLocalPort].RIO_SP_ACKID_STAT= uiRemote_In_ACK_ID;
	if(uiRemote_In_ACK_ID)
		printf("match_ACK_ID SP_ACKID_STAT=0x%x\n",srioRegs->RIO_SP[uiLocalPort].RIO_SP_ACKID_STAT); 	//for dubug
	
	do
	{		
		//set the remote OUTBOUND_ACKID to be same as local INBOUND_ACKID
		uiLocal_In_ACK_ID= (srioRegs->RIO_SP[uiLocalPort].RIO_SP_ACKID_STAT&
			CSL_SRIO_RIO_SP_ACKID_STAT_INB_ACKID_MASK)>>
			CSL_SRIO_RIO_SP_ACKID_STAT_INB_ACKID_SHIFT;

		uiMaintenanceValue= ((uiRemote_In_ACK_ID+1)<<
			CSL_SRIO_RIO_SP_ACKID_STAT_INB_ACKID_SHIFT)|uiLocal_In_ACK_ID;

		//set the remote ACK_ID through maintenance packet
		uiResult= Keystone_SRIO_Maintenance(uiLocalPort, uiLocalPort, uiDestID,
			0x148+(0x20*uiRemotePort), GLOBAL_ADDR(&uiMaintenanceValue), 
			SRIO_PKT_TYPE_MTN_WRITE);

		if(uiResult) 	//fail
			continue;	
		
		//readback the remote ID
		uiResult= Keystone_SRIO_Maintenance(uiLocalPort, uiLocalPort, 
			uiDestID, 0x148+(0x20*uiRemotePort), GLOBAL_ADDR(&uiMaintenanceValue), 
			SRIO_PKT_TYPE_MTN_READ);
		uiRemote_out_ACK_ID= uiMaintenanceValue&
			CSL_SRIO_RIO_SP_ACKID_STAT_OUTB_ACKID_MASK;
	}while(uiResult|(uiLocal_In_ACK_ID+1 != uiRemote_out_ACK_ID));
}

//Build SRIO type11 message descriptor
void Keystone_SRIO_Build_Type11_Msg_Desc(
	HostPacketDescriptor * hostDescriptor, unsigned int uiSrcID, unsigned int uiDestID, 
	unsigned int uiByteCount, unsigned int uiMailBox, unsigned int uiLetter)
{
	SRIO_Type11_Message_TX_Descriptor * type11MsgTxDesc;

	hostDescriptor->packet_type = SRIO_TYPE11_CPPI_PACKET;
	hostDescriptor->packet_length= uiByteCount;
	hostDescriptor->buffer_len= uiByteCount;
	hostDescriptor->psv_word_count= 2; 	//SRIO uses 2 Protocol Specific words

	type11MsgTxDesc= (SRIO_Type11_Message_TX_Descriptor *)
		(((unsigned int)hostDescriptor)+32);
	type11MsgTxDesc->Dest_ID = uiDestID;
	type11MsgTxDesc->SRC_ID = uiSrcID;
	type11MsgTxDesc->Retry_Count= 1;
	type11MsgTxDesc->SSIZE= SRIO_SSIZE_256_BYTES;
	type11MsgTxDesc->TT = 0;
	type11MsgTxDesc->LTR= uiLetter;
	type11MsgTxDesc->MailBox= uiMailBox;			
}

//Build SRIO type9 data stream message Descriptror
void Keystone_SRIO_Build_Type9_Msg_Desc(
	HostPacketDescriptor * hostDescriptor, unsigned int uiSrcID, unsigned int uiDestID, 
	unsigned int uiByteCount, unsigned int uiStreamID, unsigned int uiCOS)
{
	SRIO_Type9_Message_TX_Descriptor * type9MsgTxDesc;

	hostDescriptor->packet_type = SRIO_TYPE9_CPPI_PACKET;
	hostDescriptor->packet_length= uiByteCount;
	hostDescriptor->buffer_len= uiByteCount;
	hostDescriptor->psv_word_count= 2; 	//SRIO uses 2 Protocol Specific words

	type9MsgTxDesc= (SRIO_Type9_Message_TX_Descriptor *)
		(((unsigned int)hostDescriptor)+32);
	type9MsgTxDesc->Dest_ID = uiDestID;
	type9MsgTxDesc->SRC_ID = uiSrcID;
	type9MsgTxDesc->StreamID = uiStreamID;
	type9MsgTxDesc->TT = 0;
	type9MsgTxDesc->COS = uiCOS;
}


