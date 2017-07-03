
#ifndef __QMSS_OSAL_H__
#define __QMSS_OSAL_H__

/** @addtogroup QMSS_LLD_OSAL
 @{ */

/**********************************************************************
 ************************* Extern Declarations ************************
 **********************************************************************/

extern void* memset(void *_mem, signed int _ch, unsigned int _n);
extern void* memcpy(void *_s1, const void *_s2, unsigned int _n);
extern void* Osal_qmssMalloc (unsigned int num_bytes);
extern void Osal_qmssFree (void *ptr, unsigned int size);
extern void* Osal_qmssCsEnter (void);
extern void Osal_qmssCsExit (void *CsHandle);
extern void* Osal_qmssMtCsEnter (void);
extern void Osal_qmssMtCsExit (void *CsHandle);
extern void Osal_qmssLog (char *fmt, ... );
extern void Osal_qmssBeginMemAccess (void *ptr, unsigned int size);
extern void Osal_qmssEndMemAccess (void *ptr, unsigned int size);



#define Qmss_osalMalloc             Osal_qmssMalloc



#define Qmss_osalFree               Osal_qmssFree


#define Qmss_osalCsEnter            Osal_qmssCsEnter


#define Qmss_osalCsExit             Osal_qmssCsExit


#define Qmss_osalMtCsEnter          Osal_qmssMtCsEnter


#define Qmss_osalMtCsExit           Osal_qmssMtCsExit


#define Qmss_osalLog                Osal_qmssLog


#define Qmss_osalBeginMemAccess     Osal_qmssBeginMemAccess


#define Qmss_osalEndMemAccess       Osal_qmssEndMemAccess

/**
@}
*/
#endif /* __QMSS_OSAL_H__ */

