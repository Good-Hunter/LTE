/***********************************************/
/*phich_detect.c                               */
/*���ܣ�phich�������Ž������                  */
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
   int    spreading_factor[12] = {0}; /*��Ч��Ƶ����*/
   unsigned  int    i = 0;              /*ѭ������*/
   unsigned  int    phich_scrambling[12] = {0}; /*����*/
   short  est_data[12*2] = {0}; /*��������Ⱥ�Ľ��*/
   int    sum_data[3*2] = {0};  /*ÿ����Ƶ�������ĺ�*/
   int    temp_real = 0;     /*��ʱ����*/
   int    temp_imag = 0;     /*��ʱ����*/
   int    temp = 0;          /*��ʱ����*/
   int    *int_phich_data = ( int *)phich_data_ptr;/*����ָ��*/
   int    *temp_llr_data = (int *)phich_llr_ptr; /*�����洢ƽ��llrֵ*/
    for (i = 0; i < 12; i++)
    {
        phich_scrambling[i] = _extu(*scrambling_code,i,31);
    }
    /*��0��1 1 1 1*/
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
    /*��1��1 -1 1 -1*/
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
    /*��2��1 1 -1 -1*/
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
    /*��3��1 -1 -1 1*/
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
    /*��4��j j j j*/
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
    /*��5��j -j j -j*/
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
    /*��6��j j -j -j*/
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
      /*��7��j -j -j j*/
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
    /*��������Ч��Ƶ����*/
    /*������Ƶ���ӳ�������ȡ����*/
    for (i = 0;i < 12; i++)
    {
       temp_real = (1 - 2* phich_scrambling[i])* w[2* (i%4)];
       temp_imag = - (1 - 2* phich_scrambling[i])* w[2* (i%4) + 1];
       /*�ֱ�ȡ��ʵ�����鲿*/
       spreading_factor[i] = _pack2(temp_real,temp_imag);
    }
    /*���õ�Ч��Ƶ���������������Ľ��*/
    for (i = 0;i < 12; i++)
    {
        /*Q(16,3)*Q(16,16)>>2->Q(32,21)*/
       est_data[2* i] = _dotpn2(int_phich_data[i],spreading_factor[i]) >>2;
       est_data[2* i + 1] = _dotp2(int_phich_data[i],_packlh2(spreading_factor[i],spreading_factor[i])) >>2;
    }
    /*���3��bit��hi��Ϣ*/
    for (i = 0;i < 3; i++)
    {
        /*��Ҫ����4*/
        sum_data[2* i] = (est_data[8* i] + est_data[8* i + 2] + est_data[8* i + 4] + est_data[8* i + 6]) >>2;
        sum_data[2* i + 1] = (est_data[8* i + 1] + est_data[8* i + 3] + est_data[8* i + 5] + est_data[8* i + 7])>>2;
    }
    /*����3��bit��ת135��*/
    for (i = 0;i < 3; i++)
    {
        temp = _pack2(sum_data[2* i],sum_data[2* i + 1]);
      /*Q(16,5)*Q(16,1)->Q(32,6)*/
        temp = _dotpn2(temp,0xA5825A7E);
        /*��󽫽�������llr�������*/
      /*Q(16,6)*Q(16,1)->Q(32,7)<<13->Q(32,-6),ȡ��8λΪQ(8,-6)*/
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
