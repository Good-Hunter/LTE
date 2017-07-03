/***********************************************/
/*system_init                                  */
/*功能：系统初始化源文件                       */
/***********************************************/
#include <system_init.h>

void system_init()
{
    //glbCoreId = DNUM;

  BCP_initQm();
  BCP_initCppi();
  BCP_initBcp();
    BCP_setupQm();
    BCP_setupCppi();
    BCP_setupDesc();


}  
void bcp_data_copy(unsigned int len,
               unsigned int* restrict input,
               unsigned int* restrict output)
{
  double * restrict input_ptr1;
  double * restrict input_ptr2;
  unsigned int i;
  input_ptr1 = (double *)input;
  input_ptr2 = (double *)output;
  for( i = 0; i < ((len + 8) >> 3); i++ )//保证是8的倍数，即使多拷贝了也没有关系
    {
    output[i] = input[i];
    }
}
void tcp3d_data_copy(unsigned int cblen1,
                 unsigned int tc_testvector_len1,
                 signed char* restrict tc_testvector,
                 signed char* restrict tcp3d_llr_ptr)
{
  unsigned int * restrict input_ptr1;
    unsigned int * restrict input_ptr2;
  unsigned int * restrict input_ptr3;
  unsigned int * restrict tcp3d_ptr1;
  unsigned int * restrict tcp3d_ptr2;
  unsigned int * restrict tcp3d_ptr3;
  unsigned int i;

  tcp3d_ptr1 = (unsigned int*)tcp3d_llr_ptr;
  tcp3d_ptr2 = (unsigned int*)&tcp3d_llr_ptr[cblen1 + 4];
  tcp3d_ptr3 = (unsigned int*)&tcp3d_llr_ptr[2 * (cblen1 + 4)];
  input_ptr1 = (unsigned int*)tc_testvector;
  input_ptr2 = (unsigned int*)&tc_testvector[tc_testvector_len1];
  input_ptr3 = (unsigned int*)&tc_testvector[tc_testvector_len1 * 2];

  for( i = 0; i < ((cblen1 + 4)>> 2); i++ )
  {

       tcp3d_ptr1[i] = input_ptr1[i];
       tcp3d_ptr2[i] = input_ptr2[i];
       tcp3d_ptr3[i] = input_ptr3[i];
  }
}
void  tcp3d_data_bit_copy(unsigned int cblen1,
                          unsigned int used_cb_len,
                          signed char* restrict input_ptr4,
                          signed char* restrict tb_buffer)
{
  unsigned int i,j,k;
  for(i = 0; i < (cblen1 - 24) >> 3; i++ )
  {
       j = (used_cb_len + i) >> 2;
       k = i >> 2;
       tb_buffer[4 * j + 3 - ((used_cb_len + i) & 3)] = input_ptr4[4 * k + 3 - (i & 3)];/*解速率匹配结果是小端输入，需要变成大端格式*/
  }
}
void  fft_data_copy(unsigned char num_antenna,
                    unsigned char OFDMA_start,
                    unsigned char OFDMA_end,
                    unsigned int* fftc_input_data_ptr1,
                    signed char shift_factor
                   )
{
     unsigned int i;
     unsigned int OFDMA_idx_l;
     unsigned int* restrict fft_data_temp1;
     unsigned int* restrict fft_data_temp2;
     unsigned int* restrict fftc_input_data_ptr1_temp1;
     unsigned int* restrict fftc_input_data_ptr1_temp2;



     if(shift_factor > 0)
     {    
         for (OFDMA_idx_l = OFDMA_start; OFDMA_idx_l < OFDMA_end ; OFDMA_idx_l++)
         {
           fft_data_temp1 = &fft_data[num_antenna][OFDMA_idx_l * N_DL_RB * N_SC_PER_RB];
           fft_data_temp2 = &fft_data[num_antenna][OFDMA_idx_l * N_DL_RB * N_SC_PER_RB + ((N_DL_RB * N_SC_PER_RB) >> 1)];

           //CACHE_invL2(fftc_input_data_ptr1, 1201*4, CACHE_WAIT);

           fftc_input_data_ptr1_temp1 = fftc_input_data_ptr1;
           fftc_input_data_ptr1_temp2 = (fftc_input_data_ptr1 + ((N_DL_RB * N_SC_PER_RB) >> 1) + 1);
           /*拷贝数据*/
             for (i = 0; i < ((N_DL_RB * N_SC_PER_RB) >> 1); i++)
             {
               fft_data_temp1[i] = _shl2(*fftc_input_data_ptr1_temp1++,shift_factor);
               fft_data_temp2[i] = _shl2(*fftc_input_data_ptr1_temp2++,shift_factor);
               //vxx_fft_lay_buffer.fft_data[1][OFDMA_idx_l * N_DL_RB * N_SC_PER_RB + i] = *fftc_input_data_ptr2++;
             }
             //fftc_input_data_ptr1++;
             //fftc_input_data_ptr2++;
             //for (i = ((N_DL_RB * N_SC_PER_RB) >> 1); i < (N_DL_RB * N_SC_PER_RB); i++)
             //{
               //fft_data[num_antenna][OFDMA_idx_l * N_DL_RB * N_SC_PER_RB + i] = *fftc_input_data_ptr1++;
               //vxx_fft_lay_buffer.fft_data[1][OFDMA_idx_l * N_DL_RB * N_SC_PER_RB + i] = *fftc_input_data_ptr2++;
             //}
             /*423:2048-424(start)-1(DC)-1200;4:agc因子偏移;424：下个符号偏移424*/
             fftc_input_data_ptr1 = fftc_input_data_ptr1 + N_DL_RB * N_SC_PER_RB + 1 + 423 + 424;
             //fftc_input_data_ptr2 = fftc_input_data_ptr2 + 423 + 424 + 4;
         }
     }
     else
     {
         shift_factor = -shift_factor;
         for (OFDMA_idx_l = OFDMA_start; OFDMA_idx_l < OFDMA_end ; OFDMA_idx_l++)
         {
           fft_data_temp1 = &fft_data[num_antenna][OFDMA_idx_l * N_DL_RB * N_SC_PER_RB];
           fft_data_temp2 = &fft_data[num_antenna][OFDMA_idx_l * N_DL_RB * N_SC_PER_RB + ((N_DL_RB * N_SC_PER_RB) >> 1)];

           //CACHE_invL2(fftc_input_data_ptr1, 1201*4, CACHE_WAIT);

           fftc_input_data_ptr1_temp1 = fftc_input_data_ptr1;
           fftc_input_data_ptr1_temp2 = (fftc_input_data_ptr1 + ((N_DL_RB * N_SC_PER_RB) >> 1) + 1);
           /*拷贝数据*/
             for (i = 0; i < ((N_DL_RB * N_SC_PER_RB) >> 1); i++)
             {
               fft_data_temp1[i] = _shr2(*fftc_input_data_ptr1_temp1++,shift_factor);
               fft_data_temp2[i] = _shr2(*fftc_input_data_ptr1_temp2++,shift_factor);
               //vxx_fft_lay_buffer.fft_data[1][OFDMA_idx_l * N_DL_RB * N_SC_PER_RB + i] = *fftc_input_data_ptr2++;
             }
             //fftc_input_data_ptr1++;
             //fftc_input_data_ptr2++;
             //for (i = ((N_DL_RB * N_SC_PER_RB) >> 1); i < (N_DL_RB * N_SC_PER_RB); i++)
             //{
               //fft_data[num_antenna][OFDMA_idx_l * N_DL_RB * N_SC_PER_RB + i] = *fftc_input_data_ptr1++;
               //vxx_fft_lay_buffer.fft_data[1][OFDMA_idx_l * N_DL_RB * N_SC_PER_RB + i] = *fftc_input_data_ptr2++;
             //}
             /*423:2048-424(start)-1(DC)-1200;4:agc因子偏移;424：下个符号偏移424*/
             fftc_input_data_ptr1 = fftc_input_data_ptr1 + N_DL_RB * N_SC_PER_RB + 1 + 423 + 424;
             //fftc_input_data_ptr2 = fftc_input_data_ptr2 + 423 + 424 + 4;
         }
     }

     
}

void  fft_input_data_copy(
                unsigned char start_symbol,
                unsigned char end_symbol,
                 double* restrict fftc_input_data,
                     double* restrict input_data_ptr1,
                     int port
                   )
{
    unsigned int i,k;
    
    //cp_len1 = 160;
    if((0 == start_symbol) || (7 == start_symbol))
    {
        /******************************拷贝符号0*****************************************/
        for(k = 0;k < (FFT_SIZE/4);k++)
        {
            fftc_input_data[2*k] = input_data_ptr1[160/2 + 2*k];
            fftc_input_data[2*k+1] = input_data_ptr1[160/2 + 2*k+1];
        //fftc_input_data[k + N_SYM_PER_SLOT * FFT_SIZE] = input_data_ptr2[cp_len + k];
        }
        /******************************拷贝其他符号*****************************************/
        //cp_len2 = 144;
        for (i = 1; i < end_symbol - start_symbol; i++)
        {
        	if(port==0)
        	{
				for(k = 0;k < (FFT_SIZE/4);k++)
				{

					fftc_input_data[i * (FFT_SIZE/2) + 2*k] = input_data_ptr1[(i - 1) * ((FFT_SIZE + 144)/2) + (FFT_SIZE + 160 + 144)/2 + 2*k +(FFT_SIZE + 160)/2+(i - 1) * ((FFT_SIZE + 144)/2)];
					fftc_input_data[i * (FFT_SIZE/2) + 2*k+1] = input_data_ptr1[(i - 1) * ((FFT_SIZE + 144)/2) + (FFT_SIZE + 160 + 144)/2 + 2*k+1 +(FFT_SIZE + 160)/2+(i - 1) * ((FFT_SIZE + 144)/2)];
            	}
        	}
            else
            {
            	for(k = 0;k < (FFT_SIZE/4);k++)
            	{
            		fftc_input_data[i * (FFT_SIZE/2) + 2*k] = input_data_ptr1[(i - 1) * ((FFT_SIZE + 144)/2) + (FFT_SIZE + 160 + 144)/2 + 2*k +(FFT_SIZE + 144)/2+(i - 1) * ((FFT_SIZE + 144)/2)];
            		fftc_input_data[i * (FFT_SIZE/2) + 2*k+1] = input_data_ptr1[(i - 1) * ((FFT_SIZE + 144)/2) + (FFT_SIZE + 160 + 144)/2 + 2*k+1 +(FFT_SIZE + 144)/2+(i - 1) * ((FFT_SIZE + 144)/2)];
            	}
            }
        }
    }
    else
    {
    	for (i = 0; i < end_symbol - start_symbol; i++)
    	{
    	    for(k = 0;k < (FFT_SIZE/4);k++)
    	    {
    	        fftc_input_data[i * (FFT_SIZE/2) + 2*k] = input_data_ptr1[i * ((FFT_SIZE + 144)/2) + 144/2 + 2*k+i*(FFT_SIZE + 144)/2];
    	        fftc_input_data[i * (FFT_SIZE/2) + 2*k+1] = input_data_ptr1[i * ((FFT_SIZE + 144)/2) + 144/2 + 2*k+1+i*(FFT_SIZE + 144)/2];
    	    }
        }
    }
}

void  fft_input_data_copy_int(
                unsigned char start_symbol,
                unsigned char end_symbol,
                unsigned int* restrict fftc_input_data,
                    unsigned int* restrict input_data_ptr1,
                    int port
                   )
{
    unsigned int i,k;

    //cp_len1 = 160;
    if((0 == start_symbol) || (7 == start_symbol))
    {
        /******************************拷贝符号0*****************************************/
        for(k = 0;k < FFT_SIZE;k++)
        {
            fftc_input_data[k] = input_data_ptr1[160 + k];
        //fftc_input_data[k + N_SYM_PER_SLOT * FFT_SIZE] = input_data_ptr2[cp_len + k];
        }
        /******************************拷贝其他符号*****************************************/
        //cp_len2 = 144;
        for (i = 1; i < end_symbol - start_symbol; i++)
        {
        	if(port==0)
        	{
				for(k = 0;k < FFT_SIZE;k++)
				{
					fftc_input_data[i * FFT_SIZE + k] = input_data_ptr1[(i - 1) * (FFT_SIZE + 144) + FFT_SIZE + 160 + 144 + k + FFT_SIZE + 160+(i-1)*(FFT_SIZE + 144)];
					//fftc_input_data[i * FFT_SIZE + k + N_SYM_PER_SLOT * FFT_SIZE] = input_data_ptr2[(i - 1) * (FFT_SIZE + cp_len) + FFT_SIZE + 160 + cp_len + k];
				}
        	}
        	else
        	{
				for(k = 0;k < FFT_SIZE;k++)
				{
					fftc_input_data[i * FFT_SIZE + k] = input_data_ptr1[(i - 1) * (FFT_SIZE + 144) + FFT_SIZE + 160 + 144 + k + FFT_SIZE + 144+(i-1)*(FFT_SIZE + 144)];
					//fftc_input_data[i * FFT_SIZE + k + N_SYM_PER_SLOT * FFT_SIZE] = input_data_ptr2[(i - 1) * (FFT_SIZE + cp_len) + FFT_SIZE + 160 + cp_len + k];
				}
        	}
        }
    }
    else
    {
    	for (i = 0; i < end_symbol - start_symbol; i++)
    	{
    	     for(k = 0;k < FFT_SIZE;k++)
    	     {
    	          fftc_input_data[i * FFT_SIZE + k] = input_data_ptr1[i * (FFT_SIZE + 144) + 144 + k +i*(FFT_SIZE+144)];
    	          //fftc_input_data[i * FFT_SIZE + k + N_SYM_PER_SLOT * FFT_SIZE] = input_data_ptr2[(i - 1) * (FFT_SIZE + cp_len) + FFT_SIZE + 160 + cp_len + k];
    	     }
        }
    }
}
