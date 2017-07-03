/**
 *   @file  singlecore_osal.c
 *
 *   @brief   
 *      This is a sample OS Abstraction Layer (AL) file implemented
 *      using XDC/BIOS APIs.
 *
 *      System integrator is advised to review these implementations and
 *      modify them to suit it to application requirements.
 *
 *      This OSAL implementation uses the <b> Approach 1 </b> documented.
 *
 *  \par
 *  ============================================================================
 *  @n   (C) Copyright 2009, Texas Instruments, Inc.
 *  @n   Use of this software is controlled by the terms and conditions found 
 *  @n   in the license agreement under which this software has been supplied.
 *  ============================================================================ 
 *  \par
 */
/* Standard C-native includes  */


/* XDC/BIOS includes */
//#include <xdc/std.h>
//#include <xdc/runtime/IHeap.h>
//#include <xdc/runtime/System.h>
//#include <xdc/runtime/Memory.h>
//#include <xdc/runtime/Error.h>

//#include <ti/sysbios/BIOS.h>
//#include <ti/sysbios/hal/Hwi.h>
//#include <ti/sysbios/knl/Task.h>
//#include <ti/sysbios/knl/Semaphore.h>
//#include <ti/sysbios/heaps/HeapBuf.h>
//#include <ti/sysbios/heaps/HeapMem.h>

//#include <xdc/cfg/global.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "csl_intc.h"
/* CSL CHIP, SEM Functional layer includes */
#include "csl_chip.h"
#include "csl_semAux.h"

/* CSL Cache module includes */
#include "csl_cacheAux.h"
#include "pa.h"
/**********************************************************************
 ****************************** Defines *******************************
 **********************************************************************/
#define DBG_MULTI_CORE 
 

/* Number of cores on c6498 */
#define     NUM_CORES           4

/* Hardware Semaphore to synchronize access from
 * multiple applications (PA applications and non-PASS applications)
 * across different cores to the QMSS library.
 */
#define     QMSS_HW_SEM         3 

/* Hardware Semaphore to synchronize access from
 * multiple applications (PASS applications and non-PASS applications)
 * across different cores to the CPPI library.
 */
#define     CPPI_HW_SEM         4 


/**********************************************************************
 ************************** Global Variables **************************
 **********************************************************************/
Uint32      cpswCppiMallocCounter   =   0;
Uint32      cpswCppiFreeCounter     =   0;
Uint32      cpswQmssMallocCounter   =   0;
Uint32      cpswQmssFreeCounter     =   0;

Uint32      coreKey [NUM_CORES];
CSL_IntcGlobalEnableState   state_dummy;//lycan 2015-9-23与vcpcommon中的冲突了，在这里没什么用，改为dummy

/**********************************************************************
 *********************** CPPI OSAL Functions **************************
 **********************************************************************/




/**********************************************************************
 *********************** QMSS OSAL Functions **************************
 **********************************************************************/

/**
 * ============================================================================
 *  @n@b Osal_qmssMtCsExit
 *
 *  @b  brief
 *  @n  This API needs to be called to exit a previously
 *      acquired critical section lock using @a Osal_cpswQmssMtCsEnter ()
 *      API. It resets the multi-threaded lock, enabling another process
 *      on the current core to grab it.
 *
 *  @param[in]  CsHandle
 *      Handle for unlocking critical section.
 *
 *  @return     None
 * =============================================================================
 */
void Osal_qmssMtCsExit (void * CsHandle)
{
    /* Enable all interrupts and enables the OS scheduler back on.
     *
     * Release multi-threaded / multi-process lock on this core.
     */
    //Hwi_restore(key);

    return;
}

/**
 * ============================================================================
 *  @n@b Osal_qmssMalloc
 *
 *  @b  brief
 *  @n  This API allocates a memory block of a given
 *      size specified by input parameter 'num_bytes'.
 *
 *  @param[in]  num_bytes
 *      Number of bytes to be allocated.
 *
 *  @return
 *      Allocated block address
 * =============================================================================
 */
void * Osal_qmssMalloc (Uint32 num_bytes)
{
	//Error_Block	    errorBlock;

    /* Increment the allocation counter. */
    cpswQmssMallocCounter++;	

	/* Allocate memory.  */
	return malloc(num_bytes);
}

/**
 * ============================================================================
 *  @n@b Osal_qmssFree
 *
 *  @b  brief
 *  @n  This API frees and restores a given memory location 
 *      pointer 'dataPtr' of size 'num_bytes' to its
 *      original heap location. Frees up memory allocated using 
 *      @a Osal_qmssMalloc ()
 *
 *  @param[in]  dataPtr
 *      Pointer to the memory block to be cleaned up.
 *
 *  @param[in]  num_bytes
 *      Size of the memory block to be cleaned up.
 *
 *  @return
 *      Not Applicable
 * =============================================================================
 */
void Osal_qmssFree (void * dataPtr, Uint32 num_bytes)
{
    /* Increment the free counter. */
    cpswQmssFreeCounter++;	

    /* Free up the memory */
    if (dataPtr)
    {
        /* Convert the global address to local address since
         * thats what the heap understands.
         */
        free(dataPtr);
    }
}


/**********************************************************************
 *********************** PASS OSAL Functions **************************
 **********************************************************************/

void Osal_paBeginMemAccess (void * addr, Uint32 size)
{
}

void Osal_paEndMemAccess (void * addr, Uint32 size)
{
}
