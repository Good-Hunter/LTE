

#include "csl_intc.h"
#include "_csl_intc.h"

#pragma DATA_SECTION (_CSL_intcAllocMask, ".bss:csl_section:intc");
CSL_BitMask32*   _CSL_intcAllocMask = NULL;

#pragma DATA_SECTION (_CSL_intcCpuIntrTable, ".bss:csl_section:intc");
CSL_IntcVect _CSL_intcCpuIntrTable;

#pragma DATA_SECTION (_CSL_intcEventOffsetMap, ".bss:csl_section:intc");
char *_CSL_intcEventOffsetMap = NULL;

#pragma DATA_SECTION (_CSL_intcNumEvents, ".bss:csl_section:intc");
unsigned short _CSL_intcNumEvents;

#pragma DATA_SECTION (_CSL_intcEventHandlerRecord, ".bss:csl_section:intc");
CSL_IntcEventHandlerRecord* _CSL_intcEventHandlerRecord;
