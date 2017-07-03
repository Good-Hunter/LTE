/*
 * chan_eq_sdm
 * d = (H^H*H)^(-1)*H^H*e
 */

#include <c6x.h>
#include "pl_comm.h"


//W*D*U, 2*2, 码本取值W[0]-W[2], i取值奇偶对应D[0]-D[1]
//[1/2,  1/2]       //W[0]D[0]U
//[1/2, -1/2]                  
signed int WDU_0[4] = {0x3fff0000,   0x3fff0000,
             0x3fff0000,   0xc0010000};
signed int WDU_1[4] = {0x3fff0000,   0x3fff0000,        //[1/2,  1/2]       //W[0]D[1]U
                      0xc0010000,   0x3fff0000};       //[-1/2, 1/2]
signed int WDU_2[4] = {0x7fff0000,   0x00000000,        //[1,    0]         //W[1]D[0]U
                      0x00000000,   0x7fff0000};       //[0,    1]
signed int WDU_3[4] = {0x00000000,   0x7fff0000,        //[0,    1]         //W[1]D[1]U
                      0x7fff0000,   0x00000000};       //[1,    0]
signed int WDU_4[4] = {0x7fff0000,   0x00000000,        //[1,    0]         //W[2]D[0]U
                      0x00000000,   0x00007fff};       //[0,    j]
signed int WDU_5[4] = {0x00000000,   0x7fff0000,        //[0,    1]         //W[2]D[0]U
                       0x00007fff,   0x00000000};       //[j,    0]
                       
signed int* WDU_N_ptr[6] = {WDU_0, WDU_1, WDU_2, WDU_3, WDU_4, WDU_5};
                       
//给WDU表赋值  
    //WDU_0[0] = 0x3fff0000;
    //WDU_0[1] = 0x3fff0000;
    //WDU_0[2] = 0x3fff0000;
    //WDU_0[3] = 0xc0010000;
    //WDU_1[0] = 0x3fff0000;
    //WDU_1[1] = 0x3fff0000;
    //WDU_1[2] = 0xc0010000;
    //WDU_1[3] = 0x3fff0000;
    //WDU_2[0] = 0x7fff0000;
    //WDU_2[1] = 0x00000000;
    //WDU_2[2] = 0x00000000;
    //WDU_2[3] = 0x7fff0000;    
    //WDU_3[0] = 0x00000000;
    //WDU_3[1] = 0x7fff0000;
    //WDU_3[2] = 0x7fff0000;
    //WDU_3[3] = 0x00000000;
    //WDU_4[0] = 0x7fff0000;
    //WDU_4[1] = 0x00000000;
    //WDU_4[2] = 0x00000000;
    //WDU_4[3] = 0x00007fff;
    //WDU_5[0] = 0x00000000;
    //WDU_5[1] = 0x7fff0000;
    //WDU_5[2] = 0x00007fff;
    //WDU_5[3] = 0x00000000;
unsigned short inverse_table[128] =
{
0x0   , 
0x8000,
0x4000,
0x2AAA,
0x2000,
0x1999,
0x1555,
0x1249,
0x1000,
0xE38 ,
0xCCC ,
0xBA2 ,
0xAAA ,
0x9D8 ,
0x924 ,
0x888 ,
0x800 ,
0x787 ,
0x71C ,
0x6BC ,
0x666 ,
0x618 ,
0x5D1 ,
0x590 ,
0x555 ,
0x51E ,
0x4EC ,
0x4BD ,
0x492 ,
0x469 ,
0x444 ,
0x421 ,
0x400 ,
0x3E0 ,
0x3C3 ,
0x3A8 ,
0x38E ,
0x375 ,
0x35E ,
0x348 ,
0x333 ,
0x31F ,
0x30C ,
0x2FA ,
0x2E8 ,
0x2D8 ,
0x2C8 ,
0x2B9 ,
0x2AA ,
0x29C ,
0x28F ,
0x282 ,
0x276 ,
0x26A ,
0x25E ,
0x253 ,
0x249 ,
0x23E ,
0x234 ,
0x22B ,
0x222 ,
0x219 ,
0x210 ,
0x208 ,
0x200 ,
0x1F8 ,
0x1F0 ,
0x1E9 ,
0x1E1 ,
0x1DA ,
0x1D4 ,
0x1CD ,
0x1C7 ,
0x1C0 ,
0x1BA ,
0x1B4 ,
0x1AF ,
0x1A9 ,
0x1A4 ,
0x19E ,
0x199 ,
0x194 ,
0x18F ,
0x18A ,
0x186 ,
0x181 ,
0x17D ,
0x178 ,
0x174 ,
0x170 ,
0x16C ,
0x168 ,
0x164 ,
0x160 ,
0x15C ,
0x158 ,
0x155 ,
0x151 ,
0x14E ,
0x14A ,
0x147 ,
0x144 ,
0x141 ,
0x13E ,
0x13B ,
0x138 ,
0x135 ,
0x132 ,
0x12F ,
0x12C ,
0x129 ,
0x127 ,
0x124 ,
0x121 ,
0x11F ,
0x11C ,
0x11A ,
0x118 ,
0x115 ,
0x113 ,
0x111 ,
0x10E ,
0x10C ,
0x10A ,
0x108 ,
0x106 ,
0x104 ,
0x102
};                 
                                                       
void chan_eq_sdm(signed int  *restrict e_1_c1_ptr,     //In, 第1根接收天线的第1个子载波，e1=e_1_c1
                 signed int  *restrict e_2_c1_ptr,     //In, 第2根接收天线的第1个子载波，e2=e_2_c1
                 signed int  *restrict h_1_1_c1_ptr,   //In, 第1根接收天线的第1个流的第1个子载波，a=h_1_1_c1
                 signed int  *restrict h_1_2_c1_ptr,   //In, 第1根接收天线的第2个流的第1个子载波，b=-h_1_2_c1
                 signed int  *restrict h_2_1_c1_ptr,   //In, 第2根接收天线的第1个流的第1个子载波，c=h_2_1_c1
                 signed int  *restrict h_2_2_c1_ptr,   //In, 第2根接收天线的第2个流的第1个子载波，d=-h_2_2_c1
                 //signed int  WDU_index,  //Para, 结合码本和i给出wdu_index，0-5
                 signed int  data_len,
                 signed int* restrict d1_out_ptr,  //Out, d1
                 signed int* restrict d2_out_ptr,  //Out, d2
                 signed short* restrict LLR1_ptr,     //Out,LLr结果首地址
                 signed short* restrict LLR2_ptr     //Out,LLr结果首地址
           )

{  
    //给WDU表赋值  
    //WDU_0[0] = 0x3fff0000;
    //WDU_0[1] = 0x3fff0000;
    //WDU_0[2] = 0x3fff0000;
    //WDU_0[3] = 0xc0010000;
    //WDU_1[0] = 0x3fff0000;
    //WDU_1[1] = 0x3fff0000;
    //WDU_1[2] = 0xc0010000;
    //WDU_1[3] = 0x3fff0000;
    //WDU_2[0] = 0x7fff0000;
    //WDU_2[1] = 0x00000000;
    //WDU_2[2] = 0x00000000;
    //WDU_2[3] = 0x7fff0000;    
    //WDU_3[0] = 0x00000000;
    //WDU_3[1] = 0x7fff0000;
    //WDU_3[2] = 0x7fff0000;
    //WDU_3[3] = 0x00000000;
    //WDU_4[0] = 0x7fff0000;
    //WDU_4[1] = 0x00000000;
    //WDU_4[2] = 0x00000000;
    //WDU_4[3] = 0x00007fff;
    //WDU_5[0] = 0x00000000;
    //WDU_5[1] = 0x7fff0000;
    //WDU_5[2] = 0x00007fff;
    //WDU_5[3] = 0x00000000;
    signed int  WDU_index;
    
    signed int* WDU_ptr;
    
    //WDU_ptr = WDU_N_ptr[WDU_index];
    /*
    if (0 == WDU_index)
    {
        WDU_ptr = &WDU_0[0];
    }
    else if (1 == WDU_index)
    {
        WDU_ptr = &WDU_1[0];
    }
    else if (2 == WDU_index)
    {
        WDU_ptr = &WDU_2[0];
    }
    else if (3 == WDU_index)
    {
        WDU_ptr = &WDU_3[0];
    }
    else if (4 == WDU_index)
    {
        WDU_ptr = &WDU_4[0];
    }*/
    
    signed int  k;
    //signed int* restrict freq_data_a0_ptr;
    //signed int* restrict freq_data_a1_ptr;
    //signed int* restrict h_data_a0_ptr;
    //signed int* restrict h_data_a1_ptr;
    //e(接收天线，子载波）---可不用指针，输出时用指针即可
    //signed int e_1_c1;     //第1根接收天线的第1个子载波，e1=e_1_c1
    //signed int e_2_c1;     //第2根接收天线的第1个子载波，e2=e_2_c1

    //h(接收天线，发送流，子载波）
    //signed int h_1_1_c1;   //第1根接收天线的第1个流的第1个子载波，a=h_1_1_c1
    //signed int h_1_2_c1;   //第1根接收天线的第2个流的第1个子载波，b=-h_1_2_c1 
    //signed int h_2_1_c1;   //第2根接收天线的第1个流的第1个子载波，c=h_2_1_c1
    //signed int h_2_2_c1;   //第2根接收天线的第2个流的第1个子载波，d=-h_2_2_c1

    //R=H^H*H
    signed int r_1_1;      //第一行第一列
    signed int r_1_2;      //第一行第二列
    signed int r_2_1;      //第二行第一列
    signed int r_2_2;      //第二行第二列
    
    /*
    H = [h_1_1_c1   h_1_2_c1]          = [a b]
        [h_2_1_c1   h_2_2_c1]            [c d]

    H^H * H = [ah ch] * [a b]     = [a^2+c^2,   ah*b+ch*d]
              [bh dh]   [c d]       [bh*a+dh*b, b^2+d^2]

    */
    long long pack_a_b;
    long long pack_c_d;

    __x128_t  pack_a_b_c_d;
    __x128_t  pack_wdu1234;   

    __x128_t  pack_ah_bh_ch_dh;

    
    long long coeff;
    
    long long pack_ah_ch;
    long long pack_bh_dh;

    long long pack_ah_bh;
    long long pack_ch_dh;
    
    long long pack_a_c;
    long long pack_b_d;

    long long pack_wdu12;
    long long pack_wdu34;

    long long temp1_ll;
    long long temp2_ll;
    long long temp3_ll;
    long long temp4_ll;
    long long temp5_ll;
    long long temp6_ll;
    
    long long pack_e1_e2;
    
    
    //test 赋初值
    //int data_len;
    //data_len = 1;   //1 REs
    //h_1_1_c1 = 0x3fff0000;
    //h_1_2_c1 = 0x00003fff;
    //h_2_1_c1 = 0;
    //h_2_2_c1 = 0x3fff0000;
    //
    ////e_1_c1 = 0x5a7ea582;   //0.707-0.707j
    ////e_2_c1 = 0x5a7ea582;   //0.707-0.707j
    //e_1_c1 = 0x0000c001;
    //e_2_c1 = 0xe0011fff;
    
  //signed int
  //freq_data_a0_ptr = e_freq_data_ptr;         //接收天线0
  //freq_data_a1_ptr = e_freq_data_ptr + data_len;      //接收天线1，天线在外层，致密排布---？？？
  //h_data_a0_ptr = h_freq_interp_ptr;          //接收天线0
  //h_data_a1_ptr = h_freq_interp_ptr + data_len;      //接收天线1，天线在外层，致密排布

  for (k = 0; k < data_len; k++)
  {
    //20121224，地址初始化
    WDU_index = k & 1;
    WDU_ptr = WDU_N_ptr[WDU_index];
    
    signed int  e_1_c1; 
    signed int  e_2_c1;   
    signed int  h_1_1_c1;
    signed int  h_1_2_c1;
    signed int  h_2_1_c1;
    signed int  h_2_2_c1;

    e_1_c1 = e_1_c1_ptr[k];     //In, 第1根接收天线的第1个子载波，e1=e_1_c1
    e_2_c1 = e_2_c1_ptr[k];     //In, 第2根接收天线的第1个子载波，e2=e_2_c1
    h_1_1_c1 = h_1_1_c1_ptr[k];   //In, 第1根接收天线的第1个流的第1个子载波，a=h_1_1_c1
    h_1_2_c1 = h_1_2_c1_ptr[k];   //In, 第1根接收天线的第2个流的第1个子载波，b=-h_1_2_c1
    h_2_1_c1 = h_2_1_c1_ptr[k];   //In, 第2根接收天线的第1个流的第1个子载波，c=h_2_1_c1
    h_2_2_c1 = h_2_2_c1_ptr[k];   //In, 第2根接收天线的第2个流的第1个子载波，d=-h_2_2_c1
    
    //取频域数据和频域信道估计【---取数偏移的操作根据存储排布会调整---】
    
        //===========================111===========================================================，
        //H = [h1_1 h1_2]*[w1_1 w1_2], (2*2) = (2*2) * (2*2)
        //    [H2_1 h2_2] [w2_1 w2_2]
        //Q(16,1)*Q(16,1) <<1 -> Q(32,1), Q(32,1)+Q(32,1) >>1 ->Q(32,2)[复数乘，ad-bc]---所以<<1和>>1抵消，此处不移位
        //Q(32,2)+Q(32,2) >>1 -> Q(32,3)
        //先取h_1_1_c1 ~ h_2_2_c1
        
        //再转换（求负和求共轭）并pack
        //pack_ah_bh = (h_1_1_c1)^*, (h_1_2_c1)^*
        _amem8((void*)&pack_ah_bh) = _itoll(_pack2((h_1_1_c1) >> 16, -(h_1_1_c1  & 0x0000ffff)),
                                          _pack2((h_1_2_c1) >> 16, -(h_1_2_c1  & 0x0000ffff)));
            //pack_ch_dh = (h_2_1_c1)^*, (h_2_2_c1)^*                                     
        _amem8((void*)&pack_ch_dh) = _itoll(_pack2((h_2_1_c1) >> 16, -(h_2_1_c1  & 0x0000ffff)),
                                          _pack2((h_2_2_c1) >> 16, -(h_2_2_c1  & 0x0000ffff)));        
            //pack_w1234---需根据不同的i和码本选取不同的WDU
            //pack_wdu12
        _amem8((void*)&pack_wdu12) = _itoll(_pack2((WDU_ptr[0]) >> 16, (WDU_ptr[0]  & 0x0000ffff)),
                                          _pack2((WDU_ptr[1]) >> 16, (WDU_ptr[1]  & 0x0000ffff)));
            //pack_wdu34
        _amem8((void*)&pack_wdu34) = _itoll(_pack2((WDU_ptr[2]) >> 16, (WDU_ptr[2]  & 0x0000ffff)),
                                          _pack2((WDU_ptr[3]) >> 16, (WDU_ptr[3]  & 0x0000ffff)));
        pack_wdu1234 = _llto128(pack_wdu12, pack_wdu34);    //hi128,hill,loll, itoll
        
        signed int temp1_int;
        signed int temp2_int;
        signed int temp3_int;
        signed int temp4_int;
        signed int temp5_int;
        signed int temp6_int;
        signed int temp7_int;
        signed int temp8_int;
        signed int temp9_int;
        signed int temp10_int;
        signed int temp11_int;
        signed int temp12_int;
        
        signed int d1_out;
        signed int d2_out;
        
        //矩阵相乘得到H
        temp1_ll = _ccmatmpyr1(pack_ah_bh, pack_wdu1234);       //H的第一行     新的[a  b]
        temp2_ll = _ccmatmpyr1(pack_ch_dh, pack_wdu1234);       //H的第二行     新的[c  d]
        temp1_int = _hill(temp1_ll);                //a
        temp2_int = _loll(temp1_ll);                //b
        temp3_int = _hill(temp2_ll);                //c
        temp4_int = _loll(temp2_ll);                //d
        
        //===========================222===========================================================，
        //R = H^H * H，(2*2) = (2*2) * (2*2)
        //Q(16,1)*Q(16,1) <<1 -> Q(32,1), Q(32,1)+Q(32,1) >>1 ->Q(32,2)[复数乘，ad-bc]---所以<<1和>>1抵消，此处不移位
        //Q(32,2)+Q(32,2) >>1 -> Q(32,3)
        
        //再转换并pack---因为ccmatmpyr1会把1*2的矩阵元素求共轭，所以不用自己转共轭
        //pack_a_c
        _amem8((void*)&pack_a_c) = _itoll(_pack2((temp1_int) >> 16, (temp1_int  & 0x0000ffff)),
                                        _pack2((temp3_int) >> 16, (temp3_int  & 0x0000ffff)));
            //pack_b_d
        _amem8((void*)&pack_b_d) = _itoll(_pack2((temp2_int) >> 16, (temp2_int  & 0x0000ffff)),
                                        _pack2((temp4_int) >> 16, (temp4_int  & 0x0000ffff)));                                      
            
            //pack_a_b_c_d = (pack_a_b << 64) + pack_c_d;
            //pack_a_b
        _amem8((void*)&pack_a_b) = _itoll(_pack2((temp1_int) >> 16, (temp1_int  & 0x0000ffff)),
                                        _pack2((temp2_int) >> 16, (temp2_int  & 0x0000ffff)));
            //pack_c_d
        _amem8((void*)&pack_c_d) = _itoll(_pack2((temp3_int) >> 16, (temp3_int  & 0x0000ffff)),
                                        _pack2((temp4_int) >> 16, (temp4_int  & 0x0000ffff)));                                                                                                                  
            pack_a_b_c_d = _llto128(pack_a_b, pack_c_d);    //hi128,hill,loll, itoll
            
            //用于H^H*e
            //pack_ah_bh
        _amem8((void*)&pack_ah_bh) = _itoll(_pack2((temp1_int) >> 16, -(temp1_int  & 0x0000ffff)),
                                          _pack2((temp2_int) >> 16, -(temp2_int  & 0x0000ffff)));
            //pack_ch_dh
        _amem8((void*)&pack_ch_dh) = _itoll(_pack2((temp3_int) >> 16, -(temp3_int  & 0x0000ffff)),
                                      _pack2((temp4_int) >> 16, -(temp4_int  & 0x0000ffff)));                                                                                                                 
        pack_ah_bh_ch_dh = _llto128(pack_ah_bh, pack_ch_dh);
                
        //矩阵运算------------------------------------定标----------------------------------------        
        temp1_ll = _ccmatmpyr1(pack_a_c, pack_a_b_c_d);       //R的第一行     [A  B]
        temp2_ll = _ccmatmpyr1(pack_b_d, pack_a_b_c_d);       //R的第二行     [C  D]
        
        //===========================333===========================================================，
        //R^-1 = 1/|ad-bc| * [D  -B]，，，有共轭么？，，涉及开根号？？？
        //                   [-C  A]   
        temp1_int = _hill(temp1_ll);                //A
        temp2_int = _loll(temp1_ll);                //B
        temp3_int = _hill(temp2_ll);                //C
        temp4_int = _loll(temp2_ll);                //D
        temp5_int = _cmpyr1(temp1_int, temp4_int);  //AD
        temp6_int = _cmpyr1(temp2_int, temp3_int);  //BC
        
        //20120812
        temp7_int = temp5_int - temp6_int;          //AD-BC
        
        signed int inverse_final;
        //signed int inverse_final_2;
        signed short temp1_short;
       
        if (temp7_int >= 0)
        {
            signed int pos_coeff = 1;
            signed int move_coeff;// = 32 - temp8_int + 8;     //有一个提取某几位bit的指令
            move_coeff = _norm(temp7_int);
            
            temp9_int = temp7_int << (move_coeff);  
            temp10_int = temp9_int >> 24;                   //保留8位
                      
            //signed short temp1_short;
            temp1_short = inverse_table[(char)temp10_int];//查表求得倒数
                     
            inverse_final =  temp1_short << move_coeff;            //Q(32,32)
            inverse_final =  inverse_final >> 8;
            inverse_final =  inverse_final * (pos_coeff);

            //inverse_final = _sshl(inverse_final, 16);   //Q(32,16)

        }
        else
        {
            signed int pos_coeff = -1;
            temp10_int = temp7_int*pos_coeff;
            signed int move_coeff;
            move_coeff = _norm(temp10_int);
            temp9_int = temp7_int << (move_coeff);  
            temp10_int = temp9_int >> 24;                   //保留8位
            
            //signed short temp1_short;
            temp1_short = inverse_table[(char)temp10_int];//查表求得倒数,Q(16,8)
            
                       
            inverse_final =  temp1_short << move_coeff;            //Q(32,32)
            inverse_final =  inverse_final >> 8;
            inverse_final =  inverse_final * (pos_coeff);

            //inverse_final = _sshl(inverse_final, 16);   //Q(32,16)
        }

        //===========================444===========================================================，
        //H^H * e
        //pack_ah_bh_ch_dh在第二步中求得
        
        //e1=e_1_c1  
        //e2=e_2_c1
        //注意e1、e2都取了共轭，因为ccmatmpyr1还要取一次共轭，要抵消掉这个  
        _amem8((void*)&pack_e1_e2) = _itoll(_pack2((e_1_c1) >> 16, -(e_1_c1  & 0x0000ffff)),
                                      _pack2((e_2_c1) >> 16, -(e_2_c1  & 0x0000ffff)));
                                              
        temp1_ll = _ccmatmpyr1(pack_e1_e2, pack_ah_bh_ch_dh);       //H^H*e的两个元素[E F]
        
        temp7_int = _hill(temp1_ll);    //E
        temp8_int = _loll(temp1_ll);    //F
        
        //===========================555===========================================================，
        //d = R^-1 * (H^H * e)
        //  [D -B] * [E] = [DE-BF]          Q(32,?),Q(16,?)
        //  [-C A]   [F]   [-CE+AF]
        temp9_int = _cmpyr1(temp4_int, temp7_int);      //DE
        temp10_int = _cmpyr1(temp2_int, temp8_int);      //BF
        temp11_int = _cmpyr1(temp3_int, temp7_int);      //CE
        temp12_int = _cmpyr1(temp1_int, temp8_int);      //AF
        d1_out = _ssub2(temp9_int, temp10_int);
        d2_out = _ssub2(temp12_int, temp11_int);
        
        temp9_int = _mpyhl(d1_out, inverse_final) >> 2;
        temp1_short = (signed short)d1_out;
        temp10_int = _mpy(temp1_short, inverse_final) >> 2;
        d1_out = _pack2(temp9_int, temp10_int);

        temp9_int = _mpyhl(d2_out, inverse_final) >> 2;
        temp1_short = (signed short)d2_out;
        temp10_int = _mpy(temp1_short, inverse_final) >> 2;
        d2_out = _pack2(temp9_int, temp10_int);
        
        //返回结果
        *d1_out_ptr = d1_out;
        *d2_out_ptr = d2_out;
        *LLR1_ptr = 0x7fff;/*暂时写死*/
        *LLR2_ptr = 0x7fff;/*暂时写死*/
        //asm(" nop");
        
        d1_out_ptr++;
        d2_out_ptr++;
        LLR1_ptr++;
        LLR2_ptr++;

        //===========================666===========================================================，
        //LLR = 1/diag(real(R^-1))
        

  }
}

void chan_eq_sfbc(signed int  *restrict e_1_c1_ptr,     //In, 第1根接收天线的第1个子载波，e1=e_1_c1
                  signed int  *restrict e_2_c1_ptr,     //In, 第2根接收天线的第1个子载波，e2=e_2_c1
                  signed int  *restrict h_1_1_c1_ptr,   //In, 第1根接收天线的第1个流的第1个子载波，a=h_1_1_c1
                  signed int  *restrict h_1_2_c1_ptr,   //In, 第1根接收天线的第2个流的第1个子载波，b=-h_1_2_c1
                  signed int  *restrict h_2_1_c1_ptr,   //In, 第2根接收天线的第1个流的第1个子载波，c=h_2_1_c1
                  signed int  *restrict h_2_2_c1_ptr,   //In, 第2根接收天线的第2个流的第1个子载波，d=-h_2_2_c1
                  signed int  data_len,
                  signed int* restrict d1_out_ptr,  //Out, d1
                  //signed int* restrict d2_out_ptr,  //Out, d2
                  signed short* restrict LLR1_ptr,     //Out,LLr结果首地址
                  //signed short* restrict LLR2_ptr     //Out,LLr结果首地址
                  signed int chan_type

                  )
{
  signed int  k;
  //signed int* restrict freq_data_a0_ptr;
  //signed int* restrict freq_data_a1_ptr;
  //signed int* restrict h_data_a0_ptr;
  //signed int* restrict h_data_a1_ptr;
    //e(接收天线，子载波）---可不用指针，输出时用指针即可
    signed int e_1_c1;     //第1根接收天线的第1个子载波，e1=e_1_c1
    signed int e_2_c1;     //第2根接收天线的第1个子载波，e2=e_2_c1
    signed int e_1_c2;     //第1根接收天线的第2个子载波，e3=e_1_c2^*
    signed int e_2_c2;     //第2根接收天线的第2个子载波，e4=e_2_c2^*
    //h(接收天线，发送流，子载波）
    signed int h_1_1_c1;   //第1根接收天线的第1个流的第1个子载波，a=h_1_1_c1
    signed int h_1_2_c1;   //第1根接收天线的第2个流的第1个子载波，b=-h_1_2_c1
    signed int h_2_1_c1;   //第2根接收天线的第1个流的第1个子载波，c=h_2_1_c1
    signed int h_2_2_c1;   //第2根接收天线的第2个流的第1个子载波，d=-h_2_2_c1
    signed int h_1_1_c2;   //第1根接收天线的第1个流的第2个子载波，f=h_1_1_c2^*
    signed int h_1_2_c2;   //第1根接收天线的第2个流的第2个子载波，e=h_1_2_c2^*
    signed int h_2_1_c2;   //第2根接收天线的第1个流的第2个子载波，h=h_2_1_c2^*
    signed int h_2_2_c2;   //第2根接收天线的第2个流的第2个子载波，g=h_2_2_c2^*
    //R=H^H*H
    signed int r_1_1;      //第一行第一列
    signed int r_1_2;      //第一行第二列
    signed int r_2_1;      //第二行第一列
    signed int r_2_2;      //第二行第二列
    unsigned int s_num=2;
    unsigned int s1_num=1;
    
    /*
    H = [-h_1_1_c1  -h_1_2_c2]          = [a b]
        [h_1_2_c2   h_1_1_c2^*]           [c d]
        [h_2_1_c1   -h_2_2_c1]            [e f]
        [h_2_2_c2   h_2_1_c2^*]           [g h]
    H^H * H = [ah ch eh gh] * [a b]     = [a^2+c^2+e^2+g^2,  ah*b+ch*d+eh*f+gh*h]
              [bh dh fh hh]   [c d]       [(ah*b+ch*d+eh*f+gh*h)^*, b^2+d^2+f^2+h^2]
                              [e f]
                              [g h]
    */
    long long pack_a_b;
    long long pack_c_d;
    long long pack_e_f;
    long long pack_g_h;
    __x128_t  pack_a_b_c_d;
    __x128_t  pack_e_f_g_h;
    __x128_t  pack_ah_bh_ch_dh;
    __x128_t  pack_eh_fh_gh_hh;
    
    long long coeff;
    
    long long pack_ah_ch;
    long long pack_bh_dh;
    long long pack_eh_gh;
    long long pack_fh_hh;
    long long pack_ah_bh;
    long long pack_ch_dh;
    long long pack_eh_fh;
    long long pack_gh_hh;
    
    long long temp1_ll;
    long long temp2_ll;
    long long temp3_ll;
    long long temp4_ll;
    long long temp5_ll;
    long long temp6_ll;
    
    long long pack_e1_e2;
    long long pack_e3_e4;
    double temp1,temp2,temp3,temp4;
    
    

    //test 赋初值
/*  模块测试用
    int data_len;
    data_len = 2;   //2 REs
    h_1_1_c1 = 0x7fff0000;
    h_1_2_c1 = 0;
    h_2_1_c1 = 0;
    h_2_2_c1 = 0x7fff0000;
    h_1_1_c2 = 0x7fff0000;
    h_1_2_c2 = 0;
    h_2_1_c2 = 0;
    h_2_2_c2 = 0x7fff0000;
    e_1_c1 = 0x5a7e5a7e;   //0.707+0.707j
    e_2_c1 = 0xa5825a7e;   //-0.707+0.707j
    e_1_c2 = 0x5a7e5a7e;
    e_2_c2 = 0xa5825a7e;
*/

  //signed int
  //freq_data_a0_ptr = e_freq_data_ptr;         //接收天线0
  //freq_data_a1_ptr = e_freq_data_ptr + data_len;      //接收天线1，天线在外层，致密排布---？？？
  //h_data_a0_ptr = h_freq_interp_ptr;          //接收天线0
  //h_data_a1_ptr = h_freq_interp_ptr + data_len;      //接收天线1，天线在外层，致密排布

  for (k = 0; k < data_len/2; k++)
  {
      //赋初值
    temp1 = _smpy2(0x5a825a82,h_1_1_c1_ptr[2*k]);
    temp2 = _smpy2(0x5a825a82,h_1_2_c1_ptr[2*k]);
    temp3 = _smpy2(0x5a825a82,h_2_1_c1_ptr[2*k]);
    temp4 = _smpy2(0x5a825a82,h_2_2_c1_ptr[2*k]);
      h_1_1_c1 = _packh2(_hi(temp1),_lo(temp1));
      h_1_2_c1 = _packh2(_hi(temp2),_lo(temp2));
      h_2_1_c1 = _packh2(_hi(temp3),_lo(temp3));
      h_2_2_c1 = _packh2(_hi(temp4),_lo(temp4));
      temp1 = _smpy2(0x5a825a82,h_1_1_c1_ptr[2*k+1]);
      temp2 = _smpy2(0x5a825a82,h_1_2_c1_ptr[2*k+1]);
      temp3 = _smpy2(0x5a825a82,h_2_1_c1_ptr[2*k+1]);
      temp4 = _smpy2(0x5a825a82,h_2_2_c1_ptr[2*k+1]);
      h_1_1_c2 = _packh2(_hi(temp1),_lo(temp1));
      h_1_2_c2 = _packh2(_hi(temp2),_lo(temp2));
      h_2_1_c2 = _packh2(_hi(temp3),_lo(temp3));
      h_2_2_c2 = _packh2(_hi(temp4),_lo(temp4));
      e_1_c1 = e_1_c1_ptr[2*k];
      e_2_c1 = e_2_c1_ptr[2*k];
      e_1_c2 = e_1_c1_ptr[2*k+1];
      e_2_c2 = e_2_c1_ptr[2*k+1];

  /********************************************移位****************************************/
#if 1
    if(PDSCH == chan_type)
    {
  	  e_1_c1 = ((e_1_c1 & 0xffff0000) <<s_num) | ((e_1_c1 & 0x0000ffff) << s_num);
  	  e_2_c1 = ((e_2_c1 & 0xffff0000) <<s_num) | ((e_2_c1 & 0x0000ffff) << s_num);
  	  e_1_c2 = ((e_1_c2 & 0xffff0000) <<s_num) | ((e_1_c2 & 0x0000ffff) << s_num);
  	  e_2_c2 = ((e_2_c2 & 0xffff0000) <<s_num) | ((e_2_c2 & 0x0000ffff) << s_num);

  	  h_1_1_c1 = ((h_1_1_c1 & 0xffff0000) <<s_num) | ((h_1_1_c1 & 0x0000ffff) <<s_num);
  	  h_1_2_c1 = ((h_1_2_c1 & 0xffff0000) <<s_num) | ((h_1_2_c1 & 0x0000ffff) <<s_num);
  	  h_2_1_c1 = ((h_2_1_c1 & 0xffff0000) <<s_num) | ((h_2_1_c1 & 0x0000ffff) <<s_num);
  	  h_2_2_c1 = ((h_2_2_c1 & 0xffff0000) <<s_num) | ((h_2_2_c1 & 0x0000ffff) <<s_num);
  	  h_1_1_c2 = ((h_1_1_c2 & 0xffff0000) <<s_num) | ((h_1_1_c2 & 0x0000ffff) <<s_num);
  	  h_1_2_c2 = ((h_1_2_c2 & 0xffff0000) <<s_num) | ((h_1_2_c2 & 0x0000ffff) <<s_num);
  	  h_2_1_c2 = ((h_2_1_c2 & 0xffff0000) <<s_num) | ((h_2_1_c2 & 0x0000ffff) <<s_num);
  	  h_2_2_c2 = ((h_2_2_c2 & 0xffff0000) <<s_num) | ((h_2_2_c2 & 0x0000ffff) <<s_num);
    }

    if(PDCCH == chan_type)
    {
  	  e_1_c1 = ((e_1_c1 & 0xffff0000) <<s1_num) | ((e_1_c1 & 0x0000ffff) << s1_num);
  	  e_2_c1 = ((e_2_c1 & 0xffff0000) <<s1_num) | ((e_2_c1 & 0x0000ffff) << s1_num);
  	  e_1_c2 = ((e_1_c2 & 0xffff0000) <<s1_num) | ((e_1_c2 & 0x0000ffff) << s1_num);
  	  e_2_c2 = ((e_2_c2 & 0xffff0000) <<s1_num) | ((e_2_c2 & 0x0000ffff) << s1_num);

  	  h_1_1_c1 = ((h_1_1_c1 & 0xffff0000) <<s1_num) | ((h_1_1_c1 & 0x0000ffff) <<s1_num);
  	  h_1_2_c1 = ((h_1_2_c1 & 0xffff0000) <<s1_num) | ((h_1_2_c1 & 0x0000ffff) <<s1_num);
  	  h_2_1_c1 = ((h_2_1_c1 & 0xffff0000) <<s1_num) | ((h_2_1_c1 & 0x0000ffff) <<s1_num);
  	  h_2_2_c1 = ((h_2_2_c1 & 0xffff0000) <<s1_num) | ((h_2_2_c1 & 0x0000ffff) <<s1_num);
  	  h_1_1_c2 = ((h_1_1_c2 & 0xffff0000) <<s1_num) | ((h_1_1_c2 & 0x0000ffff) <<s1_num);
  	  h_1_2_c2 = ((h_1_2_c2 & 0xffff0000) <<s1_num) | ((h_1_2_c2 & 0x0000ffff) <<s1_num);
  	  h_2_1_c2 = ((h_2_1_c2 & 0xffff0000) <<s1_num) | ((h_2_1_c2 & 0x0000ffff) <<s1_num);
  	  h_2_2_c2 = ((h_2_2_c2 & 0xffff0000) <<s1_num) | ((h_2_2_c2 & 0x0000ffff) <<s1_num);
    }
#endif

    //取频域数据和频域信道估计【---取数偏移的操作根据存储排布会调整---】
    
        //===========================111===========================================================，
        //R = H^H * H，(2*2) = (2*4) * (4*2)
        //Q(16,1)*Q(16,1) <<1 -> Q(32,1), Q(32,1)+Q(32,1) >>1 ->Q(32,2)[复数乘，ad-bc]---所以<<1和>>1抵消，此处不移位
        //Q(32,2)+Q(32,2)+Q(32,2)+Q(32,2) >>2 -> Q(32,4)
        //先取h_1_1_c1 ~ h_2_2_c2
        
        //再转换（求负和求共轭）并pack成pack_ah_ch~pack_fh_hh，pack_a_b_c_d、pack_e_f_g_h
        //a=h_1_1_c1
        //b=-h_1_2_c1
        //c=h_2_1_c1
        //d=-h_2_2_c1
        //f=h_1_1_c2^*
        //e=h_1_2_c2^*
        //h=h_2_1_c2^*
        //g=h_2_2_c2^*
        //pack_ah_ch = (h_1_1_c1)^*, (h_2_1_c1)^*
    _amem8((void*)&pack_ah_ch) = _itoll(_pack2((h_1_1_c1) >> 16, -(h_1_1_c1  & 0x0000ffff)),
                                      _pack2((h_2_1_c1) >> 16, -(h_2_1_c1  & 0x0000ffff)));
        //pack_bh_dh = (-h_1_2_c1)^*, (-h_2_2_c1)^*
    _amem8((void*)&pack_bh_dh) = _itoll(_pack2(-(h_1_2_c1) >> 16, (h_1_2_c1  & 0x0000ffff)),
                                      _pack2(-(h_2_2_c1) >> 16, (h_2_2_c1  & 0x0000ffff)));
        //pack_eh_gh = (h_1_2_c2^*)^*, (h_2_2_c2^*)^*
    _amem8((void*)&pack_eh_gh) = _itoll(_pack2((h_1_2_c2) >> 16, (h_1_2_c2  & 0x0000ffff)),
                                      _pack2((h_2_2_c2) >> 16, (h_2_2_c2  & 0x0000ffff)));
        //pack_fh_hh = (h_1_1_c2^*)^*, (h_2_1_c2^*)^*
    _amem8((void*)&pack_fh_hh) = _itoll(_pack2((h_1_1_c2) >> 16, (h_1_1_c2  & 0x0000ffff)),
                                      _pack2((h_2_1_c2) >> 16, (h_2_1_c2  & 0x0000ffff)));
        
        //pack_a_b_c_d = (pack_a_b << 64) + pack_c_d;
        //pack_a_b = h_1_1_c1, -h_1_2_c1
    _amem8((void*)&pack_a_b) = _itoll(_pack2((h_1_1_c1) >> 16, (h_1_1_c1  & 0x0000ffff)),
                                      _pack2(-(h_1_2_c1) >> 16, -(h_1_2_c1  & 0x0000ffff)));
        //pack_c_d = h_2_1_c1, -h_2_2_c1
    _amem8((void*)&pack_c_d) = _itoll(_pack2((h_2_1_c1) >> 16, (h_2_1_c1  & 0x0000ffff)),
                                      _pack2(-(h_2_2_c1) >> 16, -(h_2_2_c1  & 0x0000ffff)));
        //pack_e_f = h_1_2_c2^*, h_1_1_c2^*
    _amem8((void*)&pack_e_f) = _itoll(_pack2((h_1_2_c2) >> 16, -(h_1_2_c2  & 0x0000ffff)),
                                      _pack2((h_1_1_c2) >> 16, -(h_1_1_c2  & 0x0000ffff)));
        //pack_g_h = h_2_2_c2^*, h_2_1_c2^*
    _amem8((void*)&pack_g_h) = _itoll(_pack2((h_2_2_c2) >> 16, -(h_2_2_c2  & 0x0000ffff)),
                                      _pack2((h_2_1_c2) >> 16, -(h_2_1_c2  & 0x0000ffff)));
        pack_a_b_c_d = _llto128(pack_a_b, pack_c_d);    //hi128,hill,loll, itoll
        pack_e_f_g_h = _llto128(pack_e_f, pack_g_h);
        
        //矩阵运算------------------------------------定标----------------------------------------
        temp1_ll = _cmatmpyr1(pack_ah_ch, pack_a_b_c_d);
        temp2_ll = _cmatmpyr1(pack_eh_gh, pack_e_f_g_h);
        temp3_ll = _cmatmpyr1(pack_bh_dh, pack_a_b_c_d);
        temp4_ll = _cmatmpyr1(pack_fh_hh, pack_e_f_g_h);
        
        temp5_ll = _dsadd2(temp1_ll, temp2_ll);      //_hi(temp5) = A, _lo(temp5) = B    R = [A B]
        temp6_ll = _dsadd2(temp3_ll, temp4_ll);      //_hi(temp6) = C, _lo(temp5) = D        [C D]
        
        //===========================222===========================================================，
        //R^-1 = 1/|ad-bc| * [D  -B]，，，有共轭么？，，涉及开根号？？？
        //                   [-C  A]   
        signed int temp1_int;
        signed int temp2_int;
        signed int temp3_int;
        signed int temp4_int;
        signed int temp5_int;
        signed int temp6_int;
        signed int temp7_int;
        signed int temp8_int;
        signed int temp9_int;
        signed int temp10_int;
        signed int temp11_int;
        signed int temp12_int;

        signed int d1_out;
        signed int d2_out;


        temp1_int = _hill(temp5_ll);                //A
        temp2_int = _loll(temp5_ll);                //B
        temp3_int = _hill(temp6_ll);                //C
        temp4_int = _loll(temp6_ll);                //D
        temp5_int = _cmpyr1(temp1_int, temp4_int);  //AD
        temp6_int = _cmpyr1(temp2_int, temp3_int);  //BC
        
        //20130203
        temp7_int = temp5_int - temp6_int;          //AD-BC
        
        signed int inverse_final;
        //signed int inverse_final_2;
        signed short temp1_short;
       
        if (temp7_int >= 0)
        {
            signed int pos_coeff = 1;
            signed int move_coeff;// = 32 - temp8_int + 8;     //有一个提取某几位bit的指令
            move_coeff = _norm(temp7_int);
            
            temp9_int = temp7_int << (move_coeff);  
            temp10_int = temp9_int >> 24;                   //保留8位
                      
            //signed short temp1_short;
            temp1_short = inverse_table[(char)temp10_int];//查表求得倒数
                     
            inverse_final =  temp1_short << move_coeff;            //Q(32,32)
            inverse_final =  inverse_final >> 8;
            inverse_final =  inverse_final * (pos_coeff);

            //inverse_final = _sshl(inverse_final, 16);   //Q(32,16)

        }
        else
        {
            signed int pos_coeff = -1;
            temp10_int = temp7_int*pos_coeff;
            signed int move_coeff;
            move_coeff = _norm(temp10_int);
            temp9_int = temp7_int << (move_coeff);  
            temp10_int = temp9_int >> 24;                   //保留8位
            
            //signed short temp1_short;
            temp1_short = inverse_table[(char)temp10_int];//查表求得倒数,Q(16,8)
            
                       
            inverse_final =  temp1_short << move_coeff;            //Q(32,32)
            inverse_final =  inverse_final >> 8;
            inverse_final =  inverse_final * (pos_coeff);

            //inverse_final = _sshl(inverse_final, 16);   //Q(32,16)
        }
        //20130203
        
        //===========================333===========================================================，
        //H^H * e
        //pack_ah_bh = (h_1_1_c1, -h_1_2_c1)^*
        _amem8((void*)&pack_ah_bh) = _itoll(_pack2((h_1_1_c1) >> 16, -(h_1_1_c1  & 0x0000ffff)),
                                          _pack2(-(h_1_2_c1) >> 16, (h_1_2_c1  & 0x0000ffff)));
            //pack_ch_dh = (h_2_1_c1, -h_2_2_c1)^*
        _amem8((void*)&pack_ch_dh) = _itoll(_pack2((h_2_1_c1) >> 16, -(h_2_1_c1  & 0x0000ffff)),
                                          _pack2(-(h_2_2_c1) >> 16, (h_2_2_c1  & 0x0000ffff)));
            //pack_eh_fh = (h_1_2_c2^*, h_1_1_c2^*)^*
        _amem8((void*)&pack_eh_fh) = _itoll(_pack2((h_1_2_c2) >> 16, (h_1_2_c2  & 0x0000ffff)),
                                          _pack2((h_1_1_c2) >> 16, (h_1_1_c2  & 0x0000ffff)));
            //pack_gh_hh = (h_2_2_c2^*, h_2_1_c2^*)^*
        _amem8((void*)&pack_gh_hh) = _itoll(_pack2((h_2_2_c2) >> 16, (h_2_2_c2  & 0x0000ffff)),
                                      _pack2((h_2_1_c2) >> 16, (h_2_1_c2  & 0x0000ffff)));
        pack_ah_bh_ch_dh = _llto128(pack_ah_bh, pack_ch_dh);
        pack_eh_fh_gh_hh = _llto128(pack_eh_fh, pack_gh_hh);
        //e1=e_1_c1
        //e2=e_2_c1
        //e3=e_1_c2^*
        //e4=e_2_c2^*  _ccmatmpyr1会对e求共轭，所以此处要故意构造共轭矩阵
        _amem8((void*)&pack_e1_e2) = _itoll(_pack2((e_1_c1) >> 16, -(e_1_c1  & 0x0000ffff)),
                                      _pack2((e_2_c1) >> 16, -(e_2_c1  & 0x0000ffff)));
        _amem8((void*)&pack_e3_e4) = _itoll(_pack2((e_1_c2) >> 16, (e_1_c2  & 0x0000ffff)),
                                      _pack2((e_2_c2) >> 16, (e_2_c2  & 0x0000ffff)));

        temp1_ll = _ccmatmpyr1(pack_e1_e2, pack_ah_bh_ch_dh);
        temp2_ll = _ccmatmpyr1(pack_e3_e4, pack_eh_fh_gh_hh);
        
        temp3_ll = _dsadd2(temp1_ll, temp2_ll);
        temp7_int = _hill(temp3_ll);    //E
        temp8_int = _loll(temp3_ll);    //F
        //===========================444===========================================================，
        //d = R^-1 * (H^H * e)
        //  [D -B] * [E] = [DE-BF]          Q(32,?),Q(16,?)
        //  [-C A]   [F]   [-CE+AF]
        temp9_int = _cmpyr1(temp4_int, temp7_int);      //DE
        temp10_int = _cmpyr1(temp2_int, temp8_int);      //BF
        temp11_int = _cmpyr1(temp3_int, temp7_int);      //CE
        temp12_int = _cmpyr1(temp1_int, temp8_int);      //AF
        d1_out = _ssub2(temp9_int, temp10_int);
        d2_out = _ssub2(temp12_int, temp11_int);
        
        //20130203
        temp9_int = _mpyhl(d1_out, inverse_final) >> 2;
        temp1_short = (signed short)d1_out;
        temp10_int = _mpy(temp1_short, inverse_final) >> 2;
        d1_out = _pack2(temp9_int, temp10_int);

        temp9_int = _mpyhl(d2_out, inverse_final) >> 2;
        temp1_short = (signed short)d2_out;
        temp10_int = _mpy(temp1_short, inverse_final) >> 2;
        d2_out = _pack2(temp9_int, -temp10_int);                           //jx去负号
        //20130203
        
        d1_out_ptr[2*k] = d1_out;
        d1_out_ptr[2*k+1] = d2_out;
        
        //===========================555===========================================================，
        //LLR = 1/diag(real(R^-1))
        LLR1_ptr[2*k] = 0x7fff;
        LLR1_ptr[2*k+1] = 0x7fff;
  }
}

void chan_eq(signed int*  restrict e_c,     //In, 接收天线的数据
             signed int*  restrict h_c,   //In, 第1根接收天线的第1个流的第1个子载波，a=h_1_1_c1
             signed short* symbol_start,
             signed int* restrict out_ptr,  //Out, d1Q(16,3)
             signed int ueTransMode, /*下行传输模式*/
             signed int* subcarrier_demap_num,
             signed int start_symbol_core,//本core计算的符号开始
             signed int end_symbol_core,//本core计算的符号结束
             signed int m_symbol,
             signed int chan_type,
             signed short* restrict LLR_ptr     //Out,LLr结果首地址Q(16,1)
            )
{
    signed int used_len,offset;
    signed int * restrict out_ptr1;
    signed int * restrict out_ptr2;
    signed int * restrict e_1_c;
    signed int * restrict e_2_c; 
    signed int * restrict h_1_1_c;
    signed int * restrict h_1_2_c;
    signed int * restrict h_2_1_c;
    signed int * restrict h_2_2_c;
    signed short * restrict llr_ptr1;
    signed short * restrict llr_ptr2;
    signed int i;
    
    used_len = 0;
    
    if(SPATIAL_MULTIPLEXING == ueTransMode)
    {
        out_ptr1 = out_ptr;
        out_ptr2 = &out_ptr[N_SC_PER_RB * N_DL_RB * N_SYM_PER_CORE];
        e_1_c = e_c;
        e_2_c = &e_c[N_SYM_PER_CORE * N_DL_RB * N_SC_PER_RB];
        llr_ptr1 = LLR_ptr;
        llr_ptr2 = &LLR_ptr[N_SYM_PER_CORE * N_DL_RB * N_SC_PER_RB];
        h_1_1_c = h_c;
        h_1_2_c = &h_c[N_SYM_PER_CORE * N_SC_PER_RB * N_DL_RB];
        h_2_1_c = &h_c[NUM_MAX_ANTENNA * N_SYM_PER_CORE * N_SC_PER_RB * N_DL_RB];
        h_2_2_c = &h_c[NUM_MAX_ANTENNA * N_SYM_PER_CORE * N_SC_PER_RB * N_DL_RB + N_SYM_PER_CORE * N_SC_PER_RB * N_DL_RB];
        
        
        for(i = start_symbol_core;i < end_symbol_core;i++)
        {
            //for(j = 0;j < subcarrier_demap_num[i];j++)
            //{
                //offset = i * N_DL_RB * N_SC_PER_RB + j;
                offset = (i - start_symbol_core)  * N_DL_RB * N_SC_PER_RB + symbol_start[i];
                chan_eq_sdm(&e_1_c[offset],     //In, 第1根接收天线的第1个子载波，e1=e_1_c1
                            &e_2_c[offset],     //In, 第2根接收天线的第1个子载波，e2=e_2_c1
                            &h_1_1_c[offset],   //In, 第1根接收天线的第1个流的第1个子载波，a=h_1_1_c1
                            &h_1_2_c[offset],   //In, 第1根接收天线的第2个流的第1个子载波，b=-h_1_2_c1
                            &h_2_1_c[offset],   //In, 第2根接收天线的第1个流的第1个子载波，c=h_2_1_c1
                            &h_2_2_c[offset],   //In, 第2根接收天线的第2个流的第1个子载波，d=-h_2_2_c1
                            //j & 1,  //Para, 结合码本和i给出wdu_index，0-5
                            subcarrier_demap_num[i],
                            &out_ptr1[used_len],  //Out, d1
                            &out_ptr2[used_len],  //Out, d2
                            &llr_ptr1[used_len],     //Out,LLr结果首地址
                            &llr_ptr2[used_len]     //Out,LLr结果首地址
                            );
                //used_len ++;
                used_len += subcarrier_demap_num[i];
            //}
        }
        
    } 
    else if(TRANSMIT_DIVERSITY == ueTransMode)
    {
        if(PDSCH == chan_type)
        {
            out_ptr1 = out_ptr;
            //out_ptr2 = out_ptr + m_symbol;
            e_1_c = e_c;
            e_2_c = &e_c[N_SYM_PER_CORE * N_DL_RB * N_SC_PER_RB];
            llr_ptr1 = LLR_ptr;
            //llr_ptr2 = LLR_ptr + m_symbol;
            h_1_1_c = h_c;
            h_2_1_c = &h_c[N_SYM_PER_CORE * N_SC_PER_RB * N_DL_RB];
            h_1_2_c = &h_c[NUM_MAX_ANTENNA * N_SYM_PER_CORE * N_SC_PER_RB * N_DL_RB];
            h_2_2_c = &h_c[NUM_MAX_ANTENNA * N_SYM_PER_CORE * N_SC_PER_RB * N_DL_RB + N_SYM_PER_CORE * N_SC_PER_RB * N_DL_RB];
        }
        else if((PDCCH == chan_type) || (PHICH == chan_type))//PHICH复用PDCCH的buffer
        {
          out_ptr1 = out_ptr;
          //out_ptr2 = out_ptr + m_symbol;
          e_1_c = e_c;
          e_2_c = &e_c[m_symbol];
            llr_ptr1 = LLR_ptr;
          //llr_ptr2 = LLR_ptr + m_symbol;
          h_1_1_c = h_c;
          h_2_1_c = &h_c[m_symbol];
          h_1_2_c = &h_c[NUM_MAX_ANTENNA * 3200];
          h_2_2_c = &h_c[NUM_MAX_ANTENNA * 3200 + m_symbol];
        }
        else if(PBCH == chan_type)
        {
          out_ptr1 = out_ptr;
          //out_ptr2 = out_ptr + m_symbol;
          e_1_c = e_c;
          e_2_c = &e_c[m_symbol];
            llr_ptr1 = LLR_ptr;
          //llr_ptr2 = LLR_ptr + m_symbol;
          h_1_1_c = h_c;
          h_2_1_c = &h_c[m_symbol];
          h_1_2_c = &h_c[NUM_MAX_ANTENNA * N_PBCH_SYMBOL];
          h_2_2_c = &h_c[NUM_MAX_ANTENNA * N_PBCH_SYMBOL + m_symbol];
        }
        else if(PCFICH == chan_type)
        {
          out_ptr1 = out_ptr;
          //out_ptr2 = out_ptr + m_symbol;
          e_1_c = e_c;
          e_2_c = &e_c[m_symbol];
          llr_ptr1 = LLR_ptr;
          //llr_ptr2 = LLR_ptr + m_symbol;
          h_1_1_c = h_c;
          h_2_1_c = &h_c[m_symbol];
          h_1_2_c = &h_c[NUM_MAX_ANTENNA * 16];
          h_2_2_c = &h_c[NUM_MAX_ANTENNA * 16 + m_symbol];
        }

        for(i = start_symbol_core;i < end_symbol_core;i++)
        {
            offset = (i - start_symbol_core) * N_DL_RB * N_SC_PER_RB + symbol_start[i];
            chan_eq_sfbc(&e_1_c[offset],     //In, 第1根接收天线的第1个子载波，e1=e_1_c1
                         &e_2_c[offset],     //In, 第2根接收天线的第1个子载波，e2=e_2_c1
                         &h_1_1_c[offset],   //In, 第1根接收天线的第1个流的第1个子载波，a=h_1_1_c1
                         &h_1_2_c[offset],   //In, 第1根接收天线的第2个流的第1个子载波，b=-h_1_2_c1
                         &h_2_1_c[offset],   //In, 第2根接收天线的第1个流的第1个子载波，c=h_2_1_c1
                         &h_2_2_c[offset],   //In, 第2根接收天线的第2个流的第1个子载波，d=-h_2_2_c1
                         subcarrier_demap_num[i],
                         &out_ptr1[used_len],  //Out, d1
                         //&out_ptr2[used_len],  //Out, d2
                         &llr_ptr1[used_len],     //Out,LLr结果首地址
                         //&llr_ptr2[used_len]     //Out,LLr结果首地址
                         chan_type

                         );
            
            used_len += subcarrier_demap_num[i];
        }
    }
}

