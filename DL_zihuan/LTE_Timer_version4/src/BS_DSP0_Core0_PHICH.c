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
                        12,/*以byte为单位输入*/
                        &m_temp_scramble_bits);

    if(0 != g_N_group_phich[slot_idx>>1])
    {
        memset(g_phich_data.phich_freq_data,0,g_N_group_phich[slot_idx>>1] * 12 *4);/*将频域空间清0*/
        for(m_phich_index = 0;m_phich_index < vxx_mac_para.phich_num;m_phich_index++)
        {
             m_HI = vxx_mac_para.phich_para[m_phich_index].ACK_NACK;
             m_n_seq = vxx_mac_para.phich_para[m_phich_index].n_seq;
             m_n_group = vxx_mac_para.phich_para[m_phich_index].n_group;
             f_phich_data_gen(m_n_seq,
                              m_HI,
                              m_temp_scramble_bits,
                              m_phich_temp);
             for(i =0;i < 12;i++)/*累加*/
             {
                 g_phich_data.phich_freq_data[m_n_group * 12 + i] = g_phich_data.phich_freq_data[m_n_group * 12 + i] + m_phich_temp[i];
             }
        }
        //层映射
        layermapping(
                     (signed int*)g_phich_data.phich_freq_data,/*码字0调制后的符号 Q(16,3)*/
                     (signed int*)g_phich_data.phich_freq_data,/*码字1调制后的符号 Q(16,3)*/
                     g_pdcch_data_after_layermapping,/*层映射后的符号 Q(16,3) 复用pdcch buffer*/
                     12 * g_N_group_phich[slot_idx>>1],
                     &vxx_cell_para.phich_layer_symbol,/*层映射后的每层符号个数*/
                     vxx_cell_para.num_antenna_port,/*层数*/
                     1,/*码字个数*/
                     vxx_cell_para.pbch_pcfich_pdcch_phich_ueTransMode/*下行传输模式*/
                     );
        //预编码
        precoding(
                  precoding_phich_data_buffer,/*预编码后的符号 Q(16,3)*/
                  (unsigned int*)&vxx_cell_para.phich_m_ap_symbol,/*预编码后的符号个数*/
                  g_pdcch_data_after_layermapping,/*层映射后的符号 Q(16,3) 复用pdcch buffer*/
                  g_pdcch_data_after_layermapping + vxx_cell_para.phich_layer_symbol,
                  vxx_cell_para.phich_layer_symbol,/*层映射后的每层符号个数,是2的倍数吗？*/
                  vxx_cell_para.num_antenna_port,/*层数*/
                  vxx_cell_para.num_antenna_port,/*天线端口个数*/
                  vxx_cell_para.pbch_pcfich_pdcch_phich_ueTransMode,/*下行传输模式*/
                  0 /*无delya模式*/
                  );
    }

}
