/***********************************************/
/*BitDescrambling                              */
/*功能：完成bit加扰功能                        */
/***********************************************/
#include "pl_comm.h"




void scrambling_data_gen(unsigned char *restrict x1_sequence_table,
                     unsigned int  x2_init_seq,
                     unsigned int  bit_descrambler_length,
                     unsigned int *descrambling_bit_ptr)
{

    unsigned int *restrict x1_word = (unsigned int * restrict)x1_sequence_table;

    signed int  c_nibble,c_nibble2 ;
    signed int  i;
    signed int tmp1,tmp11,i0,i1,r0,r1;
    signed int tmp2,tmp22;
    signed int tmp3;
    unsigned int x2_seed;
    unsigned int num_input_bites;
    unsigned int x2_seed_r1;
    unsigned int x2_seed_r2;
    unsigned int x2_seed_r3;
    unsigned int x2_seed_r4;
    unsigned int c_word;
    unsigned int  k;
    unsigned short num_of_words;
    unsigned char  num_of_residual_bites;



    x2_seed = x2_init_seq;
    num_input_bites = bit_descrambler_length;

  /* Generate X2 Sequence for Values between X2(31) to X2(1599) which are intermediate values, required to generate
  * values thereafter as per TS36.211 Ver 8.3.0 */

    for(i = 0;i < 50;i++)
    {
        x2_seed_r1 = x2_seed >> 1;
        x2_seed_r2 = x2_seed >> 2;
        x2_seed_r3 = x2_seed >> 3;
        x2_seed = x2_seed ^ x2_seed_r1 ^ x2_seed_r2 ^ x2_seed_r3;
        x2_seed_r1 = x2_seed << 29;
        x2_seed_r2 = x2_seed << 30;
        x2_seed_r3 = x2_seed << 31;
        x2_seed_r1 = x2_seed_r1 >> 1;
        x2_seed_r2 = x2_seed_r2 >> 1;
        x2_seed_r3 = x2_seed_r3 >> 1;
        x2_seed = x2_seed ^ x2_seed_r1 ^ x2_seed_r2 ^ x2_seed_r3;
    }
    x2_seed_r4 = x2_seed;
    x2_seed_r1 = x2_seed >> 1;
    x2_seed_r2 = x2_seed >> 2;
    x2_seed_r3 = x2_seed >> 3;
    x2_seed_r4 = x2_seed_r4 ^ x2_seed_r1 ^ x2_seed_r2 ^ x2_seed_r3;
    x2_seed_r4 = x2_seed_r4 << 13;
    x2_seed    = x2_seed >> 19;
    x2_seed_r4 = x2_seed_r4 >>1;
    x2_seed    = x2_seed | x2_seed_r4;


    num_of_words = (unsigned short)(num_input_bites >> 5);           //Provide the number of bytes multiple of 32
    num_of_residual_bites = (unsigned char)(num_input_bites & 0x1F); //Provide the number of residual bytes

    if (num_of_residual_bites != 0)              //Do one more iteration if there are residual bits
        num_of_words += 1;

    // make x2_seed have 32 effictive bits
    x2_seed = (x2_seed |(((x2_seed & 0x01)^(x2_seed >> 1 & 0x01)^(x2_seed >> 2 & 0x01)^(x2_seed >> 3 & 0x01))<< 31));
    // reverse the bit order.
    x2_seed = _bitr(x2_seed);

    if (num_of_words != 0)
    {
        for (i = 0;i < num_of_words;i++)
        {
           /*Calculate X2 and C Sequence in Byte Packed Format
            X2(n+31)=[X2(n+3)^X2(n+2)^X2(n+1)^X2(n)^]mod2
            C Sequence: C(n) = X1(n+1600)^X2(n+1600)*/


            /* b0  b1  b2  b3      ...     b28 b29 b30 b31     x2_seed
               b1  b2  b3  b4      ...     b29 b30 b31  0      x2_seed_r1
               b2  b3  b4  b5      ...     b30 b31  0   0      x2_seed_r2
               b3  b4  b5  b6      ...     b31  0   0   0      x2_seed_r3*/
            x2_seed_r1 = x2_seed << 1;
            x2_seed_r2 = x2_seed << 2;
            x2_seed_r3 = x2_seed << 3;

            /* b31 b32 b33 b34     ...     b59 t1  t2  t3      x2_seed. For example b31 = b0^b1^b2^b3*/
            /*x2_seed = x2_seed ^ x2_seed_r1 ^ x2_seed_r2 ^ x2_seed_r3;*/
            x2_seed = x2_seed ^ x2_seed_r1;
            tmp3= (signed int) (x2_seed_r2 ^ x2_seed_r3);
            x2_seed = x2_seed ^tmp3;

            /*  b31 b32 b33 b34 b35     ...     t1  t2  t3      x2_seed*/
            /*  0   0   0          ...      0  b32 b33 b34      x2_seed_r1
                0   0   0          ...      0   0  b32 b33      x2_seed_r2
                0   0   0          ...      0   0   0  b32      x2_seed_r3*/
            x2_seed_r1  = _extu(x2_seed,1,29);/*左移1，右移29*/
            x2_seed_r2  = _extu(x2_seed,1,30);/*左移1，右移30*/
            x2_seed_r3  = _extu(x2_seed,1,31);/*左移1，右移31*/


            /* b31 b32 b33 b34     ...     b59 b60 b61 b62      x2_seed.*/
            x2_seed = x2_seed ^ x2_seed_r1;
            tmp3= (signed int)(x2_seed_r2 ^ x2_seed_r3);
            x2_seed = x2_seed ^tmp3;

            c_word = ((x2_seed) ^ (*x1_word++));              /* b32 b33 b34 b35     ...     b60 b61 b62 b63      x2_seed. For next iteration*/

            x2_seed = ((x2_seed << 1)|(((x2_seed << 4)^(x2_seed << 1)^(x2_seed << 2)^(x2_seed << 3))>> 31));

            descrambling_bit_ptr[i] = c_word;
        }
    }

}


void Bitscrambling(signed int  *restrict input_pointer,
                     signed int  *restrict output_pointer,
                     unsigned char *descrambling_bit_ptr,
                     unsigned int  bit_descrambler_length
                     )
{

    signed int * inp32=input_pointer;
    signed int * out32=output_pointer;

    unsigned int  c_nibble,c_nibble2 ;
    signed int  i;
    signed int tmp1,tmp11,i0,i1,r0,r1;
    signed int tmp2,tmp22;
    signed int tmp3;    
    unsigned int x2_seed;
    unsigned int num_input_bytes;
    unsigned int x2_seed_r1;
    unsigned int x2_seed_r2;
    unsigned int x2_seed_r3;
    unsigned int x2_seed_r4;
    unsigned int c_word;
    unsigned int  k;    
    unsigned short num_of_words;      
    unsigned char  num_of_residual_bytes;
    
    num_input_bytes = bit_descrambler_length;
    //必须是4个字的倍数，因为一次运算4个字，即使最后不足4个字，超出部分由于有空余buffer，不怕溢出
    num_of_words = (unsigned short)(num_input_bytes >> 4);
      
    if (num_of_words != 0)                      
    {    
        for (i = 0;i < num_of_words;i++)            
        {
            
            c_word = descrambling_bit_ptr[i];
            /*Descrambled Output = C(i)^Input(i) in byte unpacked format*/

            for(k = 0; k < 2; k++)
            {
              c_nibble  =  _extu(c_word,(24 + k * 4),30);/*左移0，右移30 取bit31~bit30*/
              c_nibble2 =  _extu(c_word,(26 + k * 4),30);/*左移2，右移30 取bit29~bit28*/
              
              tmp1 = Negate_LLR_LUT[c_nibble];
              tmp2 = Negate_LLR_LUT[c_nibble2];
              
              i0 = (*inp32++);
              i1 = (*inp32++);
              
              r0 = i0 ^ tmp1;
              r1 = i1 ^ tmp2;

              (*out32++) = r0;
              (*out32++) = r1;

                  
              
            }
        }
    }
     
}

void f_scrambling(unsigned int *restrict c,
                  unsigned int *restrict input_data_ptr,
                  unsigned int *restrict output_data_ptr,
                  unsigned int length)
{
    unsigned int i = 0;
    unsigned int temp1,temp2;
    for(i = 0;i <(length + 31)>>5;i++)
    {
      temp1 = input_data_ptr[i];
      temp2 = c[i];
      output_data_ptr[i] = temp1^temp2;
    }
}
