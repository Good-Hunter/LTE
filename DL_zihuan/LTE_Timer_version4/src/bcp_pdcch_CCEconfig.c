
#include <stdio.h>
#include "system_init.h"
#include "pl_comm.h"
#include "TSC.h"
/*
 * hello.c
 */
extern unsigned short f_pdcch_cce_start(unsigned char pdcch_format,
                   unsigned short rnti,
                   unsigned short N_cce,
                   unsigned char subfn,
                   unsigned char common_pdcch_flag
                     );

void bcp_pdcch_CCEconfig(unsigned int numTestPkts,
        			unsigned char ns,
        			CELL_PARA *cell_para_ptr,
        			MAC_PARA *mac_para_ptr
        			)
{
	unsigned char aggregation_level_table[4] = {1,2,4,8};
	unsigned char               pdcch_format;
//	unsigned short              pdcch_bit_length;
    char                        common_pdcch_flag;
	unsigned char               DCI_cce_num,m,temp_start_CCE;
	    pdcch_format = mac_para_ptr->pdcch_para[numTestPkts].pdcch_format;
//	    pdcch_bit_length = g_const_DCI_bit_num_table[pdcch_format];//速率匹配模块会用到pdcch_bit_length
	    common_pdcch_flag = mac_para_ptr->pdcch_para[numTestPkts].common_pdcch;
	    mac_para_ptr->pdcch_para[numTestPkts].start_CCE =  f_pdcch_cce_start(pdcch_format,
	                         	 	 	 	 	 	 	 	 	 	 	 	 mac_para_ptr->pdcch_para[numTestPkts].rnti,
	                         	 	 	 	 	 	 	 	 	 	 	 	 cell_para_ptr->pdcch_total_cce_num[ns >> 1],
	                         	 	 	 	 	 	 	 	 	 	 	 	 ns >> 1,
	                         	 	 	 	 	 	 	 	 	 	 	 	 common_pdcch_flag
	                           	   	   	   	   	   	   	   	   	   	   	   	   );
	    if(0 == pdcch_cce_unuse_flag[ mac_para_ptr->pdcch_para[numTestPkts].start_CCE])//已经被同RNTI占用
	    {

	       for(m = 1;m < g_const_pdcch_serch_space[pdcch_format];m++)
	       {
	    	   temp_start_CCE =  mac_para_ptr->pdcch_para[numTestPkts].start_CCE + m * aggregation_level_table[pdcch_format];
	    	   temp_start_CCE = temp_start_CCE % (cell_para_ptr->pdcch_total_cce_num[ns >> 1] / aggregation_level_table[pdcch_format]);
	           if(1 == pdcch_cce_unuse_flag[temp_start_CCE])//说明没有被占用
	           {
	        	   mac_para_ptr->pdcch_para[numTestPkts].start_CCE = temp_start_CCE;
	        	   break;
	           }
	       }
	       pdcch_cce_unuse_flag[mac_para_ptr->pdcch_para[numTestPkts].start_CCE] = 0;
	    }
	    else
	    {
	       pdcch_cce_unuse_flag[mac_para_ptr->pdcch_para[numTestPkts].start_CCE] = 0;
	    }
}





