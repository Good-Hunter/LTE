/*
 * BS_DSP0_CORE0_PDSCH.C
 *
 *  Created on: 2015-5-6
 *      Author: JIEXIANG
 */

#include "system_init.h"
#include "pl_comm.h"

extern unsigned volatile short slot_idx;

void BS_DSP0_Core0_PDSCH(unsigned int user_index,
		                 int * precoding_data_buffer[])
{
	unsigned int flag;

    (void)user_para_update(&vxx_user_para[user_index],slot_idx,
						   &vxx_cell_para,
						   &vxx_config_para_init[user_index]);/*用户参数以每个子帧为单位重新读取,仅支持一个用户*/
	flag = 0;

	flag = bcp_lte_pdsch(&vxx_user_para[user_index],
					     &vxx_cell_para,
						 slot_idx);

	CSL_XMC_invalidatePrefetchBuffer();
  		  		             _mfence();
  		  		             _mfence();
	CACHE_invL2((signed int*)vxx_user_para[user_index].modulation_data_ptr[0], 4*vxx_user_para[user_index].m_sybol, CACHE_WAIT);
	CACHE_invL2((signed int*)vxx_user_para[user_index].modulation_data_ptr[1], 4*vxx_user_para[user_index].m_sybol, CACHE_WAIT);

    if(1 == flag)
	{
    	if(((SPATIAL_MULTIPLEXING == vxx_user_para[user_index].ueTransMode) && (NUM_CODEWORD_1 == vxx_user_para[user_index].num_code_word))
		   || ((TRANSMIT_DIVERSITY == vxx_user_para[user_index].ueTransMode) && (NUM_CODEWORD_1 == vxx_user_para[user_index].num_code_word)))
		{
		    layermapping(
					     (signed int*)vxx_user_para[user_index].modulation_data_ptr[0],/*码字0调制后的符号 Q(16,3)*/
						 (signed int*)vxx_user_para[user_index].modulation_data_ptr[1],/*码字1调制后的符号 Q(16,3)*/
						 vxx_layermapping_fft_data.layermapping_data,/*层映射后的符号 Q(16,3)*/
						 vxx_user_para[user_index].m_sybol,/*码字0调制后的符号个数,在user_para_update中计算*/
						 &vxx_user_para[user_index].laryer_symbol,/*层映射后的每层符号个数*/
						 vxx_user_para[user_index].numLayers,/*层数*/
						 vxx_user_para[user_index].num_code_word,/*码字个数*/
					     vxx_user_para[user_index].ueTransMode/*下行传输模式*/
						);
		    precoding(
					  precoding_data_buffer,/*预编码后的符号 Q(16,3)*/
					  &vxx_user_para[user_index].m_ap_symbol,/*预编码后的符号个数*/
					  vxx_layermapping_fft_data.layermapping_data,/*层映射后的符号 Q(16,3)*/
					  vxx_layermapping_fft_data.layermapping_data + vxx_user_para[user_index].laryer_symbol,
					  vxx_user_para[user_index].laryer_symbol,/*层映射后的每层符号个数,是2的倍数吗？*/
					  vxx_user_para[user_index].numLayers,/*层数*/
					  vxx_cell_para.num_antenna_port,/*天线端口个数*/
					  vxx_user_para[user_index].ueTransMode,/*下行传输模式*/
					  vxx_user_para[user_index].delay_mod /*延时模式*/
					 );
		}
		if((SPATIAL_MULTIPLEXING == vxx_user_para[user_index].ueTransMode) && (NUM_CODEWORD_2 == vxx_user_para[user_index].num_code_word))
		{
		    precoding(
					  precoding_data_buffer,/*预编码后的符号 Q(16,3)*/
					  &vxx_user_para[user_index].m_ap_symbol,/*预编码后的符号个数*/
					  (signed int*)vxx_user_para[user_index].modulation_data_ptr[0],/*码字0调制后的符号 Q(16,3)*/
					  (signed int*)vxx_user_para[user_index].modulation_data_ptr[1],/*码字1调制后的符号 Q(16,3)*/
					  vxx_user_para[user_index].m_sybol,/*层映射后的每层符号个数,是2的倍数吗？*/
					  vxx_user_para[user_index].numLayers,/*层数*/
					  vxx_cell_para.num_antenna_port,/*天线端口个数*/
					  vxx_user_para[user_index].ueTransMode,/*下行传输模式*/
					  vxx_user_para[user_index].delay_mod /*延时模式*/
					 );
		}


    }
 }


