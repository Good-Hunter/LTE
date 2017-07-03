/*
 * pdcch_scrambling.c
 *
 *  Created on: 2015-4-15
 *      Author: lycan
 */
#include <stdio.h>
#include "system_init.h"
#include "pl_comm.h"
#include "TSC.h"
void pdcch_scrambling(MAC_PARA *mac_para_ptr,
						int slot_idx,
						unsigned char *pRxDataBuffer[],
						unsigned int rxDataBufferLen[]
						)
{

	           int m_c_init;
	           int m_pdcch_index,m_start_CCE;
	           unsigned int *m_input_ptr;



				//…˙≥…»≈¬Î
				m_c_init = ((slot_idx >> 1) << 9) + vxx_cell_para.cellID;
	            scrambling_data_gen(x1_sequence_table,
	                                m_c_init,
	                                8 * g_dl_resource_mapping.symbol_pdcch_reg_num[slot_idx >> 1][0],
	                                g_pdcch_pseudo);

	            //º”»≈
	            for (m_pdcch_index = 0; m_pdcch_index < mac_para_ptr->pdcch_num; m_pdcch_index ++)
	            {
		            m_start_CCE = mac_para_ptr->pdcch_para[m_pdcch_index].start_CCE;

		            m_input_ptr = (unsigned int*)pRxDataBuffer[m_pdcch_index];

		            Bitscrambling(m_input_ptr,
		            		      &g_pdcch_bit_data_after_scram[m_start_CCE * 36],
		                          ((unsigned char*)g_pdcch_pseudo) + ((m_start_CCE * 72) >> 3),
		                          rxDataBufferLen[m_pdcch_index]);
	            }






}




