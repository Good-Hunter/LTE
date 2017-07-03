/***********************************************/
/*cc_conv_decoder.c                                */
/*¹¦ÄÜ£º¾í»ýÒëÂë                     */
/***********************************************/
#include "system_init.h"     
char out00_11[32][3] = {
0,0,0,
1,0,0,
0,0,1,
1,0,1,
1,1,0,
0,1,0,
1,1,1,
0,1,1,
1,1,1,
0,1,1,
1,1,0,
0,1,0,
0,0,1,
1,0,1,
0,0,0,
1,0,0,
0,1,1,
1,1,1,
0,1,0,
1,1,0,
1,0,1,
0,0,1,
1,0,0,
0,0,0,
1,0,0,
0,0,0,
1,0,1,
0,0,1,
0,1,0,
1,1,0,
0,1,1,
1,1,1};

char out01_10[32][3] = {
1,1,1,
0,1,1,
1,1,0,
0,1,0,
0,0,1,
1,0,1,
0,0,0,
1,0,0,
0,0,0,
1,0,0,
0,0,1,
1,0,1,
1,1,0,
0,1,0,
1,1,1,
0,1,1,
1,0,0,
0,0,0,
1,0,1,
0,0,1,
0,1,0,
1,1,0,
0,1,1,
1,1,1,
0,1,1,
1,1,1,
0,1,0,
1,1,0,
1,0,1,
0,0,1,
1,0,0,
0,0,0};


unsigned int cc_conv_decoder(int K,
                     int E,
                     char* restrict s,
                     char* restrict y1,
                     char* restrict y2,
                     short *restrict state,
                     char *restrict branch,
                     unsigned int *restrict decoded_bits,
                     char crc_length)
{
    short branchmetric[4];
    char decoded_bits_temp[MAX_COV_TBS];
    unsigned int temp1,temp2;
    short temp,i,j,k;
    char istate_index1,istate_index2,index,tail_state,istate;
    char vector[64];
    unsigned int crc_out = 0;


    memset(state,0,MAX_COV_TBS * 64 * 2);
    memset(decoded_bits,0,4 * ((MAX_COV_TBS + 31)>> 5));
    memset(branch,0,64 * (K + 1));

    for(i = 0;i < 2;i++)
    {
        for(j = 0;j < K;j++)
        {

            for(k = 0;k < 32;k++)
            {
                branchmetric[0] = state[j * 64 + 2 * k];
                branchmetric[1] = state[j * 64 + 2 * k + 1];
                
                branchmetric[0] += out00_11[k][0] * s[j];
                branchmetric[0] += out00_11[k][1] * y1[j];
                branchmetric[0] += out00_11[k][2] * y2[j];
                branchmetric[1] += out01_10[k][0] * s[j];
                branchmetric[1] += out01_10[k][1] * y1[j];
                branchmetric[1] += out01_10[k][2] * y2[j];
                
                index = 0;
                if(branchmetric[1] >= branchmetric[0])
                index = 1;
                state[(j + 1)* 64 + k] = branchmetric[index];
                branch[(j + 1)* 64 + k] = 2 * k + index;
            }
            //path[j][0] = 0;
            
            for(k = 32;k < 64;k++)
            {
                branchmetric[0] = state[j * 64 + 2 * k - 64];
                branchmetric[1] = state[j * 64 + 2 * k + 1 - 64];
                
                branchmetric[0] += out01_10[k - 32][0] * s[j];
                branchmetric[0] += out01_10[k - 32][1] * y1[j];
                branchmetric[0] += out01_10[k - 32][2] * y2[j];
                branchmetric[1] += out00_11[k - 32][0] * s[j];
                branchmetric[1] += out00_11[k - 32][1] * y1[j];
                branchmetric[1] += out00_11[k - 32][2] * y2[j];
                
                index = 0;
                if(branchmetric[1] > branchmetric[0])
                index = 1;
                state[(j + 1)* 64 + k] = branchmetric[index];
                branch[(j + 1)* 64 + k] = 2 * (k - 32) + index;
                
            }
            //path[j][1] = 0xffffffff;
        }

        tail_state = 0;
        temp = 0;
        for(j = 0;j < 64;j++)
        {
            vector[j] = j;
            if(temp < state[K * 64 + j])
            {
                temp = state[K * 64 + j];
                tail_state = j;
            }

        }
        istate = tail_state;
        
        
        for(j = K;j > 0;j--)
        {
            istate_index1 = istate >> 5;
            decoded_bits_temp[j - 1] = istate_index1;
            istate = branch[j * 64 + istate];
        }

        if(tail_state == istate)
        {
            break;
        }
        else
        {
            for(j = 0;j < 64;j++)
            {
                state[j] = state[K * 64 + j];
            } 
        }
    }
    if(2 == i)
    {
        
        for(j = 0;j < 64;j++)
        {
            
            for(k = 63;k > j;k--) 
            {
                if(state[K * 64 + k] < state[K * 64 + k - 1])
                {
                    temp1 = state[K * 64 + k - 1];
                    state[K * 64 + k - 1] = state[K * 64 + k];
                    state[K * 64 + k] = temp1;
                    temp2 = vector[k - 1];
                    vector[k - 1] = vector[k];
                    vector[k] = temp2;                    
                }
            }
            
        }
        for(j = 0;j < 64;j++)
        {
            tail_state = vector[63 - j];
            istate = tail_state;
            for(k = K;k > 0;k--)
            {
                istate_index1 = istate >> 5;
                decoded_bits_temp[j - 1] = istate_index1;
                istate = branch[k * 64 + istate];
            }
            if(tail_state == istate)
            {
                break;
            }
        }
        

    }
    for(k = 0;k < K - crc_length;k++)
    {
      istate_index1 = k >> 5;
      istate_index2 = 31 - (k & 31);
      decoded_bits[istate_index1] = (decoded_bits_temp[k] << istate_index2) ^ decoded_bits[istate_index1];

    }

    for(k = 0;k < crc_length;k++)
    {
        istate_index1 = k >> 5;
        istate_index2 = 31 - (k & 31);
        crc_out = (decoded_bits_temp[K - crc_length + k] << istate_index2) ^ crc_out;

    }

    return crc_out;

}


unsigned char f_pcfich_decode(int* restrict input_data_ptr,
                      int* restrict cfi_code_table)
{
  char *temp1_ptr;
  int temp1 = 0;
  char temp2,temp3,temp4,temp5;
  char n = 0,i = 0;
    int sum_temp = 0;
    int min_sum = 0;
    unsigned char cfi_index = 0;

  temp1_ptr = (char *)input_data_ptr;
  for(i = 0;i < 3;i++)
  {
    sum_temp = 0;
    for(n = 0;n < 8;n++)
    {
      temp1 = input_data_ptr[n] ^ cfi_code_table[i * 8 + n];
      temp2 = temp1 & 0xff;
      sum_temp+=temp2;
      temp3 = (temp1 >> 8) & 0xff;
      sum_temp+=temp2;
      temp4 = (temp1 >> 16) & 0xff;
      sum_temp+=temp2;
      temp5 = (temp1 >> 24) & 0xff;
      sum_temp+=temp2;
    }
    if (sum_temp < min_sum)
    {
      min_sum = sum_temp;
      cfi_index = i;
    }
  }
  return cfi_index + 1;
}

