/**
 *   @file  cppi_osal.h
 *
 *   @brief   
 *      This is the sample OS Adaptation layer which is used by the CPPI low level
 *      driver. The OSAL layer can be ported in either of the following 
 *      manners to a native OS:
 *
 *      <b> Approach 1: </b>
 *      @n  Use Prebuilt Libraries
 *           - Ensure that the provide an implementation of all 
 *             Osal_XXX API for their native OS.
 *           - Link the prebuilt libraries with their application.
 *           - Refer to the "example" directory for an example of this
 *       @n <b> Pros: </b>
 *           - Customers can reuse prebuilt TI provided libraries
 *       @n <b> Cons: </b>
 *           - Level of indirection in the API to get to the actual OS call
 *              
 *      <b> Approach 2: </b>
 *      @n  Rebuilt Library 
 *           - Create a copy of this file and modify it to directly 
 *             inline the native OS calls
 *           - Rebuild the CPPI low level driver library; ensure that the Include 
 *             path points to the directory where the copy of this file 
 *             has been provided.
 *           - Please refer to the "test" directory for an example of this 
 *       @n <b> Pros: </b>
 *           - Optimizations can be done to remove the level of indirection
 *       @n <b> Cons: </b>
 *           - CPPI LLD Libraries need to be rebuilt by the customer.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2009 Texas Instruments, Inc.
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

#ifndef __CPPI_OSAL_H__
#define __CPPI_OSAL_H__


/** @addtogroup CPPI_LLD_OSAL
 @{ */

extern void* memset(void *_mem, int32_t _ch, size_t _n);
extern void* memcpy(void *_s1, const void *_s2, size_t _n);
extern void* Osal_cppiMalloc (uint32_t num_bytes);
extern void Osal_cppiFree (void *ptr, uint32_t size);
extern void* Osal_cppiCsEnter (void);
extern void Osal_cppiCsExit (void *CsHandle);
extern void Osal_cppiLog ( char *fmt, ... );
extern void Osal_cppiBeginMemAccess (void *ptr, uint32_t size);
extern void Osal_cppiEndMemAccess (void *ptr, uint32_t size);

/**
 * @brief   The macro is used by the CPPI LLD to allocate memory of specified size
 *
 * Note: If the LLD is used by applications on multiple core, the "cppiHeap"
 * should be in shared memory
 *
 * <b> Prototype: </b>
 *  The following is the C prototype for the expected OSAL API.
 *
 *  @verbatim
       void* Osal_cppiMalloc (uint32_t numBytes)
    @endverbatim
 *      
 *  <b> Parameter </b>
 *  @n  Number of bytes to be allocated
 *
 *  <b> Return Value </b>
 *  @n  Pointer to the allocated block size
 */

#define Cppi_osalMalloc             Osal_cppiMalloc
/**
 * @brief   The macro is used by the CPPI LLD to free a allocated block of 
 * memory 
 *
 * <b> Prototype: </b>
 *  The following is the C prototype for the expected OSAL API.
 *
 *  @verbatim
       void Osal_cppiFree (void *ptr, uint32_t size)
    @endverbatim
 *      
 *  <b> Parameter </b>
 *  @n  Pointer to the block of memory to be cleaned up.
 *  @n  Size of the allocated memory which is being freed.
 *
 *  <b> Return Value </b>
 *  @n  Not applicable.
 */

#define Cppi_osalFree               Osal_cppiFree
/**
 * @brief   The macro is used by the CPPI LLD to provide critical sections to 
 * protect global and shared variables from
 *
 *      access from multiple cores 
 *      and 
 *      access from multiple threads on single core
 *
 * <b> Prototype: </b>
 *  The following is the C prototype for the expected OSAL API.
 *
 *  @verbatim
       void* Osal_cppiCsEnter (void)
    @endverbatim
 *     
 *  <b> Parameter </b>
 *  @n  None.
 *
 *  <b> Return Value </b>
 *  @n  Handle used to lock critical section.
 */
#define Cppi_osalCsEnter            Osal_cppiCsEnter
/**
 * @brief   The macro is used by the CPPI LLD to exit a critical section 
 *      protected using Osal_cppiCsEnter() API.
 *
 * <b> Prototype: </b>
 *  The following is the C prototype for the expected OSAL API.
 *
 *  @verbatim
       void Osal_cppiCsExit (void *CsHandle)
    @endverbatim
 *      
 *  <b> Parameter </b>
 *  @n  Handle for unlocking critical section.
 *
 *  <b> Return Value </b>
 *  @n  Not applicable.
 */
#define Cppi_osalCsExit             Osal_cppiCsExit
/**
 * @brief   The macro is used by the CPPI LLD to log various 
 * messages. 
 *
 * <b> Prototype: </b>
 *  The following is the C prototype for the expected OSAL API.
 *
 *  @verbatim
       void Osal_cppiLog( char *fmt, ... ) 
    @endverbatim
 *
 *  <b> Parameter </b>
 *  @n  printf-style format string 
 *
 *  <b> Return Value </b>
 *  @n  Not applicable.
 */
#define Cppi_osalLog                Osal_cppiLog

/**
 * @brief   The macro is used by the CPPI LLD to indicate that a block
 * of memory is about to be accessed. If the memory block is cached then
 * this indicates that the application would need to ensure that the cache
 * is updated with the data from the actual memory.
 *
 * <b> Prototype: </b>
 *  The following is the C prototype for the expected OSAL API.
 *
 *  @verbatim
       void Osal_cppiBeginMemAccess (void *ptr, uint32_t size) 
    @endverbatim
 *
 *  <b> Parameter </b>
 *  @n  Address of memory block.
 *  @n  Size of memory block.
 *
 *  <b> Return Value </b>
 *  @n  Not applicable.
 */
#define Cppi_osalBeginMemAccess     Osal_cppiBeginMemAccess

/**
 * @brief   The macro is used by the CPPI LLD to indicate that the block of 
 * memory has finished being accessed. If the memory block is cached then the 
 * application would need to ensure that the contents of the cache are updated
 * immediately to the actual memory.
 *
 * <b> Prototype: </b>
 *  The following is the C prototype for the expected OSAL API.
 *
 *  @verbatim
       void Osal_cppiEndMemAccess (void *ptr, uint32_t size) 
    @endverbatim
 *
 *  <b> Parameter </b>
 *  @n  Address of memory block.
 *  @n  Size of memory block.
 *
 *  <b> Return Value </b>
 *  @n  Not applicable.
 */
#define Cppi_osalEndMemAccess       Osal_cppiEndMemAccess
/**
@}
*/

#endif /* __CPPI_OSAL_H__ */

