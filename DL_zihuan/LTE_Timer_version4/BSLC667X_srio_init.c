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
 * @file   BSLC667X_SRIO_lsu_slave_async_ex.c
 * @brief  BSLC667X_ Specific SRIO example source file
 * @dir    \dsplibs\example\dsp\srio\BSLC667X_SRIO_lsu_async_ex
 * @author Piyush Prince
 * 
 */
 /* =============================================================================
  * History:
  * -------- 
  * Version 0.1.0
  *   Revision: 0.0.1 [Id:LMS] [Date: 11/02/2011] Initial Release
  *   Initial revision		   
  * =============================================================================
  */
/*  Include Files
 */
#include <stdio.h>
#include <string.h>
#include <dsp\srio\BSLC667X_srio.h>
#include <tsi578\BSLC667X_tsi578.h>
#include <control\BSLC667X_control.h>

/*  Global Definitions
 */
/* Timeout cycles 0f 5.4 seconds */
#define SRIO_TIMEOUT		10        

/* SRIO Doorbell Interrupt flag */
extern unsigned int g_ucDoorbellIntrFlag;

/* SRIO Doorbell Complete interrupt callback function */								
void DoorbellCallbackFunc(void *pArg, Uint32 *pDoorbellIntrStat);

Uint32 dspPort[2]  = { 9,    /* DSP1 Port 0 CPS1848 Port 9 */
                             1 };  /* DSP2 Port 0 CPS1848 Port 1 */
int temp_var = 0;
int temp_var1 = 0;
extern int   dbinfo, frame0,frame1,frame2,frame3,frame4,frame5,frame6,frame7,frame8,frame9,dberror,dbtime,doorbell[100],tmp,timecount[2][100];
extern far BSLC667X_BoardInfo_s  g_BoardInfo;        
Uint32 icsrBits;

CSL_Status status;
CSL_SrioHandle hSrio;

/** 
 *   @b main
 *
 *   @b Description
 *   @li This example demostrate the usage of SRIO specific APIs.
 *       1. Perform board specific initialisation
 *       2. Initialise SRIO in 5Gbps - 4x mode in interrupt mode
 *       3. Initialise buffer
 *       4. Wait for doorbell interrupt
 *       Run this example before the discovery algorithm to ensure the DSP SRIO interface 
 *       is enabled and can be discovered. 
 */
void SrioDevice_init (void)
{
#if 0
    /* Default configuration will only be used if FRU read fails from DSP */
	BSLC667X_Libconfig_s LibConfig = {BSLC667X_CPU_CLK_FREQ_AUTO,BSLC667X_BOARD_CFG_DEFAULT_AMC2C6670,TRUE,TRUE,TRUE};    
	
	BSLC667X_ErrorCode Err;
	BSLC667X_SrioUserConfObj_s SrioUserConfObj;
#if 1
    BSLC667X_AppCallbackFunc_s CallBackFunInfo;
	BSLC667X_SrioIntcConfObj_s SrioIntcConfObj;
#endif
	Int32 			iTimeout;
	Uint32	 uiPortControl;
	Int32 			iPortWidth;
	Uint32    uiPort = 0;

	printf("\n\nBSLC667X_srio_lsu_slave_async_ex()\n");

	Err = BSLC667X_Initialise(&LibConfig);
	if (Err)
	{	 
		if (Err != BSLC667X_INIT_BOARD_CFG_MAGICNUM_MISSING)
		{
			printf("BSLC667X_Initialise() failed!!, ErrorCode = 0x%x\n",Err);
			return;
		}
	}

	/* for Asynchronous transfer SRIO needs to be interrupt driven */
#if 0
	SrioUserConfObj.SrioModeIntrPollDriven = BSLC667X_SRIO_INTERRUPT_MODE;

	SrioUserConfObj.ucHostEn = 0;
	
	SrioUserConfObj.DoorbellCallbackFuncInfo.pArg = NULL;
	SrioUserConfObj.DoorbellCallbackFuncInfo.pCallbackFunc = (pBSLC667X_AppCallBackFunction)&DoorbellCallbackFunc;
	
	Err = BSLC667X_SrioInitialise(&SrioUserConfObj);
	if (Err)
	{
		printf("BSLC667X_SrioInitialise() failed!!, ErrorCode = 0x%x\n",Err);
		return;
	}
#endif
#if 1
    SrioUserConfObj.SrioModeIntrPollDriven = BSLC667X_SRIO_INTERRUPT_MODE;
	SrioUserConfObj.ucHostEn = 0;
	SrioUserConfObj.DoorbellCallbackFuncInfo.pArg = NULL;
	SrioUserConfObj.DoorbellCallbackFuncInfo.pCallbackFunc = DoorbellCallbackFunc;

	SrioIntcConfObj.DirectIOCallbackFuncInfo.pArg = NULL;
	SrioIntcConfObj.DirectIOCallbackFuncInfo.pCallbackFunc= NULL;
	SrioIntcConfObj.DoorbellCallbackFuncInfo.pArg = NULL;
	SrioIntcConfObj.DoorbellCallbackFuncInfo.pCallbackFunc= DoorbellCallbackFunc;

	Err = BSLC667X_SrioIntcInitialise(&SrioIntcConfObj);
	if (Err)
	{
		printf("BSLC667X_SrioInitialise() failed!!, ErrorCode = 0x%x\n",Err);
		return;
	}
#endif
#endif

    /* Default configuration will only be used if FRU read fails from DSP */
	BSLC667X_Libconfig_s LibConfig = {BSLC667X_CPU_CLK_FREQ_AUTO,BSLC667X_BOARD_CFG_DEFAULT_AMC2C6670,TRUE,TRUE,TRUE};

	BSLC667X_ErrorCode Err;
	//BSLC667X_SrioUserConfObj_s SrioUserConfObj;
	BSLC667X_SrioIntcConfObj_s SrioIntcConfObj;
	BSLC667X_SrioUserConfObj_s SrioUserConfObj;
	Int32 			iTimeout;
	Uint32	uiPortControl;
	Int32 			iPortWidth;
	Uint32    uiPort = 0;

	printf("\n\nBSLC667X_srio_lsu_slave_async_ex()\n");

	Err = BSLC667X_Initialise(&LibConfig);
	if (Err)
	{
		if (Err != BSLC667X_INIT_BOARD_CFG_MAGICNUM_MISSING)
		{
			printf("BSLC667X_Initialise() failed!!, ErrorCode = 0x%x\n",Err);
			//return;
		}
	}

	/* for Asynchronous transfer SRIO needs to be interrupt driven */
	SrioUserConfObj.SrioModeIntrPollDriven = BSLC667X_SRIO_INTERRUPT_MODE;
//	SrioIntcConfObj.SrioModeIntrPollDriven = BSLC667X_SRIO_INTERRUPT_MODE;
#if 0
	SrioUserConfObj.ucHostEn = 0;

	SrioUserConfObj.DoorbellCallbackFuncInfo.pArg = NULL;
	SrioUserConfObj.DoorbellCallbackFuncInfo.pCallbackFunc = (pBSLC667X_AppCallBackFunction)&DoorbellCallbackFunc;
#endif
#if 1
	SrioUserConfObj.ucHostEn = 0;
	SrioUserConfObj.DoorbellCallbackFuncInfo.pArg = NULL;
	SrioUserConfObj.DoorbellCallbackFuncInfo.pCallbackFunc = DoorbellCallbackFunc;

	SrioIntcConfObj.DirectIOCallbackFuncInfo.pArg = NULL;
	SrioIntcConfObj.DirectIOCallbackFuncInfo.pCallbackFunc= NULL;
	SrioIntcConfObj.DoorbellCallbackFuncInfo.pArg = NULL;
	SrioIntcConfObj.DoorbellCallbackFuncInfo.pCallbackFunc= DoorbellCallbackFunc;
#endif
#if 0
	Err = BSLC667X_SrioInitialise(&SrioUserConfObj);
	if (Err)
	{
		printf("BSLC667X_SrioInitialise() failed!!, ErrorCode = 0x%x\n",Err);
		return;
	}
#endif
	Err = BSLC667X_SrioIntcInitialise(&SrioIntcConfObj);
	if (Err)
	{
		printf("BSLC667X_SrioInitialise() failed!!, ErrorCode = 0x%x\n",Err);
		return;
	}
	printf("\n hello world...\n");                           //jx 后面注掉不影响？
#if 0
	hSrio = CSL_SRIO_Open (CSL_SRIO);
	/** Validate opened handle */
	if (hSrio == NULL) 
	{
		return;
	}
	
	for (uiPort=0; uiPort<1; uiPort++)
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
		uiPortControl = hSrio->RIO_SP[uiPort].RIO_SP_CTL;

		/* Validate the Port Mode */
		iPortWidth = (uiPortControl & CSL_SRIO_RIO_SP_CTL_INIT_PWIDTH_MASK)
										>> CSL_SRIO_RIO_SP_CTL_INIT_PWIDTH_SHIFT;

		if( iPortWidth & 0x2 )
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
			BSLC667X_MicrosecWait(10000000);
			
			/*Magic numbers to clear Port Stopped Error bits*/
			hSrio->RIO_PLM[uiPort].RIO_PLM_SP_LONG_CS_TX2 = 0x47FF0000 ;
			hSrio->RIO_PLM[uiPort].RIO_PLM_SP_LONG_CS_TX1 = 0x2001F044;

			/*Clear Error bits*/
			hSrio->RIO_SP[uiPort].RIO_SP_ERR_STAT = hSrio->RIO_SP[uiPort].RIO_SP_ERR_STAT;

			/* Read the Port Control Register for the negotiated port speed */
			uiPortControl = hSrio->RIO_SP[uiPort].RIO_SP_CTL;
			
			/* Validate the Port Mode */
			iPortWidth = (uiPortControl & CSL_SRIO_RIO_SP_CTL_INIT_PWIDTH_MASK)
											>> CSL_SRIO_RIO_SP_CTL_INIT_PWIDTH_SHIFT;
			
			if( iPortWidth & 0x2 )
			{
				printf( "After REINIT:: Using 4x port width\n");
			}
		}

		/*Error Recovery*/	
		if(hSrio->RIO_SP[uiPort].RIO_SP_ERR_STAT & (CSL_SRIO_RIO_SP_ERR_STAT_OUTPUT_ERR_STOPPED_MASK |
														CSL_SRIO_RIO_SP_ERR_STAT_INPUT_ERR_STOPPED_MASK) )
		{

			Uint16 usDspId;
			BSLC667X_Cps1848PortNum_e cps1848PortNumber;

			printf( "srioslave: DSP %d Port Stopped Error, Prefrom recovery.  Error Status = 0x%lx\n", 
					   uiPort,
					   hSrio->RIO_SP[uiPort].RIO_SP_ERR_STAT );

			BSLC667X_ControlGetDspId(BSLC667X_CONTROL_NUM_0, &usDspId);
		
			if(usDspId == BSLC667X_DSP_NODE_ID_1)
			{
				cps1848PortNumber = BSLC667X_CPS1848_PORT_DSP1_4X;
			}
			else if(usDspId == BSLC667X_DSP_NODE_ID_2)
			{
				cps1848PortNumber = BSLC667X_CPS1848_PORT_DSP2_4X;
			}

			Err =  BSLC667X_SrioDspToCps1848PortSync(cps1848PortNumber);

			if(Err)
			{
				printf("srioslave: Error Recovery Failure \n");
			}
			else
			{
				hSrio->RIO_SP[uiPort].RIO_SP_ERR_STAT = hSrio->RIO_SP[uiPort].RIO_SP_ERR_STAT;
				
				printf( "srioslave: DSP %d : After recovery.  Error Status = 0x%lx\n", 
						   uiPort,
						   hSrio->RIO_SP[uiPort].RIO_SP_ERR_STAT );
			}
		}
	}
	
	
	printf("\nWaiting for Master's doorbell...\n");
#endif
/*
	g_ucDoorbellIntrFlag = 0;
	while( 4!=g_ucDoorbellIntrFlag );
	*/
	while(0)
	{
		while( g_ucDoorbellIntrFlag )
		{
			++temp_var;
			//printf("master's doorbell received\n");
			g_ucDoorbellIntrFlag = 0;
			CycleDelay(5e9);
			//printf("master's doorbell received\n");
		}
	}
}

/* SRIO Doorbell Complete interrupt callback function */								
void DoorbellCallbackFunc(void *pArg, Uint32 *pDoorbellIntrStat)
{
	//printf("master's doorbell received\n");

	//++g_ucDoorbellIntrFlag;
#if 0
	g_ucDoorbellIntrFlag=1;
	++temp_var1;
#endif
	g_ucDoorbellIntrFlag=1;

		timecount[1][tmp] = TSCL - dbtime;
		dbtime = TSCL;

		//Uint32 icsrBits;

		icsrBits = *pDoorbellIntrStat;

		if((icsrBits & 0x0001) == 1)
		{
			dbinfo = 0;
			frame0++;
		}
		else if((icsrBits & 0x0002) == 2)
		{
			dbinfo =1;
			frame1++;
		}
		else if((icsrBits & 0x0004) == 4)
		{
			dbinfo = 2;
			frame2++;
		}
		else if((icsrBits & 0x0008) == 8)
		{
			dbinfo = 3;
			frame3++;
		}
		else if((icsrBits & 0x0010) == 16)
		{
			dbinfo = 4;
			frame4++;
		}
		else if((icsrBits & 0x0020) == 32)
		{
			dbinfo =5;
			frame5++;
		}
		else if((icsrBits & 0x0040) == 64)
		{
			dbinfo = 6;
			frame6++;
		}
		else if((icsrBits & 0x0080) == 128)
		{
			dbinfo = 7;
			frame7++;
		}
		else if((icsrBits & 0x0100) == 256)
		{
			dbinfo = 8;
			frame8++;
		}
		else if((icsrBits & 0x0200) == 512)
		{
			dbinfo = 9;
			frame9++;
		}
		else
		{
			dberror++;
			printf("wrong doorbell status reg: %x\n",icsrBits);
		}

		doorbell[tmp] = dbinfo;
		tmp = (tmp+1)%100;


}
