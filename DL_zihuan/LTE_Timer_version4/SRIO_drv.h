/*
 * SRIO_drv.h
 *
 *  Created on: 2013-11-22
 *      Author: castMemory
 */

#ifndef SRIO_DRV_H_
#define SRIO_DRV_H_

//CPU¼Ä´æÆ÷
#include <ti/csl/csl_chip.h>

/* CSL SRIO Functional Layer */
#include <ti/csl/csl_srio.h>
#include <ti/csl/csl_srioAux.h>
#include <ti/csl/csl_srioAuxPhyLayer.h>

/* register on chip */
#include <ti\csl\csl_chip.h>
//#include <ti\csl\cslr_chip.h>

#include <ti\csl\csl_cgem.h>
#include <ti\csl\csl_cgemAux.h>

/* Get global address */
#define GET_C64X_GLOBAL_ADDR( LocalAddr )		(0x10000000u)| \
												(CSL_chipReadReg(CSL_CHIP_DNUM)<<24)| \
												(LocalAddr)

/** Number of data bytes to read/write using LSU reg is limited to 1MB */
#define SRIO_LSU_MAX_DATA_XFER				    (0x100000)

/** @brief BSLC667X_ srio Interrupt Destination resource number enumeration
 */
typedef enum {
    /** SRIO LSU 0 */
    SRIO_LSU_NUM_0=0,
    /** SRIO LSU 1 */
    SRIO_LSU_NUM_1=1,
    /** SRIO LSU 2 */
    SRIO_LSU_NUM_2=2,
    /** SRIO LSU 3 */
    SRIO_LSU_NUM_3=3
}SrioLsuNum;

/** @brief BSLC667X_ srio LSU resource number enumeration
 */
typedef enum {
    /** SRIO LSU transfer priority 0 */
    SRIO_LSU_REG4_PRIORITY_L0_H3_0=0,
    /** SRIO LSU transfer priority 1 */
    SRIO_LSU_REG4_PRIORITY_L0_H3_1=1,
    /** SRIO LSU transfer priority 2 */
    SRIO_LSU_REG4_PRIORITY_L0_H3_2=2,
    /** SRIO LSU transfer priority 3 */
    SRIO_LSU_REG4_PRIORITY_L0_H3_3=3
}SrioLsuXferPriority;

/**
 *  @brief This enum describes the port number configuration for SRIO.
 */
typedef enum {
    /** Port number 0 */
    SRIO_PORT_0 = 0,
    /** Port number 1 */
    SRIO_PORT_1 = 1,
    /** Port number 2 */
    SRIO_PORT_2 = 2,
    /** Port number 3 */
    SRIO_PORT_3 = 3
} SrioPortNum;

/**
 * @brief This enumeration indicates the status of the pending command
 */
typedef enum {
    /** Transaction complete, no errors (Posted/Non-posted) */
    SRIO_TRANS_NO_ERR = 0,

    /** Transaction timeout occurred on non-posted transaction */
    SRIO_TRANS_TIMEOUT = 1,

    /** Transaction complete, packet not sent due to flow control blockade
     *  (Xoff)
     */
    SRIO_FLOW_CNTL_BLOCKADE = 2,

    /** Transaction complete, non-posted response packet (type 8 and 13)
     *  contained ERROR status, or response payload length was in error
     */
    SRIO_RESP_PKT_ERR = 3,

    /** Transaction complete, packet not sent due to unsupported transaction
     *  type or invalid programming encoding for one or more LSU register fields
     */
    SRIO_INV_PROG_ENCODING = 4,

    /** DMA data transfer error */
    SRIO_DMA_TRANS_ERR = 5,

    /** "Retry" DOORBELL response received, or atomic test-and-swap was not
     *  allowed (semaphore in use)
     */
    SRIO_RETRY_DRBL_RESP_RCVD = 6,

    /** Transaction complete, packet not sent due to unavailable outbound
     *  credit at given priority
     */
    SRIO_UNAVAILABLE_OUTBOUND_CR = 7
} SrioCompCode;

typedef struct LSU_Transfer_Parameter
{
	Uint32 		remote_addrH;
	Uint32 		remote_addrL;
	Uint32 		local_addr;
	Uint32 		byte_count;
	SrioPortNum srioPortNum;
	SrioLsuNum 	srioLsuNum;

} SRIO_Trans_Config ;

typedef struct LSU_Maintenance_Parameter
{
	Uint32 		remote_addrL;
	Uint32 		local_addr;
	SrioPortNum srioPortNum;
	SrioLsuNum 	srioLsuNum;

} SRIO_Maint_Config ;

extern Uint32 SRIO_SWriteWithDoorbell (
					SRIO_Trans_Config *transPara,
					Uint16 usDstDevId,
                    Uint8   usAntport,
                    Uint16 usDoorbellInfo,
                    SrioCompCode *pCompleteCode );

extern Uint32 SrioMaintWrSync (
					SRIO_Maint_Config 		maintPara,
					Uint16 					usDstDevId,
					Uint8 					ucHopCount,
					SrioCompCode 	*pCompleteCode );

extern Uint32 SrioMaintRdSync (
					SRIO_Maint_Config 		maintPara,
					Uint16 					usDstDevId,
					Uint8 					ucHopCount,
					SrioCompCode 	*pCompleteCode );

extern Uint32 SRIO_NWrite (
					SRIO_Trans_Config *transPara,
					Uint16 usDstDevId,
					SrioCompCode *pCompleteCode );

extern Uint32 SRIO_NWriteWithDoorbell (
					SRIO_Trans_Config *transPara,
					Uint16 usDstDevId,
					Uint8   usAntport,
					Uint16 usDoorbellInfo,
					SrioCompCode *pCompleteCode );

extern Uint32 SRIO_NRead (
					SRIO_Trans_Config *transPara,
                    Uint16 usDstDevId,
                    SrioCompCode *pCompleteCode );

extern Uint32 SRIO_Doorbell (
					SRIO_Trans_Config *transPara,
                    Uint16 usDstDevId,
                    Uint8  usAntport,
                    Uint16 usDoorbellInfo,
                    SrioCompCode *pCompleteCode );

extern Uint32 match_ACKID_module (SrioPortNum portNum, Uint16 usDstDevId);

extern void Device_Interrupt_init();

/* SRIO Doorbell Complete interrupt callback function */
//extern void DoorbellCallbackFunc(void *, Uint32 *pDoorbellIntrStat);

#endif /* SRIO_DRV_H_ */
