/***********************************************/
/*phich_detect.c                               */
/*功能：phich解扩解扰解调函数                  */
/***********************************************/
void f_phich_det(
     int      n_phich_group,
     int      n_phich_seq,
     unsigned  int     *scrambling_code,
     short      *phich_data_ptr,
     short      *phich_llr_ptr,
     int       *demond_data
    )
{
   int    w[4*2] = {0};
   int    spreading_factor[12] = {0}; /*等效扩频因子*/
   unsigned  int    i = 0;              /*循环变量*/
   unsigned  int    phich_scrambling[12] = {0}; /*扰码*/
   short  est_data[12*2] = {0}; /*乘以扰码等后的结果*/
   int    sum_data[3*2] = {0};  /*每个扩频码解扩后的和*/
   int    temp_real = 0;     /*临时变量*/
   int    temp_imag = 0;     /*临时变量*/
   int    temp = 0;          /*临时变量*/
   int    *int_phich_data = ( int *)phich_data_ptr;/*整型指针*/
   int    *temp_llr_data = (int *)phich_llr_ptr; /*用来存储平均llr值*/
    for (i = 0; i < 12; i++)
    {
        phich_scrambling[i] = _extu(*scrambling_code,i,31);
    }
    /*第0组1 1 1 1*/
    if(0 == n_phich_seq)
    {
        w[0] = 1;
        w[1] = 0;
        w[2] = 1;
        w[3] = 0;
        w[4] = 1;
        w[5] = 0;
        w[6] = 1;
        w[7] = 0;
    }
    /*第1组1 -1 1 -1*/
    else if (1 == n_phich_seq)
    {
        w[0] = 1;
        w[1] = 0;
        w[2] = -1;
        w[3] = 0;
        w[4] = 1;
        w[5] = 0;
        w[6] = -1;
        w[7] = 0;
    }
    /*第2组1 1 -1 -1*/
    else if (2 == n_phich_seq)
    {
        w[0] = 1;
        w[1] = 0;
        w[2] = 1;
        w[3] = 0;
        w[4] = -1;
        w[5] = 0;
        w[6] = -1;
        w[7] = 0;
    }
    /*第3组1 -1 -1 1*/
    else if (3 == n_phich_seq)
    {
        w[0] = 1;
        w[1] = 0;
        w[2] = -1;
        w[3] = 0;
        w[4] = -1;
        w[5] = 0;
        w[6] = 1;
        w[7] = 0;
    }
    /*第4组j j j j*/
    else if (4 == n_phich_seq)
    {
        w[0] = 0;
        w[1] = 1;
        w[2] = 0;
        w[3] = 1;
        w[4] = 0;
        w[5] = 1;
        w[6] = 0;
        w[7] = 1;
    }
    /*第5组j -j j -j*/
    else if (5 == n_phich_seq)
    {
        w[0] = 0;
        w[1] = 1;
        w[2] = 0;
        w[3] = -1;
        w[4] = 0;
        w[5] = 1;
        w[6] = 0;
        w[7] = -1;
    }
    /*第6组j j -j -j*/
    else if (6 == n_phich_seq)
    {
        w[0] = 0;
        w[1] = 1;
        w[2] = 0;
        w[3] = 1;
        w[4] = 0;
        w[5] = -1;
        w[6] = 0;
        w[7] = -1;
    }
      /*第7组j -j -j j*/
    else if (7 == n_phich_seq)
    {
        w[0] = 0;
        w[1] = 1;
        w[2] = 0;
        w[3] = -1;
        w[4] = 0;
        w[5] = -1;
        w[6] = 0;
        w[7] = 1;
    }
    /*下面计算等效扩频因子*/
    /*等于扩频因子乘以扰码取共厄*/
    for (i = 0;i < 12; i++)
    {
       temp_real = (1 - 2* phich_scrambling[i])* w[2* (i%4)];
       temp_imag = - (1 - 2* phich_scrambling[i])* w[2* (i%4) + 1];
       /*分别取出实部和虚部*/
       spreading_factor[i] = _pack2(temp_real,temp_imag);
    }
    /*利用等效扩频因子来计算解扩后的结果*/
    for (i = 0;i < 12; i++)
    {
        /*Q(16,3)*Q(16,16)>>2->Q(32,21)*/
       est_data[2* i] = _dotpn2(int_phich_data[i],spreading_factor[i]) >>2;
       est_data[2* i + 1] = _dotp2(int_phich_data[i],_packlh2(spreading_factor[i],spreading_factor[i])) >>2;
    }
    /*求出3个bit的hi信息*/
    for (i = 0;i < 3; i++)
    {
        /*需要除以4*/
        sum_data[2* i] = (est_data[8* i] + est_data[8* i + 2] + est_data[8* i + 4] + est_data[8* i + 6]) >>2;
        sum_data[2* i + 1] = (est_data[8* i + 1] + est_data[8* i + 3] + est_data[8* i + 5] + est_data[8* i + 7])>>2;
    }
    /*将这3个bit旋转135度*/
    for (i = 0;i < 3; i++)
    {
        temp = _pack2(sum_data[2* i],sum_data[2* i + 1]);
      /*Q(16,5)*Q(16,1)->Q(32,6)*/
        temp = _dotpn2(temp,0xA5825A7E);
        /*最后将解调结果和llr数据相乘*/
      /*Q(16,6)*Q(16,1)->Q(32,7)<<13->Q(32,-6),取高8位为Q(8,-6)*/
        //demond_data[i] = (char)(_mpyhl(temp,temp_llr_data[i])>>11);
        demond_data[i] = temp;
    }

    return;
}
unsigned char f_harq_ind_decode( int *harq_ind_in)
{
    int sum;

    sum = harq_ind_in[0] + harq_ind_in[1] + harq_ind_in[2];

    if (sum > 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
