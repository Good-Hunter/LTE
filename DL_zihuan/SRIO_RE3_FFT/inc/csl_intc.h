
#ifndef _CSL_INTC_H_
#define _CSL_INTC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "csl.h"
#include "cslr_intc.h"
#include "soc.h"


#define CSL_INTC_EVENTID_CNT        128

/** Indicates there is no associated event-handler */
#define CSL_INTC_EVTHANDLER_NONE    ((CSL_IntcEventHandler) 0) 

/** Invalid handle */
#define CSL_INTC_BADHANDLE          (0) 

/** None mapped */
#define CSL_INTC_MAPPED_NONE        (-1)

/**
@}
*/

/** @addtogroup CSL_INTC_ENUM
 @{ */
        
/**
 *  signed interrupt Vector IDs
 */
typedef enum {
    /** Should be used only along with CSL_intcHookIsr() */
    CSL_INTC_VECTID_NMI      =   1,   
    /** CPU Vector 4 */
    CSL_INTC_VECTID_4        =   4,
    /** CPU Vector 5 */   
    CSL_INTC_VECTID_5        =   5,   
    /** CPU Vector 6 */
    CSL_INTC_VECTID_6        =   6,   
    /** CPU Vector 7 */
    CSL_INTC_VECTID_7        =   7,   
    /** CPU Vector 8 */
    CSL_INTC_VECTID_8        =   8,
    /** CPU Vector 9 */
    CSL_INTC_VECTID_9        =   9,
    /** CPU Vector 10 */   
    CSL_INTC_VECTID_10       =   10,
    /** CPU Vector 11 */   
    CSL_INTC_VECTID_11       =   11,
    /** CPU Vector 12 */  
    CSL_INTC_VECTID_12       =   12,
    /** CPU Vector 13 */  
    CSL_INTC_VECTID_13       =   13,  
    /** CPU Vector 14 */    
    CSL_INTC_VECTID_14       =   14,
    /** CPU Vector 15 */  
    CSL_INTC_VECTID_15       =   15,  
    /** Should be used at the time of opening an Event handle
     *  to specify that the event needs to go to the combiner
     */
    CSL_INTC_VECTID_COMBINE  =   16,  
    /** Should be used at the time of opening an Event handle
     * to specify that the event needs to go to the exception
     * combiner.
     */
    CSL_INTC_VECTID_EXCEP    =   17   
} CSL_IntcVectId; 

/**
 *  signed interrupt Event IDs
 */
typedef signed int     CSL_IntcEventId; 



typedef enum {
        /**
         * @brief   Enables the event
         * @param   CSL_IntcEnableState
         */
    CSL_INTC_CMD_EVTENABLE      = 0,
        /**
         * @brief   Disables the event
         * @param   CSL_IntcEnableState
         */
    CSL_INTC_CMD_EVTDISABLE     = 1,
        /**
         * @brief   Sets the event manually
         * @param   None
         */   
    CSL_INTC_CMD_EVTSET         =2,
        /**
         * @brief   Clears the event (if pending)
         * @param   None
         */
    CSL_INTC_CMD_EVTCLEAR       =3,
        /**
         * @brief   Enables the Drop Event detection feature for this event
         * @param   None
         */    
    CSL_INTC_CMD_EVTDROPENABLE  =4,
        /**
         * @brief   Disables the Drop Event detection feature for this event
         * @param   None
         */       
    CSL_INTC_CMD_EVTDROPDISABLE =5,
        /**
         * @brief   To be used ONLY to invoke the associated Function handle 
         * with Event when the user is writing an exception handling routine. 
         * @param   None
         */             
    CSL_INTC_CMD_EVTINVOKEFUNCTION = 6
} CSL_IntcHwControlCmd;


typedef enum {
        /**
         * @brief   The Pend Status of the Event is queried 
         * @param   unsigned short
         */  
    CSL_INTC_QUERY_PENDSTATUS
}CSL_IntcHwStatusQuery;



typedef enum {
        /**
         * @brief   Symbol for EXPMASK[0]
         * @param   BitMask for EXPMASK0
         */
    CSL_INTC_EXCEP_0TO31 = 0,
        /**
         * @brief   Symbol for EXPMASK[1]
         * @param   BitMask for EXPMASK1
         */
    CSL_INTC_EXCEP_32TO63 = 1,
        /**
         * @brief   Symbol for EXPMASK[2]
         * @param   BitMask for EXPMASK2
         */   
    CSL_INTC_EXCEP_64TO95 = 2,
        /**
         * @brief   Symbol for EXPMASK[3]
         * @param   BitMask for EXPMASK3
         */
    CSL_INTC_EXCEP_96TO127 = 3
} CSL_IntcExcepEn;


typedef enum {
        /**
         * @brief   Symbol for NMI
         * @param   None
         */ 
    CSL_INTC_EXCEPTION_NMI = 31,
        /**
         * @brief   Symbol for External Exception
         * @param   None
         */ 
    CSL_INTC_EXCEPTION_EXT = 30,
        /**
         * @brief   Symbol for signed internal Exception
         * @param   None
         */ 
    CSL_INTC_EXCEPTION_INT = 1,
        /**
         * @brief   Symbol for Software Exception
         * @param   None
         */ 
    CSL_INTC_EXCEPTION_SW = 0
} CSL_IntcExcep;


typedef void (* CSL_IntcEventHandler)(void *);



typedef struct  {
    /** pointer to the event handler */
    CSL_IntcEventHandler    handler;
    /** the argument to be passed to the handler when it is invoked */  
    void *                  arg;        
                                          
} CSL_IntcEventHandlerRecord;

/**
 *  INTC Module Context. 
 */
typedef struct {
    /** Pointer to the event handle record */
    CSL_IntcEventHandlerRecord* eventhandlerRecord;

    /** Event allocation mask */
    CSL_BitMask32   eventAllocMask[(CSL_INTC_EVENTID_CNT + 31) / 32];

    /** Number of event entries */
    unsigned short          numEvtEntries;

    /** Reserved */
    signed char            offsetResv[128];
} CSL_IntcContext;


/**
 *  Event enable state
 */
typedef unsigned int CSL_IntcEventEnableState;


/**
 *  Global signed interrupt enable state
 */
typedef unsigned int CSL_IntcGlobalEnableState;

/**
 *  The interrupt handle object
 *  This object is used by the handle to identify the event.
 */
typedef struct CSL_IntcObj {
    /** The event-id */
    CSL_IntcEventId eventId;
    /** The vector-id */    
    CSL_IntcVectId   vectId;    
} CSL_IntcObj;

/**
 *  The drop status structure
 *
 *  This object is used along with the CSL_intcQueryDropStatus()
 *  API.
 */
typedef struct {
    /** whether dropped/not */
    unsigned short drop;
    /** The event-id */                  
    CSL_IntcEventId eventId;
    /** The vect-id */    
    CSL_IntcVectId  vectId;     
}CSL_IntcDropStatus;

/**
 *  INTC module parameters for open
 *
 *  This is equivalent to the Vector Id for the event number.
 */
typedef CSL_IntcVectId CSL_IntcParam;

/**
 *  The interrupt handle
 *
 *  This is returned by the CSL_intcOpen(..) API. The handle is used
 *  to identify the event of interest in all INTC calls.
 */
typedef struct CSL_IntcObj*   CSL_IntcHandle;


CSL_Status CSL_intcInit (
    CSL_IntcContext     *pContext 
);


CSL_IntcHandle   CSL_intcOpen (
    CSL_IntcObj        *intcObj,    
    CSL_IntcEventId     eventId,    
    CSL_IntcParam      *params,     
    CSL_Status         *status
);


CSL_Status  CSL_intcClose (
    CSL_IntcHandle      hIntc
);


CSL_Status CSL_intcPlugEventHandler (
    CSL_IntcHandle                  hIntc,
    CSL_IntcEventHandlerRecord      *eventHandlerRecord
);


CSL_Status CSL_intcHookIsr (
    CSL_IntcVectId  vectId,
    void            *isrAddr
);



CSL_Status CSL_intcHwControl (
    CSL_IntcHandle          hIntc,      
    CSL_IntcHwControlCmd    command,    
    void                   *commandArg  
);


CSL_Status CSL_intcGetHwStatus (
    CSL_IntcHandle          hIntc,      
    CSL_IntcHwStatusQuery   query,      
    void                    *response    
);


CSL_Status CSL_intcGlobalEnable (
    CSL_IntcGlobalEnableState * prevState   

);


CSL_Status CSL_intcGlobalDisable (
    CSL_IntcGlobalEnableState   *prevState   
);


CSL_Status CSL_intcGlobalRestore (
    CSL_IntcGlobalEnableState   prevState
);


CSL_Status CSL_intcGlobalNmiEnable (void);


CSL_Status CSL_intcGlobalExcepEnable (void);  


CSL_Status CSL_intcGlobalExtExcepEnable (void);  


CSL_Status CSL_intcGlobalExcepClear (
    CSL_IntcExcep       exc
);


CSL_Status CSL_intcExcepAllEnable (
    CSL_IntcExcepEn     excepMask,
    CSL_BitMask32       excVal,
    CSL_BitMask32       *prevState    
);


CSL_Status CSL_intcExcepAllDisable (
    CSL_IntcExcepEn             excepMask,
    CSL_BitMask32               excVal,
    CSL_BitMask32               *prevState
);


CSL_Status CSL_intcExcepAllRestore (
    CSL_IntcExcepEn             excepMask,
    CSL_IntcGlobalEnableState   restoreVal
);


CSL_Status CSL_intcExcepAllClear (
    CSL_IntcExcepEn excepMask,  
    CSL_BitMask32   excVal      
);


CSL_Status CSL_intcExcepAllStatus (
    CSL_IntcExcepEn     excepMask,
    CSL_BitMask32       *status   
);      


CSL_Status CSL_intcQueryDropStatus (
    CSL_IntcDropStatus   *dropStat
);


#ifdef __cplusplus
}
#endif

#endif
