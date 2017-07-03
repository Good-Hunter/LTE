
#ifndef _CPSW_SINGLECORE_H_
#define _CPSW_SINGLECORE_H_

/* C Standard library Include */
#include <string.h>

/* XDC types include */
//#include <xdc/std.h>

/* Chip Level definitions include */
#include "csl_chip.h"

/* CSL EMAC include */
#include "csl_cpsw.h"
#include "csl_cpsgmii.h"
#include "csl_cpsgmiiAux.h"
#include "csl_mdio.h"
#include "csl_mdioAux.h"

/* BootCfg module include */
#include "csl_bootcfg.h"
#include "csl_bootcfgAux.h"

/* CPPI LLD include */
#include "cppi_drv.h"
#include "cppi_desc.h"

/* QMSS LLD include */
#include "qmss_drv.h"



/** Number of ports in the ethernet subsystem */
#define         NUM_PORTS                   3u

/** Number of MAC/GMII ports in the ethernet switch */
#define         NUM_MAC_PORTS               2u

/** Number of host descriptors used by the CPSW example program */
#define         NUM_HOST_DESC               64

/** Host descriptor size. 
 *
 *  Big enough to hold the mandatory fields of the 
 *  host descriptor and PA Control Data
 * 
 *  = 32 bytes for Host desc + PA Control data
 */
#define         SIZE_HOST_DESC              80 

/** Number of PA Tx queues available */
#define         NUM_PA_TX_QUEUES            9

/** Number of PA Rx channels available */
#define         NUM_PA_RX_CHANNELS          24
#define         ETHERNET_PACK_HEAD_LEN           42
#define         ETHERNET_PACK_LEN           1440
/* Define LoopBack modes */  
#define CPSW_LOOPBACK_NONE           0
#define CPSW_LOOPBACK_INTERNAL       1
#define CPSW_LOOPBACK_EXTERNAL       2


extern Int cpswLpbkMode;
extern Int cpswSimTest;
extern Int cpswEvm6678;
extern Cppi_FlowHnd gRxFlowHnd;

extern Int32 Cpsw_SwitchOpen (void);
extern Int32 Mdio_Open (void);
extern Int32 Sgmii_Open (void);
extern Int32 Init_Qmss (void);
extern Int32 Init_Cppi (void);
extern Int32 Init_Cpsw (void);
extern Int32 Init_PASS (void);
extern Int32 Setup_Tx (void);
extern Int32 Setup_Rx (void);
extern Int32 Setup_PASS (void);
extern Uint32 Convert_CoreLocal2GlobalAddr (Uint32  addr);
extern Int32 SendPacket (void);
extern void CycleDelay (Int32 count);
extern Int32 VerifyPacket (Cppi_Desc* pCppiDesc);
extern Uint8 gHostDesc[SIZE_HOST_DESC * NUM_HOST_DESC];
#endif
