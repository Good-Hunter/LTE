

#include <stdio.h>
#include "Keystone_SRIO_init.h"
#include "Keystone_common.h"

#include "srio_debug.h"

//print SRIO Logical/Transport error/status
void print_SRIO_Logical_Transport_error_status()
{
	unsigned int uiErrorStatus, uiLogiTransError;
	
	//Error, Reset, and Special Event Interrupt Condition Status Register
	uiLogiTransError = srioRegs->RIO_ERR_RST_EVNT_ICSR;
	if(uiLogiTransError)
	{
		uiErrorStatus= (uiLogiTransError& CSL_SRIO_RIO_ERR_RST_EVNT_ICSR_DEVICE_RST_INT_MASK)>>
			CSL_SRIO_RIO_ERR_RST_EVNT_ICSR_DEVICE_RST_INT_SHIFT;
		if(1==uiErrorStatus)
			printf("Device reset interrupt received from any port\n");

		uiErrorStatus= (uiLogiTransError& CSL_SRIO_RIO_ERR_RST_EVNT_ICSR_PORT3_ERR_MASK)>>
			CSL_SRIO_RIO_ERR_RST_EVNT_ICSR_PORT3_ERR_SHIFT;
		if(1==uiErrorStatus)
			printf("Error detected on port 3\n");

		uiErrorStatus= (uiLogiTransError& CSL_SRIO_RIO_ERR_RST_EVNT_ICSR_PORT2_ERR_MASK)>>
			CSL_SRIO_RIO_ERR_RST_EVNT_ICSR_PORT2_ERR_SHIFT;
		if(1==uiErrorStatus)
			printf("Error detected on port 2\n");

		uiErrorStatus= (uiLogiTransError& CSL_SRIO_RIO_ERR_RST_EVNT_ICSR_PORT1_ERR_MASK)>>
			CSL_SRIO_RIO_ERR_RST_EVNT_ICSR_PORT1_ERR_SHIFT;
		if(1==uiErrorStatus)
			printf("Error detected on port 1\n");

		uiErrorStatus= (uiLogiTransError& CSL_SRIO_RIO_ERR_RST_EVNT_ICSR_PORT0_ERR_MASK)>>
			CSL_SRIO_RIO_ERR_RST_EVNT_ICSR_PORT0_ERR_SHIFT;
		if(1==uiErrorStatus)
			printf("Error detected on port 0\n");

		uiErrorStatus= (uiLogiTransError& CSL_SRIO_RIO_ERR_RST_EVNT_ICSR_LLERR_CAPTURE_MASK)>>
			CSL_SRIO_RIO_ERR_RST_EVNT_ICSR_LLERR_CAPTURE_SHIFT;
		if(1==uiErrorStatus)
			printf("Logical layer error management event capture detected\n");

		uiErrorStatus= (uiLogiTransError& CSL_SRIO_RIO_ERR_RST_EVNT_ICSR_PORT_WRITE_IN_RECEIVED_MASK)>>
			CSL_SRIO_RIO_ERR_RST_EVNT_ICSR_PORT_WRITE_IN_RECEIVED_SHIFT;
		if(1==uiErrorStatus)
			printf("Port-write-in request received on any port\n");

		uiErrorStatus= (uiLogiTransError& CSL_SRIO_RIO_ERR_RST_EVNT_ICSR_MCAST_INT_RECEIVED_MASK)>>
			CSL_SRIO_RIO_ERR_RST_EVNT_ICSR_MCAST_INT_RECEIVED_SHIFT;
		if(1==uiErrorStatus)
			printf("Multi-cast event control symbol interrupt received on any port\n");

		//Clear the error
		srioRegs->RIO_ERR_RST_EVNT_ICCR= 0xFFFFFFFF;
	}

	//Logical/Transport Layer Error Detect
	uiLogiTransError = srioRegs->RIO_ERR_DET;
	if(uiLogiTransError)
	{
		uiErrorStatus= (uiLogiTransError& CSL_SRIO_RIO_ERR_DET_IO_ERR_RESP_MASK)>>
			CSL_SRIO_RIO_ERR_DET_IO_ERR_RESP_SHIFT;
		if(1==uiErrorStatus)
			printf("An LSU received an ERROR response to an I/O logical layer request. To clear this bit, write 0 to it\n");

		uiErrorStatus= (uiLogiTransError& CSL_SRIO_RIO_ERR_DET_MSG_ERR_RESP_MASK)>>
			CSL_SRIO_RIO_ERR_DET_MSG_ERR_RESP_SHIFT;
		if(1==uiErrorStatus)
			printf("The RXU received an ERROR response to a message logical layer request. To clear this bit, write 0 to it.\n");

		uiErrorStatus= (uiLogiTransError& CSL_SRIO_RIO_ERR_DET_MSG_FMT_ERR_MASK)>>
			CSL_SRIO_RIO_ERR_DET_MSG_FMT_ERR_SHIFT;
		if(1==uiErrorStatus)
			printf("The RXU received a message data payload with an invalid size or segment. To clear this bit, write 0 to it.\n");

		uiErrorStatus= (uiLogiTransError& CSL_SRIO_RIO_ERR_DET_ILL_TRANS_DECODE_MASK)>>
			CSL_SRIO_RIO_ERR_DET_ILL_TRANS_DECODE_SHIFT;
		if(1==uiErrorStatus)
			printf("Received illegal fields in the response/request packet for an IO/message transaction. To clear this bit, write 0 to it.\n");

		uiErrorStatus= (uiLogiTransError& CSL_SRIO_RIO_ERR_DET_MSG_REQ_TIMEOUT_MASK)>>
			CSL_SRIO_RIO_ERR_DET_MSG_REQ_TIMEOUT_SHIFT;
		if(1==uiErrorStatus)
			printf("A timeout has been detected by the RXU. A required message request has not been received by the RXU within the specified time-out interval. To clear this bit, write 0 to it.\n");

		uiErrorStatus= (uiLogiTransError& CSL_SRIO_RIO_ERR_DET_PKT_RESP_TIMEOUT_MASK)>>
			CSL_SRIO_RIO_ERR_DET_PKT_RESP_TIMEOUT_SHIFT;
		if(1==uiErrorStatus)
			printf("A timeout has been detected by an LSU or the TXU. A required response has not been received by the LSU/TXU within the specified timeout interval. To clear this bit, write 0 to it.\n");

		uiErrorStatus= (uiLogiTransError& CSL_SRIO_RIO_ERR_DET_UNSOLICITED_RESP_MASK)>>
			CSL_SRIO_RIO_ERR_DET_UNSOLICITED_RESP_SHIFT;
		if(1==uiErrorStatus)
			printf("An unsolicited response packet has been received by an LSU or the TXU. To clear this bit, write 0 to it.\n");

		uiErrorStatus= (uiLogiTransError& CSL_SRIO_RIO_ERR_DET_UNSUPPORTED_TRANS_MASK)>>
			CSL_SRIO_RIO_ERR_DET_UNSUPPORTED_TRANS_SHIFT;
		if(1==uiErrorStatus)
			printf("The MAU has received an unsupported transaction. That is, the MAU received a transaction that is not supported in the destination operations CAR. To clear this bit, write 0 to it.\n");

		uiErrorStatus= (uiLogiTransError&CSL_SRIO_RIO_ERR_DET_PDU_LEN_ERR_MASK )>>
			CSL_SRIO_RIO_ERR_DET_PDU_LEN_ERR_SHIFT;
		if(1==uiErrorStatus)
			printf("Data streaming PDU length error\n");

		uiErrorStatus= (uiLogiTransError& CSL_SRIO_RIO_ERR_DET_SHORT_STREAM_SEG_MASK)>>
			CSL_SRIO_RIO_ERR_DET_SHORT_STREAM_SEG_SHIFT;
		if(1==uiErrorStatus)
			printf("Short data streaming segment error\n");

		uiErrorStatus= (uiLogiTransError& CSL_SRIO_RIO_ERR_DET_LONG_STREAM_SEG_MASK)>>
			CSL_SRIO_RIO_ERR_DET_LONG_STREAM_SEG_SHIFT;
		if(1==uiErrorStatus)
			printf("Long data streaming segment error\n");

		uiErrorStatus= (uiLogiTransError& CSL_SRIO_RIO_ERR_DET_OPEN_STREAM_CONTEXT_MASK)>>
			CSL_SRIO_RIO_ERR_DET_OPEN_STREAM_CONTEXT_SHIFT;
		if(1==uiErrorStatus)
			printf("Open existing data streaming context error\n");

		uiErrorStatus= (uiLogiTransError& CSL_SRIO_RIO_ERR_DET_MISSING_STREAM_CONTEXT_MASK)>>
			CSL_SRIO_RIO_ERR_DET_MISSING_STREAM_CONTEXT_SHIFT;
		if(1==uiErrorStatus)
			printf("Missing data streaming context error\n");

		uiErrorStatus= (uiLogiTransError& CSL_SRIO_RIO_ERR_DET_CPPI_SECURITY_VIOLATION_MASK)>>
			CSL_SRIO_RIO_ERR_DET_CPPI_SECURITY_VIOLATION_SHIFT;
		if(1==uiErrorStatus)
			printf("The RXU has detected an access block. That is, access to one of the RX queues was blocked. To clear this bit, write 0 to it.\n");

		uiErrorStatus= (uiLogiTransError& CSL_SRIO_RIO_ERR_DET_RX_DMA_ERR_MASK)>>
			CSL_SRIO_RIO_ERR_DET_RX_DMA_ERR_SHIFT;
		if(1==uiErrorStatus)
			printf("A DMA access to the MAU was blocked. To clear this bit, write 0 to it.\n");

		printf("Logical/Transport Layer High Address Capture: 0x%x\n", srioRegs->RIO_H_ADDR_CAPT);
		printf("Logical/Transport Layer Address Capture: 0x%x\n", srioRegs->RIO_ADDR_CAPT);
		printf("Logical/Transport Layer Device ID Capture: 0x%x\n", srioRegs->RIO_ID_CAPT);
		printf("Logical/Transport Layer Control Capture: 0x%x\n", srioRegs->RIO_CTRL_CAPT);

		//Clear the error
		srioRegs->RIO_ERR_DET= 0;
	}
}

//print SRIO physical layer port error/status
void print_SRIO_Port_error_status(SRIO_Block_Enable * blockEn)
{
	unsigned int uiErrorStatus, uiPortError, uiPortStatus;
	signed int i;

	for(i=0; i< SRIO_MAX_PORT_NUM; i++)
	{
		if(FALSE == blockEn->bLogic_Port_EN[i])
			continue;

		//Port Local AckID Status
		uiPortStatus = srioRegs->RIO_SP[i].RIO_SP_ACKID_STAT;
		uiErrorStatus= (uiPortStatus&CSL_SRIO_RIO_SP_ACKID_STAT_INB_ACKID_MASK)>>
			CSL_SRIO_RIO_SP_ACKID_STAT_INB_ACKID_SHIFT;
		printf("Input port %d next expected ackID value: 0x%x\n", i, uiErrorStatus);
		uiErrorStatus= (uiPortStatus&CSL_SRIO_RIO_SP_ACKID_STAT_OUTSTD_ACKID_MASK)>>
			CSL_SRIO_RIO_SP_ACKID_STAT_OUTSTD_ACKID_SHIFT;
		printf("Output port %d unacknowledged ackID: 0x%x\n", i, uiErrorStatus);
		uiErrorStatus= (uiPortStatus&CSL_SRIO_RIO_SP_ACKID_STAT_OUTB_ACKID_MASK)>>
			CSL_SRIO_RIO_SP_ACKID_STAT_OUTB_ACKID_SHIFT;
		printf("Output port %d next transmitted ackID value: 0x%x\n", i, uiErrorStatus);

		//Port Error and Status
		uiPortError = srioRegs->RIO_SP[i].RIO_SP_ERR_STAT;
		if(2 != uiPortError)
		{
			uiErrorStatus= (uiPortError& CSL_SRIO_RIO_SP_ERR_STAT_OUTPUT_DROP_MASK)>>
				CSL_SRIO_RIO_SP_ERR_STAT_OUTPUT_DROP_SHIFT;
			if(1==uiErrorStatus)
				printf("The output port %d has discarded a packet.\n", i);

			uiErrorStatus= (uiPortError& CSL_SRIO_RIO_SP_ERR_STAT_OUTPUT_FAIL_MASK)>>
				CSL_SRIO_RIO_SP_ERR_STAT_OUTPUT_FAIL_SHIFT;
			if(1==uiErrorStatus)
				printf("The output port %d has encountered a failed condition. The failed port error threshold has been reached in the Port n Error Rate Threshold Register.\n", i);

			uiErrorStatus= (uiPortError& CSL_SRIO_RIO_SP_ERR_STAT_OUTPUT_DEGR_MASK)>>
				CSL_SRIO_RIO_SP_ERR_STAT_OUTPUT_DEGR_SHIFT;
			if(1==uiErrorStatus)
				printf("The output port %d has encountered a degraded condition. The degraded port error threshold has been reached in the Port n Error Rate Threshold Register.\n", i);

			uiErrorStatus= (uiPortError& CSL_SRIO_RIO_SP_ERR_STAT_OUTPUT_RETRY_MASK)>>
				CSL_SRIO_RIO_SP_ERR_STAT_OUTPUT_RETRY_SHIFT;
			if(1==uiErrorStatus)
				printf("The output port %d has encountered a retry condition. This bit is set when bit 18 is set.\n", i);

			uiErrorStatus= (uiPortError& CSL_SRIO_RIO_SP_ERR_STAT_OUTPUT_RETRIED_MASK)>>
				CSL_SRIO_RIO_SP_ERR_STAT_OUTPUT_RETRIED_SHIFT;
			if(1==uiErrorStatus)
				printf("The output port %d has received a packet-retry control symbol and cannot make forward progress. This bit is set when bit 18 is set and is cleared when a packet-accepted or packet-not-accepted control symbol is received.\n", i);

			uiErrorStatus= (uiPortError& CSL_SRIO_RIO_SP_ERR_STAT_OUTPUT_RETRY_STOPPED_MASK)>>
				CSL_SRIO_RIO_SP_ERR_STAT_OUTPUT_RETRY_STOPPED_SHIFT;
			if(1==uiErrorStatus)
				printf("The output port %d has received a packet-retry control symbol and is in the output retry-stopped state.\n", i);

			uiErrorStatus= (uiPortError& CSL_SRIO_RIO_SP_ERR_STAT_OUTPUT_ERR_MASK)>>
				CSL_SRIO_RIO_SP_ERR_STAT_OUTPUT_ERR_SHIFT;
			if(1==uiErrorStatus)
				printf("The output port %d has encountered (and possibly recovered from) a transmission error. This bit is set when bit 16 is set.\n", i);

			uiErrorStatus= (uiPortError& CSL_SRIO_RIO_SP_ERR_STAT_OUTPUT_ERR_STOPPED_MASK)>>
				CSL_SRIO_RIO_SP_ERR_STAT_OUTPUT_ERR_STOPPED_SHIFT;
			if(1==uiErrorStatus)
				printf("The output port %d is in the output error-stopped state.\n", i);

			uiErrorStatus= (uiPortError& CSL_SRIO_RIO_SP_ERR_STAT_INPUT_RETRY_STOPPED_MASK)>>
				CSL_SRIO_RIO_SP_ERR_STAT_INPUT_RETRY_STOPPED_SHIFT;
			if(1==uiErrorStatus)
				printf("The input port %d is in the input retry-stopped state.\n", i);

			uiErrorStatus= (uiPortError& CSL_SRIO_RIO_SP_ERR_STAT_INPUT_ERR_ENCTR_MASK)>>
				CSL_SRIO_RIO_SP_ERR_STAT_INPUT_ERR_ENCTR_SHIFT;
			if(1==uiErrorStatus)
				printf("The input port %d has encountered (and possibly recovered from) a transmission error. This bit is set when bit 8 is set.\n", i);

			uiErrorStatus= (uiPortError& CSL_SRIO_RIO_SP_ERR_STAT_INPUT_ERR_STOPPED_MASK)>>
				CSL_SRIO_RIO_SP_ERR_STAT_INPUT_ERR_STOPPED_SHIFT;
			if(1==uiErrorStatus)
				printf("The input port %d is in the input error-stopped state.\n", i);

			uiErrorStatus= (uiPortError& CSL_SRIO_RIO_SP_ERR_STAT_PORT_WRITE_PEND_MASK)>>
				CSL_SRIO_RIO_SP_ERR_STAT_PORT_WRITE_PEND_SHIFT;
			if(1==uiErrorStatus)
				printf("The port %d has encountered a condition which required it to initiate a Maintenance Port-write operation.\n", i);

			uiErrorStatus= (uiPortError& CSL_SRIO_RIO_SP_ERR_STAT_PORT_ERR_MASK)>>
				CSL_SRIO_RIO_SP_ERR_STAT_PORT_ERR_SHIFT;
			if(1==uiErrorStatus)
				printf("The input or output port %d has encountered an error from which hardware was unable to recover.\n", i);

			uiErrorStatus= (uiPortError& CSL_SRIO_RIO_SP_ERR_STAT_PORT_OK_MASK)>>
				CSL_SRIO_RIO_SP_ERR_STAT_PORT_OK_SHIFT;
			if(1==uiErrorStatus)
				printf("Port %d OK condition. The input and output ports are initialized, and the port is exchanging error-free control symbols with the attached device.\n", i);

			uiErrorStatus= (uiPortError& CSL_SRIO_RIO_SP_ERR_STAT_PORT_UNINIT_MASK)>>
				CSL_SRIO_RIO_SP_ERR_STAT_PORT_UNINIT_SHIFT;
			if(1==uiErrorStatus)
				printf("Input and output port %d are not initialized.\n", i);

			//Clear the error
			srioRegs->RIO_SP[i].RIO_SP_ERR_STAT= uiErrorStatus|0x03120214;
		}
		
		//Port Error Detect
		uiPortError = srioRegs->RIO_SP_ERR[i].RIO_SP_ERR_DET;
		if(uiPortError)
		{
			uiErrorStatus= (uiPortError& CSL_SRIO_RIO_SP_ERR_DET_CS_CRC_ERR_MASK)>>
				CSL_SRIO_RIO_SP_ERR_DET_CS_CRC_ERR_SHIFT;
			if(1==uiErrorStatus)
				printf("The port %d received a control symbol with a bad CRC value.\n", i);

			uiErrorStatus= (uiPortError& CSL_SRIO_RIO_SP_ERR_DET_CS_ILL_ID_MASK)>>
				CSL_SRIO_RIO_SP_ERR_DET_CS_ILL_ID_SHIFT;
			if(1==uiErrorStatus)
				printf("The port %d received an acknowledge control symbol with an unexpected ackID (packet-accepted or packet-retry). The capture registers do not have valid information during this error detection.\n", i);

			uiErrorStatus= (uiPortError& CSL_SRIO_RIO_SP_ERR_DET_CS_NOT_ACC_MASK)>>
				CSL_SRIO_RIO_SP_ERR_DET_CS_NOT_ACC_SHIFT;
			if(1==uiErrorStatus)
				printf("The port %d received a packet-not-accepted acknowledge control symbol.\n", i);

			uiErrorStatus= (uiPortError& CSL_SRIO_RIO_SP_ERR_DET_PKT_ILL_ACKID_MASK)>>
				CSL_SRIO_RIO_SP_ERR_DET_PKT_ILL_ACKID_SHIFT;
			if(1==uiErrorStatus)
				printf("The port %d received a packet with unexpected/out-of-sequence ackID.\n", i);

			uiErrorStatus= (uiPortError& CSL_SRIO_RIO_SP_ERR_DET_PKT_CRC_ERR_MASK)>>
				CSL_SRIO_RIO_SP_ERR_DET_PKT_CRC_ERR_SHIFT;
			if(1==uiErrorStatus)
				printf("The port %d received a packet with a bad CRC value.\n", i);

			uiErrorStatus= (uiPortError& CSL_SRIO_RIO_SP_ERR_DET_PKT_ILL_SIZE_MASK)>>
				CSL_SRIO_RIO_SP_ERR_DET_PKT_ILL_SIZE_SHIFT;
			if(1==uiErrorStatus)
				printf("The port %d received packet that exceeds the maximum allowed size.\n", i);

			uiErrorStatus= (uiPortError& CSL_SRIO_RIO_SP_ERR_DET_LR_ACKID_ILL_MASK)>>
				CSL_SRIO_RIO_SP_ERR_DET_LR_ACKID_ILL_SHIFT;
			if(1==uiErrorStatus)
				printf("The port %d received a link response with an ackID that is not outstanding. The capture registers do not have valid information during this error detection.\n", i);

			uiErrorStatus= (uiPortError& CSL_SRIO_RIO_SP_ERR_DET_PROT_ERR_MASK)>>
				CSL_SRIO_RIO_SP_ERR_DET_PROT_ERR_SHIFT;
			if(1==uiErrorStatus)
				printf("The port %d received an unexpected packet or control symbol.\n", i);

			uiErrorStatus= (uiPortError& CSL_SRIO_RIO_SP_ERR_DET_DELIN_ERR_MASK)>>
				CSL_SRIO_RIO_SP_ERR_DET_DELIN_ERR_SHIFT;
			if(1==uiErrorStatus)
				printf("The port %d detected a delineation error. The port received an unaligned /SC/ or /PD/ or undefined code-group. The capture registers do not have valid information during this error detection.\n", i);

			uiErrorStatus= (uiPortError& CSL_SRIO_RIO_SP_ERR_DET_CS_ACK_ILL_MASK)>>
				CSL_SRIO_RIO_SP_ERR_DET_CS_ACK_ILL_SHIFT;
			if(1==uiErrorStatus)
				printf("The port %d received an unexpected acknowledge control symbol.\n", i);

			uiErrorStatus= (uiPortError& CSL_SRIO_RIO_SP_ERR_DET_LINK_TO_MASK)>>
				CSL_SRIO_RIO_SP_ERR_DET_LINK_TO_SHIFT;
			if(1==uiErrorStatus)
				printf("The port %d experienced a link timeout. The port did not receive an acknowledge or link-response control symbol within the specified time-out interval. The capture registers do not have valid information during this error detection.\n", i);

			uiPortError = srioRegs->RIO_SP_ERR[i].RIO_SP_ERR_ATTR_CAPT;
			uiErrorStatus= (uiPortError& CSL_SRIO_RIO_SP_ERR_ATTR_CAPT_VAL_CAPT_MASK)>>
				CSL_SRIO_RIO_SP_ERR_ATTR_CAPT_VAL_CAPT_SHIFT;
			if(uiErrorStatus)
			{
				uiErrorStatus= (uiPortError& CSL_SRIO_RIO_SP_ERR_ATTR_CAPT_INFO_TYPE_MASK)>>
					CSL_SRIO_RIO_SP_ERR_ATTR_CAPT_INFO_TYPE_SHIFT;
				printf("Type of information logged: %d (0 - packet, 1 - control symbol)\n", uiErrorStatus);
				uiErrorStatus= (uiPortError& CSL_SRIO_RIO_SP_ERR_ATTR_CAPT_ERR_TYPE_MASK)>>
					CSL_SRIO_RIO_SP_ERR_ATTR_CAPT_ERR_TYPE_SHIFT;
				printf("captured error bit in the Port Error Detect Register: %d\n", uiErrorStatus);

				printf("control symbol or Bytes 0 to 3 of the packet header that correspond to the error: 0x%x\n", srioRegs->RIO_SP_ERR[i].RIO_SP_ERR_CAPT_0);
				printf("Bytes 4 to 7 of the packet header that corresponds to the error: 0x%x\n", srioRegs->RIO_SP_ERR[i].RIO_SP_ERR_CAPT_1);
				printf("Bytes 8 to 11 of the packet header that corresponds to the error: 0x%x\n", srioRegs->RIO_SP_ERR[i].RIO_SP_ERR_CAPT_2);
				printf("Bytes 12 to 15 of the packet header that corresponds to the error: 0x%x\n", srioRegs->RIO_SP_ERR[i].RIO_SP_ERR_CAPT_3);
			}

			//Clear the error
			srioRegs->RIO_SP_ERR[i].RIO_SP_ERR_DET= 0;
			srioRegs->RIO_SP_ERR[i].RIO_SP_ERR_ATTR_CAPT= 0;
		}

		//Lane state
		uiPortError = srioRegs->RIO_LANE[i].RIO_LANE_STAT0;
		uiErrorStatus= (uiPortError& CSL_SRIO_RIO_LANE_STAT0_RX_SYNC_MASK)>>
			CSL_SRIO_RIO_LANE_STAT0_RX_SYNC_SHIFT;
		if(1!=uiErrorStatus)
			printf("lane %d receiver does achieve lane synchronization.\n", i);

		uiErrorStatus= (uiPortError& CSL_SRIO_RIO_LANE_STAT0_ERR_CNT_MASK)>>
			CSL_SRIO_RIO_LANE_STAT0_ERR_CNT_SHIFT;
		if(uiErrorStatus)
			printf("%d 8b/10b deconding error have occurred\n", uiErrorStatus);		

#if 0
		//Port Control/status Independent Register
		uiPortError = srioRegs->PORT_OPTION[i].SP_CTL_INDEP;
		uiErrorStatus= (uiPortError& CSL_SRIO_SP_CTL_INDEP_ILL_TRANS_ERR_MASK)>>
			CSL_SRIO_SP_CTL_INDEP_ILL_TRANS_ERR_SHIFT;
		if(1==uiErrorStatus)
			printf("One of the following error conditions has been detected on port %d:\n¡¤ The received transaction has a reserved value in the tt field.\n¡¤ A reserved field of Maintenance transaction type is detected.\n¡¤ The destination ID is not defined in look-up table.\nThis error is also reported in registers SP0_ERR_DET and ERR_DET.\n", i);
		uiErrorStatus= (uiPortError& CSL_SRIO_SP_CTL_INDEP_MAX_RETRY_ERR_MASK)>>
			CSL_SRIO_SP_CTL_INDEP_MAX_RETRY_ERR_SHIFT;
		if(1==uiErrorStatus)
			printf("max_retry_cnt is equal to max_retry_threshold on port %d. The Port-Write request and interrupt are generated if enabled. This error is also reported in the register SP_ERR_DET.\n", i);
		uiErrorStatus= (uiPortError& CSL_SRIO_SP_CTL_INDEP_IRQ_ERR_MASK)>>
			CSL_SRIO_SP_CTL_INDEP_IRQ_ERR_SHIFT;
		if(1==uiErrorStatus)
			printf("An error occurred and there is a Port-Write condition on port %d. IRQ_ERR remains at 1 until software writes a 1 to it.\n", i);

		//clear the error
		srioRegs->PORT_OPTION[i].SP_CTL_INDEP= uiPortError|0x00110040; 
#endif
		puts("");
	}
}

//print SRIO status
void print_SRIO_status_error(SRIO_Block_Enable * blockEn)
{
	print_SRIO_Logical_Transport_error_status();
	print_SRIO_Port_error_status(blockEn);	
}



