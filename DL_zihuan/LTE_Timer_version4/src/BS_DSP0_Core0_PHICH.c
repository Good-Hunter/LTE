/*
 * BS_DSP0_Core0_PHICH.c
 *
 *  Created on: 2015-4-13
 *      Author: Elevener
 */
#include "system_init.h"
#include "pl_comm.h"

void BS_DSP0_Core0_PHICH(char slot_idx,signed int *precoding_phich_data_buffer[NUM_ANTENNA_PORT_4])
{
    unsigned char m_HI;
    unsigned char m_n_seq;
    unsigned char m_n_group;
    int m_phich_index;
    int m_phich_temp[12];
    unsigned int m_c_init,m_temp_scramble_bits;
    int i;

    m_c_init = ((slot_idx>>1) + 1) * ((2 * vxx_cell_para.cellID + 1) << 9) + vxx_cell_para.cellID;
    scrambling_data_gen(x1_sequence_table,
        		        m_c_init,
                        12,/*��byteΪ��λ����*/
                        &m_temp_scramble_bits);

    if(0 != g_N_group_phich[slot_idx>>1])
    {
        memset(g_phich_data.phich_freq_data,0,g_N_group_phich[slot_idx>>1] * 12 *4);/*��Ƶ��ռ���0*/
        for(m_phich_index = 0;m_phich_index < vxx_mac_para.phich_num;m_phich_index++)
        {
             m_HI = vxx_mac_para.phich_para[m_phich_index].ACK_NACK;
             m_n_seq = vxx_mac_para.phich_para[m_phich_index].n_seq;
             m_n_group = vxx_mac_para.phich_para[m_phich_index].n_group;
             f_phich_data_gen(m_n_seq,
                              m_HI,
                              m_temp_scramble_bits,
                              m_phich_temp);
             for(i =0;i < 12;i++)/*�ۼ�*/
             {
                 g_phich_data.phich_freq_data[m_n_group * 12 + i] = g_phich_data.phich_freq_data[m_n_group * 12 + i] + m_phich_temp[i];
             }
        }
        //��ӳ��
        layermapping(
                     (signed int*)g_phich_data.phich_freq_data,/*����0���ƺ�ķ��� Q(16,3)*/
                     (signed int*)g_phich_data.phich_freq_data,/*����1���ƺ�ķ��� Q(16,3)*/
                     g_pdcch_data_after_layermapping,/*��ӳ���ķ��� Q(16,3) ����pdcch buffer*/
                     12 * g_N_group_phich[slot_idx>>1],
                     &vxx_cell_para.phich_layer_symbol,/*��ӳ����ÿ����Ÿ���*/
                     vxx_cell_para.num_antenna_port,/*����*/
                     1,/*���ָ���*/
                     vxx_cell_para.pbch_pcfich_pdcch_phich_ueTransMode/*���д���ģʽ*/
                     );
        //Ԥ����
        precoding(
                  precoding_phich_data_buffer,/*Ԥ�����ķ��� Q(16,3)*/
                  (unsigned int*)&vxx_cell_para.phich_m_ap_symbol,/*Ԥ�����ķ��Ÿ���*/
                  g_pdcch_data_after_layermapping,/*��ӳ���ķ��� Q(16,3) ����pdcch buffer*/
                  g_pdcch_data_after_layermapping + vxx_cell_para.phich_layer_symbol,
                  vxx_cell_para.phich_layer_symbol,/*��ӳ����ÿ����Ÿ���,��2�ı�����*/
                  vxx_cell_para.num_antenna_port,/*����*/
                  vxx_cell_para.num_antenna_port,/*���߶˿ڸ���*/
                  vxx_cell_para.pbch_pcfich_pdcch_phich_ueTransMode,/*���д���ģʽ*/
                  0 /*��delyaģʽ*/
                  );
    }

}
