/***********************************************/
/*cc_conv_derate_matching.c                                */
/*功能：卷积码解速率匹配                     */
/***********************************************/
#include "system_init.h"
#define NULL -100
#define C_SUBBLOCK_TC 32
#define PREFIXSIZE 24  //cs 15/05/12
void cc_conv_derate_matching(signed char* restrict data_for_derm,
                             int K,
                             int E,
                             char* restrict s,
                             char* restrict y1,
                             char* restrict y2)
{
    int D,r_subblock_tc,N_D,k_w,K_pai,temp;
    int i,k,index;
    char s_array[((MAX_COV_TBS + C_SUBBLOCK_TC - 1)/ C_SUBBLOCK_TC) * C_SUBBLOCK_TC];
    char y1_array[((MAX_COV_TBS + C_SUBBLOCK_TC - 1)/ C_SUBBLOCK_TC) * C_SUBBLOCK_TC];
    char y2_array[((MAX_COV_TBS + C_SUBBLOCK_TC - 1)/ C_SUBBLOCK_TC) * C_SUBBLOCK_TC];
    char P[C_SUBBLOCK_TC] = {1,17,9,25,5,21,13,29,3,19,11,27,7,23,15,31,0,16,8,24,4,20,12,28,2,18,10,26,6,22,14,30};
    char pattern_01[((MAX_COV_TBS + C_SUBBLOCK_TC - 1)/ C_SUBBLOCK_TC) * C_SUBBLOCK_TC];
    unsigned int w_array[3 * ((MAX_COV_TBS + C_SUBBLOCK_TC - 1)/ C_SUBBLOCK_TC) * C_SUBBLOCK_TC];

    D = K;// cs 15/01/14  47
    r_subblock_tc = ((D + 31) >> 5);//ceil(D/C_subblock_TC) cs 15/01/14 求行数 2
    N_D = r_subblock_tc * C_SUBBLOCK_TC - D;// cs 15/01/14  17
    K_pai = r_subblock_tc * C_SUBBLOCK_TC;// cs 15/01/14  64

    memset(s_array,0,r_subblock_tc * C_SUBBLOCK_TC);
    memset(y1_array,0,r_subblock_tc * C_SUBBLOCK_TC);
    memset(y2_array,0,r_subblock_tc * C_SUBBLOCK_TC);


    for(i = 0;i < N_D;i++)
    {
        s_array[i] = NULL;
        y1_array[i] = NULL;
        y2_array[i] = NULL;
    }

    for(i = 0;i < K_pai;i++)
    {
        index = i / r_subblock_tc;// cs 15/01/14 calcaulate the 列号，依次为0-31 [0,1,2,...,31,0,1,2,...,31]
        temp = C_SUBBLOCK_TC * (i % r_subblock_tc);// cs 15/01/14 可理解为相对于行号为0的相应列的位置偏移，为0或32，index与temp构成了一个元素的行列索引号，可定位一个元素

        pattern_01[i] = (P[index] + temp) %  K_pai; //cs pattern_01[i]存储将单行变为2*32行列的行号{1,17,9,25,5,21,13,29,3,19,11,27,7,23,15,31,0,16,8,24,4,20,12,28,2,18,10,26,6,22,14,30***33,49,41,57,...,62};
    }
    for(i = 0;i < K_pai;i++)
    {
        w_array[i] = s_array[pattern_01[i]];// cs 15/01/14将三路交织后的数据都合并到 w_array[i]，目的为：data_for_derm[k]中的数据是交织过的，我们也把w_array经过相同的交织处理，使之与data_for_derm[k]具有相同的构造，用来承接data_for_derm[k]中的数据
        w_array[i + K_pai] = y1_array[pattern_01[i]];
        w_array[i + 2 * K_pai] = y2_array[pattern_01[i]];
    }

    k_w = 3 * K_pai;//cs 3*64=192
    k = 0;
    i = 0;
    while(k < E)//cs E=72
    {
        index = i % k_w;
        if (NULL != w_array[index])//cs 此时w_array[index]中只含有NULL or 0，if判别之后去除NULL,全为0
        {
            w_array[index] = _sadd(w_array[index],(data_for_derm[k] << 24));//cs _sadd为相加并饱和，实际上w_array[index]全为0，代表含义为将data_for_derm[k] << 24饱和之后赋值给w_array[index]
            k++;
        }
        i++;
    }
    for(i = 0;i < K_pai;i++)
    {
        s_array[pattern_01[i]] = w_array[i] >> 24;//cs 将w_array[i]中的数据解交织，还原到三路信号中
        y1_array[pattern_01[i]] = w_array[i + K_pai] >> 24;
        y2_array[pattern_01[i]] = w_array[i + 2 * K_pai] >> 24;
    }
    for(i = 0;i < K_pai - N_D;i++)
    {
        s[i] = s_array[i + N_D];
        y1[i] = y1_array[i + N_D];
        y2[i] = y2_array[i + N_D];
    }


}



#if 0
void cc_conv_pdcch_derate_matching(signed char* restrict data_for_derm,
                             int K,
                             int E,
                             char* restrict s,
                             char* restrict y1,
                             char* restrict y2,
                             Int8 * restrict output)//cs 15/01/20 增加一个输出数组，存储3路数据，按混合顺序存
{

    int D,r_subblock_tc,N_D,k_w,K_pai,temp;
    int i,k,index;
    char s_array[((MAX_COV_TBS + C_SUBBLOCK_TC - 1)/ C_SUBBLOCK_TC) * C_SUBBLOCK_TC];
    char y1_array[((MAX_COV_TBS + C_SUBBLOCK_TC - 1)/ C_SUBBLOCK_TC) * C_SUBBLOCK_TC];
    char y2_array[((MAX_COV_TBS + C_SUBBLOCK_TC - 1)/ C_SUBBLOCK_TC) * C_SUBBLOCK_TC];
    char P[C_SUBBLOCK_TC] = {1,17,9,25,5,21,13,29,3,19,11,27,7,23,15,31,0,16,8,24,4,20,12,28,2,18,10,26,6,22,14,30};
    char pattern_01[((MAX_COV_TBS + C_SUBBLOCK_TC - 1)/ C_SUBBLOCK_TC) * C_SUBBLOCK_TC];
    unsigned int w_array[3 * ((MAX_COV_TBS + C_SUBBLOCK_TC - 1)/ C_SUBBLOCK_TC) * C_SUBBLOCK_TC];
    
    D = K;// cs 15/01/14  47
    r_subblock_tc = ((D + 31) >> 5);//ceil(D/C_subblock_TC) cs 15/01/14 求行数 2
    N_D = r_subblock_tc * C_SUBBLOCK_TC - D;// cs 15/01/14  17
    K_pai = r_subblock_tc * C_SUBBLOCK_TC;// cs 15/01/14  64
    
    memset(s_array,0,r_subblock_tc * C_SUBBLOCK_TC);
    memset(y1_array,0,r_subblock_tc * C_SUBBLOCK_TC);
    memset(y2_array,0,r_subblock_tc * C_SUBBLOCK_TC);
    //for(i=0;i<E;i++)
    	//data_for_derm[i]=i;  //测试用
    
    for(i = 0;i < N_D;i++)
    {
        s_array[i] = NULL;
        y1_array[i] = NULL;
        y2_array[i] = NULL;
    }
    
    for(i = 0;i < K_pai;i++)
    {
        index = i / r_subblock_tc;// cs 15/01/14 calcaulate the 列号，依次为0-31 [0,1,2,...,31,0,1,2,...,31]    cs 15/06/18 [0,0,1,1,2,2,...,31,31]
        temp = C_SUBBLOCK_TC * (i % r_subblock_tc);// cs 15/01/14 可理解为相对于行号为0的相应列的位置偏移，为0或32，index与temp构成了一个元素的行列索引号，可定位一个元素
        
        pattern_01[i] = (P[index] + temp) %  K_pai; //cs pattern_01[i]存储将单行变为2*32行列的行号{1,,33,17,49,9,41,25,57,.....5,21,13,29,3,19,11,27,7,23,15,31,0,16,8,24,4,20,12,28,2,18,10,26,6,22,14,30***33,49,41,57,...,62};
    }
    for(i = 0;i < K_pai;i++)
    {
        w_array[i] = s_array[pattern_01[i]];// cs 15/01/14将三路交织后的数据都合并到 w_array[i]，目的为：data_for_derm[k]中的数据是交织过的，我们也把w_array经过相同的交织处理，使之与data_for_derm[k]具有相同的构造，用来承接data_for_derm[k]中的数据
        w_array[i + K_pai] = y1_array[pattern_01[i]];
        w_array[i + 2 * K_pai] = y2_array[pattern_01[i]];
    }
    
    k_w = 3 * K_pai;//cs 3*64=192
    k = 0;
    i = 0;
    while(k < E)//cs E=72
    {
        index = i % k_w;
        if (NULL != w_array[index])//cs 此时w_array[index]中只含有NULL or 0，if判别之后去除NULL,全为0
        {
            w_array[index] = _sadd(w_array[index],(data_for_derm[k] << 24));//cs _sadd为相加并饱和，实际上w_array[index]全为0，代表含义为将data_for_derm[k] << 24饱和之后赋值给w_array[index]
            k++;
        }
        i++;
    }
    for(i = 0;i < K_pai;i++)
    {
        s_array[pattern_01[i]] = w_array[i] >> 24;//cs 将w_array[i]中的数据解交织，还原到三路信号中
        y1_array[pattern_01[i]] = w_array[i + K_pai] >> 24;
        y2_array[pattern_01[i]] = w_array[i + 2 * K_pai] >> 24;
    }
#if 0
    for(i = 0;i < K_pai - N_D;i++)
    {
        s[i] = s_array[i + N_D];
        y1[i] = y1_array[i + N_D];
        y2[i] = y2_array[i + N_D];
        (i+PREFIXSIZE)
        output[PREFIXSIZE+i*3]=s[i];
        output[PREFIXSIZE+i*3+1]=y1[i];
        output[PREFIXSIZE+i*3+2]=y2[i];

        output[i*3]=s_array[D-PREFIXSIZE+i+ N_D];
        output[i*3+1]=y1_array[D-PREFIXSIZE+i+ N_D];
        output[i*3+2]=y2_array[D-PREFIXSIZE+i+ N_D];
        output[(PREFIXSIZE+D+i)*3]=s[i];
        output[(PREFIXSIZE+D+i)*3+1]=y1[i];
        output[(PREFIXSIZE+D+i)*3+2]=y2[i];
        /*
        output[i*4]=0;
        output[i*4+1]=s[i];
        output[i*4+2]=y1[i];
        output[i*4+3]=y2[i];
        */

    }
#endif
    for(i = 0;i < PREFIXSIZE;i++)
        {
            s[i] = s_array[i + N_D];
            y1[i] = y1_array[i + N_D];
            y2[i] = y2_array[i + N_D];

            output[PREFIXSIZE*3+i*3]=s[i];//cs 15/05/12 the local address
            output[PREFIXSIZE*3+i*3+1]=y1[i];
            output[PREFIXSIZE*3+i*3+2]=y2[i];

            output[i*3]=s_array[D-PREFIXSIZE+i+ N_D];//cs 15/05/12 the prefix address stores the last PREFIXSIZE of the s[i]  s_array[40]---s_array[63]
            output[i*3+1]=y1_array[D-PREFIXSIZE+i+ N_D];
            output[i*3+2]=y2_array[D-PREFIXSIZE+i+ N_D];
            output[(PREFIXSIZE+D+i)*3]=s[i];
            output[(PREFIXSIZE+D+i)*3+1]=y1[i];
            output[(PREFIXSIZE+D+i)*3+2]=y2[i];
            /*
            output[i*4]=0;
            output[i*4+1]=s[i];
            output[i*4+2]=y1[i];
            output[i*4+3]=y2[i];
            */

        }
    for(i=PREFIXSIZE;i<D;i++)
    {
    	s[i] = s_array[i + N_D];
    	y1[i] = y1_array[i + N_D];
    	y2[i] = y2_array[i + N_D];

    	output[PREFIXSIZE*3+i*3]=s[i];
    	output[PREFIXSIZE*3+i*3+1]=y1[i];
    	output[PREFIXSIZE*3+i*3+2]=y2[i];

    }
    
}
#endif

/***********************************************/
/*cc_conv_derate_matching.c                                */
/*功能：卷积码解速率匹配                     */
/***********************************************/



//查表20M和10M的情况
#include "TSC.h"
#define null 1

extern short drm20m1a1table[285];//5K lift 105K SL2
extern short drm20m1a2table[285];
extern short drm20m1a4table[285];
extern short drm20m1a8table[285];
extern short drm20m2a1table[345];
extern short drm20m2a2table[345];
extern short drm20m2a4table[345];
extern short drm20m2a8table[345];
extern short drm10m1a1table[276];
extern short drm10m1a2table[276];
extern short drm10m1a4table[276];
extern short drm10m1a8table[276];
extern short drm10m2a1table[321];
extern short drm10m2a2table[321];
extern short drm10m2a4table[321];
extern short drm10m2a8table[321];
extern short drm5m1a1table[273];
extern short drm5m1a2table[273];
extern short drm5m1a4table[273];
extern short drm5m1a8table[273];
extern short drm5m2a1table[309];
extern short drm5m2a2table[309];
extern short drm5m2a4table[309];
extern short drm5m2a8table[309];
void cc_conv_pdcch_table_derate_matching(signed char* restrict data_for_derm,
                             int K,
                             int E,//576速率匹配输出比特数
                             char* restrict s,
                             char* restrict y1,
                             char* restrict y2,
                             char  output[]
                             )
{
    int D,r_subblock_tc,N_D,k_w,K_pai,temp;
    int i,k,index,j;
    char s_array[((MAX_COV_TBS + C_SUBBLOCK_TC - 1)/ C_SUBBLOCK_TC) * C_SUBBLOCK_TC];
    char y1_array[((MAX_COV_TBS + C_SUBBLOCK_TC - 1)/ C_SUBBLOCK_TC) * C_SUBBLOCK_TC];
    char y2_array[((MAX_COV_TBS + C_SUBBLOCK_TC - 1)/ C_SUBBLOCK_TC) * C_SUBBLOCK_TC];
    char P[C_SUBBLOCK_TC] = {1,17,9,25,5,21,13,29,3,19,11,27,7,23,15,31,0,16,8,24,4,20,12,28,2,18,10,26,6,22,14,30};//列变换
    char pattern_01[((MAX_COV_TBS + C_SUBBLOCK_TC - 1)/ C_SUBBLOCK_TC) * C_SUBBLOCK_TC];//存放位置
    /*unsigned int*/ char  w_array[3 * ((MAX_COV_TBS + C_SUBBLOCK_TC - 1)/ C_SUBBLOCK_TC) * C_SUBBLOCK_TC];//比特缓冲后的数据
//    unsigned int e[3 * ((MAX_COV_TBS + C_SUBBLOCK_TC - 1)/ C_SUBBLOCK_TC) * C_SUBBLOCK_TC];//0610刘源//比特选择后数据
    int matchingtime[10]={0},test_time1,test_time2,test_time3,test_time4,test_time5,test_time6;/////////////////////
    int indexre[576];
//    char output[345]={0};


    D = K;//51+16=67  数据长度 或31+16=47
    r_subblock_tc = ((D + 31) >> 5);//ceil(D/C_subblock_TC)//ceil(67/32)=3   行数 或ceil(47/32)=2
    N_D = r_subblock_tc * C_SUBBLOCK_TC - D;//3*32-67=29 末尾补零数 或 2*32-47=17
    K_pai = r_subblock_tc * C_SUBBLOCK_TC;//3*32=96 矩阵元素个数 或 2*32=64
    k_w = 3*K_pai;

    memset(s_array,0,r_subblock_tc * C_SUBBLOCK_TC);//清零
    memset(y1_array,0,r_subblock_tc * C_SUBBLOCK_TC);
    memset(y2_array,0,r_subblock_tc * C_SUBBLOCK_TC);


switch (E)
  {
	case 72:
    {
	if(47==K)
	{
		for(i = 0;i < 3*K+144;i++)//K_pai - N_D=K//去除为了交织而在开头添加的N_D个null，还原成只有数据的三路，数据从第N_D个开始，总共有K_pai - N_D个。
		{
			if(-1 == drm20m1a1table[i])
			{
				output[i] = 0;
			}
			else
			{
				output[i] = data_for_derm[drm20m1a1table[i]];
			}
		}
	}
	if(67==K)
	{
		for(i = 0;i < 3*K+144;i++)//K_pai - N_D=K//去除为了交织而在开头添加的N_D个null，还原成只有数据的三路，数据从第N_D个开始，总共有K_pai - N_D个。
		{
			if(-1 == drm20m2a1table[i])
			{
				output[i] = 0;
			}
			else
			{
				output[i] = data_for_derm[drm20m2a1table[i]];
			}
		}
	}
	if(45==K)//要修正表
	{
		for(i = 0;i < 3*K+144;i++)
		{
			if(-1 == drm10m1a1table[i])
			{
			    output[i] = 0;
			}
			else
			{
				output[i] = data_for_derm[drm10m1a1table[i]];
			}
		}
	}
	if(59==K)
	{
		for(i = 0;i < 3*K+144;i++)
		{
			if(-1 == drm10m2a1table[i])
			{
			    output[i] = 0;
			}
			else
			{
				output[i] = data_for_derm[drm10m2a1table[i]];
			}
		}
	}
	if(43==K)
	{
		for(i = 0;i < 3*K+144;i++)
		{
			if(-1 == drm5m1a1table[i])
			{
			    output[i] = 0;
			}
			else
			{
				output[i] = data_for_derm[drm5m1a1table[i]];
			}
		}
	}
	if(55==K)
	{
		for(i = 0;i < 3*K+144;i++)
		{
			if(-1 == drm5m2a1table[i])
			{
			    output[i] = 0;
			}
			else
			{
				output[i] = data_for_derm[drm5m2a1table[i]];
			}
		}
	}

	break;
    }
	case 144:
    {
	   if(47==K)
	   {
		   for(i = 0;i < 3*K+144;i++)//K_pai - N_D=K//去除为了交织而在开头添加的N_D个null，还原成只有数据的三路，数据从第N_D个开始，总共有K_pai - N_D个。
		   {
			   if(-1 == drm20m1a2table[i])
			   {
				   output[i] = 0;
			   }
			   else
			   {
				   output[i] = data_for_derm[drm20m1a2table[i]];
			   }
		   }
	   }
	   if(67==K)
	   {
			for(i = 0;i < 3*K+144;i++)//K_pai - N_D=K//去除为了交织而在开头添加的N_D个null，还原成只有数据的三路，数据从第N_D个开始，总共有K_pai - N_D个。
			{
				if(-1 == drm20m2a2table[i])
				{
					output[i] = 0;
				}
				else
				{
					output[i] = data_for_derm[drm20m2a2table[i]];
				}
			}
	   }
		if(45==K)
		{
			for(i = 0;i < 3*K+144;i++)
			{
				if(-1 == drm10m1a2table[i])
				{
				    output[i] = 0;
				}
				else
				{
					output[i] = data_for_derm[drm10m1a2table[i]];
				}
			}
		}
		if(59==K)
		{
			for(i = 0;i < 3*K+144;i++)
			{
				if(-1 == drm10m2a2table[i])
				{
				    output[i] = 0;
				}
				else
				{
					output[i] = data_for_derm[drm10m2a2table[i]];
				}
			}
		}
		if(43==K)
		{
			for(i = 0;i < 3*K+144;i++)
			{
				if(-1 == drm5m1a2table[i])
				{
				    output[i] = 0;
				}
				else
				{
					output[i] = data_for_derm[drm5m1a2table[i]];
				}
			}
		}
		if(55==K)
		{
			for(i = 0;i < 3*K+144;i++)
			{
				if(-1 == drm5m2a2table[i])
				{
				    output[i] = 0;
				}
				else
				{
					output[i] = data_for_derm[drm5m2a2table[i]];
				}
			}
		}

		break;
    }
	case 288:
    {
	    if(47==K)
		{
	    	for(i = 0;i < 3*K+144;i++)//K_pai - N_D=K//去除为了交织而在开头添加的N_D个null，还原成只有数据的三路，数据从第N_D个开始，总共有K_pai - N_D个。
			{
				if(-1 == drm20m1a4table[i])
				{
					output[i] = 0;
				}
				else
				{
					output[i] = data_for_derm[drm20m1a4table[i]];
				}
			}
		}
	    if(67==K)
		{
			for(i = 0;i < 3*K+144;i++)//K_pai - N_D=K//去除为了交织而在开头添加的N_D个null，还原成只有数据的三路，数据从第N_D个开始，总共有K_pai - N_D个。
			{
				if(-1 == drm20m2a4table[i])
				{
				   	output[i] = 0;
				}
				else
				{
				   	output[i] = data_for_derm[drm20m2a4table[i]];
				}
			}


		}
		if(45==K)
		{
			for(i = 0;i < 3*K+144;i++)
			{
				if(-1 == drm10m1a4table[i])
				{
				    output[i] = 0;
				}
				else
				{
					output[i] = data_for_derm[drm10m1a4table[i]];
				}
			}
		}
		if(59==K)
		{
			for(i = 0;i < 3*K+144;i++)
			{
				if(-1 == drm10m2a4table[i])
				{
				    output[i] = 0;
				}
				else
				{
					output[i] = data_for_derm[drm10m2a4table[i]];
				}
			}
		}
		if(43==K)
		{
			for(i = 0;i < 3*K+144;i++)
			{
				if(-1 == drm5m1a4table[i])
				{
				    output[i] = 0;
				}
				else
				{
					output[i] = data_for_derm[drm5m1a4table[i]];
				}
			}
		}
		if(55==K)
		{
			for(i = 0;i < 3*K+144;i++)
			{
				if(-1 == drm5m2a4table[i])
				{
				    output[i] = 0;
				}
				else
				{
					output[i] = data_for_derm[drm5m2a4table[i]];
				}
			}
		}
		break;
    }
	case 576:
    {
		if(47==K)
		{
			for(i = 0;i < 3*K+144;i++)//K_pai - N_D=K//去除为了交织而在开头添加的N_D个null，还原成只有数据的三路，数据从第N_D个开始，总共有K_pai - N_D个。
			{
				if(-1 == drm20m1a8table[i])
				{
					output[i] = 0;
				}
				else
				{
					output[i] = data_for_derm[drm20m1a8table[i]];
				}
			}
		}
		if(67==K)
		{
			for(i = 0;i < 3*K+144;i++)//K_pai - N_D=K//去除为了交织而在开头添加的N_D个null，还原成只有数据的三路，数据从第N_D个开始，总共有K_pai - N_D个。
			{
				if(-1 == drm20m2a8table[i])
				{
					output[i] = 0;
				}
				else
				{
					output[i] = data_for_derm[drm20m2a8table[i]];
				}
			}


		}
		if(45==K)
		{
			for(i = 0;i < 3*K+144;i++)
			{
				if(-1 == drm10m1a8table[i])
				{
				    output[i] = 0;
				}
				else
				{
					output[i] = data_for_derm[drm10m1a8table[i]];
				}
			}
		}
		if(59==K)
		{
			for(i = 0;i < 3*K+144;i++)
			{
				if(-1 == drm10m2a8table[i])
				{
				    output[i] = 0;
				}
				else
				{
					output[i] = data_for_derm[drm10m2a8table[i]];
				}
			}
		}
		if(43==K)
		{
			for(i = 0;i < 3*K+144;i++)
			{
				if(-1 == drm5m1a8table[i])
				{
				    output[i] = 0;
				}
				else
				{
					output[i] = data_for_derm[drm5m1a8table[i]];
				}
			}
		}
		if(55==K)
		{
			for(i = 0;i < 3*K+144;i++)
			{
				if(-1 == drm5m2a8table[i])
				{
				    output[i] = 0;
				}
				else
				{
					output[i] = data_for_derm[drm5m2a8table[i]];
				}
			}
		}

		break;
    }
	default:
    {
#if 0
	for(i = 0;i < N_D;i++)//初始值在开头添加NULL = -100
		       {
		           s_array[i] = null;
		           y1_array[i] = null;
		           y2_array[i] = null;
		       }

		   for(i = 0;i < K_pai;i++)
		       {
		           index = i / r_subblock_tc;
		           temp = C_SUBBLOCK_TC * (i % r_subblock_tc);

		           pattern_01[i] = (P[index] + temp) %  K_pai;
		       }
		       for(i = 0;i < K_pai;i++)
		       {
		           w_array[i] = s_array[pattern_01[i]];
		           w_array[i + K_pai] = y1_array[pattern_01[i]];
		           w_array[i + 2 * K_pai] = y2_array[pattern_01[i]];
		       }

		       k_w = 3 * K_pai;
		       k = 0;
		       i = 0;
		       while(k < E)
		       {
		           index = i % k_w;
		           if (null != w_array[index])
		           {
		               w_array[index] = _sadd(w_array[index],(data_for_derm[k] << 24));
		               k++;
		           }
		           i++;
		       }
		       for(i = 0;i < K_pai;i++)
		       {
		           s_array[pattern_01[i]] = w_array[i] >> 24;
		           y1_array[pattern_01[i]] = w_array[i + K_pai] >> 24;
		           y2_array[pattern_01[i]] = w_array[i + 2 * K_pai] >> 24;
		       }
		       for(i = 0;i < K_pai - N_D;i++)//去除为了交织而在开头添加的N_D个null，还原成只有数据的三路，数据从第N_D个开始，总共有K_pai - N_D个。
		           {
		               s[i] = s_array[i + N_D];
		               y1[i] = y1_array[i + N_D];
		               y2[i] = y2_array[i + N_D];
		           }
#endif
    }





  }


}

