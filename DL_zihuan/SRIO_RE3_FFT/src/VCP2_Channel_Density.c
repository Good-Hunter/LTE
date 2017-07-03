/*  ===========================================================================
 *   Copyright (c) Texas Instruments Inc 2006
 *
 *   Use of this software is controlled by the terms and conditions found in
 *   the license agreement under which this software has been supplied.
 *   ==========================================================================
 */


/** ===========================================================================
 * @desc VCP2 channel density test.
 *
 * ============================================================================
 */
/* ============================================================================
 *  Revision History
 *  ===============
 *  11-July-2006   Brighton     File Created.
 * ============================================================================
 */



#include <stdio.h>
#include <c6x.h>
#include <ti/csl/csl_tsc.h>
#include <csl_VCP2.h>
#include <csl_VCP2Aux.h>
#include <csl_cache.h>
#include "VCP2_Channel_Density.h"
#include "VCP2_common.h"
#include "KeyStone_common.h"
#include "channelModel.h"
#include "TSC.h"
#define DETAILED_TIME 	0
#define PREFIX_SIZE	(24)	// Set the size of the pre/post fix 24
// Global variable declaration
VCP2Handle  ghVcp2;
VCP2Obj     pVcp2Obj;



#pragma DATA_SECTION(corruptedData, ".VCP2Data")
#pragma DATA_SECTION(branch_metrics, ".VCP2Data")
#pragma DATA_SECTION(packedHD, ".VCP2Data")

#pragma DATA_ALIGN(corruptedData, 128)
#pragma DATA_ALIGN(branch_metrics, 128)
 #pragma DATA_ALIGN(packedHD, 128)

Uint8 corruptedData[(200+MAX_VCP2_TAIL_LEN+8)*4];
Uint8 corruptedtran[(200+MAX_VCP2_TAIL_LEN+8)*4];//cs
Uint8 pSoftInputsWorking[(200+MAX_VCP2_TAIL_LEN+8)*4];//cs
//Branch Metrics
Uint8 branch_metrics[(MAX_VCP2_FRAME_LEN+MAX_VCP2_TAIL_LEN+8)*8];
 //Hard decision of VCP2
Uint8 packedHD[VCP2_CHANNELS_PER_TIME][((MAX_VCP2_FRAME_LEN+7)/8+7)/8*8];
//Output parameter of VCP2
Uint32 VcpOutParms[4];
//#pragma DATA_SECTION(derateData, ".VCP2Data")
//#pragma DATA_ALIGN(derateData, 128)
//extern Int8 DerateData[300];
unsigned int pdcch_data_decode_bits1[(100 + 31)/32];
unsigned int pdcch_data_decode_bits[(100 + 31)/32];
//Uint32 derateData[]=
//			{
//					#include "datafrombankahunhe.dat"
//					//#include "datafrommatlab1015157.dat"
//			};

//Align to 16 bytes for EDMA transfer
#pragma DATA_ALIGN(VcpConfigIc, 16)
VCP2_ConfigIc    VcpConfigIc;

int VCP2_Channel_Density_Test(int len,Int8 * restrict deratingdata,unsigned int *restrict decoded_bits)
//int VCP2_Channel_Density_Test(unsigned int *restrict decoded_bits)
{
	Uint32 uiTotalErrorBits=0;
	VCP2_BaseParams vcpParameters;
	Uint32 i;
	//Uint32 uiFrameLength=len; //cs 89
	Uint32 uiRate= 3;
	Uint32 uiConstLen= 7; 	//constraint length is 9 for 3G  9
	//Uint32 uiSigma=10;//64; 	//Sigma is unisgned in Q7 format
    int    VCP2Status;
    int crcout=0;
    int *crc=&crcout;
    volatile CSL_Uint64 	CycleCounter,initialtime,initialtime1,initialtime2;

	//for(i=0; i<53; i++)
	for(i=0; i<1; i++)
	{
		/*****************************************cs*****************************************/
		CycleCounter=TSCL;
		//edma3Init(i%4,i%4);
		//edma3Init2(0,i%4);
		initialtime1=TSC_delay_cycle(CycleCounter);
		/* Initialize the VCP2 */
		// Get handle to desired VCP2 instance
		//cs 15/04/23 Reserves the specified VCP2 for use. returned by this call is The VCP2 handle.I%4 IS VCP2 instance number,and it is returned by pVcp2Obj->perNum
		/** @brief Peripheral Instance for VCP2
		#define CSL_VCP2_A                            (0)
		#define CSL_VCP2_B                            (1)
		#define CSL_VCP2_C                            (2)
		#define CSL_VCP2_D                            (3) */
		//ghVcp2 = VCP2_init(&pVcp2Obj, i%4, &VCP2Status);
		ghVcp2 = VCP2_init(&pVcp2Obj, 0, &VCP2Status);
		if ( VCP2Status != CSL_SOK)
		{
			printf("Error initilization VCP2!\n");
			return -1;
		}
		/*****************************************cs*****************************************/
		//uiFrameLength=len;//600;// 47;	//Typical3GFrame[2*i];
		uiRate=3;//2;	//Typical3GFrame[2*i+1];
	    vcpParameters.frameLen    	    = len;//47
		vcpParameters.rate        		= uiRate;//3
		vcpParameters.constLen    	    = uiConstLen; //7
		vcpParameters.decision   		= 0;		//Hard decison
		vcpParameters.readFlag    	    = 1; 		//Read output parameters
		vcpParameters.tbConvrgMode      = 1; 	//tail mode
		vcpParameters.tailBitEnable     = 0; 		//Traceback state index disabled
		vcpParameters.traceBackIndex    = 0;
		vcpParameters.yamTh       	    = 16;
		vcpParameters.stateNum   	    = 0; 		//Init state is always 0
		vcpParameters.outOrder          = 1;  //0
		vcpParameters.perf              = VCP2_PERF_MOST_CRITICAL;//VCP2_SPEED_MOST_CRITICAL; //;
		initialtime2=TSC_delay_cycle(CycleCounter);

		uiTotalErrorBits+=TestVCP2(deratingdata,&vcpParameters,crc, ghVcp2, decoded_bits);
		//uiTotalErrorBits+=TestVCP2(&vcpParameters, (Uint8)uiSigma,crc, ghVcp2, i, decoded_bits);
	}
	//crcout=*crc;
	return crcout;
}



int TestVCP2(Int8 * restrict deratingdata,VCP2_BaseParams *VcpParameters, int *crc,VCP2Handle hVcp2,unsigned int *restrict decoded_bits)
//int TestVCP2(VCP2_BaseParams *VcpParameters, Uint8 uiSigma, int *crc,VCP2Handle hVcp2,int count,unsigned int *restrict decoded_bits)
{
	VCP2_Params      VcpConfigParms;		
	Uint32 i, j;
	VCP2_Edma3_Config VCP2_Edma3_Config;
	volatile CSL_Uint64 	CycleCounter,CycleCounter1,addpretime,memcpytime=0,ConfigTime,SubTime;
	Uint32 uiPreprocessingTime=0,bmtime=0,decodingtime=0;
	Uint32 iErrorBits;
	Uint16          	 poly[4] = {0,0,0,0};    

	Uint32 WorkingNumInputBits;
	int crc_out1;
	//Clear content in cache, to make the test more close to real system
	//for repeat test, some data may alrady in cache for second test
	//This is not the truth for most real system
//	CACHE_wbInvAllL1d(CACHE_WAIT);
//	CACHE_wbInvAllL2(CACHE_WAIT);

	//Clear output buffer for hard decision 
	CycleCounter=TSCL;
	//memset(packedHD,0x5a,VCP2_CHANNELS_PER_TIME*((MAX_VCP2_FRAME_LEN+7)/8+7)/8*8);
	//CycleCounter=CSL_tscRead();
	//CycleCounter=TSCL;
	/* Fill out VCP2_ parameters */
	VCP2_genParams_bf(VcpParameters, &VcpConfigParms);  
	/* Calculate the VCP2_ IC values */
	VCP2_genIc(&VcpConfigParms, &VcpConfigIc);

	VCP2_Edma3_Config.inputConfigAddr   = (Uint32)&VcpConfigIc;
	VCP2_Edma3_Config.branchMetricsAddr = (Uint32)branch_metrics;
	VCP2_Edma3_Config.hardDecisionAddr  = (Uint32)packedHD;
	VCP2_Edma3_Config.outParmAddr       = (Uint32)VcpOutParms;
	VCP2_Edma3_Config.outParmFlag       = VcpConfigParms.readFlag;
	VCP2_Edma3_Config.traceBack         = VcpConfigParms.traceBack;
	VCP2_Edma3_Config.bmBuffLen         = 4*(VcpConfigParms.bmBuffLen+1)*(1<<(VcpConfigParms.rate-1));//cs 15/02/02  4*(SYMX+1)*2^(rate-1)
	VCP2_Edma3_Config.numBmFrames       = VcpConfigParms.numBmFrames;
	VCP2_Edma3_Config.decBuffLen        = 8*(VcpConfigParms.decBuffLen+1);//cs 15/02/02  8*(SYMR+1),SYMR指64bit个数，转换为byte为单位
	VCP2_Edma3_Config.numDecFrames      = ((VcpConfigParms.frameLen+7)/8+VCP2_Edma3_Config.decBuffLen-1)/VCP2_Edma3_Config.decBuffLen;//cs 15/02/02 (VcpConfigParms.frameLen+7)/8 i.e. ceil(VcpConfigParms.frameLen/8)单位为byte，后边也是上取整的意思，即求我们的帧长判决结果需要几个decBuff来存储
	ConfigTime=TSC_delay_cycle(CycleCounter);

	//uiPreprocessingTime= CycleCounter;
	//ConfigTime= CycleCounter;
	uiPreprocessingTime+=ConfigTime;
	CycleCounter=TSCL;
	
	/* Program EDMA3.0 to service User Channel */
	vcp2SubmitEdma_multi_Channel(&VCP2_Edma3_Config ,hVcp2);

	//CycleCounter= (CSL_tscRead()-CycleCounter)/VCP2_CHANNELS_PER_TIME;
	//SubTime= (CSL_tscRead()-CycleCounter)/VCP2_CHANNELS_PER_TIME;
	SubTime= TSC_delay_cycle(CycleCounter)/VCP2_CHANNELS_PER_TIME;

	//uiPreprocessingTime+= CycleCounter;
	uiPreprocessingTime+= SubTime;
	if (VcpConfigParms.poly0) poly[0] = VcpConfigParms.poly0|0x100;
	if (VcpConfigParms.poly1) poly[1] = VcpConfigParms.poly1|0x100;
	if (VcpConfigParms.poly2) poly[2] = VcpConfigParms.poly2|0x100;
	if (VcpConfigParms.poly3) poly[3] = VcpConfigParms.poly3|0x100; 

	/*****cs2015/01/20*****/
	CycleCounter=TSCL;
    //memcpy(corruptedData,derateData,144);  //15/05/12 to test the "datafrombankahunhe.dat"
    //memcpy(corruptedData,DerateData,300);    //15/05/12 to test the "datafrombankahunhe.dat"
	//memcpy(corruptedData,derateData,300);//15/06/15
    //little2big(corruptedData,corruptedtran,36);
    //memcpytime=CSL_tscRead()-CycleCounter;
    memcpytime=TSC_delay_cycle(CycleCounter);
    uiPreprocessingTime+= memcpytime;
    CycleCounter=TSCL;
#if 0
    WorkingNumInputBits=addCyclic(	pSoftInputsWorking,
    								corruptedData,//corruptedData,    //corruptedtran
    								47,//uiFrameLength, 	/* length of input block */
    								PREFIX_SIZE, 	/* prefixSize */
    								PREFIX_SIZE, /* prefixSize */
    								VCP2_RATE_1_3); 	/* code rate */
#endif
	//VCP2_ScaleInputData_CalcBM((Int8 *)corruptedData, (Int8 *)branch_metrics, VcpParameters->rate,(VcpParameters->frameLen+VcpParameters->constLen-1));
    addpretime=TSC_delay_cycle(CycleCounter);
    //uiPreprocessingTime+= addpretime;
    // CycleCounter=CSL_tscRead();
    CycleCounter=TSCL;
	//VCP2_ScaleInputData_CalcBM((Int8 *)corruptedData, (Int8 *)branch_metrics, VcpParameters->rate,(VcpParameters->frameLen+ VcpConfigParms.convDist));
	VCP2_ScaleInputData_CalcBM((Int8 *)deratingdata, (Int8 *)branch_metrics, VcpParameters->rate,(VcpParameters->frameLen+ VcpConfigParms.convDist));
	//VCP2_ScaleInputData_CalcBM((Int8 *)derateData, (Int8 *)branch_metrics, VcpParameters->rate,47);
	//	(VcpParameters->frameLen));
	//bmtime=CSL_tscRead()-CycleCounter;
	bmtime=TSC_delay_cycle(CycleCounter);

	uiPreprocessingTime+= bmtime;
	//printf("bmtime:%7ldns  ", bmtime);
	//printf("PrePrecessingTime:%7ldns  ", uiPreprocessingTime);


	//Keep coherency for the case the data are in external memory
//	CACHE_wbInvAllL2(CACHE_WAIT);
	// deratematching_time[ii]=TSC_delay_cycle(deratematching_start);
	//CycleCounter=CSL_tscRead();
	CycleCounter=TSCL;
	//CycleCounter1=TSCL;
	/* Start the VCP to begin the EDMA transfers and decoding data */    
	//VCP2_reset(hVcp2);
	VCP2_start(hVcp2);

	//Wait for VCP2 processing complete
	//Wait the RX EDMA interrupt flag
	while((gEDMACC2Regs->TPCC_IPR&(1<<CSL_EDMA3_CHA_VCP2REVT(hVcp2->perNum)))==0);
	//CycleCounter=CSL_tscRead()-CycleCounter;
#if 0
	CycleCounter=TSC_delay_cycle(CycleCounter);
	decodingtime=CycleCounter/VCP2_CHANNELS_PER_TIME;
	//printf("DecodingTime:%dns  ", CycleCounter/VCP2_CHANNELS_PER_TIME);
	printf("DecodingTime:%8ldns  ", decodingtime);
#endif
	iErrorBits=0;
	switch(VcpParameters->frameLen)
	{
		case 47://01a
					memcpy(&crc_out1,&packedHD[0][6],4);
					*crc=crc_out1<<7;
					*crc&=0xffff0000;
					*crc^=0xffff0000;//译码结果取反才对
					memcpy(decoded_bits,&packedHD[0][3],4);
					decoded_bits[0]&=0xfffffffe;
					decoded_bits[0]^=0xfffffffe;
					break;
		case 67://2a
					memcpy(&crc_out1,&packedHD[0][9],4);
					*crc=crc_out1<<3;
					*crc&=0xffff0000;
					*crc^=0xffff0000;
					memcpy(decoded_bits,&packedHD[0][3],7);
					decoded_bits[0]&=0xffffffff;
					decoded_bits[0]^=0xffffffff;
					decoded_bits[1]&=0xffffe000;
					decoded_bits[1]^=0xffffe000;
					break;
		case 31://1c
					memcpy(&crc_out1,&packedHD[0][4],4);
					*crc=crc_out1<<7;
					*crc&=0xffff0000;
					*crc^=0xffff0000;
					memcpy(decoded_bits,&packedHD[0][3],4);
					decoded_bits[0]&=0xfffe0000;
					decoded_bits[0]^=0xfffe0000;
					break;
		case 45://10m 01a
					memcpy(&crc_out1,&packedHD[0][6],4);
					*crc=crc_out1<<5;
					*crc&=0xffff0000;
					*crc^=0xffff0000;
					memcpy(decoded_bits,&packedHD[0][3],4);
					decoded_bits[0]&=0xfffffff8;
					decoded_bits[0]^=0xfffffff8;
					break;
		case 59://10m 2a
		            memcpy(&crc_out1,&packedHD[0][8],4);
					*crc=crc_out1<<3;
					*crc&=0xffff0000;
					*crc^=0xffff0000;
					memcpy(decoded_bits,&packedHD[0][3],6);
					decoded_bits[0]&=0xffffffff;
					decoded_bits[0]^=0xffffffff;
					decoded_bits[1]&=0xffe00000;
					decoded_bits[1]^=0xffe00000;
					break;
		case 29://10m 1c and 5m 1c
					memcpy(&crc_out1,&packedHD[0][4],4);
					*crc=crc_out1<<5;
					*crc&=0xffff0000;
					*crc^=0xffff0000;
					memcpy(decoded_bits,&packedHD[0][3],4);
					decoded_bits[0]&=0xfff80000;
					decoded_bits[0]^=0xfff80000;
					break;
		case 43://5m 01a
					memcpy(&crc_out1,&packedHD[0][6],4);
					*crc=crc_out1<<3;
					*crc&=0xffff0000;
					*crc^=0xffff0000;
					memcpy(decoded_bits,&packedHD[0][3],4);
					decoded_bits[0]&=0xFFFFFFE0;
					decoded_bits[0]^=0xFFFFFFE0;
					break;
		case 55://5m 2a
		            memcpy(&crc_out1,&packedHD[0][7],4);
					*crc=crc_out1<<7;
					*crc&=0xffff0000;
					*crc^=0xffff0000;
					memcpy(decoded_bits,&packedHD[0][3],5);
					decoded_bits[0]&=0xffffffff;
					decoded_bits[0]^=0xffffffff;
					decoded_bits[1]&=0xFE000000;
					decoded_bits[1]^=0xFE000000;
					break;


	}
    CycleCounter=TSC_delay_cycle(CycleCounter);
	decodingtime=CycleCounter/VCP2_CHANNELS_PER_TIME;
	//Verify hard decision for all user channels
	return 0;
}
/*
Scale input data to following range
Rate     Range
2       [-64;+63]
3       [-42;+42]
4       [-31;+31]
SoftInput must align to 8 byte boundary, 
For speculative optimization, 
BranchEetrics buffer must be big enough length+7 data*/
void little2big(Int8 *input,Int8 *output,Uint32 byteLength)
{
	int i=0;
	for(i=0;i<byteLength;i++)
	{
		output[4*i+0]=input[4*i+3];
		output[4*i+1]=input[4*i+2];
		output[4*i+2]=input[4*i+1];
		output[4*i+3]=input[4*i+0];

	}
}
int	addCyclic(	char *output,
				char *input,
				int length,
				int prefixSize,
				int postfixSize,
				int rate)
{
int i;

	/* Add a cyclic pre fix of size prefixsize */
	for(i=0 ; i<rate*(prefixSize) ;i++)
		output[i] = input[rate*(length)-rate*(prefixSize)+i];

	/* Copy the initial block  */
	for(i=0 ; i<rate*(length) ;i++)
		output[rate*(prefixSize)+i] = input[i];

	/* Add a cyclic post fix of size postfixsize */
	for(i=0 ; i<rate*(postfixSize) ;i++)
		output[rate*(prefixSize)+rate*(length)+i] = input[i];

	/* return the new block size */
	return(prefixSize+postfixSize+length);
}
void	VCP2_ScaleInputData_CalcBM(Int8 * restrict softInput, Int8 * restrict BranchMetrics, Uint32 Rate, Uint32 Length)
{
	Int32 i;
	Uint32 uiMaxLo=0, uiMaxHi=0;
	Uint32 uiAbsDataLo, uiAbsDataHi;
	Uint32 uiRange, uiFixScaleFactor_4, uiScaleFactor;
	Uint32 uiInput3_0, uiInput2, *uipOutData;
	Uint32 uiTemp1_0, uiBM1_0, uiBM3_2, uiBM3_0;
	double dInput,  dTemp3_0, dTemp7_4;
	int dinput;
	double * dpInData= (double *)softInput, *dpOutData;

	if(Rate==3)
		uiRange=42;
	else if(Rate==4)
		uiRange=31;
	else if(Rate==2)
		uiRange=63;
	else
		return;

 	for(i=0; i<Length*Rate/8; i++)
 	{
 		dInput=*dpInData++;//cs 15/02/02 get 64bit(2word/8char) from corrupteddata every time
 		//Get absolute value
 		uiAbsDataLo=  _minu4(_lo(dInput), _sub4(0x00000000,_lo(dInput)));
 		uiAbsDataHi= _minu4(_hi(dInput), _sub4(0x00000000,_hi(dInput)));
 		uiMaxLo=_maxu4(uiMaxLo,uiAbsDataLo);
 		uiMaxHi=_maxu4(uiMaxHi, uiAbsDataHi);//cs 15/01/15 遍历一遍，取出绝对值最大的一组来，分别为uiMaxLo，uiMaxHi
 	}
	uiMaxLo= _maxu4(uiMaxLo, uiMaxHi);
	uiMaxLo= _maxu4(uiMaxLo, _packlh2(uiMaxLo, uiMaxLo));
	uiMaxLo= _maxu4(uiMaxLo, _swap4(uiMaxLo))&0xff;

	//Compute scaling factor
	uiScaleFactor= 128*uiRange/uiMaxLo; //x256 to get Q7 formart
	if(uiScaleFactor>128)
		uiScaleFactor=128;
//	uiScaleFactor=128; 	//bypass sacling for test
//	printf("ScaleFactor= %d/128 ", uiScaleFactor);
	//Pack 4 scale factors into one 32 bit scale factor
	uiFixScaleFactor_4=_packl4(_pack2(uiScaleFactor, uiScaleFactor), 
						_pack2(uiScaleFactor, uiScaleFactor));//cs 0x80808080  0x2A2A2A2A

	if(Rate==3)
	{
		uipOutData= (Uint32 *)BranchMetrics;
		//make length to be multiple of 2, output buffer maybe speculative overwrite for optimization
		Length=(Length+1)/2*2; 	//cs 648   54
		#pragma UNROLL(2)
#if 0
	 	for(i=0; i<Length; i++)
	 	{
			//Read 4 bytes, only use 3 bytes per time, which denote表示，指示 one encoded bit
	 		/*cs 15/04/28 for example uiIput3_0=0x12345678,r0=0x78,r1=0x56,r2=0x34,0x12is going to be discarded*/
	 		uiInput3_0=  _mem4(&softInput[i*3]);//cs 15/01/15  _mem4: load and store 4byte to  uiInput3_0
            //printf("%d",uiInput3_0);
			//Scale
			//8bit x 8bit generat 16bit data, Q0 x Q7 generate Q7
	 		dTemp3_0=_mpysu4(uiInput3_0, uiFixScaleFactor_4);//cs 15/01/15  uiInput3_0的4个8bit signed generat uiFixScaleFactor_4的4个8bit unsigned data,0x2A*0X80=0X1500,qufanjia1=0xEB00;0x2A*0x7F=0x14D6
	 		//shift r2 to Q5 format
	 		//printf("%lf",dTemp3_0);
	 		uiInput2= _shr2(_hi(dTemp3_0),2);//cs 15/01/15 uiInput2=(---,r2) low 16bit=r2
	 		//printf("%d",uiInput2);
			//Calculate branch metrics for fist bit
			//swap (r1,r0) to (r0, r1)
	 		uiTemp1_0=_packlh2(_lo(dTemp3_0), _lo(dTemp3_0));//cs 15/01/15 _lo(dTemp3_0)=(r1,r0),uiTemp1_0=(r0,r1)
	 		//(r0+r1j)*(1+j)=(r0-r1)+(r0+r1)j
	 		/*0x40004000 denotes (1+1j) with Q14 format, so Q7 x Q14= Q21, 
	 		cmpyr only save higher 16 bits, so the result is Q5*/
 			uiTemp1_0=_cmpyr(uiTemp1_0, 0x40004000);//cs 15/01/15 uiTemp1_0=(r0-r1,r0+r1) uiTemp1_0高16bit为r0-r1，低16bit为r0+r1

 			//pack to get (r0+r1, r2)
 			uiBM1_0= _pack2(uiTemp1_0, uiInput2);
 			//((r0+r1)+r2j)*(1+j)=(r0+r1-r2)+(r0+r1+r2)j
	 		/*0x08000800 denotes (1+1j) with Q11 format, so Q5 x Q11= Q16, 
	 		cmpyr only save higher 16 bits, so the result is Q0, just is the result we need*/
 			uiBM1_0=_cmpyr(uiBM1_0, 0x08000800);

 			//pack to get (r0-r1, r2)
 			uiBM3_2= _packhl2(uiTemp1_0, uiInput2);
 			//((r0-r1)+r2j)*(1+j)=(r0-r1-r2)+(r0-r1+r2)j
 			uiBM3_2=_cmpyr(uiBM3_2, 0x08000800);

			//Pack the lower bytes as output
			*uipOutData++=_packl4(uiBM3_2, uiBM1_0);
	 	}
#endif
	 	for(i=0; i<Length; i++)
	 		 	{
	 				//Read 4 bytes, only use 3 bytes per time, which denote表示，指示 one encoded bit
	 		 		/*cs 15/04/28 for example uiIput3_0=0x12345678,r0=0x12,r1=0x34,r2=0x56,0x78is going to be discarded*/
	 		 		uiInput3_0=  _mem4(&softInput[i*3]);//cs 15/01/15  _mem4: load and store 4byte to  uiInput3_0
	 	            //printf("%d",uiInput3_0);
	 				//Scale
	 				//8bit x 8bit generat 16bit data, Q0 x Q7 generate Q7   //cs 2A=42 Q0,means xiaoshuwei 0,zhengshuwei 8(8,0);  80=1000 0000,1means negative, 000 0000 means 0,so 80=-0; 7F=0111 1111 ,0means postive,111 1111 means ~1,
	 		 		dTemp3_0=_mpysu4(uiInput3_0, uiFixScaleFactor_4);//cs 15/01/15  uiInput3_0的4个8bit signed generat uiFixScaleFactor_4的4个8bit unsigned data,0x2A*0X80=0X1500,qufanjia1=0xEB00;0x2A*0x7F=0x14D6
	 		 		//shift r2 to Q5 format
	 		 		//printf("%lf",dTemp3_0);
	 		 		uiInput2= _shr2(_lo(dTemp3_0),2);//cs 15/01/15 uiInput2=(r2,next) high 16bit=r2
	 		 		//printf("%d",uiInput2);
	 				//Calculate branch metrics for fist bit
	 				//(r0, r1)
	 		 		uiTemp1_0=_hi(dTemp3_0);//_packlh2(_hi(dTemp3_0), _hi(dTemp3_0));//cs 15/01/15 _lo(dTemp3_0)=(r1,r0),uiTemp1_0=(r0,r1)
	 		 		//(r0+r1j)*(1+j)=(r0-r1)+(r0+r1)j
	 		 		/*0x40004000 denotes (1+1j) with Q14 format, so Q7 x Q14= Q21,
	 		 		cmpyr only save higher 16 bits, so the result is Q5*/
	 	 			uiTemp1_0=_cmpyr(uiTemp1_0, 0x40004000);//cs 15/01/15 uiTemp1_0=(r0-r1,r0+r1) uiTemp1_0高16bit为r0-r1，低16bit为r0+r1

	 	 			//pack to get (r0+r1, r2)
	 	 			uiBM1_0= _packlh2(uiTemp1_0, uiInput2);
	 	 			//((r0+r1)+r2j)*(1+j)=(r0+r1-r2)+(r0+r1+r2)j
	 		 		/*0x08000800 denotes (1+1j) with Q11 format, so Q5 x Q11= Q16,
	 		 		cmpyr only save higher 16 bits, so the result is Q0, just is the result we need*/
	 	 			uiBM1_0=_cmpyr(uiBM1_0, 0x08000800);

	 	 			//pack to get (r0-r1, r2)
	 	 			uiBM3_2= _packh2(uiTemp1_0, uiInput2);
	 	 			//((r0-r1)+r2j)*(1+j)=(r0-r1-r2)+(r0-r1+r2)j
	 	 			uiBM3_2=_cmpyr(uiBM3_2, 0x08000800);

	 				//Pack the lower bytes as output
	 				*uipOutData++=_packl4(uiBM3_2, uiBM1_0);
	 		 	}
	}
	else if(Rate==4)
	{
		Uint32 uiTemp2_1, uiInput3, uiInput1_0, uiInput1, uiInput0, uiBM7_4;
		dpOutData= (double *)BranchMetrics;
		//make length to be multiple of 2, output buffer maybe speculative overwrite for optimization
		Length=(Length+1)/2*2; 	
		#pragma UNROLL(2)
	 	for(i=0; i<Length; i++)
	 	{
			//Read 4 bytes, which denote one encoded bits
	 		uiInput3_0=  _amem4(&softInput[i*4]);

			//Scale
			//8bit x 8bit generat 16bit data, Q0 x Q7 generate Q7
	 		dTemp3_0=_mpysu4(uiInput3_0, uiFixScaleFactor_4);

			//Calculate branch metrics for fist bit
			//pack to get (r1, r2)
	 		uiTemp2_1=_packhl2(_lo(dTemp3_0), _hi(dTemp3_0));
	 		//(r1+r2j)*(1+j)=(r1-r2)+(r1+r2)j
	 		/*0x40004000 denotes (1+1j) with Q14 format, so Q7 x Q14= Q21, 
	 		cmpyr only save higher 16 bits, so the result is Q5*/
 			uiTemp2_1=_cmpyr(uiTemp2_1, 0x40004000);

	 		//shift r3 to Q5 format
	 		uiInput3= _shr2(_hi(dTemp3_0),2);
 			//pack to get (r1+r2, r3)
 			uiBM1_0= _packlh2(uiTemp2_1, uiInput3);
 			//((r1+r2)+r3j)*(1+j)=(r1+r2-r3)+(r1+r2+r3)j
	 		/*0x08000800 denotes (1+1j) with Q11 format, so Q5 x Q11= Q16, 
	 		cmpyr only save higher 16 bits, so the result is Q0, just is the result we need*/
 			uiBM1_0=_cmpyr(uiBM1_0, 0x08000800);

 			//pack to get (r1-r2, r3)
 			uiBM3_2= _packh2(uiTemp2_1, uiInput3);
 			//((r1-r2)+r3j)*(1+j)=(r1-r2-r3)+(r1-r2+r3)j
 			uiBM3_2=_cmpyr(uiBM3_2, 0x08000800);
			//Pack the lower bytes
			uiBM3_0=_packl4(uiBM3_2, uiBM1_0);

			//shift r1, r0 7 bits to get Q0
			uiInput1_0=_shr2(_lo(dTemp3_0),7);
			uiInput1_0=_packl4(uiInput1_0, uiInput1_0);
			//pack to get packed 4 byte (r0,r0,r0,r0)
			uiInput0=_packl4(uiInput1_0, uiInput1_0);
			uiBM3_0= _add4(uiInput0, uiBM3_0);

			//pack to get packed 4 byte (r1,r1,r1,r1)
			uiInput1=_packh4(uiInput1_0, uiInput1_0);
			uiBM7_4= _sub4(_sub4(uiBM3_0, uiInput1), uiInput1);
			
			*dpOutData++= _itod(uiBM7_4, uiBM3_0);
	 	}
	}
	else 	//Rate=2
	{
		Uint32 uiBM7_6, uiBM5_4;
		dpOutData= (double *)BranchMetrics;
		//make length to be multiple of 8, output buffer maybe speculative overwrite for optimization
		Length=(Length+7)/8*8; 	//cs 15/02/02 the result is the nearest int of multiple of 8   606--->608
		#pragma UNROLL(2)
	 	for(i=0; i<(Length)/4; i++)
	 	{
			//Read 8 bytes, which denote 4 encoded bits
			dInput=  _amemd8(&softInput[i*8]);//cs dInput 64bit

			//Scale
			//8bit x 8bit generat 16bit data, Q0 x Q7 generate Q7
	 		dTemp3_0=_mpysu4(_lo(dInput), uiFixScaleFactor_4);//cs 15/01/15  uiInput3_0的4个8bit signed generat uiFixScaleFactor_4的4个8bit unsigned data,8bit*8bit generate 16bit,four 16bit is 64bit

			//Calculate branch metrics for fist bit
			//swap (r1,r0) to (r0, r1)
	 		uiBM1_0=_packlh2(_lo(dTemp3_0), _lo(dTemp3_0));
	 		//(r0+r1j)*(1+j)=(r0-r1)+(r0+r1)j
	 		/*0x02000200 denotes (1+1j) with Q9 format, so Q7 x Q9= Q16, 
	 		cmpyr only save higher 16 bits, so the result is Q0*/
 			uiBM1_0=_cmpyr(uiBM1_0, 0x02000200);

	 		uiBM3_2=_packlh2(_hi(dTemp3_0), _hi(dTemp3_0));
 			uiBM3_2=_cmpyr(uiBM3_2, 0x02000200);

			//8bit x 8bit generat 16bit data, Q0 x Q7 generate Q7
	 		dTemp7_4=_mpysu4(_hi(dInput), uiFixScaleFactor_4);

	 		uiBM5_4=_packlh2(_lo(dTemp7_4), _lo(dTemp7_4));
 			uiBM5_4=_cmpyr(uiBM5_4, 0x02000200);

	 		uiBM7_6=_packlh2(_hi(dTemp7_4), _hi(dTemp7_4));
 			uiBM7_6=_cmpyr(uiBM7_6, 0x02000200);

			//Pack the lower bytes as output
			*dpOutData++=_itod(_packl4(uiBM7_6, uiBM5_4),_packl4(uiBM3_2, uiBM1_0));
	 	}
	}

	
}

//rewrite CSL function to fix a bug
void VCP2_genParams_bf (
	VCP2_BaseParams * restrict pConfigBase,
	VCP2_Params * restrict pConfigParams
)
{
    VCP2_Rate rate = pConfigBase->rate;//3
    Bool tbConvrgMode = pConfigBase->tbConvrgMode;//0
    Bool tailBitEnable = pConfigBase->tailBitEnable;//0
    Uint8 constLen = pConfigBase->constLen;//7
    Uint8 decision = pConfigBase->decision;//0
    Uint8 readFlag = pConfigBase->readFlag;//1
    Uint8 stateNum = pConfigBase->stateNum;//0
    Uint8 outOrder = pConfigBase->outOrder;//0
    Uint8 perf = pConfigBase->perf;//3
    //Uint16 frameLen = pConfigBase->frameLen;//47
    Uint16 frameLen = pConfigBase->frameLen+PREFIX_SIZE+PREFIX_SIZE;//107
    Uint16 yamTh = pConfigBase->yamTh;//16
    Uint16 traceBackIndex = pConfigBase->traceBackIndex;//0

    Uint8 poly0 = 0;
    Uint8 poly1 = 0;
    Uint8 poly2 = 0;
    Uint8 poly3 = 0;
    Uint16 relLen = 0;
    Uint16 convDist = 0;
    Int16 minSm = 0;
    Int16 maxSm = 0x400; /* Select a maximum value for initial state metric */
    Uint8 bmBuffLen;
    Uint8 decBuffLen;
    Uint16 numBmFrames;
    Uint16 numDecFrames;
    Uint8 traceBack;

    Uint8 i;
    Uint8 convMultiplier;
    Uint16 polynomes[4];
    Uint16 fMax;
    Uint16 rcMax;
    Uint16 multiple;
    Uint16 nsw;
    Uint16 mask;
    Uint16 bmNum;
    Uint32 bmCnt;

    /* VCPIC0 parameters */
    if (rate == VCP2_RATE_1_2) {
        if (constLen == 9) {
            polynomes[0] = VCP2_POLY0_FOR_CONST9_RATE_1_2;
            polynomes[1] = VCP2_POLY1_FOR_CONST9_RATE_1_2;
        }                       /* end if constLen == 9 */
        else if (constLen == 8) {
            polynomes[0] = VCP2_POLY0_FOR_CONST8_RATE_1_2;
            polynomes[1] = VCP2_POLY1_FOR_CONST8_RATE_1_2;
        }                       /* end if constLen == 8 */
        else if (constLen == 7) {
            //polynomes[0] = VCP2_POLY0_FOR_CONST7_RATE_1_2;
            //polynomes[1] = VCP2_POLY1_FOR_CONST7_RATE_1_2;
        	polynomes[0] = 0554;//0133;
        	polynomes[1] = 0744;//0171;
        }                       /* end if constLen == 7 */
        else if (constLen == 6) {
            polynomes[0] = VCP2_POLY0_FOR_CONST6_RATE_1_2;
            polynomes[1] = VCP2_POLY1_FOR_CONST6_RATE_1_2;
        }                       /* end if constLen == 6 */
        else {                  /* (constLen == 5) */
            polynomes[0] = VCP2_POLY0_FOR_CONST5_RATE_1_2;
            polynomes[1] = VCP2_POLY1_FOR_CONST5_RATE_1_2;
        }                       /* end if constLen == 5 */
    }                           /* end if rate 1/2 */
    else if (rate == VCP2_RATE_1_3){                      /* if rate 1/3*/
        if (constLen == 9) {
            polynomes[0] = VCP2_POLY0_FOR_CONST9_RATE_1_3;
            polynomes[1] = VCP2_POLY1_FOR_CONST9_RATE_1_3;
            polynomes[2] = VCP2_POLY2_FOR_CONST9_RATE_1_3;
        }                       /* end if constLen == 9 */
        else if (constLen == 8) {
            polynomes[0] = VCP2_POLY0_FOR_CONST8_RATE_1_3;
            polynomes[1] = VCP2_POLY1_FOR_CONST8_RATE_1_3;
            polynomes[2] = VCP2_POLY2_FOR_CONST8_RATE_1_3;
        }                       /* end if constLen == 8 */
        else if (constLen == 7) {

            //polynomes[0] = VCP2_POLY0_FOR_CONST7_RATE_1_3;//0544   cs2015/3/11 133 left shift 2bit is 0554
           // polynomes[1] = VCP2_POLY1_FOR_CONST7_RATE_1_3;//0624
           // polynomes[2] = VCP2_POLY2_FOR_CONST7_RATE_1_3;//0764
        	polynomes[0] = 0554;//0133;  cs 2015/3/11 133 left shift 2bit is 0554
        	polynomes[1] = 0744;//0171;  cs 2015/3/11 171 left shift 2bit is 0744
        	polynomes[2] = 0724;//0165;  cs 2015/3/11 165 left shift 2bit is 0724
        }                       /* end if constLen == 7 */
        else if (constLen == 6) {
            polynomes[0] = VCP2_POLY0_FOR_CONST6_RATE_1_3;
            polynomes[1] = VCP2_POLY1_FOR_CONST6_RATE_1_3;
            polynomes[2] = VCP2_POLY2_FOR_CONST6_RATE_1_3;
        }                       /* end if constLen == 6 */
        else {                  /* (constLen == 5) */
            polynomes[0] = VCP2_POLY0_FOR_CONST5_RATE_1_3;
            polynomes[1] = VCP2_POLY1_FOR_CONST5_RATE_1_3;
            polynomes[2] = VCP2_POLY2_FOR_CONST5_RATE_1_3;
        }                       /* end if constLen == 5 */
    }                           
    else {                      /* if rate 1/4 */
        if (constLen == 9) {
            polynomes[0] = VCP2_POLY0_FOR_CONST9_RATE_1_4;
            polynomes[1] = VCP2_POLY1_FOR_CONST9_RATE_1_4;
            polynomes[2] = VCP2_POLY2_FOR_CONST9_RATE_1_4;
            polynomes[3] = VCP2_POLY3_FOR_CONST9_RATE_1_4;
        }                       /* end if constLen == 9 */
        else if (constLen == 8) {
            polynomes[0] = VCP2_POLY0_FOR_CONST8_RATE_1_4;
            polynomes[1] = VCP2_POLY1_FOR_CONST8_RATE_1_4;
            polynomes[2] = VCP2_POLY2_FOR_CONST8_RATE_1_4;
            polynomes[3] = VCP2_POLY3_FOR_CONST8_RATE_1_4;
        }                       /* end if constLen == 8 */
        else if (constLen == 7) {
            polynomes[0] = VCP2_POLY0_FOR_CONST7_RATE_1_4;
            polynomes[1] = VCP2_POLY1_FOR_CONST7_RATE_1_4;
            polynomes[2] = VCP2_POLY2_FOR_CONST7_RATE_1_4;
            polynomes[3] = VCP2_POLY3_FOR_CONST7_RATE_1_4;
        }                       /* end if constLen == 7 */
        else if (constLen == 6) {
            polynomes[0] = VCP2_POLY0_FOR_CONST6_RATE_1_4;
            polynomes[1] = VCP2_POLY1_FOR_CONST6_RATE_1_4;
            polynomes[2] = VCP2_POLY2_FOR_CONST6_RATE_1_4;
            polynomes[3] = VCP2_POLY3_FOR_CONST6_RATE_1_4;
        }                       /* end if constLen == 6 */
        else {                  /* (constLen == 5) */
            polynomes[0] = VCP2_POLY0_FOR_CONST5_RATE_1_4;
            polynomes[1] = VCP2_POLY1_FOR_CONST5_RATE_1_4;
            polynomes[2] = VCP2_POLY2_FOR_CONST5_RATE_1_4;
            polynomes[3] = VCP2_POLY3_FOR_CONST5_RATE_1_4;
        }                       /* end if constLen == 5 */
    }                           

    /* zero out any unneeded polynomials, based on the data rate */
    for (i = VCP2_RATE_1_4; i > rate; i--) {
        polynomes[i - 1] = 0;
    }
    /* the polynome sent to the coprocessor has always the most
       significant bit masked */
    mask = 0x00FF;

    /* Polynomes coefficients */
    poly0 = polynomes[0] & mask;//0x64=0544&0377 mask=0377=0xFF 取低8bit
    poly1 = polynomes[1] & mask;//0x94=0624&0377 mask=0377=0xFF
    poly2 = polynomes[2] & mask;//0xF4=0764&0377 mask=0377=0xFF
    poly3 = polynomes[3] & mask;//0x00


    /* Calculation of reliability length */
    /* Identify Fmax for a given constraint length and decision type */
    if (decision == VCP2_DECISION_HARD) {
        if (constLen == 9) {
            fMax = VCP2_MAX_FRAME_SIZE_FOR_HD_CONST9;
            rcMax = VCP2_MAX_RELLEN_CONV_DIST_FOR_HD_CONST9;
            multiple = VCP2_TRL_STAGES_IN_2_PASSES_FOR_HD_CONST9;
        }                       /* end if constLen == 9 */
        if (constLen == 8) {
            fMax = VCP2_MAX_FRAME_SIZE_FOR_HD_CONST8;
            rcMax = VCP2_MAX_RELLEN_CONV_DIST_FOR_HD_CONST8;
            multiple = VCP2_TRL_STAGES_IN_2_PASSES_FOR_HD_CONST8;
        }                       /* end if constLen == 8 */
        if (constLen == 7) {
            fMax = VCP2_MAX_FRAME_SIZE_FOR_HD_CONST7;  //378
            rcMax = VCP2_MAX_RELLEN_CONV_DIST_FOR_HD_CONST7;//378
            multiple = VCP2_TRL_STAGES_IN_2_PASSES_FOR_HD_CONST7;//6
        }                       /* end if constLen == 7 */
        if (constLen == 6) {
            fMax = VCP2_MAX_FRAME_SIZE_FOR_HD_CONST6;
            rcMax = VCP2_MAX_RELLEN_CONV_DIST_FOR_HD_CONST6;
            multiple = VCP2_TRL_STAGES_IN_2_PASSES_FOR_HD_CONST6;
        }                       /* end if constLen == 6 */
        if (constLen == 5) {
            fMax = VCP2_MAX_FRAME_SIZE_FOR_HD_CONST5;
            rcMax = VCP2_MAX_RELLEN_CONV_DIST_FOR_HD_CONST5;
            multiple = VCP2_TRL_STAGES_IN_2_PASSES_FOR_HD_CONST5;
        }                       /* end if constLen == 5 */
    }                           /* end if hard decision */
    else {                      /* if soft decision */
        if (constLen == 9) {
            fMax = VCP2_MAX_FRAME_SIZE_FOR_SD_CONST9;
            rcMax = VCP2_MAX_RELLEN_CONV_DIST_FOR_SD_CONST9;
            multiple = VCP2_TRL_STAGES_IN_2_PASSES_FOR_SD_CONST9;
        }                       /* end if constLen == 9 */
        if (constLen == 8) {
            fMax = VCP2_MAX_FRAME_SIZE_FOR_SD_CONST8;
            rcMax = VCP2_MAX_RELLEN_CONV_DIST_FOR_SD_CONST8;
            multiple = VCP2_TRL_STAGES_IN_2_PASSES_FOR_SD_CONST8;
        }                       /* end if constLen == 8 */
        if (constLen == 7) {
            fMax = VCP2_MAX_FRAME_SIZE_FOR_SD_CONST7;
            rcMax = VCP2_MAX_RELLEN_CONV_DIST_FOR_SD_CONST7;
            multiple = VCP2_TRL_STAGES_IN_2_PASSES_FOR_SD_CONST7;
        }                       /* end if constLen == 7 */
        if (constLen == 6) {
            fMax = VCP2_MAX_FRAME_SIZE_FOR_SD_CONST6;
            rcMax = VCP2_MAX_RELLEN_CONV_DIST_FOR_SD_CONST6;
            multiple = VCP2_TRL_STAGES_IN_2_PASSES_FOR_SD_CONST6;
        }                       /* end if constLen == 6 */
        if (constLen == 5) {
            fMax = VCP2_MAX_FRAME_SIZE_FOR_SD_CONST5;
            rcMax = VCP2_MAX_RELLEN_CONV_DIST_FOR_SD_CONST5;
            multiple = VCP2_TRL_STAGES_IN_2_PASSES_FOR_SD_CONST5;
        }                       /* end if constLen == 5 */
    }                           /* end if soft decision */

    /* If mixed or convergent traceback mode */
    if ((tbConvrgMode == TRUE) || (frameLen > fMax)) {
        if (perf == VCP2_SPEED_MOST_CRITICAL) {  /* Speed most critical */
            convMultiplier = 3;                 /* convMultiplier
			                                       values  - as
			                                       specified in the
			                                       'TPC2_VCP2_API_
			                                       Requirements_v1.4'
			                                       spread sheet */
        }
        else if (perf == VCP2_PERF_CRITICAL) {   /* Performance critical */
            if (constLen == 9) {
                convMultiplier = 6;
            }
            else {
                convMultiplier = 9;
            }

        }
        else if (perf == VCP2_PERF_MOST_CRITICAL) {
	              /* Performance most critical */
            if (constLen == 9) {
                convMultiplier = 6;
            }
            else if (constLen == 8) {
                convMultiplier = 12;
            }
            else {
                convMultiplier = 18;
            }
        }
        else {                  /* for speed critical and default case */

            if (constLen == 9) {
                convMultiplier = 3;
            }
            else {
                convMultiplier = 6;
            }
        }
		/* Calculation of Reliability length and Convergence distance */
        convMultiplier = 5;  //cs tianjia ceshiyong
        convDist = convMultiplier * (constLen - 1);//108
        nsw = VCP2_normalCeil (frameLen, rcMax - convDist);//1 len=600shi nsw=3
        relLen = multiple * VCP2_normalCeil (frameLen, nsw * multiple);//216  8  len=600shi relen=204

        if ((decision == VCP2_DECISION_SOFT) & (constLen < 7)) {
            while (relLen > VCP2_REL_LEN_LIMIT_FOR_A_SILDING_WINDOW) {
                nsw++;
                relLen = multiple * VCP2_normalCeil (frameLen, nsw * multiple);
            }
        }
    }                          /* end if mixed or convergent traceback modes */


    /* Trace back mode */
    if (tbConvrgMode == TRUE) {
        traceBack = VCP2_TRACEBACK_CONVERGENT;
    }
    else {
        if (frameLen > fMax)
            traceBack = VCP2_TRACEBACK_MIXED;//cs when F>fMax,we choose this,for example ,F=640
        else
            traceBack = VCP2_TRACEBACK_TAILED;//cs acctually we choose this ,but when we debug,it seems to choose the convergent
    }


    /* Calculation of branch metric buffer length */
    /* Number of branch metrics per trellis stage */
    if (rate == VCP2_RATE_1_4) {
        bmNum = VCP2_BM_PER_TRL_FOR_RATE_1_4;//8
        bmBuffLen = VCP2_SYMX_FOR_128_BYTE_PER_RXEVENT_RATE_1_4;//3
    }
    else if (rate == VCP2_RATE_1_3) {
        bmNum = VCP2_BM_PER_TRL_FOR_RATE_1_3;//4
        bmBuffLen = VCP2_SYMX_FOR_128_BYTE_PER_RXEVENT_RATE_1_3;//7
    }
    else if (rate == VCP2_RATE_1_2) {
        bmNum = VCP2_BM_PER_TRL_FOR_RATE_1_2;//2  每个符号周期应该计算的BM数量
        bmBuffLen = VCP2_SYMX_FOR_128_BYTE_PER_RXEVENT_RATE_1_2;//15  vcp协议table2-3
    }

    /* Maximum number of branch metrics */
    if (traceBack == VCP2_TRACEBACK_CONVERGENT)
        bmCnt = bmNum * (frameLen + convDist);//4*(47+108)=620
    else
        bmCnt = bmNum * (frameLen + constLen - 1);//212=4*(47+7-1)  cs 15/02/02 所有符号周期总共需要计算的BM数量,it seems that 每个BM占用1个byte来存储
    /* Number of frame transfers */
    numBmFrames = VCP2_normalCeil (bmCnt, VCP2_BYTE_COUNT_PER_RXEVENT);//cs 15/01/21 5  VCP2_BYTE_COUNT_PER_RXEVENT=128，见table2-3


    /* Calculation of Decision buffer length */
    if (decision == VCP2_DECISION_HARD) {
        if (frameLen > VCP2_MAX_HD_IN_OUTFIFO)
            decBuffLen = (VCP2_OUTFIFO_WORDS) - 1;
        else {
			decBuffLen = VCP2_normalCeil (frameLen, VCP2_HD_PER_OUTFIFO_WORD)
			              - 1;
            numDecFrames = 1;  /* Not being used here */
        }
    }
    else {
        if (frameLen > VCP2_MAX_SD_IN_OUTFIFO)
            decBuffLen = (VCP2_OUTFIFO_WORDS) - 1;
        else {
			decBuffLen = VCP2_normalCeil (frameLen, VCP2_SD_PER_OUTFIFO_WORD)
			   - 1;
            numDecFrames = 1;  /* Not being used here */
        }
    }

    pConfigParams->rate = rate;
    pConfigParams->constLen = constLen;
    pConfigParams->poly0 = poly0;
    pConfigParams->poly1 = poly1;
    pConfigParams->poly2 = poly2;
    pConfigParams->poly3 = poly3;
    pConfigParams->yamTh = yamTh;
    pConfigParams->frameLen = frameLen;
    pConfigParams->relLen = relLen;//48   rate=2，frame=600，relen=204
    pConfigParams->convDist = convDist;//108  rate=2，frame=600，convDist=108
    pConfigParams->traceBackEn = tailBitEnable;
    pConfigParams->traceBackIndex = traceBackIndex;
    pConfigParams->outOrder = outOrder;
    pConfigParams->maxSm = maxSm;
    pConfigParams->minSm = minSm;
    pConfigParams->stateNum = stateNum;
    pConfigParams->bmBuffLen = bmBuffLen;
    pConfigParams->decBuffLen = decBuffLen;
    pConfigParams->traceBack = traceBack;
    pConfigParams->readFlag = readFlag;
    pConfigParams->decision = decision;
    pConfigParams->numBmFrames = numBmFrames;//cs 15/01/21 2
    pConfigParams->numDecFrames = numDecFrames;//cs 15/01/21 1

}

