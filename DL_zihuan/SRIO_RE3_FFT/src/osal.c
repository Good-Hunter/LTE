#include <stdlib.h>

void Osal_qmssCsEnter(){} 
void Osal_qmssCsExit(){} 

void Osal_cppiCsEnter(){} 
void Osal_cppiCsExit(){} 

void Osal_cppiBeginMemAccess(){}
void Osal_cppiEndMemAccess(){}

void Osal_qmssBeginMemAccess(){}
void Osal_qmssEndMemAccess(){}


void Osal_bcpBeginMemAccess(){}
void Osal_bcpLog(){}
void Osal_bcpMultiCoreCsEnter(){}
void Osal_bcpMultiCoreCsExit(){}
void Osal_bcpEndMemAccess(){}
unsigned int * Osal_cppiMalloc(int allocSize)
{
    return (malloc( allocSize));
} 

void Osal_cppiFree(unsigned int * bufPtr)
{
    free(bufPtr);
}

unsigned int * Osal_bcpMalloc(int allocSize)
{
    return (malloc( allocSize));
}

void Osal_bcpFree(unsigned int * bufPtr)
{
    free(bufPtr);
}

void dummyTunnel(){}

void * Osal_qmssMtCsEnter (void)
{
    /* Disable all interrupts and OS scheduler.
     *
     * Acquire Multi threaded / process synchronization lock.
     */
    //coreKey [CSL_chipReadReg (CSL_CHIP_DNUM)] = Hwi_disable();

    return NULL;
}

