#include <stdio.h>
#include <string.h>
#include <c6x.h>
#include "csl_psc.h"
#include "csl_pscAux.h"
#include "csl_intc.h"
#include "csl_cpIntcAux.h"
#include "system_init.h"
#include "pl_comm.h"
#include "tcp3dDrv.h"


/* Global variable declaration */
#define CSL_TCP3D_DMA_TCP3D_OUT_STS0_P0_OFFSET      (0x000100u)

#define hTCP3dADmaReg               (CSL_TCP3D_A_DATA_REGS + CSL_TCP3D_DMA_TCP3D_OUT_STS0_P0_OFFSET + 4)



unsigned char tcp3d_llr_index = 0;/*交替使用LLR0和LLR1*/
void EndianConverter_8b(unsigned int * ptr, unsigned int len)
{
  unsigned int i;

  for (i = 0; i < len; i++)
    {
        /* swap bytes */
        ptr[i] = (ptr[i] >> 24) |
            (ptr[i] & 0x00ff0000) >> 8 |
            (ptr[i] & 0x0000ff00) << 8  |
            (ptr[i] << 24);
    }
}
int cycle1,cycle2,cycle3,cycle4,cycle5,cycle_start,cycle_end;
int tcp3d_lte_pdsch(USER_PARA *user_para_ptr,
                     signed char* restrict tc_testvector,
                     unsigned char* restrict tb_buffer,
                     unsigned int  cblen0,
                     unsigned int  cblen1,
                     unsigned char tcp3d_llr_index,
                     unsigned char tcp3d_flag,
                     unsigned int  used_cb_len,
                     signed short total_cbs)
{
    unsigned int  i, cb_index,k,j,crc_check;
    unsigned int  tc_testvector_len1,tc_testvector_len2,used_len;
    signed char    tailBits[12];
    signed char    d0[4],d1[4],d2[4];
    unsigned int * restrict input_ptr1;
    unsigned char * restrict input_ptr4;



    EDMA_PARAM_SET_WORD * par;
    CSL_Edma3ChannelHandle  hEdmaCh;
    cycle2 = TSCL;
    if(2 != tcp3d_flag)
    {
        //used_cb_len = 0;
    //for(cb_index = 0;cb_index < user_para_ptr->num_of_cb;cb_index++)
    //{
    //    cblen1 = user_para_ptr->cb_len[cb_index];
        
        tc_testvector_len1 = 16 * ((cblen1 + 4 + 15) >> 4);/*由于解速率匹配结果是128bit对齐，所以S Y1 Y2之间偏移长度并非是cb_len+4*/
        


        

        /* The tail bit sequence should be sys[0] */
        for( i = 0; i < 4; i++ )
        {
          d0[i] = tc_testvector[cblen1 * 1 + 0 + i];
          d1[i] = tc_testvector[tc_testvector_len1 * 1 + cblen1 * 1 + i];
          d2[i] = tc_testvector[tc_testvector_len1 * 2 + cblen1 * 1 + i];
        }
        
        /* prepare the TCP3d config */
        tailBits[0]  = d0[0];   //sys[0]
        tailBits[1]  = d1[0];   //par[0]
        tailBits[2]  = d2[0];   //sys[1]
        tailBits[3]  = d0[1];   //par[1]
        tailBits[4]  = d1[1];   //sys[2]
        tailBits[5]  = d2[1];   //par[2]
        tailBits[6]  = d0[2];   //sys1[0]
        tailBits[7]  = d1[2];   //par1[0]
        tailBits[8]  = d2[2];   //sys1[1]
        tailBits[9]  = d0[3];   //par1[1]
        tailBits[10] = d1[3];   //sys1[2]
        tailBits[11] = d2[3];   //par1[2]
        memset(tcp3d_cfg,0,(sizeof(tcp3d_cfg)));

        if(0 == (tcp3d_llr_index & 1))
        {
        	memset(tcp3d_hd[0],0xff,(sizeof(tcp3d_hd)/2));
        	//memset(tcp3d_llr[0],0,(sizeof(tcp3d_llr)/2));


        	prepare_user_info ((BCP_userInfo *)tcp3d_cfg[0], tailBits, cblen1,total_cbs);

            tcp3d_data_copy(cblen1,
            		        tc_testvector_len1,
            		        tc_testvector,
            		        &tcp3d_llr[0][0]);
           // CACHE_wbL2((void *)&tcp3d_llr[0][0],sizeof(tcp3d_llr[0]), CACHE_WAIT);



            par = (EDMA_PARAM_SET_WORD *)DEMO_EdmaTbl[LLR0_LINK_INDEX].PaRAM_Addr;
            par->opt   |= 1<<23;  //itcchEn = 1
            par->abCnt = llr0_link_index_abCnt | (cblen1/2);
            par->bIdx  = llr0_link_index_bIdx | (cblen1/2);
            par->cIdx  = llr0_link_index_cIdx | (cblen1+4);

            /* setup HD0 PaRAM field aCnt */
            par = (EDMA_PARAM_SET_WORD *)DEMO_EdmaTbl[HD0_LINK_INDEX].PaRAM_Addr;
            par->opt   |= 1<<20;
            par->abCnt = hd0_link_index_abCnt | (cblen1/8);
            /* Trigger the EDMA */
            /* Trigger channel */
            hEdmaCh = (CSL_Edma3ChannelHandle)DEMO_EdmaTbl[TRIGGER_CH_INDEX].chHandler;
            CSL_edma3HwChannelControl(hEdmaCh,CSL_EDMA3_CMD_CHANNEL_SET,NULL);
            cycle_start = TSCL;
            if(0 == tcp3d_flag)
            {

              //cycle_start = TSCL;
              while(0xffffffff == tcp3d_hd[1][0]);
              //cycle_end = TSCL;
              //cycle1 = cycle_end - cycle_start;
              crc_check = ((*(unsigned int*)hTCP3dADmaReg) >> 29) & 0x00000001;
              gxx_crc_check += crc_check;
              //printf ("CRC RESULT: %d \n", crc_check);



              tcp3d_data_bit_copy(cblen0,
                                  used_cb_len,
                                  (unsigned char*)tcp3d_hd[1],
                                  tb_buffer);
              if(crc_check==0)
              {
              	//printf("Decode error!\n");
              	return 0;
              }
              return 1;

            }





        }
        else
        {
        	cycle3 = TSCL;
        	memset(tcp3d_hd[1],0xff,(sizeof(tcp3d_hd)/2));
        	//memset(tcp3d_llr[1],0,(sizeof(tcp3d_llr)/2));


        	prepare_user_info ((BCP_userInfo *)tcp3d_cfg[1], tailBits, cblen1,total_cbs);
        	//cycle_end = TSCL;
        	//cycle3 = cycle_end - cycle_start;
        	tcp3d_data_copy(cblen1,
        	              	  tc_testvector_len1,
        	              	  tc_testvector,
        	                  &tcp3d_llr[1][0]);

        	//CACHE_wbL2((void *)&tcp3d_llr[1][0],sizeof(tcp3d_llr[1]), CACHE_WAIT);
        	//cycle_start = TSCL;
        	//cycle4 = cycle_start - cycle_end;
        	/* Trigger the EDMA */
        	/* Trigger channel */
        	par = (EDMA_PARAM_SET_WORD *)DEMO_EdmaTbl[LLR1_LINK_INDEX].PaRAM_Addr;
        	par->opt   |= 1<<23;  //itcchEn = 1
        	par->abCnt = llr1_link_index_abCnt | (cblen1/2);
        	par->bIdx  = llr1_link_index_bIdx | (cblen1/2);
        	par->cIdx  = llr1_link_index_cIdx | (cblen1+4);

        	/* setup HD0 PaRAM field aCnt */
        	par = (EDMA_PARAM_SET_WORD *)DEMO_EdmaTbl[HD1_LINK_INDEX].PaRAM_Addr;
        	par->opt   |= 1<<20;
        	par->abCnt = hd1_link_index_abCnt | (cblen1/8);
        	hEdmaCh = (CSL_Edma3ChannelHandle)DEMO_EdmaTbl[TRIGGER_CH_INDEX].chHandler;
        	CSL_edma3HwChannelControl(hEdmaCh,CSL_EDMA3_CMD_CHANNEL_SET,NULL);
        	//cycle_end = TSCL;
        	//cycle5 = cycle_end - cycle_start;
            if(0 == tcp3d_flag)
        	{
                cycle1 = TSCL;
                while(0xffffffff == tcp3d_hd[0][0]);
                cycle_end = TSCL;
                cycle4 = cycle_end - cycle_start;

                crc_check = ((*(unsigned int*)hTCP3dADmaReg) >> 29) & 0x00000001;
                gxx_crc_check += crc_check;


                //printf ("CRC RESULT: %d \n", crc_check);
                tcp3d_data_bit_copy(cblen0,
                    	        used_cb_len,
                    	        (unsigned char*)tcp3d_hd[0],
                    	        tb_buffer);
                if(crc_check==0)
                {
                	//printf("Decode error!\n");
                	return 0;
                }

                return 1;
        	}
        }
        return 1;


    }
    else//2表示最后一包译码结果，仅需要拷贝
    {
        if(1 == (tcp3d_llr_index & 1))
        {
        	while(0xffffffff == tcp3d_hd[0][0]);

        	crc_check = ((*(unsigned int*)hTCP3dADmaReg) >> 29) & 0x00000001;
        	gxx_crc_check += crc_check;



        	tcp3d_data_bit_copy(cblen0,
        	        	        used_cb_len,
        	        	        (unsigned char*)tcp3d_hd[0],
        	        	        tb_buffer);
            if(crc_check==0)
            {
            	//printf("Decode error!\n");
            	return 0;
            }

        	return 1;
        }
        else
        {
        	while(0xffffffff == tcp3d_hd[1][0]);

        	crc_check = ((*(unsigned int*)hTCP3dADmaReg) >> 29) & 0x00000001;
        	gxx_crc_check += crc_check;



        	tcp3d_data_bit_copy(cblen0,
        	        	        used_cb_len,
        	        	        (unsigned char*)tcp3d_hd[1],
        	        	         tb_buffer);

            if(crc_check==0)
            {
            	//printf("Decode error!\n");
            	return 0;
            }

        	return 1;
        }
    }


        //used_len += 3 * tc_testvector_len1;


        //if(0 == (tcp3d_llr_index & 1))
        //{
        	//input_ptr4 = (unsigned char*)tcp3d_hd[0];

        	//for(i = 0; i < (cblen1 - 24) >> 3; i++ )
        	//{
        	    //tb_buffer[i] = input_ptr4[i];
        	//}
        //}
        //else
        //{
        //

        //}
        //used_cb_len += (cblen1 - 24) >> 3;/*比特按照char类型存储的*/
        //tcp3d_llr_index = (tcp3d_llr_index + 1) & 1;

    //}

    

}

