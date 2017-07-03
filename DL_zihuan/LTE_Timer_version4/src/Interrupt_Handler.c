/*
 * Interrupt_Handler.c
 *
 *  Created on: 2014-7-21
 *      Author: castMemory
 */
#include <stdio.h>
#include <ti\csl\csl_chipAux.h>
#include <ti\csl\csl_srioAux.h>
#include <ti\csl\csl_srioAuxPhyLayer.h>

#include <Interrupt_Config.h>
#include "system_init.h"
#include <SRIO_drv.h>
#include "TSC.h"

//�ⲿģ���ȫ�־����
extern CSL_SrioHandle 	 hSrio;
extern volatile int count_num;
extern volatile int interrupt_flag;

//int slot=0,symbol_seq=0,port=0,send_len=-1;

//volatile int process_num2=0;//��ʾ���ڷ��͵���֡���

extern CSL_TmrHandle restrict hTmr;
extern CSL_TmrRegsOvly temp_reg;

extern volatile int timer_flag;

#pragma DATA_SECTION (g_SRIO_LSU_AppCallback, ".bss:srio_section:dsp_srio");
AppCallback_Func g_SRIO_LSU_AppCallback;

#pragma DATA_SECTION (g_SRIO_DoorBell_AppCallback, ".bss:srio_section:dsp_srio");
AppCallback_Func g_SRIO_DoorBell_AppCallback;

#pragma DATA_SECTION (g_Timer_AppCallback, ".bss:srio_section:dsp_srio");
AppCallback_Func g_Timer_AppCallback;

#pragma CODE_SECTION (SRIO_Lsu_InterruptHandler, ".text:srio_section:dsp_srio");
#pragma INTERRUPT (SRIO_Lsu_InterruptHandler);
void SRIO_Lsu_InterruptHandler( void )
{
	Uint32 Read_LSU_ICSR;
    /* Disable Interrupts and Save Previous Interrupt Enable State */
    asm(" dint");

    /* Clear SRIO event */
    CSL_intcEventClear(CSL_GEM_INTC0_OUT_8_PLUS_16_MUL_N);			//�����ǰ�¼�Դ�ı�־λ

	/*Clear system event*/
	CSL_CPINTC_clearSysInterrupt( cpIntc, CSL_INTC0_INTDST0); 		//���ͨ���ж�0���¼���־λ

    /* Read LSU Interrupt Condition Status Register */
	Read_LSU_ICSR = hSrio->LSU_ICSR_ICCR[0].RIO_LSU_ICSR;

    /* Clear LSU Interrupt Condition Register */
    hSrio->LSU_ICSR_ICCR[0].RIO_LSU_ICCR = Read_LSU_ICSR;

//    /* Clear INTDST 0 Interrupt Rate Control Register */
//	switch((BSLC667X_DspCoreId_e)BSLC667X_GetDspCoreId())
//	{
//		case BSLC667X_DSP_CORE_ID_0:
//			hSrio->RIO_INTDST_RATE_CNT[0] = CSL_SRIO_RIO_INTDST_RATE_CNT_RESETVAL;
//			break;
//		case BSLC667X_DSP_CORE_ID_1:
//			hSrio->RIO_INTDST_RATE_CNT[2] = CSL_SRIO_RIO_INTDST_RATE_CNT_RESETVAL;
//			break;
//		case BSLC667X_DSP_CORE_ID_2:
//			hSrio->RIO_INTDST_RATE_CNT[4] = CSL_SRIO_RIO_INTDST_RATE_CNT_RESETVAL;
//			break;
//		default:
//			break;
//	}

    printf("IN LSU interrupt! CoreID %d .\n",CSL_chipReadReg (CSL_CHIP_DNUM));

	if( g_SRIO_LSU_AppCallback.pCallbackFunc != NULL)
	{
		g_SRIO_LSU_AppCallback.pCallbackFunc(g_SRIO_LSU_AppCallback.pArg);		//�����û��Լ��ĺ���
	}

    /* Restore Previous Interrupt Enable State */
    asm(" rint");

}

#pragma CODE_SECTION (SRIO_DoorBell_InterruptHandler, ".text:srio_section:dsp_srio");
#pragma INTERRUPT (SRIO_DoorBell_InterruptHandler);
void SRIO_DoorBell_InterruptHandler( void )
{
	Uint32 Read_doorbell_ICSR;

	Uint32 CoreID = CSL_chipReadReg (CSL_CHIP_DNUM);
    /* Disable Interrupts and Save Previous Interrupt Enable State */
    asm(" dint");

    /* Clear SRIO event */
    CSL_intcEventClear(CSL_GEM_INTC0_OUT_8_PLUS_16_MUL_N);			//�����ǰ�¼�Դ�ı�־λ

	/*Clear system event*/
	CSL_CPINTC_clearSysInterrupt( cpIntc, CSL_INTC0_INTDST0); 		//���ͨ���ж�0���¼���־λ

    /* Read LSU Interrupt Condition Status Register */
	Read_doorbell_ICSR = hSrio->DOORBELL_ICSR_ICCR[CoreID].RIO_DOORBELL_ICSR;

    /* Clear LSU Interrupt Condition Register */
	hSrio->DOORBELL_ICSR_ICCR[CoreID].RIO_DOORBELL_ICCR = Read_doorbell_ICSR;

    printf("IN Doorbell interrupt! CoreID %d .\n",CoreID);

	if( g_SRIO_DoorBell_AppCallback.pCallbackFunc != NULL)
	{
		g_SRIO_DoorBell_AppCallback.pCallbackFunc(g_SRIO_DoorBell_AppCallback.pArg);		//�����û��Լ��ĺ���
	}

    /* Restore Previous Interrupt Enable State */
    asm(" rint");
}

#pragma CODE_SECTION (/*Timer_InterruptHandler*/Timer_ISR, ".text:srio_section:dsp_srio");
#pragma INTERRUPT (Timer_ISR)//Timer_InterruptHandler);
void Timer_ISR()//Timer_InterruptHandler( void )
{

	CSL_intcEventClear(CSL_GEM_TINT4L);			//�����ǰ�¼�Դ�ı�־λ
	interrupt_flag = 1;

#if 0
	if((count_num>=24&&count_num<=41)||(count_num>=94&&count_num<=111))//������֡��������֡
		return;

	//�ж���ǰ��Ҫ���͵�ʱ϶�ͷ������
	slot = ((count_num/14)*2)%20;
	symbol_seq = count_num%14;
	/*����SRIO�ķ���*/
	if(symbol_seq==0)
	{
		process_num2 = process_num2+1;
		transParameter[0].local_addr = (Uint32)total_output_data[slot>>1];//Դ��ַ��ʼ��Ϊ��������
		transParameter[0].byte_count = (2048+160)*4;
		send_len++;
	}
	if(send_len%BUF_LEN==0&&symbol_seq==0)
	{
		transParameter[0].remote_addrL =  0x89ac4800;//ÿ����BUF_LEN����֡,���ն˴�ͷ��ʼ��ȡ���ݽ��д���(FIFO)
	}

	port=0;
	LTE_Test(slot,port);
	cfg_change(symbol_seq,port,slot);

	port=1;
	LTE_Test(slot,port);
	cfg_change(symbol_seq,port,slot);
#endif
}

void Timer_InterruptHandler( void )
{

}

