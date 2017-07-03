/***********************************************/
/*BitDescrambling                              */
/*π¶ƒ‹£∫ÕÍ≥…bitΩ‚»≈π¶ƒ‹                        */
/***********************************************/
#include "pl_comm.h"

int cycle_start1 = 0;
int cycle_end1 = 0;
int cycle10 = 0;
void BitDescrambling(unsigned char *restrict x1_sequence_table,
                     signed char  *restrict input_pointer,
                     signed char  *restrict output_pointer,
                     unsigned int  x2_init_seq,
                     unsigned int  bit_descrambler_length)
{

    signed int *restrict inp32=(signed int*restrict)input_pointer;
    signed int *restrict out32=(signed int*restrict)output_pointer;
    unsigned int *restrict x1_word = (unsigned int * restrict)x1_sequence_table;

    signed int  c_nibble,c_nibble2 ;
    signed int  i;
    signed int tmp1,tmp11,i0,i1,r0,r1;
    //signed int tmp2,tmp22;
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


    cycle_start1 = TSCL;
    x2_seed = x2_init_seq;
    num_input_bytes = bit_descrambler_length;
        
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
      

    num_of_words = (unsigned short)(num_input_bytes >> 5);           //Provide the number of bytes multiple of 32 
    num_of_residual_bytes = (unsigned char)(num_input_bytes & 0x1F); //Provide the number of residual bytes 
    
    if (num_of_residual_bytes != 0)              //Do one more iteration if there are residual bits
        num_of_words += 1;

    // make x2_seed have 32 effictive bits
    x2_seed = (x2_seed |(((x2_seed & 0x01)^(x2_seed >> 1 & 0x01)^(x2_seed >> 2 & 0x01)^(x2_seed >> 3 & 0x01))<< 31));
    // reverse the bit order.
    x2_seed = _bitr(x2_seed);
    cycle_end1 = TSCL;
    cycle10 = cycle_end1 - cycle_start1;

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
            x2_seed_r1  = _extu(x2_seed,1,29);/*◊Û“∆1£¨”““∆29*/
            x2_seed_r2  = _extu(x2_seed,1,30);/*◊Û“∆1£¨”““∆30*/
            x2_seed_r3  = _extu(x2_seed,1,31);/*◊Û“∆1£¨”““∆31*/
          
            
            /* b31 b32 b33 b34     ...     b59 b60 b61 b62      x2_seed.*/
            x2_seed = x2_seed ^ x2_seed_r1; 
            tmp3= (signed int)(x2_seed_r2 ^ x2_seed_r3);
            x2_seed = x2_seed ^tmp3; 
           
            c_word = ((x2_seed) ^ (*x1_word++));              /* b32 b33 b34 b35     ...     b60 b61 b62 b63      x2_seed. For next iteration*/
           
            x2_seed = ((x2_seed << 1)|(((x2_seed << 4)^(x2_seed << 1)^(x2_seed << 2)^(x2_seed << 3))>> 31));
            
            /*Descrambled Output = C(i)^Input(i) in byte unpacked format*/

            for(k = 1052; k >= 1024; k-=32)
            {
              c_nibble  = (signed int) _extu(c_word,0,28);/*◊Û“∆0£¨”““∆28 »°bit31~bit28*/
              c_nibble2 = (signed int) _extu(c_word,4,28);/*◊Û“∆4£¨”““∆24 »°bit27~bit24*/
              
              tmp1 = Negate_LLR_LUT[c_nibble * 2];
              //tmp2 = Negate_LLR_LUT[c_nibble * 2 + 1];
              tmp11 = Negate_LLR_LUT[c_nibble2 * 2];
              //tmp22 = Negate_LLR_LUT[c_nibble2 * 2 + 1];
              
              i0 = (*inp32++);
              i1 = (*inp32++);
              
              //r0 = ((i0 ^(tmp1))  + tmp2);
              r0 = i0 ^(tmp1);
              //r1 = ((i1 ^(tmp11)) + tmp22);
              r1 = i1 ^(tmp11);

              (*out32++) = r0;
              (*out32++) = r1;

                  
              c_nibble  = (signed int) _extu(c_word,8,28);/*◊Û“∆8£¨”““∆28 »°bit23~bit20*/
              c_nibble2 = (signed int) _extu(c_word,12,28);/*◊Û“∆12£¨”““∆28 »°bit19~bit16*/
              
              
              tmp1 = Negate_LLR_LUT[c_nibble * 2];
              //tmp2 = Negate_LLR_LUT[c_nibble * 2 + 1];
              tmp11 = Negate_LLR_LUT[c_nibble2 * 2];
              //tmp22 = Negate_LLR_LUT[c_nibble2 * 2 + 1];
              

              i0 = (*inp32++);
              i1 = (*inp32++);
              
              //r0 = ((i0 ^(tmp1))  + tmp2);
              //r1 = ((i1 ^(tmp11)) + tmp22);
              r0 = i0 ^(tmp1);
              r1 = i1 ^(tmp11);

              (*out32++) = r0;
              (*out32++) = r1;
                  
              c_nibble  = (signed int) _extu(c_word,16,28);/*◊Û“∆16£¨”““∆28 »°bit15~bit12*/
              c_nibble2 = (signed int) _extu(c_word,20,28);/*◊Û“∆20£¨”““∆28 »°bit11~bit8*/

              
              tmp1 = Negate_LLR_LUT[c_nibble * 2];
              //tmp2 = Negate_LLR_LUT[c_nibble * 2 + 1];
              tmp11 = Negate_LLR_LUT[c_nibble2 * 2];
              //tmp22 = Negate_LLR_LUT[c_nibble2 * 2 + 1];
              

              i0 = (*inp32++);
              i1 = (*inp32++);
              
              //r0 = ((i0 ^(tmp1))  + tmp2);
              //r1 = ((i1 ^(tmp11)) + tmp22);
              r0 = i0 ^(tmp1);
              r1 = i1 ^(tmp11);

              (*out32++) = r0;
              (*out32++) = r1;
                  
              c_nibble  = (signed int) _extu(c_word,24,28);/*◊Û“∆24£¨”““∆28 »°bit7~bit4*/
              c_nibble2 = (signed int) _extu(c_word,28,28);/*◊Û“∆28£¨”““∆28 »°bit3~bit0*/
              
              tmp1 = Negate_LLR_LUT[c_nibble * 2];
              //tmp2 = Negate_LLR_LUT[c_nibble * 2 + 1];
              tmp11 = Negate_LLR_LUT[c_nibble2 * 2];
              //tmp22 = Negate_LLR_LUT[c_nibble2 * 2 + 1];
              

              i0 = (*inp32++);
              i1 = (*inp32++);
              
              //r0 = ((i0 ^(tmp1))  + tmp2);
              //r1 = ((i1 ^(tmp11)) + tmp22);
              r0 = i0 ^(tmp1);
              r1 = i1 ^(tmp11);

              (*out32++) = r0;
              (*out32++) = r1;
            }
        }
    }
     
}

void Descrambling_data_pbch_gen(unsigned char *restrict x1_sequence_table,
                     unsigned int  x2_init_seq,
                     unsigned int  bit_descrambler_length,
                     unsigned int *descrambling_bit_ptr)
{

    unsigned int *restrict x1_word = (unsigned int * restrict)x1_sequence_table;

    //signed int  c_nibble,c_nibble2 ;
    signed int  i;
    //signed int tmp1,tmp11,i0,i1,r0,r1;
    //signed int tmp2,tmp22;
    signed int tmp3;
    unsigned int x2_seed;
    unsigned int num_input_bytes;
    unsigned int x2_seed_r1;
    unsigned int x2_seed_r2;
    unsigned int x2_seed_r3;
    unsigned int x2_seed_r4;
    unsigned int c_word;
    //unsigned int  k;
    unsigned short num_of_words;
    unsigned char  num_of_residual_bytes;



    x2_seed = x2_init_seq;
    num_input_bytes = bit_descrambler_length;

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


    num_of_words = (unsigned short)(num_input_bytes >> 5);           //Provide the number of bytes multiple of 32
    num_of_residual_bytes = (unsigned char)(num_input_bytes & 0x1F); //Provide the number of residual bytes

    if (num_of_residual_bytes != 0)              //Do one more iteration if there are residual bits
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
            x2_seed_r1  = _extu(x2_seed,1,29);/*◊Û“∆1£¨”““∆29*/
            x2_seed_r2  = _extu(x2_seed,1,30);/*◊Û“∆1£¨”““∆30*/
            x2_seed_r3  = _extu(x2_seed,1,31);/*◊Û“∆1£¨”““∆31*/


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
void Descrambling_data_pdsch_gen(unsigned char *restrict x1_sequence_table,
                     unsigned int  x2_init_seq,
                     unsigned int  bit_descrambler_length,
                     signed char *restrict descrambling_bit_ptr)
{

    unsigned int *restrict x1_word = (unsigned int * restrict)x1_sequence_table;

    //signed int  c_nibble,c_nibble2 ;
    signed int  i,j;
    //signed int tmp1,tmp11,i0,i1,r0,r1;
    //signed int tmp2,tmp22;
    signed int tmp3;
    unsigned int x2_seed;
    unsigned int num_input_bytes;
    unsigned int x2_seed_r1;
    unsigned int x2_seed_r2;
    unsigned int x2_seed_r3;
    unsigned int x2_seed_r4;
    unsigned int c_word;
    //unsigned int  k;
    unsigned short num_of_words;
    unsigned char  num_of_residual_bytes;
    unsigned char lut_table[2] = {0,-1};



    x2_seed = x2_init_seq;
    num_input_bytes = bit_descrambler_length;

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


    num_of_words = (unsigned short)(num_input_bytes >> 5);           //Provide the number of bytes multiple of 32
    num_of_residual_bytes = (unsigned char)(num_input_bytes & 0x1F); //Provide the number of residual bytes

    if (num_of_residual_bytes != 0)              //Do one more iteration if there are residual bits
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
            x2_seed_r1  = _extu(x2_seed,1,29);/*◊Û“∆1£¨”““∆29*/
            x2_seed_r2  = _extu(x2_seed,1,30);/*◊Û“∆1£¨”““∆30*/
            x2_seed_r3  = _extu(x2_seed,1,31);/*◊Û“∆1£¨”““∆31*/


            /* b31 b32 b33 b34     ...     b59 b60 b61 b62      x2_seed.*/
            x2_seed = x2_seed ^ x2_seed_r1;
            tmp3= (signed int)(x2_seed_r2 ^ x2_seed_r3);
            x2_seed = x2_seed ^tmp3;

            c_word = ((x2_seed) ^ (*x1_word++));              /* b32 b33 b34 b35     ...     b60 b61 b62 b63      x2_seed. For next iteration*/

            x2_seed = ((x2_seed << 1)|(((x2_seed << 4)^(x2_seed << 1)^(x2_seed << 2)^(x2_seed << 3))>> 31));

            for(j = 0;j<32;j++)
            {
                descrambling_bit_ptr[i*32+j] = lut_table[(c_word >> (31-j)) & 0x1];
            }
        }
    }

}


void BitDescrambling_pbch(signed char  *restrict input_pointer,
                     signed char  *restrict output_pointer,
                     unsigned int *restrict descrambling_bit_ptr,
                     unsigned int  bit_descrambler_length
                     )
{

    signed int * inp32=(signed int*restrict)input_pointer;
    signed int * out32=(signed int*restrict)output_pointer;

    unsigned int  c_nibble,c_nibble2 ;
    signed int  i;
    signed int tmp1,tmp11,i0,i1,r0,r1;
    //signed int tmp2,tmp22;
    //signed int tmp3;
    unsigned int num_input_bytes;
    unsigned int c_word;
    unsigned int  k;    
    unsigned short num_of_words;      
    unsigned char  num_of_residual_bytes;
    
    num_input_bytes = bit_descrambler_length;
    num_of_words = (unsigned short)(num_input_bytes >> 5); 
    num_of_residual_bytes = (unsigned char)(num_input_bytes & 0x1F); //Provide the number of residual bytes

    if (num_of_residual_bytes != 0)              //Do one more iteration if there are residual bits
       num_of_words += 1;
      
    if (num_of_words != 0)                      
    {    
        for (i = 0;i < num_of_words;i++)            
        {
            
            c_word = descrambling_bit_ptr[i];
            /*Descrambled Output = C(i)^Input(i) in byte unpacked format*/

            for(k = 1052; k >= 1024; k-=32)
            {
              c_nibble  =  _extu(c_word,0,28);/*◊Û“∆0£¨”““∆28 »°bit31~bit28*/
              c_nibble2 =  _extu(c_word,4,28);/*◊Û“∆4£¨”““∆24 »°bit27~bit24*/
              
              tmp1 = Negate_LLR_LUT[c_nibble * 2];
              //tmp2 = Negate_LLR_LUT[c_nibble * 2 + 1];
              tmp11 = Negate_LLR_LUT[c_nibble2 * 2];
              //tmp22 = Negate_LLR_LUT[c_nibble2 * 2 + 1];
              
              i0 = (*inp32++);
              i1 = (*inp32++);
              
              //r0 = ((i0 ^(tmp1))  + tmp2);
              //r1 = ((i1 ^(tmp11)) + tmp22);
              r0 = i0 ^(tmp1);
              r1 = i1 ^(tmp11);

              (*out32++) = r0;
              (*out32++) = r1;

                  
              c_nibble  =  _extu(c_word,8,28);/*◊Û“∆8£¨”““∆28 »°bit23~bit20*/
              c_nibble2 =  _extu(c_word,12,28);/*◊Û“∆12£¨”““∆28 »°bit19~bit16*/
              
              
              tmp1 = Negate_LLR_LUT[c_nibble * 2];
              //tmp2 = Negate_LLR_LUT[c_nibble * 2 + 1];
              tmp11 = Negate_LLR_LUT[c_nibble2 * 2];
              //tmp22 = Negate_LLR_LUT[c_nibble2 * 2 + 1];
              

              i0 = (*inp32++);
              i1 = (*inp32++);
              
              //r0 = ((i0 ^(tmp1))  + tmp2);
              //r1 = ((i1 ^(tmp11)) + tmp22);
              r0 = i0 ^(tmp1);
              r1 = i1 ^(tmp11);

              (*out32++) = r0;
              (*out32++) = r1;
                  
              c_nibble  =  _extu(c_word,16,28);/*◊Û“∆16£¨”““∆28 »°bit15~bit12*/
              c_nibble2 =  _extu(c_word,20,28);/*◊Û“∆20£¨”““∆28 »°bit11~bit8*/

              
              tmp1 = Negate_LLR_LUT[c_nibble * 2];
              //tmp2 = Negate_LLR_LUT[c_nibble * 2 + 1];
              tmp11 = Negate_LLR_LUT[c_nibble2 * 2];
              //tmp22 = Negate_LLR_LUT[c_nibble2 * 2 + 1];
              

              i0 = (*inp32++);
              i1 = (*inp32++);
              
              //r0 = ((i0 ^(tmp1))  + tmp2);
              //r1 = ((i1 ^(tmp11)) + tmp22);
              r0 = i0 ^(tmp1);
              r1 = i1 ^(tmp11);


              (*out32++) = r0;
              (*out32++) = r1;
                  
              c_nibble  =  _extu(c_word,24,28);/*◊Û“∆24£¨”““∆28 »°bit7~bit4*/
              c_nibble2 =  _extu(c_word,28,28);/*◊Û“∆28£¨”““∆28 »°bit3~bit0*/
              
              tmp1 = Negate_LLR_LUT[c_nibble * 2];
              //tmp2 = Negate_LLR_LUT[c_nibble * 2 + 1];
              tmp11 = Negate_LLR_LUT[c_nibble2 * 2];
              //tmp22 = Negate_LLR_LUT[c_nibble2 * 2 + 1];
              

              i0 = (*inp32++);
              i1 = (*inp32++);
              
              //r0 = ((i0 ^(tmp1))  + tmp2);
              //r1 = ((i1 ^(tmp11)) + tmp22);
              r0 = i0 ^(tmp1);
              r1 = i1 ^(tmp11);

              (*out32++) = r0;
              (*out32++) = r1;
            }
        }
    }
     
}

