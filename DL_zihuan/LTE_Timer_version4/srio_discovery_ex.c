/* =============================================================================
 * Copyright (c) CommAgility Limited 2011-2012 
 *
 * Use of this software is controlled by the terms and conditions found in the
 * license agreement under which this software has been supplied
 *
 * CommAgility Limited,                		    email:   support@commagility.com  
 * Charnwood Building, Holywell Park,      	    web:     www.commagility.com 
 * Ashby Road, Loughborough,          		    general: +44 (0) 1509 228866
 * LEICESTERSHIRE, LE11 3AQ, UK.
 * =============================================================================
 */
/** 
 * @file   BSLC667X_srio_discovery_ex.c
 * @brief  BSLC667X_ Specific SRIO discovery example source file
 * @dir    \dsplib\source\BSLC667X_\example\dsp\srio\BSLC667X_srio_discovery_ex\
 *         source
 * @author Paul A. Moakes
 * 
 */
 /* =============================================================================
  * History:
  * -------- 
  * Version 0.3.0
  *   Revision: 0.0.1 [Id:LMS] [Date: 11/02/2011] Initial Release
  *   Initial revision
  *	  Revision: 0.0.2 [Id:PP] [Date: 26/04/2011] setDiscoveredBit fix
  *   Read - modify - write to set the Discovered bit of the end point
  *   Revision: 0.0.3 [Id:PAM] [Date: 17/06/2011] 8-bit SRIO Device IDs
  *   Convert all LSU operations to 8-bit transactions to support 8-bit Spartan 
  *   Device IDs		   
  *	  Revision: 0.1.0 [Id:PP] [Date: 09/09/2011] 1000MHz Fix
  *   Change PLL configuration to 1000MHz
  *   Revision: 0.1.1 [Id:PP] [Date: 15/09/2011] Added Frontpanel SRIO support
  *   Added new comp_tag id for 2c66xx with Frontpanel SRIO
  *   Revision: 0.1.2 [Id:PP] [Date: 06/12/2011] Updated printf
  *   Added spartan small_dev_id value in printf and space aligned screen printout
  *   for better readability
  *   Revision: 0.1.3 [Id:PP] [Date: 04/01/2012] Updated Board Init
  *   Initialise default card configuration
  * =============================================================================
  */
/*  Include Files
 */
#if 0
#include <stdio.h>
#include <string.h>
#include <BSLC667X.h>
#include <dsp\srio\BSLC667X_srio.h>
#include <control\BSLC667X_control.h>

/* Timeout cycles 0f 5.4 seconds */
#define SRIO_TIMEOUT		2

/* SRIO definitions for Xilinx SRIO IP Core if loaded during testing
 * These can be modified according to the FPGA image used.
 */
#define FPGA_DEVICE_ID   0x00000000
#define FPGA_DEVICE_REV  0x00000420
#define FPGA_VENDOR_ID   0x00000E01
#define FPGA_VENDOR_ID_N 0x0000000E

#define ALTERA_VENDOR_ID 0x00000009

#define FREESCALE_VENDOR_ID 0x00000002

#define WINTEGRA_WINPATH_DEVICE_ID 0x00000003
#define WINTEGRA_WINPATH_VENDOR_ID 0x00000084

/* Routing discovery limitations.
 * These can be extended at the expense of SRAM.
 */
#define SRIO_DISCO_MAX_SWITCHES	     8
#define SRIO_DISCO_MAX_ENDPOINTS	 32

/** It is not recommended to use 16-bit SRIO Id on AMC-2C6670 and AMC-V6
 *  as it will remove SRIO support for FPGA1 which can not be updated by customer
 *  However if you want to test 16-bit SRIO discovery example then please comment following
 *  definition
 *
 *  Please note you will also need to change DSP SRIO library to work with this.
 *  Please contact CommAgility for further support
 */
#define SRIO_DISCOVERY_8_BIT

#ifdef SRIO_DISCOVERY_8_BIT

/*  Global Definitions */

/* Own 16-bit large id */
#define LARGE_DEV_ID        0xFFFF
/* Own 8-bit large id */
#define SMALL_DEV_ID        0x01
/* Host unlock id (0xFFFF won't lock host device id)*/
#define HOST_DEV_ID_UNLOCK 	0xFFFF
/* 8-bit default Address for configuration */
#define DEST_DEV_ID         0xFF

#define SPARTAN_DEVICE_ID 0x0601
#define SPARTAN_VENDOR_ID 0xCACA

/* Routing discovery limitations.
 * These can be extended at the expense of SRAM.
 */
#define SRIO_DISCO_MAX_SWITCH_PORTS	 18
#define SRIO_DISCO_MAX_ROUTES	     0x100

/* Use a flag to indicate a route to to a SRIO switch
 * This must be in the upper 8 bits of the device ID which are not use for routing
 */
#define SWITCH_DEVICE_ID_FLAG       0x8000

/* Structure for signal quality measurement */
struct srioSQM {
    Uint16 portErrOK;          /* Number of Port OK errors */
    Uint16 portErrOutput;      /* Number of Port Output Error Encountered errors */
    Uint16 portErrInput;       /* Number of Port Input Error Encountered errors */
    Uint16 portErrRate;        /* Number of invalid CRC and protocol errors */
    Uint16 laneErr8B10B[4];    /* Number of 8B/10B decode errors on the lane */
} srioSQM;

/* Define the SRIO switch structure */
struct srioSwitch {
    Uint16 switchDevId;     /* Switch Device Identity */
    Uint16 switchVendId;    /* Switch Vendor Identity */
    Uint8  hopCount;        /* Hop Count to reach this switch */
    Uint16 DeviceId;        /* Device ID allocated to this switch. DeviceId[7:0] used for routing */
    Uint16 destId[SRIO_DISCO_MAX_SWITCH_PORTS];
    Uint16 vendId[SRIO_DISCO_MAX_SWITCH_PORTS];
    Uint16 devId[SRIO_DISCO_MAX_SWITCH_PORTS];
                                    /* destId of end point connected to specified port of the switch */
	Uint8  RouteTable[SRIO_DISCO_MAX_ROUTES];
									/* Global LUT for flat routing */
    struct srioSQM SQM[SRIO_DISCO_MAX_SWITCH_PORTS];

} srioSwitch;

struct srioSwitch   Switches[SRIO_DISCO_MAX_SWITCHES];

struct srioEndpoint {
    Uint16 devId;           /* Endpoint Identity */
    Uint16 vendId;          /* Endpoint Identity */
    Uint16 switchId;        /* Switch structure index to fid this Endpoint */
    Uint8  portId;          /* Switch port for this device */
    Uint8  hopCount;        /* Hop count for this device */
    Uint16 largeDeviceID;   /* Associated 16-bit Device ID for this endpoint */
    Uint8  smallDeviceID;   /* Associated 8-bit Device ID for this endpoint */
} srioEndpoint;

struct srioEndpoint Endpoints[SRIO_DISCO_MAX_ENDPOINTS];

Uint32 dspPort[2]  = { 9,    /* DSP1 Port 0 CPS1848 Port */
                             1 };  /* DSP2 Port 0 CPS1848 Port */

Uint8 cps1848Lane0[18] = { 0,    /* CPS1848 Port 0 Lane 0 */
                                   4,    /* CPS1848 Port 1 Lane 0 */
                                   8,    /* CPS1848 Port 2 Lane 0 */
                                   12,   /* CPS1848 Port 3 Lane 0 */
                                   16,   /* CPS1848 Port 4 Lane 0 */
                                   20,   /* CPS1848 Port 5 Lane 0 */
                                   24,   /* CPS1848 Port 6 Lane 0 */
                                   28,   /* CPS1848 Port 7 Lane 0 */
                                   32,   /* CPS1848 Port 8 Lane 0 */
                                   36,   /* CPS1848 Port 9 Lane 0 */
                                   40,   /* CPS1848 Port 10 Lane 0 */
                                   44,   /* CPS1848 Port 11 Lane 0 */
                                   2,    /* CPS1848 Port 12 Lane 0 */
                                   6,    /* CPS1848 Port 13 Lane 0 */
                                   10,   /* CPS1848 Port 14 Lane 0 */
                                   14,   /* CPS1848 Port 15 Lane 0 */
                                   3,    /* CPS1848 Port 16 Lane 0 */
                                   7};   /* CPS1848 Port 17 Lane 0 */

/* Currently available Device ID and Switch ID to be assigned to the
 * end point device  */
static Int16 NextAvailDeviceID = SMALL_DEV_ID;
static Int16 NextAvailSwitchID = -1;

extern far BSLC667X_BoardInfo_s g_BoardInfo;

/*  Function definitions
 */
Int32	SrioDiscoverSwitchRouting(
    Uint8 hopCount,
    Uint8 currDeviceID,
    Uint8 ucRootSwitchId,
    Uint8 outboundPort,
    Uint8 *ucSourcePort,
    Uint8 *ucSwitchFoundReturn );

void SrioDiscoverFullRouting( void );
void SrioDiscoverFullRoutingFlat( void );

Int32 SrioSetRoute(
    Uint16 devId,
    Uint16 vendId,
    Uint16 usDestId,
    Uint8  hopCount,
    Uint8  srcPort,
    Uint16 cfgDstId,
    Uint8  cfgDstPort );

Int32 SrioClearLut(
    Uint16 devId,
    Uint16 vendId,
    Uint16 usDestId,
    Uint8  hopCount );

void SrioReset( void );

Int32 SrioSwitchRouteConfigure( Uint16 devId,
                              Uint16 vendId,
                              Uint16 usDestId,
                              Uint8  hopCount,
                              Uint8  switchId,
                              Uint8  inboundPort );

void setDiscoveredBit( Uint16 usDestId,
                       Int32 hopCount);

Int32 isDiscovered( Uint16 usDestId,
                  Int32 hopCount);

Int32 SrioComponentTagTest( Uint16 usDestId,
                          Int32 hopCount,
                          Uint32 rioCompTag );

Int32 SrioPortSync( Uint16 usDestId,
                  Uint8  hopCount,
                  Uint32   portNum,
                  Uint32   *pGlobalReadAddress );

Int32 SrioPortSQMInit( Uint8  switchId,
                     Uint32   portNum,
                     Uint8  portWidth,
                     Uint8  laneNum );

Int32 SrioPortSQM( Uint8  switchId,
                 Uint32   portNum,
                 Uint8  portWidth,
                 Uint8  laneNum );

void SrioPortSQMLog( Uint32   uiIteration,
                     Uint8  switchId,
                     Uint32   portNum,
                     Uint8  portWidth );

CSL_Status status;
CSL_SrioHandle hSrio;

/* Global flag used to reset SRIO */
Uint8 gSrioIsConfigured = FALSE;
Uint8 gSrioIsDiscovery  = FALSE;

/** 
 *   @b main
 *
 *   @b Description
 *   @li This example is used to discover all devices in a SRIO network. It configures
 *       an heirarchical topology with Device IDs based on <switch ID><port>. This 
 *       information is stored in a global array to all other tests to access the 
 *       topology quickly. Since the function uses the discovery bit to track configured 
 *       devices to re-run the test the discovery bits must first be flushed.
 *
 *       The test can be run on any DSP and relies on the target DSPs having their 
 *       SRIO interfaces electrically enabled, e.g. selecting the SRIO bootmode or
 *       running the srio slave code in the srio examples directory. 
 *       ( Note: If the target DSP is not configured in SRIO bootmode it must set 
 *         CSL_SRIO_DEVICEID_REG1 = CSL_SRIO_BASE_ID manually )
 *
 *       0. This algorithm should be run from reset as it sets the SRIO discoverd bit to detect 
 *          srio loopback. 
 *       1. Perform board specific initialisation
 *       2. Initialise SRIO in 5Gbps - 4x mode in polling mode
 *       3. Clear the Switches and Endpoints tables which will be completed during discovery
 *       4. Check for basic port synchronisation to the local SRIO switch
 *       5. Perform recursive discovery of SRIO system starting with the connection to the 
 *          local switch
 *       6. Configure the routing tables of the switches discovered using heirarchical 
 *          switch routing, based on all the devices discovered in the network
 *
 *      NOTE: This example assign SRIO device ID to SRIO end point (such as DSP, FPGA) and
 *            add routing table for packet forwarding on TSI578/CPS1848 (on board switch) 
 *            based on SRIO device ID assigned during enumeration.
 *            Please pay attention to debug window in order to find out all switch port enumeration 
 *            SRIO id allocation. Please refere to appropriate hardware manual supplied
 *            with this release CD to find out which end point is connected to which port or switch. 
 *   
 */
extern void SRIO_Doorbell_ISR();

void srio_discovery()
{
    /* Default configuration will only be used if FRU read fails from DSP */
	BSLC667X_Libconfig_s LibConfig = {BSLC667X_CPU_CLK_FREQ_AUTO,BSLC667X_BOARD_CFG_DEFAULT_AMC2C6670,TRUE,TRUE,TRUE};    
	
	BSLC667X_ErrorCode Err;
	BSLC667X_SrioUserConfObj_s SrioUserConfObj;

	Int32 			iTimeout;
	Uint32 	portControl, devId;
	Int32		        portWidth;  
	Uint8   ucSourcePort=0xFF;
	Uint8   ucSwitchFound=0;

	Uint32    uiPort;
	Uint32 	uiSQMIteration;

	BSLC667X_Cps1848PortNum_e cps1848PortNumber;
	BSLC667X_DspNodeId_e DspNodeId;

	/* Initialise SRIO module */
	Err = BSLC667X_Initialise(&LibConfig);
	if (Err)
	{
		printf("BSLC667X_Initialise() failed!!, ErrorCode = 0x%x\n",Err);
	}

	/* Power down the SRIO interface to flush it if already configured */
	if( gSrioIsConfigured )
	{
    	SrioReset();
    }	
	    	
	/* for Asynchronous transfer SRIO needs to be interrupt driven */
	SrioUserConfObj.SrioModeIntrPollDriven = BSLC667X_SRIO_INTERRUPT_MODE;//BSLC667X_SRIO_POLLING_MODE;
    
	SrioUserConfObj.DevIdSetup.smallTrBaseDevId=
        SrioUserConfObj.DevIdSetup2.smallTrBaseDevId=SMALL_DEV_ID;
	SrioUserConfObj.DevIdSetup.largeTrBaseDevId=
        SrioUserConfObj.DevIdSetup2.largeTrBaseDevId=LARGE_DEV_ID;
	SrioUserConfObj.DevIdSetup.hostBaseDevId=HOST_DEV_ID_UNLOCK;

	SrioUserConfObj.ucHostEn = 1;
	SrioUserConfObj.DoorbellCallbackFuncInfo.pArg = NULL;
	SrioUserConfObj.DoorbellCallbackFuncInfo.pCallbackFunc = (pBSLC667X_AppCallBackFunction)&SRIO_Doorbell_ISR;

	/* Initialize RapidIO interface */
	Err = BSLC667X_SrioInitialise(&SrioUserConfObj);
	if (Err)
	{
		printf("BSLC667X_SrioInitialise() failed!!, ErrorCode = 0x%x\n",Err);
		return;
	}

	hSrio = CSL_SRIO_Open (CSL_SRIO);
	/** Validate opened handle */
	if ( (hSrio == NULL) ) 
    {
        printf("CSL_srioOpen() failed, ErrorCode=0x%x",BSLC667X_TICSL_TO_BSLC667X_ERR_MAKE(status));    
	    return;
    }
    
	gSrioIsConfigured = TRUE;
	
    /* Clear the discovery and endpoints tables table
     */
	memset( Switches,  0xee, sizeof( Switches ) ); 
	memset( Endpoints, 0xee, sizeof( Endpoints ) ); 

	for (uiPort=0;uiPort<1;uiPort++)
	{       
        /* Wait for the port to synchronise by reading the port error status */
	    iTimeout = 0;
        while( hSrio->RIO_SP[uiPort].RIO_SP_ERR_STAT & (1 << CSL_SRIO_RIO_SP_ERR_STAT_PORT_UNINIT_SHIFT) )
        {
            /* Delay processor */
    		BSLC667X_MicrosecWait(1000);

    		iTimeout++;
    		if( iTimeout > SRIO_TIMEOUT*5400 )
		    {		
			    printf("Fail SRIO Port Synchronisation Timeout\n");
			    return;
   	        }
        }

	    printf( "Port Synchronised\n" );

	    /* Read the Port Control Register for the negotiated port speed */
	    portControl = hSrio->RIO_SP[uiPort].RIO_SP_CTL;

	    /* Validate the Port Mode */
	    portWidth = (portControl & CSL_SRIO_RIO_SP_CTL_INIT_PWIDTH_MASK) 
									    >> CSL_SRIO_RIO_SP_CTL_INIT_PWIDTH_SHIFT;

	    if( portWidth & 0x2 )
	    {
		    printf("Using 4x port width\n");
	    }
	    else
	    {
			printf( "Using 1x port width \n");


			/*Reset LM Request*/
			hSrio->RIO_SP[uiPort].RIO_SP_LM_REQ = 0x3;			

			hSrio->RIO_PLM[uiPort].RIO_PLM_SP_LONG_CS_TX2 = 0x0 ;
			hSrio->RIO_PLM[uiPort].RIO_PLM_SP_LONG_CS_TX1 = 0x0;

			/* Delay processor */
			BSLC667X_MicrosecWait(100000);
			
			/*Magic numbers to clear Port Stopped Error bits*/
			hSrio->RIO_PLM[uiPort].RIO_PLM_SP_LONG_CS_TX2 = 0x47FF0000 ;
			hSrio->RIO_PLM[uiPort].RIO_PLM_SP_LONG_CS_TX1 = 0x2001F044;

			/*Clear Error bits*/
			hSrio->RIO_SP[uiPort].RIO_SP_ERR_STAT = hSrio->RIO_SP[uiPort].RIO_SP_ERR_STAT;

			/* Read the Port Control Register for the negotiated port speed */
			portControl = hSrio->RIO_SP[uiPort].RIO_SP_CTL;
			
			/* Validate the Port Mode */
			portWidth = (portControl & CSL_SRIO_RIO_SP_CTL_INIT_PWIDTH_MASK) 
											>> CSL_SRIO_RIO_SP_CTL_INIT_PWIDTH_SHIFT;
			
			if( portWidth & 0x2 )
			{
				printf( "After Reinit:: Using 4x port width\n");
			}
		}
		
		if(hSrio->RIO_SP[uiPort].RIO_SP_ERR_STAT & (CSL_SRIO_RIO_SP_ERR_STAT_OUTPUT_ERR_STOPPED_MASK |
														CSL_SRIO_RIO_SP_ERR_STAT_INPUT_ERR_STOPPED_MASK) )
		{
			printf( "sriodiscoverytest: DSP %d Port Stopped Error, Perfom recovery.  Error Status = 0x%08lx\n",
			           uiPort,
			           hSrio->RIO_SP[uiPort].RIO_SP_ERR_STAT );
		
			BSLC667X_DspGetGeoAddr( &DspNodeId);
		
			if(DspNodeId == BSLC667X_DSP_NODE_ID_1)
			{
				cps1848PortNumber = BSLC667X_CPS1848_PORT_DSP1_4X;
			}
			else if(DspNodeId == BSLC667X_DSP_NODE_ID_2)
			{
				cps1848PortNumber = BSLC667X_CPS1848_PORT_DSP2_4X;
			}

			Err =  BSLC667X_SrioDspToCps1848PortSync(cps1848PortNumber);

			if(Err)
			{
				printf("sriodiscoverytest: Error Recovery Failure \n");
			}
			else
			{
				hSrio->RIO_SP[uiPort].RIO_SP_ERR_STAT = hSrio->RIO_SP[uiPort].RIO_SP_ERR_STAT;
				
				printf( "sriodiscoverytest: DSP %d : After recovery.  Error Status = 0x%08lx\n",
						   uiPort,
						   hSrio->RIO_SP[uiPort].RIO_SP_ERR_STAT );
			}
		}
	}
                            
	/* Ensure DSP multicast addresses are 0xFFFF and 0xFF */
    hSrio->RIO_MULTIID_REG[1] = 0x00FFFFFF;
    hSrio->RIO_MULTIID_REG[2] = 0x00FFFFFF;
    hSrio->RIO_MULTIID_REG[3] = 0x00FFFFFF;
    		
    /* Initialise base parameters for recursive discovery algorithm */    		
	NextAvailSwitchID = -1;
	NextAvailDeviceID = SMALL_DEV_ID;
		
	SrioDiscoverSwitchRouting( 0, 
  			                   NextAvailDeviceID++,
            				   NextAvailSwitchID,
                       		   0,
                       		   &ucSourcePort,
                       		   &ucSwitchFound );
	if( ucSourcePort != 0xFF )
	{
		/* We found a switch, fill in our details */
		devId = hSrio->RIO_DEV_ID;

	    Switches[0].destId[ucSourcePort] = SMALL_DEV_ID;
	    Switches[0].devId[ucSourcePort]  = (devId & BSLC667X_RIO_DEV_ID_DEV_ID) >> 16;
		Switches[0].vendId[ucSourcePort] = devId & BSLC667X_RIO_DEV_ID_DEV_VEN_ID;
	}

	SrioDiscoverFullRouting( );
	
}

/*
 * Recursive algorithm to discover SRIO endpoints 
 */
Int32	SrioDiscoverSwitchRouting( Uint8 hopCount, 
                               Uint8 currDeviceID,
                               Uint8 ucRootSwitchId,
                               Uint8 outboundPort,
                               Uint8 *ucSourcePort,
                               Uint8 *ucSwitchFoundReturn )
{
	Int32 	        fail = 0;
	BSLC667X_SrioCompCode  	ucCompletionCode;
	Uint16 	usDestId = DEST_DEV_ID;
	Uint32 	iTimeout, maintReadValue, maintWriteValue, features, devId;
	Uint8	portTotal, portNum, portWidth, portWidthInit;
	Uint32 	rioCompTag = 0x11223344;
	Uint8   inboundPort;
    Uint16  devLargeBaseID;
    Uint8   devSmallBaseID;
    Uint8   destDevId;
    Uint32   *pGlobalReadAddress, *pGlobalWriteAddress;
	Uint32    cfgDomainRegister;    
    Uint8   ucThisSwitchId;
	Uint32 	fpgaAssblyID;
	Uint8   ucFirstSwitchDeviceID;
	Uint8	ucSwitchFound;
	
    BSLC667X_ErrorCode Err;

    printf( "    - Current Switch Id %d, Hop Count= %d\n", 
    		 ucRootSwitchId, 
    		 hopCount);
    
	/* Calculate the global SRAM addresses required for the EDMA transfer */
	pGlobalReadAddress = (Uint32 *)BSLC667X_C64X_GLOBAL_ADDR( 
										(Uint32)BSLC667X_GetDspCoreId(),
										(Uint32)&maintReadValue );

	pGlobalWriteAddress = (Uint32 *)BSLC667X_C64X_GLOBAL_ADDR( 
										(Uint32)BSLC667X_GetDspCoreId(),
										(Uint32)&maintWriteValue );

	/* 
	 * Use maintenance packets to interrogate the device features
	 * First access can take time to sync.
	 */
	iTimeout = 0;
	do {
    	/* Get the device ID */ 
		Err = BSLC667X_SrioMaintRdSync(
						(Uint32 *)BSLC667X_RIO_DEV_ID,   
						pGlobalReadAddress,  
						usDestId,			   
						hopCount,   			
		                BSLC667X_SRIO_PORT_0,
	                    BSLC667X_SRIO_LSU_NUM_0,
						&ucCompletionCode	);
    
		if( Err || ucCompletionCode ) 
		{
	        /* Delay processor 5.4 seconds */
			BSLC667X_MicrosecWait(540);

			iTimeout++;
			if( iTimeout > SRIO_TIMEOUT )
			{		
				printf(	"Maintenance Read Timeout. Completion Code = 0x%x\n",
						ucCompletionCode );
				return ++fail;
	   	    }
		}
	} while (Err || ucCompletionCode);
	
    devId = *pGlobalReadAddress;
	printf("    - Found Device ID = 0x%08x\n", devId );
	           
	Err = BSLC667X_SrioMaintRdSync(
					(Uint32 *)BSLC667X_RIO_PE_FEAT,  
					pGlobalReadAddress,   
					usDestId,			
					hopCount, 			
	                BSLC667X_SRIO_PORT_0,
                    BSLC667X_SRIO_LSU_NUM_0,
					&ucCompletionCode	);

   	if (Err || ucCompletionCode)
	    goto maintRdFail;
	    
    features = *pGlobalReadAddress;
    printf("    - Device Features = 0x%08x\n", features);
      
    /* Endpoint or switch ? */  
    if (BSLC667X_RIO_PE_FEAT_SW == (features & BSLC667X_RIO_PE_FEAT_SW))
    { 
        /* If neighbour device is a switch */         
	    Err = BSLC667X_SrioMaintRdSync( (Uint32*)BSLC667X_RIO_SW_PORT, 
	                                  pGlobalReadAddress, 
	                                  usDestId, 
	                                  hopCount, 
					                  BSLC667X_SRIO_PORT_0,
					                  BSLC667X_SRIO_LSU_NUM_0,
	                                  &ucCompletionCode );
	                                  
       	if (Err || ucCompletionCode)
    	    goto maintRdFail;

        portTotal   = (*pGlobalReadAddress & BSLC667X_RIO_SW_PORT_PORT_TOTAL) >> 8;      
        inboundPort = *pGlobalReadAddress & BSLC667X_RIO_SW_PORT_PORT_NUM;

        printf("    - Switch found, Inbound Port = %d, Total Ports %d\n", 
                   inboundPort,
                   portTotal );

		/* Return value of discovery DSP's port so we can enter details in the switch */
		if( hopCount == 0 )
		{
		 	*ucSourcePort = inboundPort;
		}

		/* Check whether the switch has been discovered and we have found a loop */
        if (isDiscovered(usDestId, hopCount)) 
        { 
            Int16 switchId;
            
            /* Since we assumed that there are no loops in the configuration, we cannot
             * come to discovered switch again unless it's a looped back port of the same switch
             * or this is a reconfiguration exercise
             */            
             
            /* Read the global SPBC_ROUTE_BASE/RIO_DOMAIN
             * All other addresses use global LUT
             */
		    if ( devId == BSLC667X_TSI578_RIO_DEV_ID_RESET_VALUE )
		    {
    		    cfgDomainRegister = BSLC667X_TSI578_SPBC_ROUTE_BASE;
		    }
		    else if ( devId == BSLC667X_CPS10Q_RIO_DEV_ID_RESET_VALUE ||
						devId == BSLC667X_CPS1848_RIO_DEV_ID_RESET_VALUE )
		    {
		        cfgDomainRegister = BSLC667X_CPS10Q_RIO_DOMAIN;
		    }
		    else
		    {
      			/* Stop recursion through this switch */                
                return 0;
            }
 
     	    Err = BSLC667X_SrioMaintRdSync( (Uint32*)cfgDomainRegister, 
    	                                  pGlobalReadAddress, 
    	                                  usDestId, 
    	                                  hopCount, 
							              BSLC667X_SRIO_PORT_0,
						                  BSLC667X_SRIO_LSU_NUM_0,
    	                                  &ucCompletionCode );
    	                                  
           	if (Err || ucCompletionCode)
        	    goto maintRdFail;

        	/* Get the switch ID from the route base */        	    
		    if (( devId == BSLC667X_TSI578_RIO_DEV_ID_RESET_VALUE ) ||
		        ( devId == BSLC667X_CPS10Q_RIO_DEV_ID_RESET_VALUE) )
		    {
    		    switchId = (*pGlobalReadAddress & 0xFF000000) >> 24;
		    }
		    else 
		    {
		        /* CPS-1848 RIO_DOMAIN */
                switchId = (*pGlobalReadAddress & 0xFF);
		    }
            
            /* Has this routeBase already been allocated by the discovery algorithm? */
            if( switchId <= NextAvailSwitchID )
            {
                if( switchId == NextAvailSwitchID )
                {
                    if( inboundPort == outboundPort )
                    {
                        printf("    - Switch local loopback detected to switch %d\n",
                               switchId);
                    }
                    else
                    {
                        printf("    - Switch external loopback detected to switch %d\n",
                               switchId);
                    }
                } else if ( switchId < NextAvailSwitchID )
                {
                    printf("    - Switch local loopback detected to switch %d\n",
                           switchId);
                } 
                    
    	        /* Fill in the device ID for reference
    	         */
    	        Switches[ucRootSwitchId].destId[outboundPort] = Switches[switchId].DeviceId;
    	        Switches[ucRootSwitchId].devId[outboundPort]  = (devId & BSLC667X_RIO_DEV_ID_DEV_ID) >> 16;
    			Switches[ucRootSwitchId].vendId[outboundPort] = devId & BSLC667X_RIO_DEV_ID_DEV_VEN_ID;
    			         
    			/* Stop recursion through this switch */                
                return 0;
                
            } else
            {
                printf("    - Reconfiguring discovered switch\n");
            }
        }            
        
        *ucSwitchFoundReturn = 1;

        /* Return the sub-assembly ID to detect CommAgility Cards
         */
    	/* Get the device assembly ID */ 
    	Err = BSLC667X_SrioMaintRdSync(
    					(Uint32 *)BSLC667X_RIO_ASBLY_ID, 
    					pGlobalReadAddress,      
    					usDestId,				
    					hopCount, 				
		                BSLC667X_SRIO_PORT_0,
			            BSLC667X_SRIO_LSU_NUM_0,
    					&ucCompletionCode	);
    
       	if (Err || ucCompletionCode)
       	{
    	    goto maintRdFail;
        }            	    

		if(( *pGlobalReadAddress & BSLC667X_RIO_ASBLY_ID_ASBLY_VEN_ID ) !=
				BSLC667X_RIO_ASBLY_ID_ASBLY_VEN_ID_CA)
		{
			/* Retry the test using the value in the component tag register
			 * AMC-V5F and AMC-3C87F use the component tag to store the 
			 * CommAgility device ID rather than ASBLY_ID register
			 */
	    	/* Get the device assembly ID */ 
	    	Err = BSLC667X_SrioMaintRdSync(
	    					(Uint32 *)BSLC667X_RIO_COMP_TAG, 
	    					pGlobalReadAddress,      
	    					usDestId,				
	    					hopCount, 				
			                BSLC667X_SRIO_PORT_0,
				            BSLC667X_SRIO_LSU_NUM_0,
	    					&ucCompletionCode	);
	    
	       	if (Err || ucCompletionCode)
	       	{
	    	    goto maintRdFail;
	        }            	    
		}

		if(( *pGlobalReadAddress & BSLC667X_RIO_ASBLY_ID_ASBLY_VEN_ID ) ==
				BSLC667X_RIO_ASBLY_ID_ASBLY_VEN_ID_CA)
		{
            printf("    - CommAgility Card Found: 0x%08x : ", *pGlobalReadAddress);
			switch ( (*pGlobalReadAddress & BSLC667X_RIO_ASBLY_ID_ASBLY_ID) >>
						BSLC667X_RIO_ASBLY_ID_ASBLY_ID_SHIFT)
			{
			  case BSLC667X_RIO_ASBLY_ID_ASBLY_ID_AMCD4F1:
	            printf("AMC-D4F1\n");
				break;
			  case BSLC667X_RIO_ASBLY_ID_ASBLY_ID_AMC6487C:
	            printf("AMC-6487C\n");
				break;
			  case BSLC667X_RIO_ASBLY_ID_ASBLY_ID_AMCV5F:
	            printf("AMC-V5F\n");
				break;
			  case BSLC667X_RIO_ASBLY_ID_ASBLY_ID_AMC3C87F:
	            printf("AMC-3C87F\n");
				break;
			  case BSLC667X_RIO_ASBLY_ID_ASBLY_ID_AMC3C87F3:
			    printf("AMC-3C87F3\n");
			  	break;
			  case BSLC667X_RIO_ASBLY_ID_ASBLY_ID_AMC3D74:
			    printf("AMC-3D74\n");
			  	break;
			  case BSLC667X_RIO_ASBLY_ID_ASBLY_ID_AMCV5F3:
			    printf("AMC-V5F3\n");
			  	break;
			  case BSLC667X_RIO_ASBLY_ID_ASBLY_ID_AMCV5F10G:
			    printf("AMC-V5F10G\n");
			  	break;
			  case BSLC667X_RIO_ASBLY_ID_ASBLY_ID_AMC2C87W3:
			    printf("AMC-2C87W3\n");
			  	break;				  
			  case BSLC667X_RIO_ASBLY_ID_ASBLY_ID_AMCV6:
			    printf("AMC-V6\n");
			  	break;			  
			  case BSLC667X_RIO_ASBLY_ID_ASBLY_ID_AMC2C6670:
			    printf("AMC-2C6670\n");
			  	break;			  
			  case BSLC667X_RIO_ASBLY_ID_ASBLY_ID_AMC2C6670_FP_SRIO:
			    printf("AMC-2C6670-FP-SRIO\n");
				break;
			  default:			
	            printf("Unknown\n");
			}
		}
		else
		{
            printf("    - Unrecognised Card Found: 0x%08x\n",*pGlobalReadAddress );
		}

        /* Test maintenance write commands before configuring 
         * routing tables
         */
        if( SrioComponentTagTest( usDestId, hopCount, rioCompTag ) )
            return 1; 
                                     
        /* Set the discovered bit so we don't loop in the port discovery
         * We do not use the discovery bit in the EDVT code as we stop at a hop count of 1 and
         * the system does not have SRIO loops
         */
        setDiscoveredBit(usDestId, hopCount);     
            
        NextAvailSwitchID++;   /* Increment the available switch ID */
        ucThisSwitchId = NextAvailSwitchID;
        
        if( ucThisSwitchId >= SRIO_DISCO_MAX_SWITCHES )
        {
            printf("    - Too many switches (%d) discovered. Stopping recursion.\n",
             		   ucThisSwitchId );
            return 1;
        }
        
        devLargeBaseID = ( SWITCH_DEVICE_ID_FLAG |
        		 	 	  ( ucThisSwitchId << 8) |
        		          currDeviceID );

		Switches[ucThisSwitchId].switchDevId    = (devId & BSLC667X_RIO_DEV_ID_DEV_ID) >> 16;
		Switches[ucThisSwitchId].switchVendId   =  devId & BSLC667X_RIO_DEV_ID_DEV_VEN_ID;
        Switches[ucThisSwitchId].hopCount       = hopCount;
    	Switches[ucThisSwitchId].DeviceId       = devLargeBaseID;

        if( devId == BSLC667X_TSI578_RIO_DEV_ID_RESET_VALUE )
        {
            printf("    - Switch is a Tundra Tsi578, configuring switch\n" );
        }
        else if( devId == BSLC667X_CPS10Q_RIO_DEV_ID_RESET_VALUE )
        {
            printf("    - Switch is an IDT CPS10Q, configuring switch\n" );
        } 
        else if( devId == BSLC667X_CPS1848_RIO_DEV_ID_RESET_VALUE )
        {
            printf("    - Switch is an IDT CPS1848, configuring switch\n" );
        } 
        else 
        {
            /* If this is not a recognised switch do not progress */
            printf("    - Unrecognised switch, can't progress recursion\n" );
            return 0;
        }
        
        /* Configure the switch in flat mode
         * Clear the LUTs, set the default port to the inbound port and the 
         * set initialize the current switch routing table to add entries for all previously discovered devices
         */
        if( SrioSwitchRouteConfigure( Switches[ucThisSwitchId].switchDevId, 
                                      Switches[ucThisSwitchId].switchVendId,
                                      usDestId, 
        							  hopCount, 
        							  ucThisSwitchId, 
        							  inboundPort ) )
		{
            return 1;
        }
            
        for (destDevId=SMALL_DEV_ID; destDevId<NextAvailDeviceID; destDevId++)
        {
            /* Add the inbound port Device ID to the switch routing table 
             * for all ports. All upstream ports can now return traffic.
             */
            SrioSetRoute( Switches[ucThisSwitchId].switchDevId,
                          Switches[ucThisSwitchId].switchVendId,
                          usDestId, 
                          hopCount, 
                          0xff,         /* Set all ports */
                          destDevId, 
                          inboundPort );                                

            Switches[ucThisSwitchId].RouteTable[destDevId] = inboundPort;
        }
        
		if( hopCount > 0 )
		{        
	        Switches[ucRootSwitchId].destId[outboundPort] = devLargeBaseID;
	        Switches[ucRootSwitchId].devId[outboundPort]  = (devId & BSLC667X_RIO_DEV_ID_DEV_ID) >> 16;
			Switches[ucRootSwitchId].vendId[outboundPort] = devId & BSLC667X_RIO_DEV_ID_DEV_VEN_ID;
          	Switches[ucRootSwitchId].RouteTable[(devLargeBaseID & 0xFF)] = outboundPort;

	        Switches[ucThisSwitchId].destId[inboundPort] = Switches[ucRootSwitchId].DeviceId; 
	        Switches[ucThisSwitchId].devId[inboundPort]  = Switches[ucRootSwitchId].switchDevId; 
	        Switches[ucThisSwitchId].vendId[inboundPort] = Switches[ucRootSwitchId].switchVendId; 
          	Switches[ucThisSwitchId].RouteTable[(Switches[ucRootSwitchId].DeviceId & 0xFF)]
          	                                             = inboundPort; 
		}

        printf("    - Switch maintenance packet routing set back to DSP\n");

        /* Investigate this switch for further devices
         * Check the synchronisation of all available ports
		 * skip our own DSP port
		 */
        NextAvailDeviceID++;

		for( portNum = 0; portNum < portTotal; portNum ++ )
		{
			printf("Switch %d Port %d ", 
			           ucThisSwitchId,
			           portNum);
			           
			if( portNum == inboundPort )
			{
				printf("ignored as this is the incoming port\n");
				continue;
			}

			/* Get the port status register */
			Err = BSLC667X_SrioMaintRdSync(
							(Uint32 *)BSLC667X_RIO_SP_ERR_STATUS(portNum),  
							pGlobalReadAddress, 
							usDestId,		 	 
							hopCount, 			  
			                BSLC667X_SRIO_PORT_0,
		                    BSLC667X_SRIO_LSU_NUM_0,
							&ucCompletionCode	);
		
	    	if (Err || ucCompletionCode)
        	    goto maintRdFail;
			
			/* Ensure that the port has trained */
			if( (*pGlobalReadAddress & BSLC667X_RIO_SP_ERR_STATUS_PORT_OK) 
						!= BSLC667X_RIO_SP_ERR_STATUS_PORT_OK )
			{
				printf("Not Trained 0x%x\n",
				           *pGlobalReadAddress);
				continue;
			}
			else
			{
				printf("Trained 0x%x\n",
				           *pGlobalReadAddress);
			}

			/* Verify Port status and Make sure there are no pending OUTPUT ERROR-STOPPED and/or INPUT ERROR-STOPPED
			 * error status in SP(n)_ERR_STAT register
			 */		
			/* Clear port errors */	
			Err = BSLC667X_SrioMaintWrSync(
							pGlobalReadAddress, 
							(Uint32 *)BSLC667X_RIO_SP_ERR_STATUS(portNum),  
							usDestId,		 	 
							hopCount, 			  
			                BSLC667X_SRIO_PORT_0,
		                    BSLC667X_SRIO_LSU_NUM_0,
							&ucCompletionCode	);
								
	    	if (Err || ucCompletionCode)
        	    goto maintRdFail;
			
			/* Get the port status register */
			Err = BSLC667X_SrioMaintRdSync(
							(Uint32 *)BSLC667X_RIO_SP_ERR_STATUS(portNum),  
							pGlobalReadAddress, 
							usDestId,		 	 
							hopCount, 			  
			                BSLC667X_SRIO_PORT_0,
		                    BSLC667X_SRIO_LSU_NUM_0,
							&ucCompletionCode	);
		
	    	if (Err || ucCompletionCode)
        	    goto maintRdFail;
			
			/* Perform Port Sync operation if INPUT and/or OUTPUT ERROR-STOPPED are set */
			if( *pGlobalReadAddress & 
				 ( BSLC667X_RIO_SP_ERR_STATUS_OUTPUT_ERR_STOP |
				  BSLC667X_RIO_SP_ERR_STATUS_INPUT_ERR_STOP |
				  BSLC667X_RIO_SP_ERR_STATUS_PORT_ERR ) )
			{

				if(BSLC667X_SrioCps1848ToEndPointPortSync(BSLC667X_SRIO_PORT_0,
														portNum,
														0,
														usDestId,
														hopCount,
														1))
				{
					printf("BSLC667X_SrioCps1848ToEndPointPortSync() Failed!! \n");
				}									
            }
            
			printf("    - Switch %d Port %d Status Reg=0x%08x\n",
	           ucThisSwitchId,
	           portNum,
	          *pGlobalReadAddress );	

			/* Get the port control register */
			Err = BSLC667X_SrioMaintRdSync(
							(Uint32 *)BSLC667X_RIO_SP_CTL(portNum),  
							pGlobalReadAddress, 
							usDestId,		 	 
							hopCount, 				
			                BSLC667X_SRIO_PORT_0,
		                    BSLC667X_SRIO_LSU_NUM_0,
							&ucCompletionCode	);
		
	    	if (Err || ucCompletionCode)
        	    goto maintRdFail;
			
			portWidth     = (*pGlobalReadAddress & BSLC667X_RIO_SP_CTL_PORT_WIDTH) 
											>> BSLC667X_RIO_SP_CTL_PORT_WIDTH_SHIFT;
											
			portWidthInit = (*pGlobalReadAddress & BSLC667X_RIO_SP_CTL_INIT_PWIDTH) 
											>> BSLC667X_RIO_SP_CTL_INIT_PWIDTH_SHIFT;
											
			if( portWidthInit == 0x1 )
			{
				    printf("    - Switch %d Port %d port width = 1x - Lane 2,  Port Control = 0x%08x\n",
								    ucThisSwitchId, portNum, *pGlobalReadAddress);
			}
			else if( portWidthInit == 0x0 )
			{
				    printf("    - Switch %d Port %d port width = 1x - Lane 0, Port Control = 0x%08x\n",
								    ucThisSwitchId, portNum, *pGlobalReadAddress);
			}
			else if( portWidthInit & 0x2 )
			{
				    printf("    - Switch %d Port %d port width 4x\n",
								    ucThisSwitchId, portNum);
			}
			else 
			{
				printf("    - Switch %d Port %d port width Invalid, Port Control = 0x%08x\n",
								ucThisSwitchId, portNum, *pGlobalReadAddress);
		    }

			/* This port is enabled and active 
             * set the route from/to that port so we can access it with a hop count increment
             */
            SrioSetRoute( Switches[ucThisSwitchId].switchDevId,
                          Switches[ucThisSwitchId].switchVendId,
                          usDestId,		/* Use default value */
    					  hopCount, 	
     					  inboundPort,  /* Use local DSP port LUT  */ 
            		  	  usDestId,     /* Use default */
    	        		  portNum );    /* Port we are discovering */

            /* Make a note of current device ID before we recurse into a potential switch
             * with multiple endpoints behind it 
             */
            ucFirstSwitchDeviceID = NextAvailDeviceID;
            ucSwitchFound		  = 0;
            
    	    /* Recursively investigate the devices from this port */          
            SrioDiscoverSwitchRouting( hopCount+1, 
                                       NextAvailDeviceID,
                                       ucThisSwitchId,
                                       portNum,
                                       ucSourcePort,
                                       &ucSwitchFound );

            /* If a switch was found set up the routing of all endpoints beyond this port
             * to go through this port
             */
            if( ucSwitchFound == 1 )
            {
                for ( destDevId=ucFirstSwitchDeviceID; destDevId<NextAvailDeviceID; destDevId++ )
                {
          	        /* Add the outbound port Device ID to the previous switch routing 
	                 * table for all ports. The route node can now send maintenance
	                 * packets upstream to the switch device ID.
	                 * Required to enable the full routing algorithm later.
	                 */
        	        SrioSetRoute( Switches[ucThisSwitchId].switchDevId,
                                  Switches[ucThisSwitchId].switchVendId,
                                  usDestId, 
        	                      hopCount, 
        	                      0xff,         /* Set all ports */
        	                      destDevId, 
        	                      portNum );      
        	                                                
                    Switches[ucThisSwitchId].RouteTable[destDevId] = portNum;
                }
            }                                  
        }
    }
    else
    { 
        devLargeBaseID = LARGE_DEV_ID;
    	devSmallBaseID = currDeviceID;

        /* Test device writes */     
        if( SrioComponentTagTest( usDestId, hopCount, rioCompTag ) )
            return 1; 

        /* Set the endpoint's discovered bit */                                     
        setDiscoveredBit( usDestId, 
                          hopCount );
      
        printf("    - Endpoint found. ");

        /* Set the small Base Device ID of the DSP Found
	     */
        *pGlobalWriteAddress = (((Uint32)devSmallBaseID << BSLC667X_RIO_BASE_ID_DEVICE_ID_SHIFT)|
                                ((Uint32)devLargeBaseID & BSLC667X_RIO_BASE_ID_LARGE_DEVICE_ID));
    
        Err = BSLC667X_SrioMaintWrSync(
    				    pGlobalWriteAddress,      
    				    (Uint32 *)BSLC667X_RIO_BASE_ID,  
    				    usDestId,		 	  
    				    hopCount, 				
                        BSLC667X_SRIO_PORT_0,
                        BSLC667X_SRIO_LSU_NUM_0,
    				    &ucCompletionCode	);
        
        /* Ignore any errors since changing the endpoint device ID can make the Ack
         * response invalid 
         */
        
        NextAvailDeviceID++;
        
        switch ( devId & CSL_SRIO_RIO_DEV_ID_DEV_VEN_ID_MASK )
        {
        	case BSLC667X_SRIO_DEV_ID_DEVICE_VENDORIDENTITY_TITCI6487:
                printf("Configuring TI DSP, DevLargeBaseID = 0x%04x, DevSmallBaseID = 0x%02x\n",
         		           devLargeBaseID, devSmallBaseID );                                           

		        /* Set the DSP DEVICE_ID_REG1 register to be the same as the 
		         * SRIO BASE_DEVICEID
				 * This will only work if the DSP bootmode is to boot from SRIO
				 * as 0x02d12004[bit 28] must == 1 to circumvent the mismatch
				 * of DEVICEID_REG1 and DEVICE_ID
				 * This normally read only bit is asserted by the DSP bootrom. 
		         */
			    Err = BSLC667X_SrioDirectIOWrSync (
					    pGlobalWriteAddress,
					    (Uint32 *)&(hSrio->RIO_MULTIID_REG[0]),
					    4,				
					    usDestId,	
	                    BSLC667X_SRIO_PORT_0,
		                BSLC667X_SRIO_LSU_NUM_0,
					    &ucCompletionCode );
							
        		break;

        	case FPGA_VENDOR_ID:
			case FPGA_VENDOR_ID_N:
				/* Get the Assembly info to check if SPARTAN or FPGA */ 
				Err = BSLC667X_SrioMaintRdSync(
								(Uint32 *)BSLC667X_RIO_ASBLY_ID,  
								pGlobalReadAddress,   
								usDestId,			
								hopCount,			
								BSLC667X_SRIO_PORT_0,
								BSLC667X_SRIO_LSU_NUM_0,
								&ucCompletionCode	);
				
				if (Err || ucCompletionCode)
					goto maintRdFail;

				fpgaAssblyID = *pGlobalReadAddress;
				
				if(fpgaAssblyID == 0x0601CACA)
				{
					devId = fpgaAssblyID;					
		            printf("Configuring SPARTAN, DevLargeBaseID = 0x%04x, DevSmallBaseID = 0x%02x, AssemblyID = 0x%08lx\n",
	        	               devLargeBaseID, devSmallBaseID, fpgaAssblyID );            
				}
				else
					printf("Configuring XILINX FPGA, DevLargeBaseID = 0x%04x, DevSmallBaseID = 0x%02x, AssemblyID = 0x%08lx\n",
        	        	       devLargeBaseID, devSmallBaseID, fpgaAssblyID );            
        		break;
				
			case WINTEGRA_WINPATH_VENDOR_ID:
				printf("Configuring WINTEGRA WINPATH, DevLargeBaseID = 0x%04x, DevSmallBaseID = 0x%02x\n",
						   devLargeBaseID, devSmallBaseID );			
				break;					
			case ALTERA_VENDOR_ID:
                printf("Configuring ALTERA FPGA, DevLargeBaseID = 0x%04x, DevSmallBaseID = 0x%02x\n",
        	               devLargeBaseID, devSmallBaseID );            
				break;			
			case FREESCALE_VENDOR_ID:
                printf("Configuring FREESCALE, DevLargeBaseID = 0x%04x, DevSmallBaseID = 0x%02x\n",
        	               devLargeBaseID, devSmallBaseID  );            
				break;
			default:
                printf("Device not recognised, DevId = 0x%x\n",
         			       devId );
        }    


        /* If this is a TI DSP or Xilinx FPGA register it as a test endpoint */
        if( (( devId & CSL_SRIO_RIO_DEV_ID_DEV_VEN_ID_MASK ) 
	           	== BSLC667X_SRIO_DEV_ID_DEVICE_VENDORIDENTITY_TITCI6487)    || 
            (( devId & CSL_SRIO_RIO_DEV_ID_DEV_VEN_ID_MASK ) 
	           	== FPGA_VENDOR_ID)											||
           	(( devId & CSL_SRIO_RIO_DEV_ID_DEV_VEN_ID_MASK ) 
	           	== WINTEGRA_WINPATH_VENDOR_ID)								||
           	(( devId & CSL_SRIO_RIO_DEV_ID_DEV_VEN_ID_MASK ) 
	           	== FPGA_VENDOR_ID_N)										||
           	(( devId & CSL_SRIO_RIO_DEV_ID_DEV_VEN_ID_MASK )
	           	== ALTERA_VENDOR_ID)									    ||
           	(( devId & CSL_SRIO_RIO_DEV_ID_DEV_VEN_ID_MASK )
	           	== FREESCALE_VENDOR_ID) 									||
           	(( devId & CSL_SRIO_RIO_DEV_ID_DEV_VEN_ID_MASK )
	           	== SPARTAN_VENDOR_ID))
        {
	        if( currDeviceID >= SRIO_DISCO_MAX_ENDPOINTS )
	        {
	            printf("    - Endpoint too many endpoints (%d) detected\n",
	             		   currDeviceID );
	            return 1;
	        }
	        
	        Endpoints[devSmallBaseID].devId    = (devId & BSLC667X_RIO_DEV_ID_DEV_ID) >> 16;
			Endpoints[devSmallBaseID].vendId   = devId & BSLC667X_RIO_DEV_ID_DEV_VEN_ID;
	        Endpoints[devSmallBaseID].switchId = ucRootSwitchId;
	        Endpoints[devSmallBaseID].portId   = outboundPort;
	        Endpoints[devSmallBaseID].hopCount = hopCount;
		}

        /* Set the local switch routing table to route all traffic 
         * to this ID to the outbound port.
         * Global routing is done later.
         */
		if( hopCount > 0 )
		{
	        SrioSetRoute( Switches[ucRootSwitchId].switchDevId,
                          Switches[ucRootSwitchId].switchVendId,
                          usDestId, 
	                      hopCount-1, 
	                      0xff,         /* Set all ports */
	                      devSmallBaseID,
	                      outboundPort );                                
			         
	        Switches[ucRootSwitchId].RouteTable[devSmallBaseID] = outboundPort;
	        Switches[ucRootSwitchId].destId[outboundPort] 		= devSmallBaseID;
	        Switches[ucRootSwitchId].devId[outboundPort]  		= (devId & BSLC667X_RIO_DEV_ID_DEV_ID) >> 16;
			Switches[ucRootSwitchId].vendId[outboundPort] 		= devId & BSLC667X_RIO_DEV_ID_DEV_VEN_ID;
		}
    }    

    /* Jump back to production test code */
    return fail;

maintRdFail:
	printf("BSLC667X_SrioMaintRdSync() Failed!! ErrorCode = %d CompletionCode = %d\n",
	            Err, ucCompletionCode );
	return ++fail;
}

/** 
 *   @b SrioDiscoverFullRouting
 *
 *   @b Description
 *   @li This function is used to parse the discovered switches to set up the switch routing tables. 
 *       This function is called recusrively and uses an heirarchical mapping.
 *
 *   @b Arguments
 *   @li None
 *
 *   <b> Return Value </b>  
 *   @li None
 *                                              
 *   <b> Pre Condition </b>
 *   @li None
 *
 *   <b> Post Condition </b>
 *   @li None
 *
 *   @b Modifies
 *   @li None
 *
 *   @b Example
     ...
     SrioDiscoverFullRouting( );
     ...
     @endverbatim
 *  
 */
void SrioDiscoverFullRouting( void )
{
    Uint8 i, j, k;    
    Uint8 ucRouteTableEntry, ucRouteTableSwitchId;

    for (i=0; i<=NextAvailSwitchID; i++)    
	{
        for (k=0; k<SRIO_DISCO_MAX_SWITCH_PORTS; k++)        
		{
	        if( Switches[i].destId[k] != 0xeeee )
	        {
	            /* If this is a valid endpoint 
	             * Fill out routing details in all other switches
	             */

	        	/* Mask routing for a switch entry */
	        	ucRouteTableEntry = Switches[i].destId[k] & 0xFF;

  	            for( j=0; j<=NextAvailSwitchID; j++)
				{
				    /* Skip yourself, any routes to this switch deviceID, and any previously discovered endpoints */
		            if ((j!=i) &&
						(Switches[i].destId[k] != Switches[j].DeviceId ) &&
						(Switches[j].RouteTable[ucRouteTableEntry] == 0xee ))
		            {
		            	ucRouteTableSwitchId = Switches[i].DeviceId & 0xFF;

		            	printf("BSLC667X_SrioFullRouting Swi-%02d Prt-%02d DevId-0x%02x DstSwi-%02d DstPrt-%02x\n",
                        		i,
                        		k,
                        		ucRouteTableEntry,
                        		j,
                        		Switches[j].RouteTable[ucRouteTableSwitchId] );
	
	              	    Switches[j].RouteTable[ucRouteTableEntry]
	              		    =  Switches[j].RouteTable[ucRouteTableSwitchId];
	              		    
              		    SrioSetRoute( Switches[i].switchDevId,
                                      Switches[i].switchVendId,
                                      (Switches[j].DeviceId & 0xFF),
	              	  			      Switches[j].hopCount, 
								      0xFF,	/* All Ports */
	              				      Switches[i].destId[k], 
	              				      Switches[j].RouteTable[ucRouteTableSwitchId] );
		            }   
				}
	        }
		}
	}
}

/** 
 *   @b SrioSetRoute
 *
 *   @b Description
 *   @li This function is used to set the switch routing table. This is either port specific
 *       or global depending on the srcPort
 *
 *   @b Arguments
     @verbatim
     devId          - switch device ID for switch specific local LUT algorithm  
     vendId         - switch vendor ID for switch specific local LUT algorithm
     usDestId		- SRIO destination address of incoming packet.
     hopCount       - Hop Count, if 0 configure local switch, if > 0 configure chained switch
     srcPort	 	- Incoming port. If < 16 ports configured individually else
     				  else all ports are configured the same.
     cfgDestId	 	- SRIO destination address of outgoing packet.
     cfgDstPort	 	- Outgoing port for the given usDestId.
     @endverbatim
 *
 *   <b> Return Value </b>  
 *   @li Failure Count - Number of failed operations.
 *   @li 0             - Success
 *                                              
 *   <b> Pre Condition </b>
 *   @li  None.
 *
 *   <b> Post Condition </b>
 *   @li None
 *
 *   @b Modifies
 *   @li None
 *
 *   @b Example
 *   @verbatim
     Int32 fail;
     ...
     fail = SrioSetRoute( TUNDRA_RIO_DEV_ID_578, TUNDRA_RIO_VEN_ID, 
                          0x08, 0, 8, 0x0A, 10 );
     ...
     @endverbatim
 *  
 */
Int32 SrioSetRoute( Uint16 devId, 
                  Uint16 vendId, 
                  Uint16 usDestId,
                  Uint8  hopCount, 
                  Uint8  srcPort, 
                  Uint16 cfgDstId, 
                  Uint8  cfgDstPort )
{
    BSLC667X_ErrorCode 	Err;
	BSLC667X_SrioCompCode 	ucCompletionCode; 
	Uint32 		cfgDstRegister  = BSLC667X_RIO_ROUTE_CFG_DESTID;
	Uint32 		cfgPortRegister = BSLC667X_RIO_ROUTE_CFG_PORT;
	Uint32        cfgDstIdTmp  =(Uint32)(cfgDstId & 0xFF);  /* Routing using only lower 8-bits of device ID */  
	Uint32        cfgDstPortTmp=(Uint32)cfgDstPort;
	
    printf("    * SrioSetRoute() dstId-0x%04x hc-%d srcPort-%d cfgDstId-0x%04x cfgDstPort-%d\n",
            usDestId,
            hopCount, 
            srcPort, 
            cfgDstId, 
            cfgDstPort );

	/* Use per port registers */
	if( ( devId == TUNDRA_RIO_DEV_ID_578 ) &&
	    ( vendId == TUNDRA_RIO_VEN_ID ))
	{
        if( srcPort < 16 )
        {
	        cfgDstRegister  = BSLC667X_TSI578_SPBC_ROUTE_CFG_DESTID + ( 0x1000 + (srcPort << 8));
	        cfgPortRegister = BSLC667X_TSI578_SPBC_ROUTE_CFG_PORT   + ( 0x1000 + (srcPort << 8));
	    }
	}
	else if( ( vendId == IDT_RIO_VEN_ID ) && 
				( ( devId == IDT_RIO_DEV_ID_CPS10Q ) || ( devId == IDT_RIO_DEV_ID_CPS1848) ) )
	{
    	Uint32 cfgSrcIdTmp;
    	
        if( srcPort < 18 )
        {
			if(devId == IDT_RIO_DEV_ID_CPS10Q)
	        	cfgSrcIdTmp = (srcPort + 1) << BSLC667X_CPS10Q_LOCAL_RTE_CONF_DESTID_SEL_CSR_SHIFT;
			else // if(devId == IDT_RIO_DEV_ID_CPS1848)
	        	cfgSrcIdTmp = (srcPort + 1);
        }
        else
        {
            cfgSrcIdTmp = 0;	
        }

        /* Configure incoming port-specific register */
        Err  = BSLC667X_SrioMaintWrSync(
                        (Uint32 *)BSLC667X_C64X_GLOBAL_ADDR( 
									        (Uint32)BSLC667X_GetDspCoreId(),
									        (Uint32)&cfgSrcIdTmp ),
				        (Uint32 *)BSLC667X_CPS10Q_LOCAL_RTE_CONF_DESTID_SEL_CSR,
				        usDestId,
				        hopCount, 					
                        BSLC667X_SRIO_PORT_0,
                        BSLC667X_SRIO_LSU_NUM_0,
				        &ucCompletionCode	);
				        
	    if (Err || ucCompletionCode )
	    {
		    printf("BSLC667X_SrioMaintWrSync() Failed!! ErrorCode = %d Completion Code = %d\n",
                   Err,
                   ucCompletionCode);
		    return 1;
	    }
	}
	else
	{
		printf("SrioSetRoute() switch not supported vendId 0x%x, devId 0x%x",
               devId, vendId);
		return 1;
	}
	
	/* Configure incoming port-specific register */
	Err  = BSLC667X_SrioMaintWrSync(
	                (Uint32 *)BSLC667X_C64X_GLOBAL_ADDR( 
										(Uint32)BSLC667X_GetDspCoreId(),
										(Uint32)&cfgDstIdTmp ),
					(Uint32 *)cfgDstRegister,
					usDestId,
					hopCount, 					
	                BSLC667X_SRIO_PORT_0,
                    BSLC667X_SRIO_LSU_NUM_0,
					&ucCompletionCode	);

	Err |= BSLC667X_SrioMaintWrSync(
	                (Uint32 *)BSLC667X_C64X_GLOBAL_ADDR( 
										(Uint32)BSLC667X_GetDspCoreId(),
										(Uint32)&cfgDstPortTmp ),
					(Uint32 *)cfgPortRegister, 
					usDestId,
					hopCount, 					
	                BSLC667X_SRIO_PORT_0,
                    BSLC667X_SRIO_LSU_NUM_0,
					&ucCompletionCode	);

	if (Err || ucCompletionCode )
	{
		printf("BSLC667X_SrioMaintWrSync() Failed!! ErrorCode = %d Completion Code = %d\n",
               Err,
               ucCompletionCode);
		return 1;
	}
	
	return 0;
}

/* Clear Tsi578 switch local and global LUTs 
 * Tsi578 must be in heirarchical mode
 */
Int32 SrioClearLutTsi578( Uint16 usDestId, 
                        Uint8  hopCount )
{	
    BSLC667X_ErrorCode 	Err;
	BSLC667X_SrioCompCode 	ucCompletionCode; 
	Uint32 		cfgDstRegister, cfgPortRegister;
	Uint32        clearLut;
	Uint32        clearPort = 0xFF;
	
	/* Use broadcast registers */
	cfgDstRegister  = BSLC667X_TSI578_SPBC_ROUTE_CFG_DESTID;
	cfgPortRegister = BSLC667X_TSI578_SPBC_ROUTE_CFG_PORT;
	
    /* Set global SPBC_ROUTE_BASE = 0 to use ID 0-0xFF to clear LUT
     * All other addresses use global LUT
     */
	clearLut = 0x0000;
	Err = BSLC667X_SrioMaintWrSync(
	                (Uint32 *)BSLC667X_C64X_GLOBAL_ADDR( 
										(Uint32)BSLC667X_GetDspCoreId(),
										(Uint32)&clearLut ),
					(Uint32 *)BSLC667X_TSI578_SPBC_ROUTE_BASE,  
					usDestId,		 	   
					hopCount, 					 
	                BSLC667X_SRIO_PORT_0,
                    BSLC667X_SRIO_LSU_NUM_0,
					&ucCompletionCode	);

	if (Err || ucCompletionCode)
	    return 1;
	    
	/* Configure incoming port-specific register */
	while( clearLut < 0x10000 )
	{
    	Err  = BSLC667X_SrioMaintWrSync(
	                    (Uint32 *)BSLC667X_C64X_GLOBAL_ADDR( 
										    (Uint32)BSLC667X_GetDspCoreId(),
										    (Uint32)&clearLut ),
    					(Uint32 *)cfgDstRegister,	
    					usDestId,		 				
    					hopCount, 					
		                BSLC667X_SRIO_PORT_0,
	                    BSLC667X_SRIO_LSU_NUM_0,
    					&ucCompletionCode	);
    
    	Err |= BSLC667X_SrioMaintWrSync(
	                    (Uint32 *)BSLC667X_C64X_GLOBAL_ADDR( 
										    (Uint32)BSLC667X_GetDspCoreId(),
										    (Uint32)&clearPort ),
    					(Uint32 *)cfgPortRegister,
    					usDestId,		 				
    					hopCount, 					
		                BSLC667X_SRIO_PORT_0,
	                    BSLC667X_SRIO_LSU_NUM_0,
    					&ucCompletionCode	);
    					
    	if (Err || ucCompletionCode)
    	    return 1;
    	    
    	if( clearLut > 0xFF )
    	    clearLut += 0x100; /* We are clearing the global LUT */
    	else
    	    clearLut++; /* We are still clearing the local LUT */    
    }
    
	return 0;
}

/* Clear IDT CPS-10Q switch local and global LUTs 
 */
Int32 SrioClearLutCps10q( Uint16 usDestId, 
                        Uint8  hopCount )
{	
    BSLC667X_ErrorCode 	Err;
	BSLC667X_SrioCompCode 	ucCompletionCode; 
	Uint32        clearLut;
	Uint32        clearPort = 0xDEDEDEDE; /* 4 ports cleared at a time */
	Uint32        uiDstId, uiRouteTable;
	
    /* Set global RIO_DOMAIN = 0 
     */
	clearLut = 0x0000;
	Err = BSLC667X_SrioMaintWrSync(
	                (Uint32 *)BSLC667X_C64X_GLOBAL_ADDR( 
										(Uint32)BSLC667X_GetDspCoreId(),
										(Uint32)&clearLut ),
					(Uint32 *)BSLC667X_CPS10Q_RIO_DOMAIN,  
					usDestId,		 	   
					hopCount, 					 
	                BSLC667X_SRIO_PORT_0,
                    BSLC667X_SRIO_LSU_NUM_0,
					&ucCompletionCode	);

	if (Err || ucCompletionCode)
	    return 1;
	    
	/* Configure incoming port-specific register */
	for( uiRouteTable = 0; uiRouteTable < 17; uiRouteTable++ )
	{
        /* Configure incoming port-specific register */
        Err  = BSLC667X_SrioMaintWrSync(
                        (Uint32 *)BSLC667X_C64X_GLOBAL_ADDR( 
									        (Uint32)BSLC667X_GetDspCoreId(),
									        (Uint32)&uiRouteTable ),
				        (Uint32 *)BSLC667X_CPS10Q_LOCAL_RTE_CONF_DESTID_SEL_CSR,
				        usDestId,		 				
				        hopCount, 					
                        BSLC667X_SRIO_PORT_0,
                        BSLC667X_SRIO_LSU_NUM_0,
				        &ucCompletionCode	);
				        
	    if (Err || ucCompletionCode )
	    {
		    printf("BSLC667X_SrioMaintWrSync() Failed!! ErrorCode = %d Completion Code = %d\n",
                   Err,
                   ucCompletionCode);
		    return 1;
	    }
	    
	    /* Use extended configuration to clear 4 LUT entries at a time */  
	    for( uiDstId = 0x80000000; uiDstId <= 0x800000FF; uiDstId+=4 )
	    {
	        /* Configure incoming port-specific register */
	        Err  = BSLC667X_SrioMaintWrSync(
	                        (Uint32 *)BSLC667X_C64X_GLOBAL_ADDR( 
										        (Uint32)BSLC667X_GetDspCoreId(),
										        (Uint32)&uiDstId ),
					        (Uint32 *)BSLC667X_RIO_ROUTE_CFG_DESTID,
					        usDestId,		 				
					        hopCount, 					
	                        BSLC667X_SRIO_PORT_0,
                            BSLC667X_SRIO_LSU_NUM_0,
					        &ucCompletionCode	);

	        Err |= BSLC667X_SrioMaintWrSync(
	                        (Uint32 *)BSLC667X_C64X_GLOBAL_ADDR( 
										        (Uint32)BSLC667X_GetDspCoreId(),
										        (Uint32)&clearPort ),
					        (Uint32 *)BSLC667X_RIO_ROUTE_CFG_PORT, 
					        usDestId,		 				
					        hopCount, 					
	                        BSLC667X_SRIO_PORT_0,
                            BSLC667X_SRIO_LSU_NUM_0,
					        &ucCompletionCode	);

	        if (Err || ucCompletionCode )
	        {
		        printf("BSLC667X_SrioMaintWrSync() Failed!! ErrorCode = %d Completion Code = %d\n",
                       Err,
                       ucCompletionCode);
		        return 1;
	        }
	    }
	}
    
	return 0;
}

/* Clear switch local and global LUTs 
 * Select approriate switch algorithm
 */
Int32 SrioClearLut( Uint16 devId, 
                  Uint16 vendId, 
                  Uint16 usDestId, 
                  Uint8  hopCount )
{	
    Int32 	            Err;
	
	/* Use per port registers */
	if( ( devId == TUNDRA_RIO_DEV_ID_578 ) &&
	    ( vendId == TUNDRA_RIO_VEN_ID ))
	{
	    Err = SrioClearLutTsi578( usDestId, hopCount );
	}
	else if( (( devId == IDT_RIO_DEV_ID_CPS10Q ) &&
	         ( vendId == IDT_RIO_VEN_ID )) ||
	          ( devId == IDT_RIO_DEV_ID_CPS1848 ) &&
	         ( vendId == IDT_RIO_VEN_ID ))
	{
	    Err = SrioClearLutCps10q( usDestId, hopCount );
	}
	else
	{
		printf("SrioClearLut() switch not dupported vendId 0x%x, devId 0x%x",
               devId, vendId);
		Err = 1;
	}
	return Err;
}

/* If Code Composer has SRIO memory windows open when SRIO is released
 * you may have to power-cycle the board.
 * You have been warned!
 */
void SrioReset( void )
{    
    Uint8 		i;

    /* Reset PLL1 enable bit in PER_SET_CNTL register to indicate Serdes are setup */  
    //hSrio->RIO_PER_SET_CNTL &= ~CSL_SRIO_PER_SET_CNTL_ENPLL1_ENABLE;
    
	/* Disable Data Flow control */  
	hSrio->RIO_PCR &= ~(1 << CSL_SRIO_RIO_PCR_PEREN_SHIFT);
	BSLC667X_MicrosecWait(100000);
	                                    	
	/* Reset all clock domains */  
    for (i = 0; i < BSLC667X_SRIO_BLOCKS_MAX; i++)
    {
        CSL_FINS(hSrio->BLOCK_ENABLE_STATUS[i].RIO_BLK_EN, SRIO_RIO_BLK_EN_EN, BSLC667X_SRIO_BLK_EN_EN_DISABLE);
    }        
       
	/* global disable */
	CSL_FINS(hSrio->RIO_GBL_EN, SRIO_RIO_GBL_EN_EN, BSLC667X_SRIO_GBL_EN_EN_DISABLE);
	
	BSLC667X_MicrosecWait(10);
}    

/* Set the ROUTE_BASE for all ports to the given route base.
 * Cleat the local and global routing tables.
 * Route all unrecognised traffic to the discovery port.
 * Note that 8-bit transfers (TT=0) will use the local LUT using destID [7:0] on IDT switches
 *  which is the same as a 16-bit transfer matching destID[15:8] = ROUTE_BASE 
 *  So this code does not need to change for 8-bit or 16-bit hierarchical transfers
 */
Int32 SrioSwitchRouteConfigure( Uint16 devId, 
                              Uint16 vendId, 
                              Uint16 usDestId, 
                              Uint8  hopCount, 
                              Uint8  switchId,
                              Uint8  inboundPort )
{
    BSLC667X_ErrorCode Err;
	BSLC667X_SrioCompCode  	 ucCompletionCode;
	Uint32     maintWriteValue;
    Uint32    *pGlobalWriteAddress;
	Uint32 	cfgDomainRegister;
	Uint32    routeBase;
	
	/* Calculate the global SRAM addresses required for the EDMA transfer */
	pGlobalWriteAddress = (Uint32 *)BSLC667X_C64X_GLOBAL_ADDR( 
										(Uint32)BSLC667X_GetDspCoreId(),
										(Uint32)&maintWriteValue );

    /* Ensure device is in heirarchical look-up table
     * SPBC_MODE[LUT_512] = 0 
     */
	/* Use per port registers */
	if( ( devId == TUNDRA_RIO_DEV_ID_578 ) &&
	    ( vendId == TUNDRA_RIO_VEN_ID ))
	{
	    /* Disable hierarchical mode on Tsi578 */
        *pGlobalWriteAddress = BSLC667X_TSI578_SPBC_MODE_RESET_VALUE;
	    Err = BSLC667X_SrioMaintWrSync(
					    pGlobalWriteAddress, 
					    (Uint32 *)BSLC667X_TSI578_SPBC_MODE,  
					    usDestId,		 	        
					    hopCount,			   
	                    BSLC667X_SRIO_PORT_0,
                        BSLC667X_SRIO_LSU_NUM_0,
					    &ucCompletionCode	);

	    if (Err || ucCompletionCode)
	        return 1;
	        
        cfgDomainRegister = BSLC667X_TSI578_SPBC_ROUTE_BASE; 
        routeBase         = switchId << 24;
	}
	else if(( devId == IDT_RIO_DEV_ID_CPS10Q ) &&
	        ( vendId == IDT_RIO_VEN_ID ))
	{
        cfgDomainRegister = BSLC667X_CPS10Q_RIO_DOMAIN; 
        routeBase         = switchId << 24;
	}
	else if(( devId == IDT_RIO_DEV_ID_CPS1848 ) &&
	        ( vendId == IDT_RIO_VEN_ID ) )
	{
        cfgDomainRegister = BSLC667X_CPS1848_RIO_DOMAIN; 
        routeBase         = switchId;
	}
	else
	{
		printf("SrioSwitchRouteConfigure() switch not supported vendId 0x%x, devId 0x%x\n",
               devId, vendId);
		return 1;
	}
    
	/* Clear local and global LUTs */
	if( SrioClearLut( devId, vendId, usDestId,	hopCount ) )
	    return 1;
	    
    /* Set global SPBC_ROUTE_BASE or RIO_DOMAIN register
     * All other addresses use global LUT
     */
    *pGlobalWriteAddress = routeBase;
	Err = BSLC667X_SrioMaintWrSync(
					pGlobalWriteAddress, 
					(Uint32 *)cfgDomainRegister,  
					usDestId,		 	   
					hopCount, 					 
	                BSLC667X_SRIO_PORT_0,
                    BSLC667X_SRIO_LSU_NUM_0,
					&ucCompletionCode	);

	if (Err || ucCompletionCode)
	    return 1;
	    
    /* Set default egress port to this incoming port
     * This will catch return maintenance traffic and and erroneous packets.
     */
    *pGlobalWriteAddress = inboundPort;
	Err = BSLC667X_SrioMaintWrSync(
					pGlobalWriteAddress, 
					(Uint32 *)BSLC667X_RIO_LUT_ATTR,  
					usDestId,		 	   
					hopCount, 					  
	                BSLC667X_SRIO_PORT_0,
                    BSLC667X_SRIO_LSU_NUM_0,
					&ucCompletionCode );

	if (Err || ucCompletionCode)
	    return 1;
	    
	return 0;
}

void setDiscoveredBit( Uint16 usDestId, 
                       Int32 hopCount)
{
	Uint32 portGenCtrlStatus ;
	BSLC667X_SrioCompCode ucCompletionCode;
	
	BSLC667X_SrioMaintRdSync((Uint32*)BSLC667X_RIO_SW_GEN_CTL, 
							(Uint32 *)BSLC667X_C64X_GLOBAL_ADDR( 
								(Uint32)BSLC667X_GetDspCoreId(),
								(Uint32)&portGenCtrlStatus ),	
							usDestId, 
							hopCount, 
				            BSLC667X_SRIO_PORT_0,
			                BSLC667X_SRIO_LSU_NUM_0,
							&ucCompletionCode );
	
	portGenCtrlStatus |= (BSLC667X_RIO_SW_GEN_CTL_DISC);
	
	BSLC667X_SrioMaintWrSync( (Uint32 *)BSLC667X_C64X_GLOBAL_ADDR( 
										(Uint32)BSLC667X_GetDspCoreId(),
										(Uint32)&portGenCtrlStatus ),
							(Uint32*)BSLC667X_RIO_SW_GEN_CTL, 
							usDestId, 
							hopCount, 
				            BSLC667X_SRIO_PORT_0,
			                BSLC667X_SRIO_LSU_NUM_0,
							&ucCompletionCode );
    return;
}

Int32 isDiscovered( Uint16 usDestId, 
                  Int32 hopCount)
{
	Uint32 portGenCtrlStatus;
	BSLC667X_SrioCompCode ucCompletionCode;
	
    BSLC667X_SrioMaintRdSync( 
                            (Uint32*)BSLC667X_RIO_SW_GEN_CTL, 
							(Uint32 *)BSLC667X_C64X_GLOBAL_ADDR( 
										(Uint32)BSLC667X_GetDspCoreId(),
										(Uint32)&portGenCtrlStatus ),
							usDestId, 
							hopCount, 
			                BSLC667X_SRIO_PORT_0,
			                BSLC667X_SRIO_LSU_NUM_0,
							&ucCompletionCode);
    								  
    return ((portGenCtrlStatus & BSLC667X_RIO_SW_GEN_CTL_DISC) ==
        									BSLC667X_RIO_SW_GEN_CTL_DISC);
}

Int32 SrioComponentTagTest( Uint16 usDestId, 
                          Int32 hopCount, 
                          Uint32 rioCompTag ) 
{
    BSLC667X_ErrorCode 	Err;
	BSLC667X_SrioCompCode ucCompletionCode;
	Uint32 maintReadValue, rioCompRestore;

    /* Read the RIO_COMP_TAG register to enable a restore */
    Err = BSLC667X_SrioMaintRdSync(
    				(Uint32 *)BSLC667X_RIO_COMP_TAG,     
    				(Uint32 *)BSLC667X_C64X_GLOBAL_ADDR( 
										(Uint32)BSLC667X_GetDspCoreId(),
										(Uint32)&rioCompRestore ),
    				usDestId,			  
    				hopCount, 			  
	                BSLC667X_SRIO_PORT_0,
                    BSLC667X_SRIO_LSU_NUM_0,
    				&ucCompletionCode	);
    
    if (Err || ucCompletionCode)
        return 1;
    
    /* Write to the RIO_COMP_TAG register to test maintenance write commands */
    Err = BSLC667X_SrioMaintWrSync(
    				(Uint32 *)BSLC667X_C64X_GLOBAL_ADDR( 
										(Uint32)BSLC667X_GetDspCoreId(),
										(Uint32)&rioCompTag ),
    				(Uint32 *)BSLC667X_RIO_COMP_TAG,  
    				usDestId,				
    				hopCount, 			    
	                BSLC667X_SRIO_PORT_0,
                    BSLC667X_SRIO_LSU_NUM_0,
    				&ucCompletionCode	);
    
    if (Err || ucCompletionCode)
        return 1;
    
    /* Read back the RIO_COMP_TAG register to ensure we read what we wrote */
    Err = BSLC667X_SrioMaintRdSync(
    				(Uint32 *)BSLC667X_RIO_COMP_TAG,     
    				(Uint32 *)BSLC667X_C64X_GLOBAL_ADDR( 
										(Uint32)BSLC667X_GetDspCoreId(),
										(Uint32)&maintReadValue ),
    				usDestId,			  
    				hopCount, 			  
	                BSLC667X_SRIO_PORT_0,
                    BSLC667X_SRIO_LSU_NUM_0,
    				&ucCompletionCode	);
    
    if (Err || ucCompletionCode)
        return 1;
    
    /* Write to the RIO_COMP_TAG register to restore the original value */
    Err = BSLC667X_SrioMaintWrSync(
    				(Uint32 *)BSLC667X_C64X_GLOBAL_ADDR( 
										(Uint32)BSLC667X_GetDspCoreId(),
										(Uint32)&rioCompRestore ),
    				(Uint32 *)BSLC667X_RIO_COMP_TAG,  
    				usDestId,				
    				hopCount, 			    
	                BSLC667X_SRIO_PORT_0,
                    BSLC667X_SRIO_LSU_NUM_0,
    				&ucCompletionCode	);
    
    if (Err || ucCompletionCode)
        return 1;
    
    if( maintReadValue != rioCompTag )
    {
    	printf("Maintenance write test failed!! RIO_Component Tag, read = 0x%x expected 0x%x\n",
    	            maintReadValue, 0x11223344);
    	return 1;
    }
    
    return 0;
}



Int32 SrioPortSQMInit( Uint8  switchId,
                     Uint32   portNum,
                     Uint8  portWidth,
                     Uint8  laneNum )
{            							
    /* Signal Quality Measurement, SRIO Gen2 Switch Sig Qual Optimization Apps Note
     * Port Configuration to Count Physical Layer Errors
     */
	Int32 	        fail = 0;
    Uint32    uiRegValue;
    Uint32   *pGlobalRegValueAddr;

    Uint8   usDestId = Switches[switchId].DeviceId;
    Uint8   hopCount = Switches[switchId].hopCount;

	BSLC667X_SrioCompCode  	ucCompletionCode;
    BSLC667X_ErrorCode 		Err;

    struct srioSQM *portSQM = &Switches[switchId].SQM[portNum];
    
    pGlobalRegValueAddr = (Uint32 *)BSLC667X_C64X_GLOBAL_ADDR(
										(Uint32)BSLC667X_GetDspCoreId(),
										(Uint32)&uiRegValue );

	/* Enable all events indicating the reception of a transmission error
	 */
	uiRegValue = 0x004E8015;
	Err = BSLC667X_SrioMaintWrSync(
					(Uint32 *)BSLC667X_CPS1848_SP_RATE_EN(portNum),  
					pGlobalRegValueAddr,
					usDestId,		 	 
					hopCount, 			  
	                BSLC667X_SRIO_PORT_0,
                    BSLC667X_SRIO_LSU_NUM_0,
					&ucCompletionCode	);

	if (Err || ucCompletionCode)
	    goto maintWrFail;
	
	/* Set maximum error counter value = 0xFF, leake rate 0, clear counter
	 */
	uiRegValue = 0x00030000;
	Err = BSLC667X_SrioMaintWrSync(
					(Uint32 *)BSLC667X_CPS1848_SP_ERR_RATE(portNum),  
					pGlobalRegValueAddr,
					usDestId,		 	 
					hopCount, 			  
	                BSLC667X_SRIO_PORT_0,
                    BSLC667X_SRIO_LSU_NUM_0,
					&ucCompletionCode	);

	if (Err || ucCompletionCode)
	    goto maintWrFail;
	
	/* Disable notification thresholds for physical layer error counts
	 */
	uiRegValue = 0x00000000;
	Err = BSLC667X_SrioMaintWrSync(
					(Uint32 *)BSLC667X_CPS1848_SP_ERR_THRESH(portNum),  
					pGlobalRegValueAddr,
					usDestId,		 	 
					hopCount, 			  
	                BSLC667X_SRIO_PORT_0,
                    BSLC667X_SRIO_LSU_NUM_0,
					&ucCompletionCode	);

	if (Err || ucCompletionCode)
	    goto maintWrFail;

    /* Dummy read to clear the error registers */
	SrioPortSQM( switchId,
                 portNum,
                 portWidth,
                 laneNum );

    /* Clear the error counters */
    memset( portSQM, 0x00, sizeof( srioSQM ));

    return fail;
    
maintWrFail:
	printf("BSLC667X_SrioMaintWrSync() Failed!! ErrorCode = %d CompletionCode = %d\n",
	            Err, ucCompletionCode );
	return ++fail;
}    			

Int32 SrioPortSQM( Uint8  switchId,
                 Uint32   portNum,
                 Uint8  portWidth,
                 Uint8  laneNum )
{            							
	Int32 	        fail = 0;
    Uint32    uiRegValue;
    Uint32   *pGlobalRegValueAddr;
    Uint8   lane;
    Uint8   uc8B10Berrors;
    
    Uint8   usDestId = Switches[switchId].DeviceId;
    Uint8   hopCount = Switches[switchId].hopCount;

	BSLC667X_SrioCompCode  	ucCompletionCode;
    BSLC667X_ErrorCode 		Err;

    struct srioSQM *portSQM;

    portSQM = &Switches[switchId].SQM[portNum];

	/* Calculate the global SRAM addresses required for the EDMA transfer */
	pGlobalRegValueAddr = (Uint32 *)BSLC667X_C64X_GLOBAL_ADDR(
										(Uint32)BSLC667X_GetDspCoreId(),
										(Uint32)&uiRegValue );

	/* Get the port status register */
	Err = BSLC667X_SrioMaintRdSync(
					(Uint32 *)BSLC667X_RIO_SP_ERR_STATUS(portNum),  
					pGlobalRegValueAddr, 
					usDestId,		 	 
					hopCount, 			  
	                BSLC667X_SRIO_PORT_0,
                    BSLC667X_SRIO_LSU_NUM_0,
					&ucCompletionCode	);

	if (Err || ucCompletionCode)
	    goto maintRdFail;
	
	/* Ensure that the port has trained */
	if( (uiRegValue & BSLC667X_RIO_SP_ERR_STATUS_PORT_OK) 
				!= BSLC667X_RIO_SP_ERR_STATUS_PORT_OK )
	{
	    portSQM->portErrOK++;
	    fail++;
	}

	/* Check for INPUT and/or OUTPUT ERROR-STOPPED are set */
	if( uiRegValue & (BSLC667X_RIO_SP_ERR_STATUS_OUTPUT_ERR_STOP |
		  BSLC667X_RIO_SP_ERR_STATUS_INPUT_ERR_STOP) )
	{
	    fail++;
	    
        if( uiRegValue & BSLC667X_RIO_SP_ERR_STATUS_OUTPUT_ERR_STOP )
    	    portSQM->portErrOutput++;
    	    
        if( uiRegValue & BSLC667X_RIO_SP_ERR_STATUS_INPUT_ERR_STOP )
    	    portSQM->portErrInput++;
    	    
		/* Clear port errors */	
		Err = BSLC667X_SrioMaintWrSync(
						pGlobalRegValueAddr, 
						(Uint32 *)BSLC667X_RIO_SP_ERR_STATUS(portNum),  
						usDestId,		 	 
						hopCount, 			  
		                BSLC667X_SRIO_PORT_0,
	                    BSLC667X_SRIO_LSU_NUM_0,
						&ucCompletionCode	);
							
    	if (Err || ucCompletionCode)
    	    goto maintWrFail;
    }
    
	/* Get the port error rate counter */
	Err = BSLC667X_SrioMaintRdSync(
					(Uint32 *)BSLC667X_CPS1848_SP_ERR_RATE(portNum),  
					pGlobalRegValueAddr, 
					usDestId,		 	 
					hopCount, 			  
	                BSLC667X_SRIO_PORT_0,
                    BSLC667X_SRIO_LSU_NUM_0,
					&ucCompletionCode	);

	if (Err || ucCompletionCode)
	    goto maintRdFail;
	    
	/* Increment the error counter */    
	portSQM->portErrRate += uiRegValue & 0xFF;
	
	/* Clear the error counter */    
	uiRegValue &= 0xFFFFFF00; 
	Err = BSLC667X_SrioMaintWrSync(
					(Uint32 *)BSLC667X_CPS1848_SP_ERR_RATE(portNum),  
					pGlobalRegValueAddr, 
					usDestId,		 	 
					hopCount, 			  
	                BSLC667X_SRIO_PORT_0,
                    BSLC667X_SRIO_LSU_NUM_0,
					&ucCompletionCode	);

	if (Err || ucCompletionCode)
	    goto maintWrFail;
	
	/* Check for 8B/10B lane errors */
	for( lane=0; lane < portWidth; lane++ )
	{
	    /* Get the port error rate counter */
	    Err = BSLC667X_SrioMaintRdSync(
					    (Uint32 *)BSLC667X_CPS1848_LA_STAT0(lane + laneNum),  
					    pGlobalRegValueAddr, 
					    usDestId,		 	 
					    hopCount, 			  
	                    BSLC667X_SRIO_PORT_0,
                        BSLC667X_SRIO_LSU_NUM_0,
					    &ucCompletionCode	);

	    if (Err || ucCompletionCode)
	        goto maintRdFail;
	        
        uc8B10Berrors = ((uiRegValue & BSLC667X_CPS1848_LA0_STAT0_CSR_8B10B_DECODE_ERR_MASK)
                              >> BSLC667X_CPS1848_LA0_STAT0_CSR_8B10B_DECODE_ERR_SHIFT);;

        portSQM->laneErr8B10B[lane] += uc8B10Berrors;
        
        fail += uc8B10Berrors;
    }	    
     return fail;
    
maintWrFail:
	printf("BSLC667X_SrioMaintWrSync() Failed!! ErrorCode = %d CompletionCode = %d\n",
	            Err, ucCompletionCode );
	return ++fail;
    
maintRdFail:
	printf("BSLC667X_SrioMaintRdSync() Failed!! ErrorCode = %d CompletionCode = %d\n",
	            Err, ucCompletionCode );
	return ++fail;
}    			

void SrioPortSQMLog( Uint32   uiIteration,
                     Uint8  switchId,
                     Uint32   portNum,
                     Uint8  portWidth )
{            							
    Uint8   lane;

    struct srioSQM *portSQM = &Switches[switchId].SQM[portNum];

    printf( "SQM-0x%08x: Swi-%01d Port-%02d NOK-%04x OP-0x%04x IP-0x%04x Rate-0x%04x ",
	        uiIteration,
	        switchId,
	        portNum,
	        portSQM->portErrOK,
   	        portSQM->portErrOutput,
   	        portSQM->portErrInput,
    	    portSQM->portErrRate );
	
	/* Check for 8B/10B lane errors */
	for( lane=0; lane < portWidth; lane++ )
	{
        printf( "LA%d-0x%04x ",
                lane,
                portSQM->laneErr8B10B[lane] );
    }	    
    printf( "\n" );
}    			
#else
/*  Global Definitions
 */

/* Own 16-bit large id */
#define LARGE_DEV_ID        0x0001
/* Own 8-bit large id */
#define SMALL_DEV_ID        0xFF
/* Host unlock id (0xFFFF won't lock host device id )*/
#define HOST_DEV_ID_UNLOCK 	0xFFFF
/* 16-bit default Address for configuration */
#define DEST_DEV_ID         0xFFFF

/* Routing discovery limitations.
 * These can be extended at the expense of SRAM.
 */
#define SRIO_DISCO_MAX_SWITCH_PORTS	 16

/* Global LUT routes only. 256 entries, but config will be limited to
 * how many switches we detect, one per switch
 */
#define SRIO_DISCO_MAX_SWITCH_ROUTES SRIO_DISCO_MAX_SWITCHES

/* Define the SRIO switch structure */
struct srioSwitch {
    unsigned short switchDevId;     /* Switch Identity */
    unsigned short switchVendId;    /* Switch Identity */
    unsigned char  hopCount;        /* Hop Count to reach this switch */
    unsigned short DeviceID;        /* Base Device ID in the path to this switch */
    unsigned short destId[SRIO_DISCO_MAX_SWITCH_PORTS];
    unsigned short devId[SRIO_DISCO_MAX_SWITCH_PORTS];
    unsigned short vendId[SRIO_DISCO_MAX_SWITCH_PORTS];
                                    /* destId of end point connected to specified port of the switch */
	unsigned char  RouteTable[SRIO_DISCO_MAX_SWITCH_ROUTES];
									/* Global LUT for heirarchical routing */
} srioSwitch;

struct srioSwitch   Switches[SRIO_DISCO_MAX_SWITCHES];

struct srioEndpoint {
    unsigned short devId;           /* Endpoint Identity */
    unsigned short vendId;          /* Endpoint Identity */
    unsigned short switchId;        /* Switch structure index to fid this Endpoint */
    unsigned char  portId;          /* Switch port for this device */
    unsigned char  hopCount;        /* Hop count for this device */
    unsigned short DeviceID;        /* Associated Device ID for this endpoint */
} srioEndpoint;

struct srioEndpoint Endpoints[SRIO_DISCO_MAX_ENDPOINTS];

unsigned int dspPort[2]  = { 9,    /* DSP1 Port 0 CPS1848 Port */
                             1 };  /* DSP2 Port 0 CPS1848 Port */

/* Currently available Device ID and Switch ID to be assigned to the
 * end point device  */
static short NextAvailEndpoint = 0;
static short NextAvailSwitchID = -1;

/* Global flag used to reset SRIO */
unsigned char gSrioIsConfigured = FALSE;

extern far BSLC667X_BoardInfo_s g_BoardInfo;


/*  Function definitions
 */
int	SrioDiscoverSwitchRouting(
    unsigned char hopCount,
    unsigned char currDeviceID,
    unsigned char currSwitchID,
    unsigned char outboundPort,
    unsigned char *sourcePort );

int SrioDiscoverFullRouting( void );

int SrioSetRoute(
    unsigned short devId,
    unsigned short vendId,
    unsigned short destId,
    unsigned char  hopCount,
    unsigned char  srcPort,
    unsigned short cfgDstId,
    unsigned char  cfgDstPort );

int SrioClearLut(
    unsigned short devId,
    unsigned short vendId,
    unsigned short usDestId,
    unsigned char  hopCount );

void SrioReset( void );

int SrioSwitchRouteConfigure( unsigned short devId,
                              unsigned short vendId,
                              unsigned short usDestId,
                              unsigned char  hopCount,
                              unsigned short routeBase,
                              unsigned short inboundPort );

void setDiscoveredBit( unsigned short usDestId,
                       int hopCount);

int isDiscovered( unsigned short usDestId,
                  int hopCount);

int SrioComponentTagTest( unsigned short usDestId,
                          int hopCount,
                          unsigned int rioCompTag );

CSL_Status status;
CSL_SrioHandle hSrio;

/**
 *   @b main
 *
 *   @b Description
 *   @li This example is used to discover all devices in a SRIO network. It configures
 *       an heirarchical topology with Device IDs based on <switch ID><port>. This
 *       information is stored in a global array to all other tests to access the
 *       topology quickly. Since the function uses the discovery bit to track configured
 *       devices to re-run the test the discovery bits must first be flushed.
 *
 *       The test can be run on any DSP and relies on the target DSPs having their
 *       SRIO interfaces electrically enabled, e.g. selecting the SRIO bootmode or
 *       running the srio slave code in the srio examples directory.
 *       ( Note: If the target DSP is not configured in SRIO bootmode it must set
 *         CSL_SRIO_DEVICEID_REG1 = CSL_SRIO_BASE_ID manually )
 *
 *       0. This algorithm should be run from reset as it sets the SRIO discoverd bit to detect
 *          srio loopback.
 *       1. Perform board specific initialisation
 *       2. Initialise SRIO in 3.125Gbps - 4x mode in polling mode
 *       3. Clear the Switches and Endpoints tables which will be completed during discovery
 *       4. Check for basic port synchronisation to the local SRIO switch
 *       5. Perform recursive discovery of SRIO system starting with the connection to the
 *          local switch
 *       6. Configure the routing tables of the switches discovered using heirarchical
 *          switch routing, based on all the devices discovered in the network
 *
 *      NOTE: This example assign SRIO device ID to SRIO end point (such as DSP, FPGA) and
 *            add routing table for packet forwarding on TSI578 (on board switch)
 *            based on SRIO device ID assigned during enumeration.
 *            Please pay attention to debug window in order to find out all switch port enumeration
 *            SRIO id allocation. Please refere to appropriate hardware manual supplied
 *            with this release CD to find out which end point is connected to which port or switch.
 *
 */
void srio_discovery()
{
	BSLC667X_ErrorCode Err;
	BSLC667X_SrioUserConfObj_s SrioUserConfObj;
    /* Default configuration will only be used if FRU read fails from DSP */
	BSLC667X_Libconfig_s LibConfig = {BSLC667X_CPU_CLK_FREQ_AUTO,BSLC667X_BOARD_CFG_DEFAULT_AMC2C6670,TRUE,TRUE,TRUE};

	int 			timeout;
	unsigned int 	portControl, devId;
	int		        portWidth;
	unsigned char   sourcePort=0xFF;

	unsigned char   destPort;

	unsigned int uiPort;

	BSLC667X_SrioCompCode completeCode;
	unsigned int     maintWriteValue;
    unsigned int    *pGlobalWriteAddress;
	unsigned int lane4x = 0, laneRetry=0;

	/* Calculate the global SRAM addresses required for the EDMA transfer */
	pGlobalWriteAddress = (unsigned int *)BSLC667X_C64X_GLOBAL_ADDR(
										(unsigned int)BSLC667X_GetDspCoreId(),
										(unsigned int)&maintWriteValue );


	/* Initialise SRIO module */
	Err = BSLC667X_Initialise(&LibConfig);
	if (Err)
	{
		printf("BSLC667X_Initialise() failed!!, ErrorCode = 0x%x\n",Err);
		return;
	}

	/* Power down the SRIO interface to flush it if already configured */
	if( gSrioIsConfigured )
	{
    	SrioReset();
    }

	/* for Asynchronous transfer SRIO needs to be interrupt driven */
	SrioUserConfObj.SrioModeIntrPollDriven = BSLC667X_SRIO_INTERRUPT_MODE;//BSLC667X_SRIO_POLLING_MODE;

	SrioUserConfObj.DevIdSetup.smallTrBaseDevId=
        SrioUserConfObj.DevIdSetup2.smallTrBaseDevId=SMALL_DEV_ID;
	SrioUserConfObj.DevIdSetup.largeTrBaseDevId=
        SrioUserConfObj.DevIdSetup2.largeTrBaseDevId=LARGE_DEV_ID;
	SrioUserConfObj.DevIdSetup.hostBaseDevId=HOST_DEV_ID_UNLOCK;

	SrioUserConfObj.PortSpeed = BSLC667X_SRIO_PORT_SPEED_5;
	SrioUserConfObj.PortWidth = BSLC667X_SRIO_PORT_WIDTH_4X;

	SrioUserConfObj.ucHostEn = 1;
	SrioUserConfObj.DoorbellCallbackFuncInfo.pArg = NULL;
	SrioUserConfObj.DoorbellCallbackFuncInfo.pCallbackFunc = (pBSLC667X_AppCallBackFunction)&DoorbellCallbackFunc;

	/* Initialize RapidIO interface */
	Err = BSLC667X_SrioInitialise(&SrioUserConfObj);
	if (Err)
	{
		printf("BSLC667X_SrioInitialise() failed!!, ErrorCode = 0x%x\n",Err);
		return;
	}

	hSrio = CSL_SRIO_Open (CSL_SRIO);
	/** Validate opened handle */
	if ( (hSrio == NULL) )
    {
        printf("CSL_srioOpen() failed, ErrorCode=0x%x",BSLC667X_TICSL_TO_BSLC667X_ERR_MAKE(status));
	    return;
    }

	gSrioIsConfigured = TRUE;

    /* Clear the discovery and endpoints tables table
     */
	memset( Switches,  0xee, sizeof( Switches ) );
	memset( Endpoints, 0xee, sizeof( Endpoints ) );

	for (uiPort=0;uiPort<1;uiPort++)
	{
        /* Wait for the port to synchronise by reading the port error status */
	    timeout = 0;
        while( hSrio->RIO_SP[uiPort].RIO_SP_ERR_STAT & (1 << CSL_SRIO_RIO_SP_ERR_STAT_PORT_UNINIT_SHIFT) )
        {
            /* Delay processor */
    		BSLC667X_MicrosecWait(1000);

    		timeout++;
    		if( timeout > SRIO_TIMEOUT*5400 )
		    {
			    printf("Fail SRIO Port Synchronisation Timeout\n");
			    return;
   	        }
        }

	    printf( "Port Synchronised\n" );

	    /* Read the Port Control Register for the negotiated port speed */
	    portControl = hSrio->RIO_SP[uiPort].RIO_SP_CTL;

	    /* Validate the Port Mode */
	    portWidth = (portControl & CSL_SRIO_RIO_SP_CTL_INIT_PWIDTH_MASK)
									    >> CSL_SRIO_RIO_SP_CTL_INIT_PWIDTH_SHIFT;

	    if( portWidth & 0x2 )
	    {
		    printf("Using 4x port width\n");
	    }
	    else
	    {
			printf("Using port width 1x expected 4x \n");

			do
			{
				#if 0
				*pGlobalWriteAddress = 0x02500000;
				BSLC667X_SrioMaintWrSync(pGlobalWriteAddress,
										(unsigned int*)BSLC667X_CPS1848_SP_PORT_OPS(dspPort[g_BoardInfo.DspId-1]),
										0xFFFF,
										0,0,0,
										&completeCode);
				#endif

				hSrio->RIO_SP[uiPort].RIO_SP_LM_REQ = 0x4;

				/* Delay processor */
				BSLC667X_MicrosecWait(1000000);

				/* Read the Port Control Register for the negotiated port speed */
				portControl = hSrio->RIO_SP[uiPort].RIO_SP_CTL;

				/* Validate the Port Mode */
				portWidth = (portControl & CSL_SRIO_RIO_SP_CTL_INIT_PWIDTH_MASK)
												>> CSL_SRIO_RIO_SP_CTL_INIT_PWIDTH_SHIFT;

				if( portWidth & 0x2 )
				{
					printf("After REINIT:: Using 4x port width\n");
					lane4x = 1;
				}
				laneRetry++;
			}while((lane4x == 0)&&(laneRetry < 4));
	    }
	}

	/* Ensure DSP multicast addresses are 0xFFFF and 0xFF */
    hSrio->RIO_MULTIID_REG[1] = 0x00FFFFFF;
    hSrio->RIO_MULTIID_REG[2] = 0x00FFFFFF;
    hSrio->RIO_MULTIID_REG[3] = 0x00FFFFFF;

    /* Initialise base parameters for recursive discovery algorithm */
	NextAvailSwitchID = -1;
	NextAvailEndpoint = 0;

	SrioDiscoverSwitchRouting( 0,
  			                   LARGE_DEV_ID,
            				   0,
                       		   0,
                       		   &sourcePort );
	if( sourcePort != 0xFF )
	{
		/* We found a switch, fill in our details */
		devId = hSrio->RIO_DEV_ID;

	    Switches[0].destId[sourcePort] = LARGE_DEV_ID;
	    Switches[0].devId[sourcePort]  = (devId & BSLC667X_RIO_DEV_ID_DEV_ID) >> 16;
		Switches[0].vendId[sourcePort] = devId & BSLC667X_RIO_DEV_ID_DEV_VEN_ID;
	}

	SrioDiscoverFullRouting( );

}

/*
 * Recursive algorithm to discover SRIO endpoints
 */
int	SrioDiscoverSwitchRouting( unsigned char hopCount,
                               unsigned char currDeviceID,
                               unsigned char currSwitchID,
                               unsigned char outboundPort,
                               unsigned char *sourcePort )
{
	int 	        fail = 0;
	BSLC667X_SrioCompCode  	ucCompletionCode;
	unsigned short 	usDestId = DEST_DEV_ID;
	unsigned int 	timeout, maintReadValue, maintWriteValue, features, devId;
	unsigned char	portTotal, portNum, portWidth, portWidthInit;
	unsigned int 	rioCompTag = 0x11223344;
	unsigned int    inboundPort;
    unsigned short  devLargeBaseID;
    unsigned int   *pGlobalReadAddress, *pGlobalWriteAddress;
	unsigned int    cfgDomainRegister;
	unsigned int fpgaAssblyID;

    BSLC667X_ErrorCode Err;

    printf( "    - Current Switch Id %d, Hop Count= %d\n",
    		 currSwitchID,
    		 hopCount);

	/* Calculate the global SRAM addresses required for the EDMA transfer */
	pGlobalReadAddress = (unsigned int *)BSLC667X_C64X_GLOBAL_ADDR(
										(unsigned int)BSLC667X_GetDspCoreId(),
										(unsigned int)&maintReadValue );

	pGlobalWriteAddress = (unsigned int *)BSLC667X_C64X_GLOBAL_ADDR(
										(unsigned int)BSLC667X_GetDspCoreId(),
										(unsigned int)&maintWriteValue );

	/*
	 * Use maintenance packets to interrogate the device features
	 * First access can take time to sync.
	 */
	timeout = 0;
	do {
    	/* Get the device ID */
		Err = BSLC667X_SrioMaintRdSync(
						(unsigned int *)BSLC667X_RIO_DEV_ID,
						pGlobalReadAddress,
						usDestId,
						hopCount,
		                BSLC667X_SRIO_PORT_0,
	                    BSLC667X_SRIO_LSU_NUM_0,
						&ucCompletionCode	);

		if( Err || ucCompletionCode )
		{
	        /* Delay processor 5.4 seconds */
			BSLC667X_MicrosecWait(540);

			timeout++;
			if( timeout > SRIO_TIMEOUT )
			{
				printf(	"Maintenance Read Timeout. Completion Cod e = 0x%x\n",
						ucCompletionCode );
				return ++fail;
	   	    }
		}
	} while (Err || ucCompletionCode);

    devId = *pGlobalReadAddress;
	printf("    - Found Device ID = 0x%x\n", devId );

	/* Get the device features */
	Err = BSLC667X_SrioMaintRdSync(
					(unsigned int *)BSLC667X_RIO_PE_FEAT,
					pGlobalReadAddress,
					usDestId,
					hopCount,
	                BSLC667X_SRIO_PORT_0,
                    BSLC667X_SRIO_LSU_NUM_0,
					&ucCompletionCode	);

   	if (Err || ucCompletionCode)
	    goto maintRdFail;

    features = *pGlobalReadAddress;
    printf("    - Device Features = 0x%x\n", features);

    /* Endpoint or switch ? */
    if (BSLC667X_RIO_PE_FEAT_SW == (features & BSLC667X_RIO_PE_FEAT_SW))
    {
        /* If neighbour device is a switch */
	    Err = BSLC667X_SrioMaintRdSync( (unsigned int*)BSLC667X_RIO_SW_PORT,
	                                  pGlobalReadAddress,
	                                  usDestId,
	                                  hopCount,
					                  BSLC667X_SRIO_PORT_0,
					                  BSLC667X_SRIO_LSU_NUM_0,
	                                  &ucCompletionCode );

       	if (Err || ucCompletionCode)
    	    goto maintRdFail;

        portTotal   = (*pGlobalReadAddress & BSLC667X_RIO_SW_PORT_PORT_TOTAL) >> 8;
        inboundPort = *pGlobalReadAddress & BSLC667X_RIO_SW_PORT_PORT_NUM;

        printf("    - Switch found, Inbound Port = %d, Total Ports %d\n",
                   inboundPort,
                   portTotal );

		/* Return value of discovery DSP's port so we can enter details in the switch */
		if( hopCount == 0 )
		{
		 	*sourcePort = inboundPort;
		}

		/* Check whether the switch has been discovered and we have found a loop */
        if (isDiscovered(usDestId, hopCount))
        {
            short switchId;

            /* Since we assumed that there are no loops in the configuration, we cannot
             * come to discovered switch again unless it's a looped back port of the same switch
             * or this is a reconfiguration exercise
             */

            /* Read the global SPBC_ROUTE_BASE/RIO_DOMAIN
             * All other addresses use global LUT
             */
		    if ( devId == BSLC667X_TSI578_RIO_DEV_ID_RESET_VALUE )
		    {
    		    cfgDomainRegister = BSLC667X_TSI578_SPBC_ROUTE_BASE;
		    }
		    else if ( devId == BSLC667X_CPS10Q_RIO_DEV_ID_RESET_VALUE ||
						devId == BSLC667X_CPS1848_RIO_DEV_ID_RESET_VALUE )
		    {
		        cfgDomainRegister = BSLC667X_CPS10Q_RIO_DOMAIN;
		    }
		    else
		    {
      			/* Stop recursion through this switch */
                return 0;
            }

     	    Err = BSLC667X_SrioMaintRdSync( (unsigned int*)cfgDomainRegister,
    	                                  pGlobalReadAddress,
    	                                  usDestId,
    	                                  hopCount,
							              BSLC667X_SRIO_PORT_0,
						                  BSLC667X_SRIO_LSU_NUM_0,
    	                                  &ucCompletionCode );

           	if (Err || ucCompletionCode)
        	    goto maintRdFail;

        	/* Shift down the route base to get the switch ID */
            switchId = (*pGlobalReadAddress & 0xFF000000) >> 24;

            /* Has this routeBase already been allocated by the discovery algorithm? */
            if( switchId <= NextAvailSwitchID )
            {
                if( switchId == NextAvailSwitchID )
                {
                    if( inboundPort == outboundPort )
                    {
                        printf("    - Switch local loopback detected to switch %d\n",
                               switchId);
                    }
                    else
                    {
                        printf("    - Switch external loopback detected to switch %d\n",
                               switchId);
                    }
                } else if ( switchId < NextAvailSwitchID )
                {
                    printf("    - Switch local loopback detected to switch %d\n",
                           switchId);
                }

    	        /* Fill in the device ID for reference
    	         */
    	        Switches[currSwitchID].destId[outboundPort] = (switchId << 8) | 0xFF;
    	        Switches[currSwitchID].devId[outboundPort]  = (devId & BSLC667X_RIO_DEV_ID_DEV_ID) >> 16;
    			Switches[currSwitchID].vendId[outboundPort] = devId & BSLC667X_RIO_DEV_ID_DEV_VEN_ID;

    			/* Stop recursion through this switch */
                return 0;

            } else
            {
                printf("    - Reconfiguring discovered switch\n");
            }
        }

        /* Return the sub-assembly ID to detect CommAgility Cards
         */
    	/* Get the device assembly ID */
    	Err = BSLC667X_SrioMaintRdSync(
    					(unsigned int *)BSLC667X_RIO_ASBLY_ID,
    					pGlobalReadAddress,
    					usDestId,
    					hopCount,
		                BSLC667X_SRIO_PORT_0,
			            BSLC667X_SRIO_LSU_NUM_0,
    					&ucCompletionCode	);

       	if (Err || ucCompletionCode)
       	{
    	    goto maintRdFail;
        }

		if(( *pGlobalReadAddress & BSLC667X_RIO_ASBLY_ID_ASBLY_VEN_ID ) !=
				BSLC667X_RIO_ASBLY_ID_ASBLY_VEN_ID_CA)
		{
			/* Retry the test using the value in the component tag register
			 * AMC-V5F and AMC-3C87F use the component tag to store the
			 * CommAgility device ID rather than ASBLY_ID register
			 */
	    	/* Get the device assembly ID */
	    	Err = BSLC667X_SrioMaintRdSync(
	    					(unsigned int *)BSLC667X_RIO_COMP_TAG,
	    					pGlobalReadAddress,
	    					usDestId,
	    					hopCount,
			                BSLC667X_SRIO_PORT_0,
				            BSLC667X_SRIO_LSU_NUM_0,
	    					&ucCompletionCode	);

	       	if (Err || ucCompletionCode)
	       	{
	    	    goto maintRdFail;
	        }
		}

		if(( *pGlobalReadAddress & BSLC667X_RIO_ASBLY_ID_ASBLY_VEN_ID ) ==
				BSLC667X_RIO_ASBLY_ID_ASBLY_VEN_ID_CA)
		{
            printf("    - CommAgility Card Found: 0x%x : ", *pGlobalReadAddress);
			switch ( (*pGlobalReadAddress & BSLC667X_RIO_ASBLY_ID_ASBLY_ID) >>
						BSLC667X_RIO_ASBLY_ID_ASBLY_ID_SHIFT)
			{
			  case BSLC667X_RIO_ASBLY_ID_ASBLY_ID_AMCD4F1:
	            printf("AMC-D4F1\n");
				break;
			  case BSLC667X_RIO_ASBLY_ID_ASBLY_ID_AMC6487C:
	            printf("AMC-6487C\n");
				break;
			  case BSLC667X_RIO_ASBLY_ID_ASBLY_ID_AMCV5F:
	            printf("AMC-V5F\n");
				break;
			  case BSLC667X_RIO_ASBLY_ID_ASBLY_ID_AMC3C87F:
	            printf("AMC-3C87F\n");
				break;
			  case BSLC667X_RIO_ASBLY_ID_ASBLY_ID_AMC3C87F3:
			    printf("AMC-3C87F3\n");
			  break;
			  case BSLC667X_RIO_ASBLY_ID_ASBLY_ID_AMC3D74:
			    printf("AMC-3D74\n");
			  break;
			  case BSLC667X_RIO_ASBLY_ID_ASBLY_ID_AMCV5F3:
			    printf("AMC-V5F3\n");
			  break;
			  case BSLC667X_RIO_ASBLY_ID_ASBLY_ID_AMCV5F10G:
			    printf("AMC-V5F10G\n");
			  break;
			  case BSLC667X_RIO_ASBLY_ID_ASBLY_ID_AMC2C87W3:
			    printf("AMC-2C87W3\n");
			  break;
			  case BSLC667X_RIO_ASBLY_ID_ASBLY_ID_AMC2C6670:
			    printf("AMC-2C6670\n");
			  break;
			  default:
	            printf("Unknown\n");
			}
		}
		else
		{
            printf("    - Unrecognised Card Found: 0x%x\n",*pGlobalReadAddress );
		}

        /* Test maintenance write commands before configuring
         * routing tables
         */
        if( SrioComponentTagTest( usDestId, hopCount, rioCompTag ) )
            return 1;

        /* Set the discovered bit so we don't loop in the port discovery
         * We do not use the discovery bit in the EDVT code as we stop at a hop count of 1 and
         * the system does not have SRIO loops
         */
        setDiscoveredBit(usDestId, hopCount);

        NextAvailSwitchID++;   /* Increment the available switch ID */

        if( NextAvailSwitchID >= SRIO_DISCO_MAX_SWITCHES )
        {
            printf("    - Too many switches (%d) discovered. Stopping recursion.\n",
             		   NextAvailSwitchID );
            return 1;
        }

		Switches[NextAvailSwitchID].switchDevId    = (devId & BSLC667X_RIO_DEV_ID_DEV_ID) >> 16;
		Switches[NextAvailSwitchID].switchVendId   =  devId & BSLC667X_RIO_DEV_ID_DEV_VEN_ID;
        Switches[NextAvailSwitchID].hopCount       = hopCount;

        if( devId == BSLC667X_TSI578_RIO_DEV_ID_RESET_VALUE )
        {
            printf("    - Switch is a Tundra Tsi578, configuring switch\n" );
        }
        else if( devId == BSLC667X_CPS10Q_RIO_DEV_ID_RESET_VALUE )
        {
            printf("    - Switch is an IDT CPS10Q, configuring switch\n" );
        }
        else if( devId == BSLC667X_CPS1848_RIO_DEV_ID_RESET_VALUE )
        {
            printf("    - Switch is an IDT CPS1848, configuring switch\n" );
        }
        else
        {
            /* If this is not a recognised switch do not progress */
            printf("    - Unrecognised switch, can't progress recursion\n" );
            return 0;
        }

        /* Configure the switch in heirarchical mode
         * Clear the LUTs, set the default port to the inbound port and the
         * set route base of all ports to the switch ID
         */
        if( SrioSwitchRouteConfigure( Switches[NextAvailSwitchID].switchDevId,
                                      Switches[NextAvailSwitchID].switchVendId,
                                      usDestId,
        							  hopCount,
        							  (NextAvailSwitchID << 8),
        							  inboundPort ) )
		{
            return 1;
        }

		devLargeBaseID = (NextAvailSwitchID << 8) | 0xFF;
        Switches[NextAvailSwitchID].DeviceID = devLargeBaseID;

        /* Add the inbound port Device ID to the switch routing table
         * for all ports. All upstream ports can now return traffic.
         */
        SrioSetRoute( Switches[NextAvailSwitchID].switchDevId,
                      Switches[NextAvailSwitchID].switchVendId,
                      usDestId,
                      hopCount,
                      0xff,         /* Set all ports */
                      currDeviceID,
                      inboundPort );

		if( hopCount > 0 )
		{
	        Switches[currSwitchID].destId[outboundPort] = devLargeBaseID;
	        Switches[currSwitchID].devId[outboundPort]  = (devId & BSLC667X_RIO_DEV_ID_DEV_ID) >> 16;
			Switches[currSwitchID].vendId[outboundPort] = devId & BSLC667X_RIO_DEV_ID_DEV_VEN_ID;
          	Switches[currSwitchID].RouteTable[devLargeBaseID >> 8]
          											    = outboundPort;

	        /* Add the outbound port Device ID to the previous switch routing
	         * table for all ports. The route node can now send maintenance
	         * packets upstream to the switch device ID.
	         * Required to enable the full routing algorithm later.
	         */
	        SrioSetRoute( Switches[currSwitchID].switchDevId,
                          Switches[currSwitchID].switchVendId,
                          usDestId,
	                      hopCount - 1,
	                      0xff,         /* Set all ports */
	                      devLargeBaseID,
	                      outboundPort );

	        Switches[NextAvailSwitchID].destId[inboundPort]
							= Switches[currSwitchID].DeviceID;
	        Switches[NextAvailSwitchID].devId[inboundPort]
							= Switches[currSwitchID].switchDevId;
	        Switches[NextAvailSwitchID].vendId[inboundPort]
							= Switches[currSwitchID].switchVendId;
          	Switches[NextAvailSwitchID].RouteTable[Switches[currSwitchID].DeviceID >> 8]
							= inboundPort;
		}

        printf("    - Switch maintenace packet routing set back to DSP\n");

        /* Investigate this switch for further devices
         * Check the synchronisation of all available ports
		 * skip our own DSP port
		 */
		currSwitchID = NextAvailSwitchID;

		for( portNum = 0; portNum < portTotal; portNum ++ )
		{
			if( portNum == inboundPort )
			{
				printf("Switch %d Port %d ignored as this is the incoming port\n",
				           currSwitchID,
				           portNum);
				continue;
			}

			/* Get the port status register */
			Err = BSLC667X_SrioMaintRdSync(
							(unsigned int *)BSLC667X_RIO_SP_ERR_STATUS(portNum),
							pGlobalReadAddress,
							usDestId,
							hopCount,
			                BSLC667X_SRIO_PORT_0,
		                    BSLC667X_SRIO_LSU_NUM_0,
							&ucCompletionCode	);

	    	if (Err || ucCompletionCode)
        	    goto maintRdFail;

			/* Ensure that the port has trained */
			if( (*pGlobalReadAddress & BSLC667X_RIO_SP_ERR_STATUS_PORT_OK)
						!= BSLC667X_RIO_SP_ERR_STATUS_PORT_OK )
			{
				printf("Switch %d Port %d Not Trained 0x%x\n",
				           currSwitchID,
				           portNum,
				           *pGlobalReadAddress);
				continue;
			}
			else
			{
				printf("Switch %d Port %d  Trained 0x%x\n",
				           currSwitchID,
				           portNum,
				           *pGlobalReadAddress);
			}

			/* Verify Port status and Make sure there are no pending OUTPUT ERROR-STOPPED and/or INPUT ERROR-STOPPED
			 * error status in SP(n)_ERR_STAT register
			 */
			/* Clear port errors */
			Err = BSLC667X_SrioMaintWrSync(
							pGlobalReadAddress,
							(unsigned int *)BSLC667X_RIO_SP_ERR_STATUS(portNum),
							usDestId,
							hopCount,
			                BSLC667X_SRIO_PORT_0,
		                    BSLC667X_SRIO_LSU_NUM_0,
							&ucCompletionCode	);

	    	if (Err || ucCompletionCode)
        	    goto maintRdFail;

			/* Get the port status register */
			Err = BSLC667X_SrioMaintRdSync(
							(unsigned int *)BSLC667X_RIO_SP_ERR_STATUS(portNum),
							pGlobalReadAddress,
							usDestId,
							hopCount,
			                BSLC667X_SRIO_PORT_0,
		                    BSLC667X_SRIO_LSU_NUM_0,
							&ucCompletionCode	);

	    	if (Err || ucCompletionCode)
        	    goto maintRdFail;

			/* Perform Port Sync operation if INPUT and/or OUTPUT ERROR-STOPPED are set */
			if( *pGlobalReadAddress &
				 (BSLC667X_RIO_SP_ERR_STATUS_OUTPUT_ERR_STOP |
				  BSLC667X_RIO_SP_ERR_STATUS_INPUT_ERR_STOP) )
			{
				printf("Switch %d Port %d Performing PortSync 0x%x\n",
				           currSwitchID,
				           portNum,
				           *pGlobalReadAddress);
				/*TODO ::: port sync between Nyquist & CPS1848 */
				if(Err)
				{
					if(Err == BSLC667X_SRIO_TIMEOUT)
					{
						/* Get the port status register */
						Err = BSLC667X_SrioMaintRdSync(
										(unsigned int *)BSLC667X_RIO_SP_ERR_STATUS(portNum),
										pGlobalReadAddress,
										usDestId,
										hopCount,
						                BSLC667X_SRIO_PORT_0,
					                    BSLC667X_SRIO_LSU_NUM_0,
										&ucCompletionCode	);

				    	if (Err || ucCompletionCode)
			        	    goto maintRdFail;

						printf("Switch %d Port %d Not Trained 0x%x\n",
				           currSwitchID,
				           portNum,
				          *pGlobalReadAddress );
				        continue;

					}
					else
					{
						printf("BSLC667X_SrioTsi578ToEndPointPortSync() Failed!! Err = 0x%x\n",
						            Err );
						fail++;
					}
				}
			}

			/* Get the port status register */
			Err = BSLC667X_SrioMaintRdSync(
							(unsigned int *)BSLC667X_RIO_SP_ERR_STATUS(portNum),
							pGlobalReadAddress,
							usDestId,
							hopCount,
			                BSLC667X_SRIO_PORT_0,
		                    BSLC667X_SRIO_LSU_NUM_0,
							&ucCompletionCode	);

	    	if (Err || ucCompletionCode)
        	    goto maintRdFail;

			printf("Switch %d Port %d Status Reg=0x%x\n",
	           currSwitchID,
	           portNum,
	          *pGlobalReadAddress );

			/* Get the port control register */
			Err = BSLC667X_SrioMaintRdSync(
							(unsigned int *)BSLC667X_RIO_SP_CTL(portNum),
							pGlobalReadAddress,
							usDestId,
							hopCount,
			                BSLC667X_SRIO_PORT_0,
		                    BSLC667X_SRIO_LSU_NUM_0,
							&ucCompletionCode	);

	    	if (Err || ucCompletionCode)
        	    goto maintRdFail;

			portWidth     = (*pGlobalReadAddress & BSLC667X_RIO_SP_CTL_PORT_WIDTH)
											>> BSLC667X_RIO_SP_CTL_PORT_WIDTH_SHIFT;

			portWidthInit = (*pGlobalReadAddress & BSLC667X_RIO_SP_CTL_INIT_PWIDTH)
											>> BSLC667X_RIO_SP_CTL_INIT_PWIDTH_SHIFT;

			if( portWidthInit == 0x1 )
			{
				    printf("Switch %d Port %d port width = 1x - Lane 2,  Port Control = 0x%x\n",
								    currSwitchID, portNum, *pGlobalReadAddress);
			}
			else if( portWidthInit == 0x0 )
			{
				    printf("Switch %d Port %d port width = 1x - Lane 0, Port Control = 0x%x\n",
								    currSwitchID, portNum, *pGlobalReadAddress);
			}
			else if( portWidthInit & 0x2 )
			{
				    printf("Switch %d Port %d port width 4x\n",
								    currSwitchID, portNum);
			}
			else
			{
				printf("Switch %d Port %d port width Invalid, Port Control = 0x%x\n",
								currSwitchID, portNum, *pGlobalReadAddress);
		    }

			/* This port is enabled and active
             * set the route from/to that port so we can access it with a hop count increment
             */
            SrioSetRoute( Switches[currSwitchID].switchDevId,
                          Switches[currSwitchID].switchVendId,
                          usDestId,		/* Use default value */
    					  hopCount,
     					  inboundPort,  /* Use local DSP port LUT  */
            		  	  usDestId,     /* Use default */
    	        		  portNum );    /* Port we are discovering */

    	    /* Recursively investigate the devices from this port */
            SrioDiscoverSwitchRouting( hopCount+1,
                                       currDeviceID,
                                       currSwitchID,
                                       portNum,
                                       sourcePort );
        }
    }
    else
    {
        /* Endpoint */
        devLargeBaseID = (currSwitchID << 8) | outboundPort;

        /* Test device writes */
        if( SrioComponentTagTest( usDestId, hopCount, rioCompTag ) )
            return 1;

        /* Set the endpoint's discovered bit */
        setDiscoveredBit( usDestId,
                          hopCount );

        printf("    - Endpoint found, Switch = %d, hopCount = %d, devId = %x\n",
         			currSwitchID,
         			hopCount,
    				devId );

            /* Set the Large Base Device ID of the DSP Found
    	     * and leave the Small ID in its reset configuration
             */
        *pGlobalWriteAddress = ((unsigned int)devLargeBaseID & BSLC667X_RIO_BASE_ID_LARGE_DEVICE_ID) |
    				       (BSLC667X_SRIO_BASE_ID_BASE_DEVICEID_RESETVAL << BSLC667X_RIO_BASE_ID_DEVICE_ID_SHIFT);

        Err = BSLC667X_SrioMaintWrSync(
    				    pGlobalWriteAddress,
    				    (unsigned int *)BSLC667X_RIO_BASE_ID,
    				    usDestId,
    				    hopCount,
                        BSLC667X_SRIO_PORT_0,
                        BSLC667X_SRIO_LSU_NUM_0,
    				    &ucCompletionCode	);

        /* Ignore any errors since changing the endpoint device ID can make the Ack
         * response invalid
         */

        switch ( devId & CSL_SRIO_RIO_DEV_ID_DEV_VEN_ID_MASK )
        {
        	case BSLC667X_SRIO_DEV_ID_DEVICE_VENDORIDENTITY_TITCI6487:
                printf("    - Endpoint configuring TI DSP, DevLargeBaseID = 0x%x\n",
         		           devLargeBaseID );

		        /* Set the DSP DEVICE_ID_REG1 register to be the same as the
		         * SRIO BASE_DEVICEID
				 * This will only work if the DSP bootmode is to boot from SRIO
				 * as 0x02d12004[bit 28] must == 1 to circumvent the mismatch
				 * of DEVICEID_REG1 and DEVICE_ID
				 * This normally read only bit is asserted by the DSP bootrom.
		         */
			    Err = BSLC667X_SrioDirectIOWrSync (
					    pGlobalWriteAddress,
					    (unsigned int *)&(hSrio->RIO_MULTIID_REG[0]),
					    4,
					    usDestId,
	                    BSLC667X_SRIO_PORT_0,
		                BSLC667X_SRIO_LSU_NUM_0,
					    &ucCompletionCode );

        		break;

        	case FPGA_VENDOR_ID:
			case FPGA_VENDOR_ID_N:
				/* Get the Assembly info to check if SPARTAN or FPGA */
				Err = BSLC667X_SrioMaintRdSync(
								(unsigned int *)BSLC667X_RIO_ASBLY_ID,
								pGlobalReadAddress,
								usDestId,
								hopCount,
								BSLC667X_SRIO_PORT_0,
								BSLC667X_SRIO_LSU_NUM_0,
								&ucCompletionCode	);

				if (Err || ucCompletionCode)
					goto maintRdFail;

				fpgaAssblyID = *pGlobalReadAddress;

                printf("    - Endpoint configuring XILINX FPGA, DevLargeBaseID = 0x%x\n",
        	               devLargeBaseID );
        		break;

			case WINTEGRA_WINPATH_VENDOR_ID:
				printf("sriodiscoverytest: Info Endpoint configuring WINTEGRA WINPATH, DevLargeBaseID = 0x%x\n",
						   devLargeBaseID );
				break;
			case ALTERA_VENDOR_ID:
                printf("    - Endpoint configuring ALTERA FPGA, DevLargeBaseID = 0x%x\n",
        	               devLargeBaseID );
				break;
			case FREESCALE_VENDOR_ID:
                printf("    - Endpoint configuring FREESCALE, DevLargeBaseID = 0x%x\n",
        	               devLargeBaseID );
				break;
			default:
                printf("    - Endpoint device not recognised, DevId = 0x%x\n",
         			        devId );
        }


        /* If this is a TI DSP or Xilinx FPGA register it as a test endpoint */
        if( (( devId & CSL_SRIO_RIO_DEV_ID_DEV_VEN_ID_MASK )
	           	== BSLC667X_SRIO_DEV_ID_DEVICE_VENDORIDENTITY_TITCI6487)    ||
            (( devId & CSL_SRIO_RIO_DEV_ID_DEV_VEN_ID_MASK )
	           	== FPGA_VENDOR_ID)											||
           	(( devId & CSL_SRIO_RIO_DEV_ID_DEV_VEN_ID_MASK )
	           	== WINTEGRA_WINPATH_VENDOR_ID)								||
           	(( devId & CSL_SRIO_RIO_DEV_ID_DEV_VEN_ID_MASK )
	           	== FPGA_VENDOR_ID_N)										||
           	(( devId & CSL_SRIO_RIO_DEV_ID_DEV_VEN_ID_MASK )
	           	== ALTERA_VENDOR_ID)									    ||
           	(( devId & CSL_SRIO_RIO_DEV_ID_DEV_VEN_ID_MASK )
	           	== FREESCALE_VENDOR_ID) )
        {
	        if( NextAvailEndpoint >= SRIO_DISCO_MAX_ENDPOINTS )
	        {
	            printf("    - Endpoint too many endpoints (%d) detected\n",
	             		   NextAvailEndpoint );
	            return 1;
	        }

	        Endpoints[NextAvailEndpoint].devId    = (devId & BSLC667X_RIO_DEV_ID_DEV_ID) >> 16;
			Endpoints[NextAvailEndpoint].vendId   = devId & BSLC667X_RIO_DEV_ID_DEV_VEN_ID;
	        Endpoints[NextAvailEndpoint].switchId = currSwitchID;
	        Endpoints[NextAvailEndpoint].portId   = outboundPort;
	        Endpoints[NextAvailEndpoint].DeviceID = devLargeBaseID;
	        Endpoints[NextAvailEndpoint].hopCount = hopCount;

	        NextAvailEndpoint++;
		}

        /* Set the local switch routing table to route all traffic
         * to this ID to the outbound port.
         * Global routing is done later.
         */
		if( hopCount > 0 )
		{
	        SrioSetRoute( Switches[currSwitchID].switchDevId,
                          Switches[currSwitchID].switchVendId,
                          usDestId,
	                      hopCount-1,
	                      0xff,         /* Set all ports */
	                      devLargeBaseID,
	                      outboundPort );

	        Switches[currSwitchID].destId[outboundPort] = devLargeBaseID;
	        Switches[currSwitchID].devId[outboundPort]  = (devId & BSLC667X_RIO_DEV_ID_DEV_ID) >> 16;
			Switches[currSwitchID].vendId[outboundPort] = devId & BSLC667X_RIO_DEV_ID_DEV_VEN_ID;
		}
    }

    /* Jump back to production test code */
    return fail;

maintRdFail:
	printf("BSLC667X_SrioMaintRdSync() Failed!! ErrorCode = %d CompletionCode = %d\n",
	            Err, ucCompletionCode );
	return ++fail;
}


int SrioDiscoverFullRouting( void )
{
    unsigned char i=0, j, k;

    for (; i<=NextAvailSwitchID; i++)
	{
        for (k=0; k<SRIO_DISCO_MAX_SWITCH_PORTS; k++)
		{
	        if( (Switches[i].destId[k] & 0xff) == 0xff)
	        {
	            /* Switch found
	             * Fill out heirarchical routing details in all other switches
	             */
  	            for( j=0; j<=NextAvailSwitchID; j++)
				{
				    /* Skip yourself */
		            if (j!=i)
		            {
		              	Switches[j].RouteTable[(Switches[i].destId[k] >> 8)]
		              		=  Switches[j].RouteTable[(Switches[i].DeviceID >> 8)];

	              		SrioSetRoute( Switches[i].switchDevId,
                                      Switches[i].switchVendId,
                                      Switches[j].DeviceID,
		              	  			  Switches[j].hopCount,
									  0xFF,	/* All Ports */
		              				  Switches[i].DeviceID,
		              				  Switches[j].RouteTable[(Switches[i].DeviceID >> 8)] );
		            }
				}
	        }
		}
	}

    return 0;
}

/**
 *   @b SrioSetRoute
 *
 *   @b Description
 *   @li This function is used to set the Tsi578 routing table. This is either port specific
 *       or global depending on the srcPort
 *
 *   @b Arguments
     @verbatim
     devId          - switch device ID for switch specific local LUT algorithm
     vendId         - switch vendor ID for switch specific local LUT algorithm
     destId	 		- SRIO destination address of incoming packet.
     hopCount       - Hop Count, if 0 configure local switch, if > 0 configure chained switch
     srcPort	 	- Incoming port. If < 16 ports configured individually else
     				  else all ports are configured the same.
     cfgDestId	 	- SRIO destination address of outgoing packet.
     cfgDstPort	 	- Outgoing port for the given destId.
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @li Failure Count - Number of failed operations.
 *   @li 0             - Success
 *
 *   <b> Pre Condition </b>
 *   @li  None.
 *
 *   <b> Post Condition </b>
 *   @li None
 *
 *   @b Modifies
 *   @li None
 *
 *   @b Example
 *   @verbatim
     int fail;
     ...
     fail = SrioSetRoute( TUNDRA_RIO_DEV_ID_578, TUNDRA_RIO_VEN_ID,
                          0x0108, 0, 8, 0x010A, 10 );
     ...
     @endverbatim
 *
 */
int SrioSetRoute( unsigned short devId,
                  unsigned short vendId,
                  unsigned short destId,
                  unsigned char  hopCount,
                  unsigned char  srcPort,
                  unsigned short cfgDstId,
                  unsigned char  cfgDstPort )
{
    BSLC667X_ErrorCode 	Err;
	BSLC667X_SrioCompCode 	ucCompletionCode;
	unsigned int 		cfgDstRegister  = BSLC667X_RIO_ROUTE_CFG_DESTID;
	unsigned int 		cfgPortRegister = BSLC667X_RIO_ROUTE_CFG_PORT;
	unsigned int        cfgDstIdTmp  =(unsigned int)cfgDstId;
	unsigned int        cfgDstPortTmp=(unsigned int)cfgDstPort;

	/* Use per port registers */
	if( ( devId == TUNDRA_RIO_DEV_ID_578 ) &&
	    ( vendId == TUNDRA_RIO_VEN_ID ))
	{
        if( srcPort < 16 )
        {
	        cfgDstRegister  = BSLC667X_TSI578_SPBC_ROUTE_CFG_DESTID + ( 0x1000 + (srcPort << 8));
	        cfgPortRegister = BSLC667X_TSI578_SPBC_ROUTE_CFG_PORT   + ( 0x1000 + (srcPort << 8));
	    }
	}
	else if( ( vendId == IDT_RIO_VEN_ID ) &&
				( ( devId == IDT_RIO_DEV_ID_CPS10Q ) || ( devId == IDT_RIO_DEV_ID_CPS1848) ) )
	{
    	unsigned int cfgSrcIdTmp;

        if( srcPort < 16 )
        {
			if(devId == IDT_RIO_DEV_ID_CPS10Q)
	        	cfgSrcIdTmp = (srcPort + 1) << BSLC667X_CPS10Q_LOCAL_RTE_CONF_DESTID_SEL_CSR_SHIFT;
			else // if(devId == IDT_RIO_DEV_ID_CPS1848)
	        	cfgSrcIdTmp = (srcPort + 1);
        }
        else
        {
            cfgSrcIdTmp = 0;
        }

        /* Configure incoming port-specific register */
        Err  = BSLC667X_SrioMaintWrSync(
                        (unsigned int *)BSLC667X_C64X_GLOBAL_ADDR(
									        (unsigned int)BSLC667X_GetDspCoreId(),
									        (unsigned int)&cfgSrcIdTmp ),
				        (unsigned int *)BSLC667X_CPS10Q_LOCAL_RTE_CONF_DESTID_SEL_CSR,
				        destId,
				        hopCount,
                        BSLC667X_SRIO_PORT_0,
                        BSLC667X_SRIO_LSU_NUM_0,
				        &ucCompletionCode	);

	    if (Err || ucCompletionCode )
	    {
		    printf("BSLC667X_SrioMaintWrSync() Failed!! ErrorCode = %d Completion Code = %d\n",
                   Err,
                   ucCompletionCode);
		    return 1;
	    }
	}
	else
	{
		printf("SrioSetRoute() switch not dupported vendId 0x%x, devId 0x%x",
               devId, vendId);
		return 1;
	}

	/* Configure incoming port-specific register */
	Err  = BSLC667X_SrioMaintWrSync(
	                (unsigned int *)BSLC667X_C64X_GLOBAL_ADDR(
										(unsigned int)BSLC667X_GetDspCoreId(),
										(unsigned int)&cfgDstIdTmp ),
					(unsigned int *)cfgDstRegister,
					destId,
					hopCount,
	                BSLC667X_SRIO_PORT_0,
                    BSLC667X_SRIO_LSU_NUM_0,
					&ucCompletionCode	);

	Err |= BSLC667X_SrioMaintWrSync(
	                (unsigned int *)BSLC667X_C64X_GLOBAL_ADDR(
										(unsigned int)BSLC667X_GetDspCoreId(),
										(unsigned int)&cfgDstPortTmp ),
					(unsigned int *)cfgPortRegister,
					destId,
					hopCount,
	                BSLC667X_SRIO_PORT_0,
                    BSLC667X_SRIO_LSU_NUM_0,
					&ucCompletionCode	);

	if (Err || ucCompletionCode )
	{
		printf("BSLC667X_SrioMaintWrSync() Failed!! ErrorCode = %d Completion Code = %d\n",
               Err,
               ucCompletionCode);
		return 1;
	}

	return 0;
}

/* Clear Tsi578 switch local and global LUTs
 * Tsi578 must be in heirarchical mode
 */
int SrioClearLutTsi578( unsigned short usDestId,
                        unsigned char  hopCount )
{
    BSLC667X_ErrorCode 	Err;
	BSLC667X_SrioCompCode 	ucCompletionCode;
	unsigned int 		cfgDstRegister, cfgPortRegister;
	unsigned int        clearLut;
	unsigned int        clearPort = 0xFF;

	/* Use broadcast registers */
	cfgDstRegister  = BSLC667X_TSI578_SPBC_ROUTE_CFG_DESTID;
	cfgPortRegister = BSLC667X_TSI578_SPBC_ROUTE_CFG_PORT;

    /* Set global SPBC_ROUTE_BASE = 0 to use ID 0-0xFF to clear LUT
     * All other addresses use global LUT
     */
	clearLut = 0x0000;
	Err = BSLC667X_SrioMaintWrSync(
	                (unsigned int *)BSLC667X_C64X_GLOBAL_ADDR(
										(unsigned int)BSLC667X_GetDspCoreId(),
										(unsigned int)&clearLut ),
					(unsigned int *)BSLC667X_TSI578_SPBC_ROUTE_BASE,
					usDestId,
					hopCount,
	                BSLC667X_SRIO_PORT_0,
                    BSLC667X_SRIO_LSU_NUM_0,
					&ucCompletionCode	);

	if (Err || ucCompletionCode)
	    return 1;

	/* Configure incoming port-specific register */
	while( clearLut < 0x10000 )
	{
    	Err  = BSLC667X_SrioMaintWrSync(
	                    (unsigned int *)BSLC667X_C64X_GLOBAL_ADDR(
										    (unsigned int)BSLC667X_GetDspCoreId(),
										    (unsigned int)&clearLut ),
    					(unsigned int *)cfgDstRegister,
    					usDestId,
    					hopCount,
		                BSLC667X_SRIO_PORT_0,
	                    BSLC667X_SRIO_LSU_NUM_0,
    					&ucCompletionCode	);

    	Err |= BSLC667X_SrioMaintWrSync(
	                    (unsigned int *)BSLC667X_C64X_GLOBAL_ADDR(
										    (unsigned int)BSLC667X_GetDspCoreId(),
										    (unsigned int)&clearPort ),
    					(unsigned int *)cfgPortRegister,
    					usDestId,
    					hopCount,
		                BSLC667X_SRIO_PORT_0,
	                    BSLC667X_SRIO_LSU_NUM_0,
    					&ucCompletionCode	);

    	if (Err || ucCompletionCode)
    	    return 1;

    	if( clearLut > 0xFF )
    	    clearLut += 0x100; /* We are clearing the global LUT */
    	else
    	    clearLut++; /* We are still clearing the local LUT */
    }

	return 0;
}

/* Clear IDT CPS-10Q switch local and global LUTs
 */
int SrioClearLutCps10q( unsigned short usDestId,
                        unsigned char  hopCount )
{
    BSLC667X_ErrorCode 	Err;
	BSLC667X_SrioCompCode 	ucCompletionCode;
	unsigned int        clearLut;
	unsigned int        clearPort = 0xDEDEDEDE; /* 4 ports cleared at a time */
	unsigned int        uiDstId, uiRouteTable;

    /* Set global RIO_DOMAIN = 0
     */
	clearLut = 0x0000;
	Err = BSLC667X_SrioMaintWrSync(
	                (unsigned int *)BSLC667X_C64X_GLOBAL_ADDR(
										(unsigned int)BSLC667X_GetDspCoreId(),
										(unsigned int)&clearLut ),
					(unsigned int *)BSLC667X_CPS10Q_RIO_DOMAIN,
					usDestId,
					hopCount,
	                BSLC667X_SRIO_PORT_0,
                    BSLC667X_SRIO_LSU_NUM_0,
					&ucCompletionCode	);

	if (Err || ucCompletionCode)
	    return 1;

	/* Configure incoming port-specific register */
	for( uiRouteTable = 0; uiRouteTable < 17; uiRouteTable++ )
	{
        /* Configure incoming port-specific register */
        Err  = BSLC667X_SrioMaintWrSync(
                        (unsigned int *)BSLC667X_C64X_GLOBAL_ADDR(
									        (unsigned int)BSLC667X_GetDspCoreId(),
									        (unsigned int)&uiRouteTable ),
				        (unsigned int *)BSLC667X_CPS10Q_LOCAL_RTE_CONF_DESTID_SEL_CSR,
				        usDestId,
				        hopCount,
                        BSLC667X_SRIO_PORT_0,
                        BSLC667X_SRIO_LSU_NUM_0,
				        &ucCompletionCode	);

	    if (Err || ucCompletionCode )
	    {
		    printf("BSLC667X_SrioMaintWrSync() Failed!! ErrorCode = %d Completion Code = %d\n",
                   Err,
                   ucCompletionCode);
		    return 1;
	    }

	    /* Use extended configuration to clear 4 LUT entries at a time */
	    for( uiDstId = 0x80000000; uiDstId <= 0x800000FF; uiDstId+=4 )
	    {
	        /* Configure incoming port-specific register */
	        Err  = BSLC667X_SrioMaintWrSync(
	                        (unsigned int *)BSLC667X_C64X_GLOBAL_ADDR(
										        (unsigned int)BSLC667X_GetDspCoreId(),
										        (unsigned int)&uiDstId ),
					        (unsigned int *)BSLC667X_RIO_ROUTE_CFG_DESTID,
					        usDestId,
					        hopCount,
	                        BSLC667X_SRIO_PORT_0,
                            BSLC667X_SRIO_LSU_NUM_0,
					        &ucCompletionCode	);

	        Err |= BSLC667X_SrioMaintWrSync(
	                        (unsigned int *)BSLC667X_C64X_GLOBAL_ADDR(
										        (unsigned int)BSLC667X_GetDspCoreId(),
										        (unsigned int)&clearPort ),
					        (unsigned int *)BSLC667X_RIO_ROUTE_CFG_PORT,
					        usDestId,
					        hopCount,
	                        BSLC667X_SRIO_PORT_0,
                            BSLC667X_SRIO_LSU_NUM_0,
					        &ucCompletionCode	);

	        if (Err || ucCompletionCode )
	        {
		        printf("BSLC667X_SrioMaintWrSync() Failed!! ErrorCode = %d Completion Code = %d\n",
                       Err,
                       ucCompletionCode);
		        return 1;
	        }
	    }
	}

	return 0;
}

/* Clear switch local and global LUTs
 * Select approriate switch algorithm
 */
int SrioClearLut( unsigned short devId,
                  unsigned short vendId,
                  unsigned short usDestId,
                  unsigned char  hopCount )
{
    int 	            Err;

	/* Use per port registers */
	if( ( devId == TUNDRA_RIO_DEV_ID_578 ) &&
	    ( vendId == TUNDRA_RIO_VEN_ID ))
	{
	    Err = SrioClearLutTsi578( usDestId, hopCount );
	}
	else if( (( devId == IDT_RIO_DEV_ID_CPS10Q ) &&
	         ( vendId == IDT_RIO_VEN_ID )) ||
	          ( devId == IDT_RIO_DEV_ID_CPS1848 ) &&
	         ( vendId == IDT_RIO_VEN_ID ))
	{
	    Err = SrioClearLutCps10q( usDestId, hopCount );
	}
	else
	{
		printf("SrioClearLut() switch not dupported vendId 0x%x, devId 0x%x",
               devId, vendId);
		Err = 1;
	}
	return Err;
}

/* If Code Composer has SRIO memory windows open when SRIO is released
 * you may have to power-cycle the board.
 * You have been warned!
 */
void SrioReset( void )
{
    unsigned char 		i;

    /* Reset PLL1 enable bit in PER_SET_CNTL register to indicate Serdes are setup */
    //hSrio->RIO_PER_SET_CNTL &= ~CSL_SRIO_PER_SET_CNTL_ENPLL1_ENABLE;

	/* Disable Data Flow control */
	hSrio->RIO_PCR &= ~(1 << CSL_SRIO_RIO_PCR_PEREN_SHIFT);
	BSLC667X_MicrosecWait(100000);

	/* Reset all clock domains */
    for (i = 0; i < BSLC667X_SRIO_BLOCKS_MAX; i++)
    {
        CSL_FINS(hSrio->BLOCK_ENABLE_STATUS[i].RIO_BLK_EN, SRIO_RIO_BLK_EN_EN, BSLC667X_SRIO_BLK_EN_EN_DISABLE);
    }

	/* global disable */
	CSL_FINS(hSrio->RIO_GBL_EN, SRIO_RIO_GBL_EN_EN, BSLC667X_SRIO_GBL_EN_EN_DISABLE);

	BSLC667X_MicrosecWait(10);
}

/* Configure the switch to use heirarchical routing.
 * Set the ROUTE_BASE for all ports to the given route base.
 * Cleat the local and global routing tables.
 * Route all unrecognised traffic to the discovery port.
 */
int SrioSwitchRouteConfigure( unsigned short devId,
                              unsigned short vendId,
                              unsigned short usDestId,
                              unsigned char  hopCount,
                              unsigned short routeBase,
                              unsigned short inboundPort )
{
    BSLC667X_ErrorCode Err;
	BSLC667X_SrioCompCode  	 ucCompletionCode;
	unsigned int     maintWriteValue;
    unsigned int    *pGlobalWriteAddress;
	unsigned int 	cfgDomainRegister;

	/* Calculate the global SRAM addresses required for the EDMA transfer */
	pGlobalWriteAddress = (unsigned int *)BSLC667X_C64X_GLOBAL_ADDR(
										(unsigned int)BSLC667X_GetDspCoreId(),
										(unsigned int)&maintWriteValue );

    /* Ensure device is in heirarchical look-up table
     * SPBC_MODE[LUT_512] = 0
     */
	/* Use per port registers */
	if( ( devId == TUNDRA_RIO_DEV_ID_578 ) &&
	    ( vendId == TUNDRA_RIO_VEN_ID ))
	{
        *pGlobalWriteAddress = BSLC667X_TSI578_SPBC_MODE_RESET_VALUE;
	    Err = BSLC667X_SrioMaintWrSync(
					    pGlobalWriteAddress,
					    (unsigned int *)BSLC667X_TSI578_SPBC_MODE,
					    usDestId,
					    hopCount,
	                    BSLC667X_SRIO_PORT_0,
                        BSLC667X_SRIO_LSU_NUM_0,
					    &ucCompletionCode	);

	    if (Err || ucCompletionCode)
	        return 1;

        cfgDomainRegister = BSLC667X_TSI578_SPBC_ROUTE_BASE;
	}
	else if( (( devId == IDT_RIO_DEV_ID_CPS10Q ) &&
	         ( vendId == IDT_RIO_VEN_ID )) ||
	         (( devId == IDT_RIO_DEV_ID_CPS1848 ) &&
	         ( vendId == IDT_RIO_VEN_ID )) )
	{
        cfgDomainRegister = BSLC667X_CPS10Q_RIO_DOMAIN;
	}
	else
	{
		printf("SrioSwitchRouteConfigure() switch not dupported vendId 0x%x, devId 0x%x\n",
               devId, vendId);
		return 1;
	}

	/* Clear local and global LUTs */
	if( SrioClearLut( devId, vendId, usDestId,	hopCount ) )
	    return 1;

    /* Set global SPBC_ROUTE_BASE
     * All other addresses use global LUT
     */
	*pGlobalWriteAddress = (routeBase << 16);

	if(( devId == IDT_RIO_DEV_ID_CPS1848 ) &&
	         ( vendId == IDT_RIO_VEN_ID ))
	{
		*pGlobalWriteAddress = (*pGlobalWriteAddress&0xFF000000)>>24;
	}

	Err = BSLC667X_SrioMaintWrSync(
					pGlobalWriteAddress,
					(unsigned int *)cfgDomainRegister,
					usDestId,
					hopCount,
	                BSLC667X_SRIO_PORT_0,
                    BSLC667X_SRIO_LSU_NUM_0,
					&ucCompletionCode	);

	if (Err || ucCompletionCode)
	    return 1;

    /* Set default egress port to this incoming port
     * This will catch return maintenance traffic and and erroneous packets.
     */
    *pGlobalWriteAddress = inboundPort;
	Err = BSLC667X_SrioMaintWrSync(
					pGlobalWriteAddress,
					(unsigned int *)BSLC667X_RIO_LUT_ATTR,
					usDestId,
					hopCount,
	                BSLC667X_SRIO_PORT_0,
                    BSLC667X_SRIO_LSU_NUM_0,
					&ucCompletionCode );

	if (Err || ucCompletionCode)
	    return 1;

	return 0;
}

void setDiscoveredBit( unsigned short usDestId,
                       int hopCount)
{
	unsigned int portGenCtrlStatus = BSLC667X_RIO_SW_GEN_CTL_DISC;
	BSLC667X_SrioCompCode ucCompletionCode;

	BSLC667X_SrioMaintWrSync( (unsigned int *)BSLC667X_C64X_GLOBAL_ADDR(
										(unsigned int)BSLC667X_GetDspCoreId(),
										(unsigned int)&portGenCtrlStatus ),
							(unsigned int*)BSLC667X_RIO_SW_GEN_CTL,
							usDestId,
							hopCount,
				            BSLC667X_SRIO_PORT_0,
			                BSLC667X_SRIO_LSU_NUM_0,
							&ucCompletionCode );
    return;
}

int isDiscovered( unsigned short usDestId,
                  int hopCount)
{
	unsigned int portGenCtrlStatus;
	BSLC667X_SrioCompCode ucCompletionCode;

    BSLC667X_SrioMaintRdSync(
                            (unsigned int*)BSLC667X_RIO_SW_GEN_CTL,
							(unsigned int *)BSLC667X_C64X_GLOBAL_ADDR(
										(unsigned int)BSLC667X_GetDspCoreId(),
										(unsigned int)&portGenCtrlStatus ),
							usDestId,
							hopCount,
			                BSLC667X_SRIO_PORT_0,
			                BSLC667X_SRIO_LSU_NUM_0,
							&ucCompletionCode);

    return ((portGenCtrlStatus & BSLC667X_RIO_SW_GEN_CTL_DISC) ==
        									BSLC667X_RIO_SW_GEN_CTL_DISC);
}

int SrioComponentTagTest( unsigned short usDestId,
                          int hopCount,
                          unsigned int rioCompTag )
{
    BSLC667X_ErrorCode 	Err;
	BSLC667X_SrioCompCode ucCompletionCode;
	unsigned int maintReadValue, rioCompRestore;

    /* Read the RIO_COMP_TAG register to enable a restore */
    Err = BSLC667X_SrioMaintRdSync(
    				(unsigned int *)BSLC667X_RIO_COMP_TAG,
    				(unsigned int *)BSLC667X_C64X_GLOBAL_ADDR(
										(unsigned int)BSLC667X_GetDspCoreId(),
										(unsigned int)&rioCompRestore ),
    				usDestId,
    				hopCount,
	                BSLC667X_SRIO_PORT_0,
                    BSLC667X_SRIO_LSU_NUM_0,
    				&ucCompletionCode	);

    if (Err || ucCompletionCode)
        return 1;

    /* Write to the RIO_COMP_TAG register to test maintenance write commands */
    Err = BSLC667X_SrioMaintWrSync(
    				(unsigned int *)BSLC667X_C64X_GLOBAL_ADDR(
										(unsigned int)BSLC667X_GetDspCoreId(),
										(unsigned int)&rioCompTag ),
    				(unsigned int *)BSLC667X_RIO_COMP_TAG,
    				usDestId,
    				hopCount,
	                BSLC667X_SRIO_PORT_0,
                    BSLC667X_SRIO_LSU_NUM_0,
    				&ucCompletionCode	);

    if (Err || ucCompletionCode)
        return 1;

    /* Read back the RIO_COMP_TAG register to ensure we read what we wrote */
    Err = BSLC667X_SrioMaintRdSync(
    				(unsigned int *)BSLC667X_RIO_COMP_TAG,
    				(unsigned int *)BSLC667X_C64X_GLOBAL_ADDR(
										(unsigned int)BSLC667X_GetDspCoreId(),
										(unsigned int)&maintReadValue ),
    				usDestId,
    				hopCount,
	                BSLC667X_SRIO_PORT_0,
                    BSLC667X_SRIO_LSU_NUM_0,
    				&ucCompletionCode	);

    if (Err || ucCompletionCode)
        return 1;

    /* Write to the RIO_COMP_TAG register to restore the original value */
    Err = BSLC667X_SrioMaintWrSync(
    				(unsigned int *)BSLC667X_C64X_GLOBAL_ADDR(
										(unsigned int)BSLC667X_GetDspCoreId(),
										(unsigned int)&rioCompRestore ),
    				(unsigned int *)BSLC667X_RIO_COMP_TAG,
    				usDestId,
    				hopCount,
	                BSLC667X_SRIO_PORT_0,
                    BSLC667X_SRIO_LSU_NUM_0,
    				&ucCompletionCode	);

    if (Err || ucCompletionCode)
        return 1;

    if( maintReadValue != rioCompTag )
    {
    	printf("Maintenance write test failed!! RIO_Component Tag, read = 0x%x expected 0x%x\n",
    	            maintReadValue, 0x11223344);
    	return 1;
    }

    return 0;
}
#endif

#endif
