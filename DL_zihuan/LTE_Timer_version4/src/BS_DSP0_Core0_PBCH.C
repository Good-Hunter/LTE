/*
 * BS_DSP0_CORE0_PBCH.C
 *
 *  Created on: 2015-5-6
 *      Author: JIEXIANG
 */

#include "system_init.h"
#include "pl_comm.h"
#include "TSC.h"

extern unsigned volatile short slot_idx;
extern unsigned volatile short sfn;

void pbch_data_gen( )
{
	signed short m_sfn_temp;

	if((slot_idx == 0) && (0 == (sfn & 3)))
    {
		m_sfn_temp = sfn >> 2;//0~1023,取高8位 sfn本身范围是0~4095
		 /*dl-Bandwidth(3):5代表100prb
		   phich-Durtion(1):0
		   phich-Resource(2):0
		   systemFrmeNumber(8):
		   spare(10)*/
		vxx_pbch_data = ((vxx_cell_para.dl_Bandwidth << 21) & 0x00ffffff) | ((vxx_cell_para.phich_Durtion << 20) & 0x00ffffff)
							 | ((vxx_cell_para.g_phich_Ng << 18) & 0x00ffffff) |((m_sfn_temp << 10) & 0x00ffffff);
		vxx_pbch_data = _bitr(vxx_pbch_data) >> 8;
     }
}

void BS_DSP0_Core0_PBCH(int * precoding_pbch_data_buffer[])
{
	signed short m_pbch_start_index;
	int pbch_cycle,start_pbch = TSCL;

	if((slot_idx == 0) && (0 == (sfn & 3)))
	{
    /******************产生PBCH编码前bit***********************/
	    pbch_data_gen();
	/******************PBCH的BCP配置和接收函数********************/
	    bcp_lte_pbch(&vxx_cell_para,
	                 slot_idx);

	 }
	 if(slot_idx == 0)
	 {
		 m_pbch_start_index = sfn & 3;

	 /******************PBCH也要进行层映射和预编码*****************************/
	     layermapping(
	                  (signed int*)vxx_cell_para.pbch_modulation_data_ptr + m_pbch_start_index * PBCH_SYMBOL_LEN_SLOT0,/*码字0调制后的符号 Q(16,3)*/
	                  (signed int*)vxx_cell_para.pbch_modulation_data_ptr,/*PBCH只有一个码字，所以该参数不会使用*/
	                  vxx_pbch_layermapping_data,/*层映射后的符号 Q(16,3)*/
	                  vxx_cell_para.pbch_m_symbol,/*码字0调制后的符号个数,在cell_para_update中计算*/
	                  &vxx_cell_para.pbch_layer_symbol,/*层映射后的每层符号个数*/
	                  vxx_cell_para.num_antenna_port,/*层数*/
	                  1,/*码字个数*/
	                  vxx_cell_para.pbch_pcfich_pdcch_phich_ueTransMode/*下行传输模式*/
	                 );

	     precoding(
	               precoding_pbch_data_buffer,/*预编码后的符号 Q(16,3)*/
	               &vxx_cell_para.pbch_m_ap_symbol,/*预编码后的符号个数*/
	               vxx_pbch_layermapping_data,/*层映射后的符号 Q(16,3)*/
	               vxx_pbch_layermapping_data + vxx_cell_para.pbch_layer_symbol,
	               vxx_cell_para.pbch_layer_symbol,/*层映射后的每层符号个数,是2的倍数吗？*/
	               vxx_cell_para.num_antenna_port,/*层数*/
	               vxx_cell_para.num_antenna_port,/*天线端口个数*/
	               vxx_cell_para.pbch_pcfich_pdcch_phich_ueTransMode,/*下行传输模式*/
	               0 /*无delya模式*/
	               );
	     pbch_cycle = TSC_delay_cycle(start_pbch);
    }

}







