/***********************************************/
/*rsgen                                        */
/*功能：完成小区专用参考信号生成               */
/***********************************************/
#include "pl_comm.h"

/*Pseudo_random_sequences_gen：完成伪随机序列生成*/
void Pseudo_random_sequences_gen(unsigned char  *gold_x1_byte_table,                        
                                  unsigned int c_init,
                                  unsigned char  *c_basic_sequence_Output,
                                  unsigned char num_of_words
                                  )
{
    unsigned int x2_seed;
    unsigned char i;
    unsigned int c_sequence;
    char j;
    unsigned short k = 0;

    unsigned int x2_seed_r1, x2_seed_r2, x2_seed_r3, x2_seed_r4;

    unsigned int * restrict x1_word = (unsigned int * restrict)gold_x1_byte_table;


    x2_seed = c_init;

    /*根据c_init 计算X2 Sequence 从 X2(31) 到 X2(1599)*/
    /*30+31*50达到 X2(1580)*/
    for (i = 0; i < 50; i++)
    {
        x2_seed_r1 = x2_seed >> 1;
        x2_seed_r2 = x2_seed >> 2;
        x2_seed_r3 = x2_seed >> 3;
        /*X2(n+31)=[X2(n+3)^X2(n+2)^X2(n+1)^X2(n)^]mod2*/
        /* 0    b30  b29  b28      ...     b3  b2  b1  b0     x2_seed */
        /* 0    0    b30  b29      ...     b4  b3  b2  b1     x2_seed_r1 */
        /* 0    0    0    b30      ...     b5  b4  b3  b2     x2_seed_r2 */
        /* 0    0    0    0        ...     b6  b5  b4  b3     x2_seed_r3 */
        /* 0    t2   t1   t0       ...     b34 b33 b32 b31    x2_seed */
        x2_seed = x2_seed ^ x2_seed_r1 ^ x2_seed_r2 ^ x2_seed_r3;
        /* 0    t2   t1   t0       ...     b34 b33 b32 b31    x2_seed */
        /* 0    b33  b32  b31      ...      0   0   0   0     x2_seed_r1 */
        /* 0    b32  b31  0        ...      0   0   0   0     x2_seed_r2 */
        /* 0    b31  0    0        ...      0   0   0   0     x2_seed_r3 */
        /* 0    b61  b60  b59      ...     b34 b33 b32 b31    x2_seed */
        x2_seed_r1 = x2_seed << 29;
        x2_seed_r2 = x2_seed << 30;
        x2_seed_r3 = x2_seed << 31;
        x2_seed_r1 = x2_seed_r1 >> 1;
        x2_seed_r2 = x2_seed_r2 >> 1;
        x2_seed_r3 = x2_seed_r3 >> 1;
        x2_seed = x2_seed ^ x2_seed_r1 ^ x2_seed_r2 ^ x2_seed_r3;
    }
    /*X2(1580)到X2(1600)*/
    /* 0    b1580  b1579  b1578  b1577    ...     b1553 b1552 b1551 b1550    x2_seed */
    /* 0    0      b1580  b1579  b1578    ...     b1554 b1553 b1552 b1551    x2_seed_r1 */
    /* 0    0      0      b1580  b1579    ...     b1555 b1554 b1553 b1552    x2_seed_r2 */
    /* 0    0      0      0      b1580    ...     b1556 b1555 b1554 b1553    x2_seed_r3 */
    /* 0    t2     t1     t0     b1608    ...     b1584 b1583 b1582 b1581    x2_seed_r4 */
    x2_seed_r4 = x2_seed;
    x2_seed_r1 = x2_seed >> 1;
    x2_seed_r2 = x2_seed >> 2;
    x2_seed_r3 = x2_seed >> 3;
    x2_seed_r4 = x2_seed_r4 ^ x2_seed_r1 ^ x2_seed_r2 ^ x2_seed_r3;
    /* b1600 b1599 b1598 b1597   ...  b1584 b1583 b1582 b1581  ...  0     0     0     0        x2_seed_r4 */
    /* 0     0     0     0       ...  0     0     0     0      ...  b1572 b1571 b1570 b1569    x2_seed */
    /* b1600 b1599 b1598 b1597   ...  b1584 b1583 b1582 b1581  ...  b1572 b1571 b1570 b1569    x2_seed */
    x2_seed_r4 = x2_seed_r4 << 12;
    x2_seed = x2_seed >> 19;
    //x2_seed_r4 = x2_seed_r4 >> 1;
    x2_seed = x2_seed | x2_seed_r4;

    //x2_seed = (x2_seed | (((x2_seed & 0x01) ^ (x2_seed >> 1 & 0x01) ^ (x2_seed
    //                    >> 2 & 0x01) ^ (x2_seed >> 3 & 0x01)) << 31));

    /* reverse the bit order.*/
    x2_seed = _bitr(x2_seed);


    if (num_of_words != 0) 
    {
        for (i = 0; i < num_of_words; i++) 
        {  
            /*C Sequence: C(n) = X1(n+1600)^X2(n+1600)*/
            //Scrambled Output = C(i)^Input(i)

            /* b1569  b1570  b1571  b1572      ...     b1597 b1598 b1599 b1600     x2_seed */
            /* b1570  b1571  b1572  b1573      ...     b1598 b1599 b1600 0         x2_seed_r1 */
            /* b1571  b1572  b1573  b1574      ...     b1599 b1600 0     0         x2_seed_r2 */
            /* b1572  b1573  b1574  b1575      ...     b1600 0     0     0         x2_seed_r3 */
            x2_seed_r1 = x2_seed << 1;
            x2_seed_r2 = x2_seed << 2;
            x2_seed_r3 = x2_seed << 3;

            /* b1600  b1601  b1602 b1603       ...     b1628 t0    t1    t2        x2_seed */
            x2_seed = x2_seed ^ x2_seed_r1 ^ x2_seed_r2 ^ x2_seed_r3;

            /* b1601  b1602 b1603  b1604       ...     t0  t1      t2      0        x2_seed_r4 */
            x2_seed_r4 = x2_seed << 1;
            /* 0      0     0      0           ...     0   b1601   b1602   b1603    x2_seed_r3 */
            /* 0      0     0      0           ...     0   0       b1601   b1602    x2_seed_r2 */
            /* 0      0     0      0           ...     0   0       0       b1601    x2_seed_r3 */
            x2_seed_r1 = x2_seed_r4 >> 29;
            x2_seed_r2 = x2_seed_r4 >> 30;
            x2_seed_r3 = x2_seed_r4 >> 31;

            /* b1600  b1601  b1602 b1603     ...     b1628  b1629  b1630  b1631      x2_seed */
            x2_seed = x2_seed ^ x2_seed_r1 ^ x2_seed_r2 ^ x2_seed_r3;

            c_sequence = ((x2_seed) ^ (*x1_word++)); /* c(n)=(x1(n+Nc)+x2(n+Nc))mod2 */

            for (j=15;j>0;j-=4)
            {
                c_basic_sequence_Output[k++]= (unsigned char)((c_sequence >> (2*j))& 0x3);
                c_basic_sequence_Output[k++]= (unsigned char)((c_sequence >> (2*j-2))& 0x3);

                c_basic_sequence_Output[k++]= (unsigned char)((c_sequence >> (2*j-4))& 0x3);
                c_basic_sequence_Output[k++]= (unsigned char)((c_sequence >> (2*j-6))& 0x3);
            }

            /* b1601 b1602 b1603 b1604    ...     b1629  b1630  b1631 b1632     x2_seed */
            x2_seed = ((x2_seed << 1)|(((x2_seed << 4)^(x2_seed << 1)^
            (x2_seed << 2)^(x2_seed << 3))>> 31));
        }

    }
}

/*rs_sequences_gen：完成小区参考信号生成*/
void rs_sequences_gen(unsigned char  *restrict rs_sequence_Output,/*最多20slot*3symbol*220char,存储的是实虚部共同索引*/
                      unsigned short Ncell_id,
                      unsigned char N_cp
                      )
{
    unsigned char num_of_words;
    unsigned char OFDMA_idx_l;
    unsigned char i;
    unsigned char j;
    unsigned int p;
    unsigned int q;
    unsigned int r;
    unsigned int s;
    unsigned int Cinit;
    num_of_words = (N_MAX_DL_RB * 4 + 31) >> 5;/*4=2*2 2:IQ,2:2*N_MAX_DL_RB，四舍五入*/
    

    p = (unsigned int)(Ncell_id << 1);
    q = p + N_cp;
    for (i = 0; i < 20; i++)
    {

        for (j = 0; j < 3; j ++)
        {
            /* clear num of pilots per symbol info: needed to calculate offset */
            OFDMA_idx_l = rs_symbol_index_per_slot[j];
            /* pilot mapping on 0th OFDM symbol in a slot*/
            r = (7 * (i + 1) + OFDMA_idx_l + 1)*(p + 1);
            s = r << 10;
            Cinit = (unsigned int)(s + q);
            Pseudo_random_sequences_gen (gold_x1_byte_table, Cinit, &rs_sequence_Output[i * 3 * 220 + j * 220],num_of_words);
        }
    }
}
