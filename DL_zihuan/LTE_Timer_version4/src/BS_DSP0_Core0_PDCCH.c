/*
 * BS_DSP0_Core0_PDCCH.c
 *
 *  Created on: 2015-4-9
 *      Author: lycan
 */
#include <stdio.h>
#include "system_init.h"
#include "pl_comm.h"
#include "TSC.h"

extern unsigned volatile short  subframeN;
extern unsigned volatile int temp_num;

extern void bcp_pdcch_CCEconfig(unsigned int numTestPkts,
							unsigned char ns,
							CELL_PARA *cell_para_ptr,
							MAC_PARA *mac_para_ptr);

extern void pdcch_scrambling(MAC_PARA *mac_para_ptr,
						int slot_idx,
						unsigned char *m_pRxDataBuffer[],
						unsigned int m_rxDataBufferLen[]
						);

/*
 * hello.c
 */
void BS_DSP0_Core0_PDCCH()
{


	           int m_pdcch_index,m_i;
	           unsigned int m_rxDataBufferLen[10];
	           unsigned char *m_pRxDataBuffer[10];
	           signed int * m_precoding_pdcch_data_buffer[NUM_ANTENNA_PORT_2];


	           for(m_i = 0;m_i < NUM_ANTENNA_PORT_2;m_i++)
	           {
	        	  m_precoding_pdcch_data_buffer[m_i] = g_pdcch_data_after_precoding[m_i];
	           }



				//����
	            memset(pdcch_cce_unuse_flag,0x01,100);
	            memset(g_pdcch_bit_data_after_scram,0x0,3200);
	            //��������
	            for (m_pdcch_index = 0; m_pdcch_index < vxx_mac_para.pdcch_num; m_pdcch_index ++)
	            {
	            	bcp_pdcch_CCEconfig(m_pdcch_index,
	            					slot_idx,
	            					&vxx_cell_para,
	            					&vxx_mac_para);
	            }
	            /*****************bcp����***********************/
	            //���룬����ƥ�䣬����
	            bcp_lte_pdcch(&vxx_cell_para,//�ǵø�pl_comm.h�ĺ�������
	                         slot_idx,
	                         &vxx_mac_para,
	                         g_pdcch_pseudo,
	                         m_rxDataBufferLen,
	                         m_pRxDataBuffer);

	            /**************����*****************************/
	            pdcch_scrambling(&vxx_mac_para,
	            		         slot_idx,
	            		         m_pRxDataBuffer,
	            		         m_rxDataBufferLen);

	            //��ӳ��
	            layermapping(
	                         (signed int*)g_pdcch_bit_data_after_scram,/*����0���ƺ�ķ��� Q(16,3)*/
	                         (signed int*)g_pdcch_bit_data_after_scram,/*����1���ƺ�ķ��� Q(16,3)*/
	                         g_pdcch_data_after_layermapping,/*��ӳ���ķ��� Q(16,3)*/
	                         4 * g_dl_resource_mapping.symbol_pdcch_reg_num[subframeN][0],/*����0���ƺ�ķ��Ÿ���,��user_para_update�м���*/
	                         &vxx_mac_para.pdcch_layer_symbol,/*��ӳ����ÿ����Ÿ���*/
	                         vxx_cell_para.num_antenna_port,/*����*/
	                         1,/*���ָ���*/
	                         vxx_cell_para.pbch_pcfich_pdcch_phich_ueTransMode/*���д���ģʽ*/
	                         );
	            //Ԥ����
	            precoding(
//	            		  &g_pdcch_data_after_precoding[0],/*Ԥ�����ķ���ֱ���õ�ȫ�ֱ���*/
	                      m_precoding_pdcch_data_buffer,/*Ԥ�����ķ��� Q(16,3)*/
	                      (unsigned int*)&vxx_mac_para.pdcch_m_ap_symbol,/*Ԥ�����ķ��Ÿ���*/
	                      g_pdcch_data_after_layermapping,/*��ӳ���ķ��� Q(16,3)*/
	                      g_pdcch_data_after_layermapping + vxx_mac_para.pdcch_layer_symbol,
	                      vxx_mac_para.pdcch_layer_symbol,/*��ӳ����ÿ����Ÿ���,��2�ı�����*/
	                      vxx_cell_para.num_antenna_port,/*����*/
	                      vxx_cell_para.num_antenna_port,/*���߶˿ڸ���*/
	                      vxx_cell_para.pbch_pcfich_pdcch_phich_ueTransMode,/*���д���ģʽ*/
	                      0 /*��delyaģʽ*/
	                      );

	            f_block_interleaver((long long*)g_pdcch_data_after_precoding[0],
	                                (long long*)g_pdcch_data_after_precoding[1],
	                                (long long*)g_pdcch_data_after_interleaver[0],
	                                (long long*)g_pdcch_data_after_interleaver[1],
	                                vxx_cell_para.pdcch_total_reg_num[subframeN],
	                                vxx_cell_para.cellID);//Ϊ��֯�����


}





