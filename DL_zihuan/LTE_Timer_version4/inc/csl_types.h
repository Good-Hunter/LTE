
 

#ifndef _CSL_TYPES_H_
#define _CSL_TYPES_H_



#ifndef TRUE

#define TRUE		((unsigned short) 1)
#define FALSE		((unsigned short) 0)

#endif

typedef signed short           CSL_Uid;
typedef signed short           CSL_ModuleId;
typedef unsigned int          CSL_Xio;

typedef unsigned char           CSL_BitMask8;
typedef unsigned short          CSL_BitMask16;
typedef unsigned int          CSL_BitMask32;

typedef volatile unsigned char  CSL_Reg8;
typedef volatile unsigned short CSL_Reg16;
typedef volatile unsigned int CSL_Reg32;

typedef signed short           CSL_Status;
typedef signed short           CSL_InstNum;
typedef signed short           CSL_ChaNum;

typedef unsigned long long int  CSL_Uint64;

typedef enum {
  CSL_EXCLUSIVE = 0,
  CSL_SHARED    = 1
} CSL_OpenMode;

typedef enum {
  CSL_FAIL      = 0,
  CSL_PASS      = 1
} CSL_Test;

#ifndef NULL
#define NULL            ((void*)0)
#endif

#endif /* _CSL_TYPES_H_ */

