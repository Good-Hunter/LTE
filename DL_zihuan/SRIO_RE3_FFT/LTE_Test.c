
#include <stdio.h>
#include <string.h>
#include <SRIO_drv.h>

#define DstDevId 		0x0						//目标设备ID号
#define HOP_COUNT		1

extern Uint8 g_ucDoorbellIntrFlag;
extern SRIO_Trans_Config transParameter[];
Uint32 uiBuffer = 2;
extern CSL_SrioHandle      hSrio;		//这是个指针，指向的SRIO的寄存器

#pragma CODE_SECTION (LTE_Test, ".text:srio_section:dsp_srio");
void LTE_Test(char doorbellvalid,unsigned char event )
{
	Uint32 						Err;
	SrioCompCode 				ucCompleteCode;
	SRIO_Trans_Config 			transPara;
	SRIO_Maint_Config 			maintPara = {0};

	Uint32           	uiLoop,uiCount;
	Uint32				uiByte = transPara.byte_count;
	Uint32				pGlobalAddress;
	volatile Uint32 	*pLocalData;

	transPara= event==0?transParameter[0]:transParameter[1];

//开启SRIO模块的句柄，需要运行在测试之前，后期可加入到接口初始化模块中
	hSrio = CSL_SRIO_Open (CSL_SRIO);
	/** Validate opened handle */
	if ( (hSrio == NULL) )
	{
		printf("CSL_srioOpen() failed!\n");
		return;
	}
	for( uiCount=0;uiCount<1;++uiCount )
	{
#if 0
	/* Clear destination memory */
	memset((void *)transPara.local_addr, 0, transPara.byte_count);

	/* Write pattern in local memory */
	pLocalData = (Uint32 *)transPara.local_addr;
	for (uiLoop = 0; uiLoop < uiByte/4; uiLoop++)
		*pLocalData++ = 0xA;

	for( uiCount=0;uiCount<2;++uiCount )
	{
		/* Write pattern in local memory */
		pLocalData = (Uint32 *)transPara.local_addr;
		for (uiLoop = 0; uiLoop < uiByte/4; uiLoop++)
			(*pLocalData++) ++;

		Err = SRIO_NWrite (
						&transPara,
						DstDevId,
						&ucCompleteCode );
		if (Err)
		{
			printf("SRIO_NWrite() return failed!!, ErrorCode = 0x%x\n",Err);
			return;
		}
		puts("\nNWrite is over!\n");



		/* Write pattern in local memory */
		pLocalData = (Uint32 *)transPara.local_addr;
		for (uiLoop = 0; uiLoop < uiByte/4; uiLoop++)
			(*pLocalData++) ++;

		Err = SRIO_NRead (
						&transPara,
						DstDevId,
						&ucCompleteCode );
		if (Err)
		{
			printf("SRIO_NRead() return failed!!, ErrorCode = 0x%x\n",Err);
			return;
		}
		puts("\nNRead is over!\n");



		Err = SRIO_Doorbell (
						&transPara,
						DstDevId,
						0x1,
						&ucCompleteCode );
		if (Err)
		{
			printf("SRIO_Doorbell() return failed!!, ErrorCode = 0x%x\n",Err);
			return;
		}
		puts("\nMaster has sent doorbell!\n");


	//测试maintenance
		/* Calculate the global SRAM address required for the EDMA transfer */
		pGlobalAddress = (Uint32)GET_C64X_GLOBAL_ADDR( (Uint32)&uiBuffer );

		/* Read Slave DSP COMP_TAG */
		maintPara.remote_addrL = 0x006C;
		maintPara.local_addr   = (Uint32)pGlobalAddress;
		maintPara.srioPortNum  = SRIO_PORT_0;
		maintPara.srioLsuNum   = SRIO_LSU_NUM_0;

		Err = SrioMaintRdSync (	maintPara,
								DstDevId,
								HOP_COUNT,
								&ucCompleteCode );

		if (Err)
		{
			printf("SrioMaintRdSync() return failed!!, ErrorCode = 0x%x\n",Err);
			return;
		}
		if(ucCompleteCode != 0)
			printf("SrioMaintRdSync() transfer failed, Completion Code:0x%x\n",ucCompleteCode);
		else
			printf("COMP_TAG read successfully!!COMP_TAG: 0x%08x\n",uiBuffer);

		//调用maintenance_write写COMP_TAG
		uiBuffer += 2;
		/* Calculate the global SRAM address required for the EDMA transfer */
		pGlobalAddress = (Uint32)GET_C64X_GLOBAL_ADDR( (Uint32)&uiBuffer );
		/* Change DSP component tag */
		maintPara.remote_addrL = 0x006C;
		maintPara.local_addr   = (Uint32)pGlobalAddress;
		maintPara.srioPortNum  = SRIO_PORT_0;
		maintPara.srioLsuNum   = SRIO_LSU_NUM_0;

		Err = SrioMaintWrSync (	maintPara,
								DstDevId,
								HOP_COUNT,
								&ucCompleteCode );

		if (Err)
		{
			printf("SrioMaintWrSync() return failed!!, ErrorCode = 0x%x\n",Err);
			return;
		}
		if(ucCompleteCode != 0)
			printf("SrioMaintWrSync() transfer failed, Completion Code:0x%x\n",ucCompleteCode);
		else
			printf("COMP_TAG Write successfully!\n");

		Err = SrioMaintRdSync (	maintPara,
								DstDevId,
								HOP_COUNT,
								&ucCompleteCode );

		if (Err)
		{
			printf("SrioMaintRdSync() return failed!!, ErrorCode = 0x%x\n",Err);
			return;
		}
		if(ucCompleteCode != 0)
			printf("SrioMaintRdSync() transfer failed, Completion Code:0x%x\n",ucCompleteCode);
		else
			printf("COMP_TAG read successfully!!COMP_TAG: 0x%08x\n",uiBuffer);



		/* Write pattern in local memory */
		pLocalData = (Uint32 *)transPara.local_addr;
		for (uiLoop = 0; uiLoop < uiByte/4; uiLoop++)
			(*pLocalData++) ++;
#endif
		/* Copy the pattern to slave DSP memory */
		Err = SRIO_SWriteWithDoorbell (
									&transPara,
									DstDevId,
									doorbellvalid,
									0x01,
									&ucCompleteCode );

		if (Err)
		{
			printf("SRIO_SendWithDoorbell() return failed!!, ErrorCode = 0x%x\n",Err);
			return;
		}
		if(ucCompleteCode != 0)
			printf("SRIO_SendWithDoorbell() transfer failed, Completion Code:0x%x\n",ucCompleteCode);
		//puts("SWrite is over!\n");

	}
}
