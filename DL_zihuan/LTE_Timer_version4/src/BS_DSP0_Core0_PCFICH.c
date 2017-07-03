/*
 * BS_DSP0_Core0_PCFICH.c
 *
 *  Created on: 2015-4-13
 *      Author: Elevener
 */

#include "system_init.h"
#include "pl_comm.h"


void BS_DSP0_Core0_PCFICH(char slot_idx,signed int *precoding_pcfich_data_buffer[NUM_ANTENNA_PORT_4])
{
	unsigned int m_pcfich_data_origin;
	unsigned int m_c_init,m_temp_scramble_bits;
	unsigned int m_g_pcfich_data_after_scram;

	unsigned int *m_p_data_after_modulation;
	unsigned int *m_p_data_after_layermapping;

	m_p_data_after_modulation=&g_pcfich_data_after_modulation[0];
	m_p_data_after_layermapping=&g_pcfich_data_after_layermapping[0];

	//Coding
	m_pcfich_data_origin = g_const_cfi_code_table[vxx_cell_para.pcfich_index  % 3];

	//Scrambing
	m_c_init = ((slot_idx>>1) + 1) * ((2 * vxx_cell_para.cellID + 1) << 9) + vxx_cell_para.cellID;

    scrambling_data_gen(x1_sequence_table,
    		            m_c_init,
                        PCFICH_BIT_LEN,/*以byte为单位输入*/
                        &m_temp_scramble_bits);

    m_g_pcfich_data_after_scram = m_pcfich_data_origin ^ m_temp_scramble_bits;

    //Modulation
    f_qpsk_modulation((unsigned char*)&m_g_pcfich_data_after_scram,
    		           m_p_data_after_modulation,
                       PCFICH_BIT_LEN);

    //Layermapping
    layermapping(
    		     m_p_data_after_modulation,/*码字0调制后的符号 Q(16,3)*/
    		     m_p_data_after_modulation,/*码字1调制后的符号 Q(16,3)*/
                 m_p_data_after_layermapping,/*层映射后的符号 Q(16,3)*/
                 PCFICH_BIT_LEN/2,/*码字0调制后的符号个数,在user_para_update中计算*/
                 &vxx_cell_para.pcfich_layer_symbol,/*层映射后的每层符号个数*/
                 vxx_cell_para.num_antenna_port,/*层数*/
                 1,/*码字个数*/
                 vxx_cell_para.pbch_pcfich_pdcch_phich_ueTransMode/*下行传输模式*/
                 );

    //Precoding
    precoding(
              precoding_pcfich_data_buffer,/*预编码后的符号 Q(16,3)*/
              (unsigned int*)&vxx_cell_para.pcfich_m_ap_symbol,/*预编码后的符号个数*/
              g_pcfich_data_after_layermapping,/*层映射后的符号 Q(16,3)*/
              g_pcfich_data_after_layermapping + vxx_cell_para.pcfich_layer_symbol,
              vxx_cell_para.pcfich_layer_symbol,/*层映射后的每层符号个数,是2的倍数吗？*/
              vxx_cell_para.num_antenna_port,/*层数*/
              vxx_cell_para.num_antenna_port,/*天线端口个数*/
              vxx_cell_para.pbch_pcfich_pdcch_phich_ueTransMode,/*下行传输模式*/
              0 /*无delya模式*/
              );

}
