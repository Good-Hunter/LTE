/***********************************************/
/*demodulation.c                               */
/*功能：解调功能                               */
/***********************************************/
 
#include "system_init.h"

unsigned char agc_64qam = 3;
unsigned char agc_16qam = 5;
unsigned char agc_qpsk = 6;

void demodulation_64qam(signed short *restrict detect_data_ptr,//Q(16,3)
                        signed short *restrict llr_data_ptr,//Q(16,1)
                        signed char *restrict demod_data_ptr,//Q(8,6)
                        unsigned char mimo_type,
                        unsigned int data_length
                        )
{
    unsigned int m = 0;
    signed int int_llr_factor4 = 0,int_llr_factor2 = 0;
    signed int temp1 = 0,temp2 = 0,temp3 = 0,temp4 = 0,temp5 = 0,temp6 = 0,tempa = 0,tempb = 0,tempc = 0;
    signed int firtemp = 0;
    signed int *restrict int_detect_ptr = (signed int *)detect_data_ptr;
    signed short *restrict short_demod_ptr = (signed short*)demod_data_ptr;
    double temp = 0;
    if(SINGLE_ANTENNA_PORT == mimo_type)
    {
        for(m = 0;m < data_length;m++)
        {
            int_llr_factor4 = _mpy(llr_data_ptr[m],0xc31);//Q(16,3)
            int_llr_factor2 = _mpy(llr_data_ptr[m],0x618);
            firtemp = int_detect_ptr[m]; //Q(16,3)
            temp = _mpy2(firtemp,0xec40ec40);//b1=-3240*C_Re;b2=-3240*C_Im Q(16,3)
            temp1 = _hi(temp);
            temp2 = _lo(temp);
            temp3 = _abs(temp1) - int_llr_factor4;//b3 = |b1|-125*16*256
            temp4 = _abs(temp2) - int_llr_factor4;//b4 = |b2|-125*16*256
            temp5 = _abs(temp3) - int_llr_factor2;//b5 = |b3|-125*8*256
            temp6 = _abs(temp4) - int_llr_factor2;//b6 = |b4|-125*8*256
            temp1 = _sshl(temp1,9 - agc_64qam);
            temp2 = _sshl(temp2,9 - agc_64qam);
            temp3 = _sshl(temp3,9 - agc_64qam);
            temp4 = _sshl(temp4,9 - agc_64qam);
            temp5 = _sshl(temp5,9 - agc_64qam);
            temp6 = _sshl(temp6,9 - agc_64qam);
            tempa = _packh2(temp1,temp2);
            tempb = _packh2(temp3,temp4);
            tempc = _packh2(temp5,temp6);
            short_demod_ptr[3 * m] = _packh4(0,tempa);//得到8bit软比特
            short_demod_ptr[3 * m + 1] = _packh4(0,tempb);//得到8bit软比特 
            short_demod_ptr[3 * m + 2] = _packh4(0,tempc);//得到8bit软比特
        }
    }
    else
    {
        for(m = 0;m < data_length;m++)
        {
            int_llr_factor4 = _mpy(llr_data_ptr[m],0xc31);//0xc31 Q(16,3)  ->Q(32,4)
            int_llr_factor2 = _mpy(llr_data_ptr[m],0x618);
            firtemp = int_detect_ptr[m]; //Q(16,3)
            temp = _mpy2(firtemp,0xb100b100);//b1=-5056*C_Re;b2=-5056*C_Im  0xec40ec40 Q(16,3) 0xb100b100 Q(16,1)
            temp1 = _hi(temp);
            temp2 = _lo(temp);
            temp3 = _abs(temp1) - int_llr_factor4;//b3 = |b1|-125*16*256
            temp4 = _abs(temp2) - int_llr_factor4;//b4 = |b2|-125*16*256
            temp5 = _abs(temp3) - int_llr_factor2;//b5 = |b3|-125*8*256
            temp6 = _abs(temp4) - int_llr_factor2;//b6 = |b4|-125*8*256
            temp1 = _sshl(temp1,13 - agc_64qam) >> 2;
            temp2 = _sshl(temp2,13 - agc_64qam) >> 2;
            temp3 = _sshl(temp3,13 - agc_64qam) >> 2;
            temp4 = _sshl(temp4,13 - agc_64qam) >> 2;
            temp5 = _sshl(temp5,13 - agc_64qam) >> 2;
            temp6 = _sshl(temp6,13 - agc_64qam) >> 2;
            tempa = _packh2(temp1,temp2);
            tempb = _packh2(temp3,temp4);
            tempc = _packh2(temp5,temp6);
            short_demod_ptr[3 * m] = _packh4(0,tempa);//得到8bit软比特
            short_demod_ptr[3 * m + 1] = _packh4(0,tempb);//得到8bit软比特 
            short_demod_ptr[3 * m + 2] = _packh4(0,tempc);//得到8bit软比特
        }
    }
}

void demodulation_16qam(signed short *restrict detect_data_ptr,//Q(16,3)
                        signed short *restrict llr_data_ptr,//Q(16,1)
                        signed char *restrict demod_data_ptr,//Q(8,6)
                        unsigned char mimo_type,
                        unsigned int data_length
                        )
{
    unsigned int m = 0;
    signed int int_llr_factor = 0;
    signed int temp1 = 0,temp2 = 0,temp3 = 0,temp4 = 0,tempa = 0,tempb = 0;
    signed int firtemp = 0;
    signed int *restrict int_detect_ptr = (signed int *)detect_data_ptr;
    signed int *restrict int_demod_ptr = (signed int*)demod_data_ptr;
    double temp = 0;
    if(SINGLE_ANTENNA_PORT == mimo_type)
    {
        for(m = 0;m < data_length;m++)
        {
        	int_llr_factor = _mpy((llr_data_ptr[m] >> 2),0x199a);//2*D2*LLR llr_data_ptrQ(16,1) 0x199a Q(16,3)
            firtemp = int_detect_ptr[m]; //Q(16,3)
            temp = _mpy2(firtemp,0xd786d786);//b1=-6641*C_Re;b2=-6641*C_Im Q(16,3)
            temp1 = _hi(temp);
            temp2 = _lo(temp);
            temp3 = _abs(temp1) - int_llr_factor;//b3 = |b1|-525*8*256
            temp4 = _abs(temp2) - int_llr_factor;//b4 = |b2|-525*8*256
            temp1 = _sshl(temp1,9 - agc_16qam);
            temp2 = _sshl(temp2,9 - agc_16qam);
            temp3 = _sshl(temp3,9 - agc_16qam);
            temp4 = _sshl(temp4,9 - agc_16qam);
            tempa = _packh2(temp1,temp2);
            tempb = _packh2(temp3,temp4);
            int_demod_ptr[m] = _packh4(tempa,tempb);//得到8bit软比特 
        }
    }
    else
    {
        for(m = 0;m < data_length;m++)
        {
            int_llr_factor = _mpy((llr_data_ptr[m] >> 2),0x199a);//2*D2*LLR Q(16,3)
            firtemp = int_detect_ptr[m]; //Q(16,3)
            temp = _mpy2(firtemp,0xd786d786);//b1=-6641*C_Re;b2=-6641*C_Im
            temp1 = _hi(temp);
            temp2 = _lo(temp);
            temp3 = _abs(temp1) - int_llr_factor;//b3 = |b1|-525*8*256
            temp4 = _abs(temp2) - int_llr_factor;//b4 = |b2|-525*8*256
            temp1 = _sshl(temp1,12 - agc_16qam);
            temp2 = _sshl(temp2,12 - agc_16qam);
            temp3 = _sshl(temp3,12 - agc_16qam);
            temp4 = _sshl(temp4,12 - agc_16qam);
            tempa = _packh2(temp1,temp2);
            tempb = _packh2(temp3,temp4);
            int_demod_ptr[m] = _packh4(tempa,tempb);//得到8bit软比特 
        }
    }
}

void demodulation_qpsk(signed short *restrict detect_data_ptr,//Q(16,3)
                       signed char *restrict demod_data_ptr,//Q(8,6)
                       unsigned char mimo_type,
                       unsigned int data_length
                       )
{
    unsigned int m = 0;
    signed int temp1 = 0,temp2 = 0;
    double temp = 0;
    signed int *restrict int_detec_ptr = (signed int *)detect_data_ptr;
    signed short *restrict short_demod_ptr = (signed short*)demod_data_ptr;
    if(SINGLE_ANTENNA_PORT == mimo_type)
    {
        for(m = 0;m < data_length;m++)
        {
            temp = _mpy2(int_detec_ptr[m],0xa582a582);//Q(16,3)*Q(16,1)=Q(32,4)
            temp1 = _hi(temp);
            temp2 = _lo(temp);
            temp1 = _sshl(temp1,9 - agc_qpsk);
            temp2 = _sshl(temp2,9 - agc_qpsk);
            short_demod_ptr[m] = _packh4(0,_packh2(temp1,temp2));//得到8bit软比特 
        }
    }
    else
    {
        for(m = 0;m < data_length;m++)
        {
            temp = _mpy2(int_detec_ptr[m],0xa582a582);//Q(16,3)*Q(16,1)=Q(32,4)
            temp1 = _hi(temp);
            temp2 = _lo(temp);
            temp1 = _sshl(temp1,12 - agc_qpsk);
            temp2 = _sshl(temp2,12 - agc_qpsk);
            short_demod_ptr[m] = _packh4(0,_packh2(temp1,temp2));//得到8bit软比特 
        }
    }
}

void demodulation(signed short *restrict detect_data_ptr,//Q(16,3)
                  signed short *restrict llr_data_ptr,//Q(16,1)
                  signed char *restrict demod_data_ptr,//Q(8,6)
                  unsigned char mimo_type,
                  unsigned char modulation_type,
                  unsigned int data_length
                  )
{
    if(QAM64 == modulation_type)
    {
         demodulation_64qam(detect_data_ptr,//Q(16,3)
                            llr_data_ptr,//Q(16,1)
                            demod_data_ptr,//Q(8,6)
                            mimo_type,
                            data_length
                           );
    }
    else if(QAM16 == modulation_type)
    {
         demodulation_16qam(detect_data_ptr,//Q(16,3)
                            llr_data_ptr,//Q(16,1)
                            demod_data_ptr,//Q(8,6)
                            mimo_type,
                            data_length
                           );
    }
    else if(QPSK == modulation_type)
    {
        demodulation_qpsk(detect_data_ptr,//Q(16,3)
                          demod_data_ptr,//Q(8,6)
                          mimo_type,
                          data_length
                          );
    }
    
}


/***********************************************/
/*demodulation_descramble.c                     */
/*功能：解调解扰功能                               */
/***********************************************/

void demodulation_descramble_64qam(signed short *restrict detect_data_ptr,//Q(16,3)
                        signed short *restrict llr_data_ptr,//Q(16,1)
                        signed char *restrict demod_data_ptr,//Q(8,6)
                        unsigned char mimo_type,
                        unsigned int data_length,
                        signed short *restrict scramble_ptr
                        )
{
    unsigned int m = 0;
    signed int int_llr_factor4 = 0,int_llr_factor2 = 0;
    signed int temp1 = 0,temp2 = 0,temp3 = 0,temp4 = 0,temp5 = 0,temp6 = 0,tempa = 0,tempb = 0,tempc = 0;
    short temp11 = 0,temp22 = 0,temp33 = 0;
    signed int firtemp = 0;
    signed int *restrict int_detect_ptr = (signed int *)detect_data_ptr;
    signed short *restrict short_demod_ptr = (signed short*)demod_data_ptr;
    double temp = 0;
    if(SINGLE_ANTENNA_PORT == mimo_type)
    {
        for(m = 0;m < data_length;m++)
        {
            int_llr_factor4 = _mpy(llr_data_ptr[m],0xc31);//Q(16,3)
            int_llr_factor2 = _mpy(llr_data_ptr[m],0x618);
            firtemp = int_detect_ptr[m]; //Q(16,3)
            temp = _mpy2(firtemp,0xec40ec40);//b1=-3240*C_Re;b2=-3240*C_Im Q(16,3)
            temp1 = _hi(temp);
            temp2 = _lo(temp);
            temp3 = _abs(temp1) - int_llr_factor4;//b3 = |b1|-125*16*256
            temp4 = _abs(temp2) - int_llr_factor4;//b4 = |b2|-125*16*256
            temp5 = _abs(temp3) - int_llr_factor2;//b5 = |b3|-125*8*256
            temp6 = _abs(temp4) - int_llr_factor2;//b6 = |b4|-125*8*256
            temp1 = _sshl(temp1,9 - agc_64qam);
            temp2 = _sshl(temp2,9 - agc_64qam);
            temp3 = _sshl(temp3,9 - agc_64qam);
            temp4 = _sshl(temp4,9 - agc_64qam);
            temp5 = _sshl(temp5,9 - agc_64qam);
            temp6 = _sshl(temp6,9 - agc_64qam);
            temp1 = temp1 * scramble_ptr[6*m+0];
            temp2 = temp2 * scramble_ptr[6*m+1];
            temp3 = temp3 * scramble_ptr[6*m+2];
            temp4 = temp4 * scramble_ptr[6*m+3];
            temp5 = temp5 * scramble_ptr[6*m+4];
            temp6 = temp6 * scramble_ptr[6*m+5];
            tempa = _packh2(temp1,temp2);
            tempb = _packh2(temp3,temp4);
            tempc = _packh2(temp5,temp6);
            temp11 = _packh4(0,tempa);//得到8bit软比特
            temp22 = _packh4(0,tempb);//得到8bit软比特
            temp33 = _packh4(0,tempc);//得到8bit软比特
            short_demod_ptr[3 * m] = temp11 ^ scramble_ptr[3 * m];
            short_demod_ptr[3 * m + 1] = temp22 ^ scramble_ptr[3 * m + 1];
            short_demod_ptr[3 * m + 2] = temp33 ^ scramble_ptr[3 * m + 2];
        }
    }
    else
    {
        for(m = 0;m < data_length;m++)
        {
            int_llr_factor4 = _mpy(llr_data_ptr[m],0xc31);//0xc31 Q(16,3)  ->Q(32,4)
            int_llr_factor2 = _mpy(llr_data_ptr[m],0x618);
            firtemp = int_detect_ptr[m]; //Q(16,3)
            temp = _mpy2(firtemp,0xb100b100);//b1=-5056*C_Re;b2=-5056*C_Im  0xec40ec40 Q(16,3) 0xb100b100 Q(16,1)
            temp1 = _hi(temp);
            temp2 = _lo(temp);
            temp3 = _abs(temp1) - int_llr_factor4;//b3 = |b1|-125*16*256
            temp4 = _abs(temp2) - int_llr_factor4;//b4 = |b2|-125*16*256
            temp5 = _abs(temp3) - int_llr_factor2;//b5 = |b3|-125*8*256
            temp6 = _abs(temp4) - int_llr_factor2;//b6 = |b4|-125*8*256
            temp1 = _sshl(temp1,13 - agc_64qam) >> 2;
            temp2 = _sshl(temp2,13 - agc_64qam) >> 2;
            temp3 = _sshl(temp3,13 - agc_64qam) >> 2;
            temp4 = _sshl(temp4,13 - agc_64qam) >> 2;
            temp5 = _sshl(temp5,13 - agc_64qam) >> 2;
            temp6 = _sshl(temp6,13 - agc_64qam) >> 2;
            tempa = _packh2(temp1,temp2);
            tempb = _packh2(temp3,temp4);
            tempc = _packh2(temp5,temp6);
            temp11 = _packh4(0,tempa);//得到8bit软比特
            temp22 = _packh4(0,tempb);//得到8bit软比特
            temp33 = _packh4(0,tempc);//得到8bit软比特
            short_demod_ptr[3 * m] = temp11 ^ scramble_ptr[3 * m];
            short_demod_ptr[3 * m + 1] = temp22 ^ scramble_ptr[3 * m + 1];
            short_demod_ptr[3 * m + 2] = temp33 ^ scramble_ptr[3 * m + 2];
        }
    }
}

void demodulation_descramble_16qam(signed short *restrict detect_data_ptr,//Q(16,3)
                        signed short *restrict llr_data_ptr,//Q(16,1)
                        signed char *restrict demod_data_ptr,//Q(8,6)
                        unsigned char mimo_type,
                        unsigned int data_length,
                        signed int *restrict scramble_ptr
                        )
{
    unsigned int m = 0;
    signed int int_llr_factor = 0;
    signed int temp1 = 0,temp2 = 0,temp3 = 0,temp4 = 0,tempa = 0,tempb = 0;
    signed int firtemp = 0,temp11 = 0;
    signed int *restrict int_detect_ptr = (signed int *)detect_data_ptr;
    signed int *restrict int_demod_ptr = (signed int*)demod_data_ptr;
    double temp = 0;
    if(SINGLE_ANTENNA_PORT == mimo_type)
    {
        for(m = 0;m < data_length;m++)
        {
        	int_llr_factor = _mpy((llr_data_ptr[m] >> 2),0x199a);//2*D2*LLR llr_data_ptrQ(16,1) 0x199a Q(16,3)
            firtemp = int_detect_ptr[m]; //Q(16,3)
            temp = _mpy2(firtemp,0xd786d786);//b1=-6641*C_Re;b2=-6641*C_Im Q(16,3)
            temp1 = _hi(temp);
            temp2 = _lo(temp);
            temp3 = _abs(temp1) - int_llr_factor;//b3 = |b1|-525*8*256
            temp4 = _abs(temp2) - int_llr_factor;//b4 = |b2|-525*8*256
            temp1 = _sshl(temp1,9 - agc_16qam);
            temp2 = _sshl(temp2,9 - agc_16qam);
            temp3 = _sshl(temp3,9 - agc_16qam);
            temp4 = _sshl(temp4,9 - agc_16qam);
            tempa = _packh2(temp1,temp2);
            tempb = _packh2(temp3,temp4);
            temp11 = _packh4(tempa,tempb);//得到8bit软比特
            int_demod_ptr[m] = temp11 ^ scramble_ptr[m];
        }
    }
    else
    {
        for(m = 0;m < data_length;m++)
        {
            int_llr_factor = _mpy((llr_data_ptr[m] >> 2),0x199a);//2*D2*LLR Q(16,3)
            firtemp = int_detect_ptr[m]; //Q(16,3)
            temp = _mpy2(firtemp,0xd786d786);//b1=-6641*C_Re;b2=-6641*C_Im
            temp1 = _hi(temp);
            temp2 = _lo(temp);
            temp3 = _abs(temp1) - int_llr_factor;//b3 = |b1|-525*8*256
            temp4 = _abs(temp2) - int_llr_factor;//b4 = |b2|-525*8*256
            temp1 = _sshl(temp1,12 - agc_16qam);
            temp2 = _sshl(temp2,12 - agc_16qam);
            temp3 = _sshl(temp3,12 - agc_16qam);
            temp4 = _sshl(temp4,12 - agc_16qam);
            tempa = _packh2(temp1,temp2);
            tempb = _packh2(temp3,temp4);
            temp11 = _packh4(tempa,tempb);//得到8bit软比特
            int_demod_ptr[m] = temp11 ^ scramble_ptr[m];
        }
    }
}

void demodulation_descramble_qpsk(signed short *restrict detect_data_ptr,//Q(16,3)
                       signed char *restrict demod_data_ptr,//Q(8,6)
                       unsigned char mimo_type,
                       unsigned int data_length,
                       signed short *restrict scramble_ptr
                       )
{
    unsigned int m = 0;
    signed int temp1 = 0,temp2 = 0;
    short temp11 = 0;
    double temp = 0;
    signed int *restrict int_detec_ptr = (signed int *)detect_data_ptr;
    signed short *restrict short_demod_ptr = (signed short*)demod_data_ptr;
    if(SINGLE_ANTENNA_PORT == mimo_type)
    {
        for(m = 0;m < data_length;m++)
        {
            temp = _mpy2(int_detec_ptr[m],0xa582a582);//Q(16,3)*Q(16,1)=Q(32,4)
            temp1 = _hi(temp);
            temp2 = _lo(temp);
            temp1 = _sshl(temp1,9 - agc_qpsk);
            temp2 = _sshl(temp2,9 - agc_qpsk);
            temp11 = _packh4(0,_packh2(temp1,temp2));//得到8bit软比特
            short_demod_ptr[m] = temp11 ^ scramble_ptr[m];
        }
    }
    else
    {
        for(m = 0;m < data_length;m++)
        {
            temp = _mpy2(int_detec_ptr[m],0xa582a582);//Q(16,3)*Q(16,1)=Q(32,4)
            temp1 = _hi(temp);
            temp2 = _lo(temp);
            temp1 = _sshl(temp1,12 - agc_qpsk);
            temp2 = _sshl(temp2,12 - agc_qpsk);
            temp11 = _packh4(0,_packh2(temp1,temp2));//得到8bit软比特
            short_demod_ptr[m] = temp11 ^ scramble_ptr[m];
        }
    }
}

void demodulation_descramble(signed short *restrict detect_data_ptr,//Q(16,3)
                  signed short *restrict llr_data_ptr,//Q(16,1)
                  signed char *restrict demod_data_ptr,//Q(8,6)
                  unsigned char mimo_type,
                  unsigned char modulation_type,
                  unsigned int data_length,
                  signed char * scramble_ptr
                  )
{
    if(QAM64 == modulation_type)
    {
    	demodulation_descramble_64qam(detect_data_ptr,//Q(16,3)
                            llr_data_ptr,//Q(16,1)
                            demod_data_ptr,//Q(8,6)
                            mimo_type,
                            data_length,
                            (signed short *)scramble_ptr
                           );
    }
    else if(QAM16 == modulation_type)
    {
    	demodulation_descramble_16qam(detect_data_ptr,//Q(16,3)
                            llr_data_ptr,//Q(16,1)
                            demod_data_ptr,//Q(8,6)
                            mimo_type,
                            data_length,
                            (signed int *)scramble_ptr
                           );
    }
    else if(QPSK == modulation_type)
    {
    	demodulation_descramble_qpsk(detect_data_ptr,//Q(16,3)
                          demod_data_ptr,//Q(8,6)
                          mimo_type,
                          data_length,
                          (signed short *)scramble_ptr
                          );
    }

}
