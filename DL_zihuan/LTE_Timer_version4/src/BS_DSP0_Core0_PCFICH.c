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
                        PCFICH_BIT_LEN,/*��byteΪ��λ����*/
                        &m_temp_scramble_bits);

    m_g_pcfich_data_after_scram = m_pcfich_data_origin ^ m_temp_scramble_bits;

    //Modulation
    f_qpsk_modulation((unsigned char*)&m_g_pcfich_data_after_scram,
    		           m_p_data_after_modulation,
                       PCFICH_BIT_LEN);

    //Layermapping
    layermapping(
    		     m_p_data_after_modulation,/*����0���ƺ�ķ��� Q(16,3)*/
    		     m_p_data_after_modulation,/*����1���ƺ�ķ��� Q(16,3)*/
                 m_p_data_after_layermapping,/*��ӳ���ķ��� Q(16,3)*/
                 PCFICH_BIT_LEN/2,/*����0���ƺ�ķ��Ÿ���,��user_para_update�м���*/
                 &vxx_cell_para.pcfich_layer_symbol,/*��ӳ����ÿ����Ÿ���*/
                 vxx_cell_para.num_antenna_port,/*����*/
                 1,/*���ָ���*/
                 vxx_cell_para.pbch_pcfich_pdcch_phich_ueTransMode/*���д���ģʽ*/
                 );

    //Precoding
    precoding(
              precoding_pcfich_data_buffer,/*Ԥ�����ķ��� Q(16,3)*/
              (unsigned int*)&vxx_cell_para.pcfich_m_ap_symbol,/*Ԥ�����ķ��Ÿ���*/
              g_pcfich_data_after_layermapping,/*��ӳ���ķ��� Q(16,3)*/
              g_pcfich_data_after_layermapping + vxx_cell_para.pcfich_layer_symbol,
              vxx_cell_para.pcfich_layer_symbol,/*��ӳ����ÿ����Ÿ���,��2�ı�����*/
              vxx_cell_para.num_antenna_port,/*����*/
              vxx_cell_para.num_antenna_port,/*���߶˿ڸ���*/
              vxx_cell_para.pbch_pcfich_pdcch_phich_ueTransMode,/*���д���ģʽ*/
              0 /*��delyaģʽ*/
              );

}
