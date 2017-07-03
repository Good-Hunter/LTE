/***********************************************/
/*system_init                                  */
/*功能：系统初始化源文件                       */
/***********************************************/
#include <system_init.h>
//#include <pl_comm.h>
#define N_DL_RE 1200//N_DL_RB * N_SC_PER_RB
void system_init()
{

    //glbCoreId = DNUM;
    enable_bcp();
    BCP_initQm();
    BCP_initCppi();
    BCP_initBcp();
    BCP_setupQm();
    BCP_setupCppi(); 
    BCP_setupDesc();
}  



void  fft_data_copy(unsigned char num_antenna_port,
                    unsigned char OFDMA_start,
                    unsigned char OFDMA_end,
                    unsigned int* tx_buffer_ptr,
                    unsigned int ant_index
                   )
{
     unsigned int i;
     unsigned int OFDMA_idx_l;
     unsigned int* restrict fftc_input_data_ptr1;
     unsigned int* restrict fftc_input_data_ptr2;


     if(num_antenna_port == NUM_ANTENNA_PORT_2) 
     {

         for (OFDMA_idx_l = OFDMA_start; OFDMA_idx_l < OFDMA_end ; OFDMA_idx_l++)
         {

        	 fftc_input_data_ptr1 = tx_buffer_ptr + (OFDMA_idx_l - OFDMA_start) * (N_DL_RE + 4);
        	 fftc_input_data_ptr2 = tx_buffer_ptr + (OFDMA_idx_l - OFDMA_start) * (N_DL_RE + 4) + 1 + ((N_DL_RE) >> 1);
        	 /*拷贝数据*/

#if 0
        	 memcpy(fftc_input_data_ptr1,&vxx_output_data[ant_index][OFDMA_idx_l * N_DL_RB * N_SC_PER_RB],4*((N_DL_RB * N_SC_PER_RB) >> 1));
        	 memcpy(fftc_input_data_ptr2,&vxx_output_data[ant_index][OFDMA_idx_l * N_DL_RB * N_SC_PER_RB+ ((N_DL_RB * N_SC_PER_RB) >> 1)],4*((N_DL_RB * N_SC_PER_RB) >> 1));
        	 fftc_input_data_ptr1 = fftc_input_data_ptr1 + ((N_DL_RB * N_SC_PER_RB) >> 1);
        	 fftc_input_data_ptr2 = fftc_input_data_ptr2 + ((N_DL_RB * N_SC_PER_RB) >> 1);
#endif
#if 1
			#pragma MUST_ITERATE(, , 8);
        	 for (i = 0; i < ((N_DL_RE) >> 1); i++)
             {
                 *fftc_input_data_ptr1++ = vxx_output_data[ant_index][OFDMA_idx_l * N_DL_RE + i];
                 *fftc_input_data_ptr2++ = vxx_output_data[ant_index][OFDMA_idx_l * N_DL_RE + i + ((N_DL_RE) >> 1)];
             }

#endif
             *fftc_input_data_ptr1++ = 0;
             //*fftc_input_data_ptr2++ = 0;
             //#pragma MUST_ITERATE(, , 8);
             //for (i = ((N_DL_RB * N_SC_PER_RB) >> 1); i < (N_DL_RB * N_SC_PER_RB); i++)
             //{
             //    *fftc_input_data_ptr1++ = vxx_output_data[0][OFDMA_idx_l * N_DL_RB * N_SC_PER_RB + i];
                 //*fftc_input_data_ptr2++ = vxx_output_data[1][OFDMA_idx_l * N_DL_RB * N_SC_PER_RB + i];
             //}
             for (i = (N_DL_RE); i < (N_DL_RE + 3); i++)
             {
                 *fftc_input_data_ptr2++ = 0;
                 //*fftc_input_data_ptr2++ = 0;
             }

         
         }
     }    
     
}
#if 1
void  fft_output_data_copy(unsigned char num_antenna_port,
					unsigned int* restrict fftc_output_data_ptr1,
					unsigned int* restrict fftc_output_data_ptr2,
                    unsigned int length,
                    int factor0,
                    int factor1
                   )
{
     unsigned int i;
     double temp1,temp2;
     double temp11,temp21;
     int temp3,temp4,temp5,temp6;
     int temp31,temp41,temp51,temp61;
     //unsigned int* restrict temp_data_ptr1 = &vxx_layermapping_fft_data.fft_output_data[0][used_len];
     //unsigned int* restrict temp_data_ptr2 = &vxx_layermapping_fft_data.fft_output_data[1][used_len];
     //2015.5.26修改，将FFTC产生数据直接产生至total数组
     unsigned int* restrict temp_data_ptr1 = &total_output_data[slot_idx>>1][0][used_len];
     unsigned int* restrict temp_data_ptr2 = &total_output_data[slot_idx>>1][1][used_len];


     if(num_antenna_port == NUM_ANTENNA_PORT_2)
     {
        	 /*拷贝数据*/
             #pragma MUST_ITERATE(, , 8);
             for (i = 0; i < (length / 2); i++)
             {
            	 /*2015.1.26 用移位来替代乘法，舍去根号2*/
            	 /*temp1 = _shr2(*fftc_output_data_ptr1++,factor0);
            	 temp2 = _shr2(*fftc_output_data_ptr2++,factor1);
            	 temp11 = _shr2(*fftc_output_data_ptr1++,factor0);
            	 temp21 = _shr2(*fftc_output_data_ptr2++,factor1);*/
            	 //将数组用指针来代替
            	 temp1 = _mpy2(*fftc_output_data_ptr1++,factor0);
            	 temp2 = _mpy2(*fftc_output_data_ptr2++,factor1);
            	 temp11 = _mpy2(*fftc_output_data_ptr1++,factor0);
            	 temp21 = _mpy2(*fftc_output_data_ptr2++,factor1);

            	 /*temp1 = _mpy2(fftc_output_data_ptr1[2 * i],factor0);//偏移agc因子
            	 temp2 = _mpy2(fftc_output_data_ptr2[2 * i],factor1);
            	 temp11 = _mpy2(fftc_output_data_ptr1[2 * i + 1],factor0);//偏移agc因子
            	 temp21 = _mpy2(fftc_output_data_ptr2[2 * i + 1],factor1);
            	 temp3 = _hi(temp1);
            	 temp4 = _lo(temp1);
            	 temp5 = _hi(temp2);
            	 temp6 = _lo(temp2);
            	 temp31 = _hi(temp11);
            	 temp41 = _lo(temp11);
            	 temp51 = _hi(temp21);
            	 temp61 = _lo(temp21);
            	 temp_data_ptr1[2 * i] = _packh2(temp3,temp4);
            	 temp_data_ptr2[2 * i] = _packh2(temp5,temp6);
            	 temp_data_ptr1[2 * i + 1] = _packh2(temp31,temp41);
            	 temp_data_ptr2[2 * i + 1] = _packh2(temp51,temp61);*/

            	 *temp_data_ptr1++ = _packh2(_hi(temp1),_lo(temp1));
            	 *temp_data_ptr2++ = _packh2(_hi(temp2),_lo(temp2));
            	 *temp_data_ptr1++ = _packh2(_hi(temp11),_lo(temp11));
            	 *temp_data_ptr2++ = _packh2(_hi(temp21),_lo(temp21));

            	 //vxx_layermapping_fft_data.fft_output_data[0][used_len + i] = fftc_output_data_ptr1[i];
            	 //vxx_layermapping_fft_data.fft_output_data[1][used_len + i] = fftc_output_data_ptr2[i];
             }
             used_len = used_len + length;

     }

}
#endif

