/*
 * SRIO_error.h
 *
 *  Created on: 2013-12-30
 *      Author: castMemory
 */

#ifndef SRIO_ERROR_H_
#define SRIO_ERROR_H_

//���涨����ֳ�����ֵ
#define SRIO_MODULE_INVALID									(0x40D00001u)
#define SRIO_LSU_BUSY_FULL_INVALID							(0x40D00002u)
#define SRIO_GET_COMPLETECODE_TIMEOUT						(0x40D00003u)

#define FUNC_SUCCESS										0
#define FUNC_FAIL											100
/** Generic API Argument invalid */
#define GEN_FUNC_ARG_INVALID                       			(0x40000001u)
/** Generic API call invalid */
#define GEN_FUNCALL_INVALID									(0x40000002u)
/** Generic LIB invalid */
#define GEN_LIB_INVALID										(0x40000004u)

#define SRIO_SYNC_XFER_TIMEOUT_INUS							50

/** @name SRIO module specific ERROR Codes
 */
/** SRIO Success code */
#define BSLC667X_SRIO_OK                                     (BSLC667X_GEN_OK)

/** SRIO Time out */
#define BSLC667X_SRIO_TIMEOUT                                (0x40D00002u)
/** SRIO module is in use */
#define BSLC667X_SRIO_INUSE			                         (0x40D00004u)
/** SRIO Synchronous "Asynchronous queue" timeout, Timedout on "Asynchronous queue complete" wait */
#define BSLC667X_SRIO_SYNC_ASYNCQUEUE_TIMEOUT                (0x40D00008u)

/** Invalid DSP function call */
#define BSLC667X_SRIO_FUNC_INV								 (0x40D00020u)
/** Conflict between Discovered bit being set and host enable requested */
#define BSLC667X_SRIO_DISC_HOST_RQ_INVALID				     (0x40D00040u)
/** Invalid Port Number */
#define BSLC667X_SRIO_PORT_NUM_INVALID						 (0x40D00080u)

#endif /* SRIO_ERROR_H_ */
