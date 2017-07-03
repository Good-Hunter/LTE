/**
 *   @file  qmss_acc.h
 *
 *   @brief   
 *      This is the Queue Manager accumulator header file.
 *
 *  \par
 *  ============================================================================
 *  @n   (C) Copyright 2009-2011, Texas Instruments, Inc.
 * 
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions 
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the   
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  \par
*/


#ifndef QMSS_ACC_H_
#define QMSS_ACC_H_



/* QMSS LLD includes */
#include "qmss_drv.h"

/**
@addtogroup QMSS_LLD_SYMBOL
@{
*/

/** QMSS accumulator return and Error Codes */
/** QMSS accumulator idle return code */
#define QMSS_ACC_IDLE                               0
/** QMSS accumulator successful return code */
#define QMSS_ACC_SOK                                1
/** QMSS accumulator invalid command return code */
#define QMSS_ACC_INVALID_COMMAND                    2
/** QMSS accumulator invalid channel return code */
#define QMSS_ACC_INVALID_CHANNEL                    3
/** QMSS accumulator channel not active return code */
#define QMSS_ACC_CHANNEL_NOT_ACTIVE                 4
/** QMSS accumulator channel already active */
#define QMSS_ACC_CHANNEL_ALREADY_ACTIVE             5
/** QMSS accumulator invalid queue number */
#define QMSS_ACC_INVALID_QUEUE_NUMBER               6

/**
@}
*/

/**
@addtogroup QMSS_LLD_ENUM
@{
*/

/** 
 * @brief Accumulator channel commands
 */
typedef enum
{
    /** Accumulator command to disable channel */
    Qmss_AccCmd_DISABLE_CHANNEL = 0x80,
    /** Accumulator command to enable channel */
    Qmss_AccCmd_ENABLE_CHANNEL = 0x81,
    /** Accumulator command to configure timer constant */
    Qmss_AccCmd_CONFIG_TIMER_CONSTANT = 0x82,
    /** Accumulator command to configure reclamation queue */
    Qmss_AccCmd_CONFIG_RECLAIM_QUEUE = 0x83,
    /** Accumulator command to configure diversion queue */
    Qmss_AccCmd_CONFIG_DIVERSION_QUEUE = 0x84

}Qmss_AccCmdType;

/** 
 * @brief Accumulator configuration - interrupt pacing mode
 */
typedef enum
{
    /** Interrupt on entry threshold count only */
    Qmss_AccPacingMode_NONE = 0,
    /** Time delay since last interrupt */
    Qmss_AccPacingMode_LAST_INTERRUPT,
    /** Time delay since first new packet */
    Qmss_AccPacingMode_FIRST_NEW_PACKET,
    /** Time delay since last new packet */
    Qmss_AccPacingMode_LAST_NEW_PACKET
}Qmss_AccPacingMode;

/** 
 * @brief Accumulator configuration - list entry size
 */
typedef enum
{
    /** 'D' register only (4 byte entries)
     * Word 0 : Packet Descriptor Pointer 
     */
    Qmss_AccEntrySize_REG_D = 0,
    /** 'C,D' registers (8 byte entries) 
     * Word 0 : Packet Length (as reported by queue manager)
     * Word 1 : Packet Descriptor Pointer
     */
    Qmss_AccEntrySize_REG_CD,
    /** 'A,B,C,D' registers (16 byte entries) 
     * Word 0 : Packet Count on Queue (when read)
     * Word 1 : Byte Count on Queue (when read)
     * Word 2 : Packet Length (as reported by queue manager)
     * Word 3 : Packet Descriptor Pointer
     */
    Qmss_AccEntrySize_REG_ABCD
}Qmss_AccEntrySize;

/** 
 * @brief Accumulator configuration - list count mode
 */
typedef enum
{
    /** NULL Terminate Mode - The last list entry is used to store a NULL pointer 
     * record (NULL terminator) to mark the end of list. In either case there is room for one less 
     * list entry in a page than is actually specified by the host.
     */
    Qmss_AccCountMode_NULL_TERMINATE = 0,
    /** Entry Count Mode - The first list entry is used to store the total list entry 
     * count (not including the length entry). 
     */
    Qmss_AccCountMode_ENTRY_COUNT
}Qmss_AccCountMode;


/** 
 * @brief Accumulator configuration - Multi-Queue Mode
 */
typedef enum
{
    /** Single Queue Mode - The channel monitors a single queue. */
    Qmss_AccQueueMode_SINGLE_QUEUE = 0,
    /** Multi-Queue Mode - The channel monitors up to 32 queues starting at the supplied base queue index. */
    Qmss_AccQueueMode_MULTI_QUEUE
}Qmss_AccQueueMode;

/**
@}
*/

/** @addtogroup QMSS_LLD_DATASTRUCT
@{ 
*/

/** 
 * @brief Accumulator Command interface structure
 */
typedef struct
{
    /** Accumulator channel affected (0-47) */
	unsigned char               channel;
    /** Accumulator channel command - Qmss_AccCmd_ENABLE_CHANNEL : Enable channel 
     * Qmss_AccCmd_DISABLE_CHANNEL : Disable channel */
    Qmss_AccCmdType     command;
    /** This field specifies which queues are to be included in the queue group. 
     * Bit 0 corresponds to the base queue index, and bit 31 corresponds to the base 
     * queue index plus 31. For any bit set in this mask, the corresponding queue index 
     * is included in the monitoring function.
     *
     * This field is ignored in single-queue mode.*/
    unsigned int              queueEnMask;
    /** Physical pointer to list ping/pong buffer. NULL when channel disabled */
    unsigned int              listAddress;
    /** Queue Manager and Queue Number index to monitor. This serves as a base queue index when the 
     * channel in multi-queue mode, and must be a multiple of 32 when multi-queue mode is enabled. */
    unsigned short              queMgrIndex;
    /** Max entries per list buffer page */
    unsigned short              maxPageEntries;
    /** Number of timer ticks to delay interrupt */
    unsigned short              timerLoadCount;
    /** Interrupt pacing mode. Specifies when the interrupt should be trigerred */
    Qmss_AccPacingMode  interruptPacingMode;
    /** List entry size. Specifies the size of each data entry */
    Qmss_AccEntrySize   listEntrySize;
    /** List count Mode. The number of entries in the list */
    Qmss_AccCountMode   listCountMode; 
    /** Queue mode. Moitor single or multiple queues */
    Qmss_AccQueueMode   multiQueueMode;
} Qmss_AccCmdCfg;

/** 
@} 
*/

/* Internal data structure to write accumulator command */
typedef struct
{
    /* Channel, Command */
	unsigned int              word0;
    /* Queue enable mask */
	unsigned int              word1;
    /* List buffer physical address */
	unsigned int              word2;
    /* Queue manager and base index, Maximum page entries */
	unsigned int              word3;
    /* Timer Load Count, Configuration */
	unsigned int              word4;
} Qmss_AccCmd;

/* Exported functions */
extern Qmss_Result Qmss_programAccumulator (Qmss_PdspId pdspId, Qmss_AccCmdCfg *cfg);
extern Qmss_Result Qmss_disableAccumulator (Qmss_PdspId pdspId, unsigned char channel);
extern Qmss_Result Qmss_configureAccTimer (Qmss_PdspId pdspId, unsigned short timerConstant);
extern Qmss_Result Qmss_programReclaimQueue (Qmss_PdspId pdspId, Qmss_QueueHnd hnd);
extern Qmss_Result Qmss_programDiversionQueue (Qmss_PdspId pdspId, Qmss_QueueHnd divQ,
                                               Qmss_QueueHnd divCompletionQ);

#ifdef __cplusplus
}
#endif

#endif /* QMSS_ACC_H_ */

