
#include "tcp3d_drv_types.h"
#include "tcp3dDrv.h"
#include "cslr_tcp3d_cfg.h"
#include "cslr_tcp3d_dma.h"
#include "system_init.h"
/** 
 *  \brief  Calculates initial beta states using the tail bits for use with the
 *          TCP3D input configuration registers.
 */
#define COMPUTE_KEXT(a)         (4*((a+3)>>2))
#define COMPUTE_KT(x)           (3 -(COMPUTE_KEXT(x)-(x)))

extern signed int TCP3D_codeBlkSeg (
        unsigned int  blockLengthK,
        unsigned char   numMAP,
        unsigned char  *sw0NomLen,
        unsigned char  *sw0LenSel,
        unsigned char  *sw1Len,
        unsigned char  *sw2LenSel,
        unsigned char  *numsw0);

void Tcp3d_betaStates(
                    signed char    * restrict tailBits,
                    signed int   signChange,
                    signed int   Kt,
                    signed char    * restrict beta0Ptr,
                    signed char    * restrict beta1Ptr)
{
    signed int Tmp0,Tmp1,Tmp2;
    long long  llRes, llFin;
    signed int bit10, bit32, bit54, bit76, bit98, bitba;

    signed int scale;
    signed int beta0,beta1,beta2,beta3,beta4,beta5,beta6,beta7,betaMax;

    if(signChange)
    {
        scale = 0x00800080;   //+128
    }
    else
    {
        scale = 0xff80ff80;  //-128
    }

    //Load 12 tail samples
    Tmp0   = _mem4(&tailBits[0]);
    Tmp1   = _mem4(&tailBits[4]);
    Tmp2   = _mem4(&tailBits[8]);

#ifdef _BIG_ENDIAN
    Tmp0 = _rotl(_swap4(Tmp0), 16);
    Tmp1 = _rotl(_swap4(Tmp1), 16);
    Tmp2 = _rotl(_swap4(Tmp2), 16);
#endif

    //***************************************************
    //Initial Beta States Calculation for MAP 0 and MAP1
    //***************************************************
    //Process 4 samples
    llRes = _mpysu4ll(Tmp0, 0x01010101);
    llFin = _smpy2ll(_loll(llRes), scale);
    bit10 = _shr2(_spack2(_hill(llFin),_loll(llFin)), 8);
    llFin = _smpy2ll(_hill(llRes), scale);
    bit32 = _shr2(_spack2(_hill(llFin),_loll(llFin)), 8);

    //Process 4 samples
    //llRes = _mpysu4ll(_hill(llTmp), 0x01010101);
    llRes = _mpysu4ll(Tmp1, 0x01010101);
    llFin = _smpy2ll(_loll(llRes), scale);
    bit54 = _shr2(_spack2(_hill(llFin),_loll(llFin)), 8);
    llFin = _smpy2ll(_hill(llRes), scale);
    bit76 = _shr2(_spack2(_hill(llFin),_loll(llFin)), 8);

    //Process 4 samples
    llRes = _mpysu4ll(Tmp2, 0x01010101);
    llFin = _smpy2ll(_loll(llRes), scale);
    bit98 = _shr2(_spack2(_hill(llFin),_loll(llFin)), 8);
    llFin = _smpy2ll(_hill(llRes), scale);
    bitba = _shr2(_spack2(_hill(llFin),_loll(llFin)), 8);

    //Compute beta state values based on the Kt (# of trellis stages)
    if(Kt==3)
    {
        //Beta MAP0
        beta0 = 0;
        beta1 = _ext(bit10,16,16) + _ext(bit10,0,16); //xt0[0] + xt0[1];
        beta2 = _ext(bit10,16,16) + _ext(bit32,16,16) + _ext(bit32,0,16); //xt0[0] + xt0[2] + xt0[3];
        beta3 = _ext(bit10,0,16) + _ext(bit32,16,16) + _ext(bit32,0,16);  //xt0[1] + xt0[2] + xt0[3];
        beta4 = _ext(bit10,0,16) + _ext(bit32,16,16) + _ext(bit54,16,16) + _ext(bit54,0,16); //xt0[1] + xt0[2]+ xt0[4] + xt0[5];
        beta5 = _ext(bit10,16,16) + _ext(bit32,16,16) + _ext(bit54,16,16) + _ext(bit54,0,16);//xt0[0]+ xt0[2]+ xt0[4] + xt0[5];
        beta6 = _ext(bit10,16,16) + _ext(bit10,0,16) + _ext(bit32,0,16) + _ext(bit54,16,16) + _ext(bit54,0,16); //xt0[0] + xt0[1]+ xt0[3] + xt0[4] + xt0[5];
        beta7 = _ext(bit32,0,16) + _ext(bit54,16,16) + _ext(bit54,0,16);// xt0[3] + xt0[4] + xt0[5];

        betaMax = _max2(beta0,beta1);
        betaMax = _max2(betaMax,beta2);
        betaMax = _max2(betaMax,beta3);
        betaMax = _max2(betaMax,beta4);
        betaMax = _max2(betaMax,beta5);
        betaMax = _max2(betaMax,beta6);
        betaMax = _max2(betaMax,beta7);
        betaMax = _ext(betaMax,16,16);
        betaMax = 127 - betaMax;

        beta0 = beta0 + betaMax;
        beta1 = beta1 + betaMax;
        beta2 = beta2 + betaMax;
        beta3 = beta3 + betaMax;
        beta4 = beta4 + betaMax;
        beta5 = beta5 + betaMax;
        beta6 = beta6 + betaMax;
        beta7 = beta7 + betaMax;

        beta0 = _max2(0x8000ff81, beta0);
        beta1 = _max2(0x8000ff81, beta1);
        beta2 = _max2(0x8000ff81, beta2);
        beta3 = _max2(0x8000ff81, beta3);
        beta4 = _max2(0x8000ff81, beta4);
        beta5 = _max2(0x8000ff81, beta5);
        beta6 = _max2(0x8000ff81, beta6);
        beta7 = _max2(0x8000ff81, beta7);

#ifdef _BIG_ENDIAN
        _mem4(&beta0Ptr[0]) = _packl4(_pack2(beta0,beta1),_pack2(beta2,beta3));
        _mem4(&beta0Ptr[4]) = _packl4(_pack2(beta4,beta5),_pack2(beta6,beta7));
#else
        _mem4(&beta0Ptr[0]) = _packl4(_pack2(beta3,beta2),_pack2(beta1,beta0));
        _mem4(&beta0Ptr[4]) = _packl4(_pack2(beta7,beta6),_pack2(beta5,beta4));
#endif

        //Beta MAP1
        beta0 = 0;
        beta1 = _ext(bit76,16,16) + _ext(bit76,0,16); //xt1[0] + xt1[1];
        beta2 = _ext(bit76,16,16) + _ext(bit98,16,16) + _ext(bit98,0,16); //xt1[0] + xt1[2] + xt1[3];
        beta3 = _ext(bit76,0,16) + _ext(bit98,16,16) + _ext(bit98,0,16);  //xt1[1] + xt1[2] + xt1[3];
        beta4 = _ext(bit76,0,16) + _ext(bit98,16,16) + _ext(bitba,16,16) + _ext(bitba,0,16); //xt1[1] + xt1[2]+ xt1[4] + xt1[5];
        beta5 = _ext(bit76,16,16) + _ext(bit98,16,16) + _ext(bitba,16,16) + _ext(bitba,0,16);//xt1[0]+ xt1[2]+ xt1[4] + xt1[5];
        beta6 = _ext(bit76,16,16) + _ext(bit76,0,16) + _ext(bit98,0,16) + _ext(bitba,16,16) + _ext(bitba,0,16); //xt1[0] + xt1[1]+ xt1[3] + xt1[4] + xt1[5];
        beta7 = _ext(bit98,0,16) + _ext(bitba,16,16) + _ext(bitba,0,16);// xt1[3] + xt1[4] + xt1[5];

        betaMax = _max2(beta0,beta1);
        betaMax = _max2(betaMax,beta2);
        betaMax = _max2(betaMax,beta3);
        betaMax = _max2(betaMax,beta4);
        betaMax = _max2(betaMax,beta5);
        betaMax = _max2(betaMax,beta6);
        betaMax = _max2(betaMax,beta7);
        betaMax = _ext(betaMax,16,16);
        betaMax = 127 - betaMax;

        beta0 = beta0 + betaMax;
        beta1 = beta1 + betaMax;
        beta2 = beta2 + betaMax;
        beta3 = beta3 + betaMax;
        beta4 = beta4 + betaMax;
        beta5 = beta5 + betaMax;
        beta6 = beta6 + betaMax;
        beta7 = beta7 + betaMax;

        beta0 = _max2(0x8000ff81, beta0);
        beta1 = _max2(0x8000ff81, beta1);
        beta2 = _max2(0x8000ff81, beta2);
        beta3 = _max2(0x8000ff81, beta3);
        beta4 = _max2(0x8000ff81, beta4);
        beta5 = _max2(0x8000ff81, beta5);
        beta6 = _max2(0x8000ff81, beta6);
        beta7 = _max2(0x8000ff81, beta7);

#ifdef _BIG_ENDIAN
        _mem4(&beta1Ptr[0]) = _packl4(_pack2(beta0,beta1),_pack2(beta2,beta3));
        _mem4(&beta1Ptr[4]) = _packl4(_pack2(beta4,beta5),_pack2(beta6,beta7));
#else
        _mem4(&beta1Ptr[0]) = _packl4(_pack2(beta3,beta2),_pack2(beta1,beta0));
        _mem4(&beta1Ptr[4]) = _packl4(_pack2(beta7,beta6),_pack2(beta5,beta4));
#endif
    }
    else if(Kt==2)
    {
        //Beta MAP0
        beta0 = 0;
        beta1 = _ext(bit32,16,16) + _ext(bit32,0,16); //xt0[0] + xt0[1];
        beta2 = _ext(bit32,16,16) + _ext(bit54,16,16) + _ext(bit54,0,16); //xt0[0] + xt0[2] + xt0[3];
        beta3 = _ext(bit32,0,16) + _ext(bit54,16,16) + _ext(bit54,0,16);  //xt0[1] + xt0[2] + xt0[3];

        betaMax = _max2(beta0,beta1);
        betaMax = _max2(betaMax,beta2);
        betaMax = _max2(betaMax,beta3);
        betaMax = _ext(betaMax,16,16);
        betaMax = 127 - betaMax;

        beta0 = beta0 + betaMax;
        beta1 = beta1 + betaMax;
        beta2 = beta2 + betaMax;
        beta3 = beta3 + betaMax;

        beta0 = _max2(0x8000ff81, beta0);
        beta1 = _max2(0x8000ff81, beta1);
        beta2 = _max2(0x8000ff81, beta2);
        beta3 = _max2(0x8000ff81, beta3);

#ifdef _BIG_ENDIAN
        _mem4(&beta0Ptr[0]) = _packl4(_pack2(beta0,beta1),_pack2(beta2,beta3));
#else
        _mem4(&beta0Ptr[0]) = _packl4(_pack2(beta3,beta2),_pack2(beta1,beta0));
#endif
        _mem4(&beta0Ptr[4]) = 0x81818181;

        //Beta MAP1
        //temp
        beta0 = _ext(bit98,16,16);
        beta0 = _ext(bit98,0,16);
        beta0 = _ext(bitba,16,16);
        beta0 = _ext(bitba,0,16);

        //temp
        beta0 = 0;
        beta1 = _ext(bit98,16,16) + _ext(bit98,0,16); //xt1[0] + xt1[1];
        beta2 = _ext(bit98,16,16) + _ext(bitba,16,16) + _ext(bitba,0,16); //xt1[0] + xt1[2] + xt1[3];
        beta3 = _ext(bit98,0,16) + _ext(bitba,16,16) + _ext(bitba,0,16);  //xt1[1] + xt1[2] + xt1[3];

        betaMax = _max2(beta0,beta1);
        betaMax = _max2(betaMax,beta2);
        betaMax = _max2(betaMax,beta3);
        betaMax = _ext(betaMax,16,16);
        betaMax = 127 - betaMax;

        beta0 = beta0 + betaMax;
        beta1 = beta1 + betaMax;
        beta2 = beta2 + betaMax;
        beta3 = beta3 + betaMax;

        beta0 = _max2(0x8000ff81, beta0);
        beta1 = _max2(0x8000ff81, beta1);
        beta2 = _max2(0x8000ff81, beta2);
        beta3 = _max2(0x8000ff81, beta3);

#ifdef _BIG_ENDIAN
        _mem4(&beta1Ptr[0]) = _packl4(_pack2(beta0,beta1),_pack2(beta2,beta3));
#else
        _mem4(&beta1Ptr[0]) = _packl4(_pack2(beta3,beta2),_pack2(beta1,beta0));
#endif
        _mem4(&beta1Ptr[4]) = 0x81818181;
    }
    else if(Kt==1)
    {
        //Beta MAP0
        beta0 = 0;
        beta1 = _ext(bit54,16,16) + _ext(bit54,0,16); //xt0[0] + xt0[1];

        betaMax = _max2(beta0,beta1);
        betaMax = _ext(betaMax,16,16);
        betaMax = 127 - betaMax;

        beta0 = beta0 + betaMax;
        beta1 = beta1 + betaMax;

        beta0 = _max2(0x8000ff81, beta0);
        beta1 = _max2(0x8000ff81, beta1);

#ifdef _BIG_ENDIAN
        _mem4(&beta0Ptr[0]) = _packl4(_pack2(beta0,beta1),0x81818181);
#else
        _mem4(&beta0Ptr[0]) = _packl4(0x81818181,_pack2(beta1,beta0));
#endif
        _mem4(&beta0Ptr[4]) = 0x81818181;

        //Beta MAP1
        beta0 = 0;
        beta1 = _ext(bitba,16,16) + _ext(bitba,0,16); //xt1[0] + xt1[1];

        betaMax = _max2(beta0,beta1);
        betaMax = _ext(betaMax,16,16);
        betaMax = 127 - betaMax;

        beta0 = beta0 + betaMax;
        beta1 = beta1 + betaMax;

        beta0 = _max2(0x8000ff81, beta0);
        beta1 = _max2(0x8000ff81, beta1);

#ifdef _BIG_ENDIAN
        _mem4(&beta1Ptr[0]) = _packl4(_pack2(beta0,beta1),0x81818181);
#else
        _mem4(&beta1Ptr[0]) = _packl4(0x81818181,_pack2(beta1,beta0));
#endif
        _mem4(&beta1Ptr[4]) = 0x81818181;

    }
    else if(Kt==0)
    {
#ifdef _BIG_ENDIAN
        _mem4(&beta0Ptr[0]) = 0x7f818181;
        _mem4(&beta1Ptr[0]) = 0x7f818181;
#else
        _mem4(&beta0Ptr[0]) = 0x8181817f;
        _mem4(&beta1Ptr[0]) = 0x8181817f;
#endif
        _mem4(&beta0Ptr[4]) = 0x81818181;
        _mem4(&beta1Ptr[4]) = 0x81818181;
    }
}

/* nothing past this line */


void prepare_user_info (BCP_userInfo *userInfo, signed char *tailBits, unsigned short cblen,signed short total_cbs)
{
    unsigned int      reg;
    unsigned char       mode, sw0LenSel, sw2LenSel, sw1Len, numsw0;
    cbConfig    param; 
    signed int signChange;
    signed int Kt;
    signed char beta0[8], beta1[8];

    memset ((void *) &param, 0, sizeof (cbConfig)); 

    /* Assigning values for configuration. */
    param.sw0Length        = 64;
    param.maxStar          = 0;
    param.outStsRead       = 0;
    param.extScale         = 1;
    param.softOutRead      = 0;
    param.softOutFrmtSel   = 1;
    param.minIter          = 1;
    param.maxIter          = 6;/*Max iteration only*/
    param.snrVal           = 14;
    param.snrReport        = 1;
    param.stopSel          = 1;//0;
    param.crcIterSel       = 0;
    if(1 == total_cbs)
        param.crcPolySel       = 1;//TB crc
    else
    	param.crcPolySel       = 0;//CB crc
    param.maxStarThres     = 4;
    param.maxStarValue     = 2;
    param.extrScale_0      = 24;
    param.extrScale_1      = 24;
    param.extrScale_2      = 24;
    param.extrScale_3      = 24;
    param.extrScale_4      = 24;
    param.extrScale_5      = 24;
    param.extrScale_6      = 24;
    param.extrScale_7      = 24;
    param.extrScale_8      = 24;
    param.extrScale_9      = 24;
    param.extrScale_10     = 24;
    param.extrScale_11     = 24;
    param.extrScale_12     = 24;
    param.extrScale_13     = 24;
    param.extrScale_14     = 24;
    param.extrScale_15     = 24;

    mode = CSL_TCP3D_CFG_TCP3_MODE_MODE_SEL_LTE;

    //userInfo->cblength = cblen;
    //userInfo->cblengthover2 = cblen >> 1;
    //userInfo->cblengthover2_1 = userInfo->cblengthover2;
    //userInfo->cblengthplus4 = cblen + 4;
    //userInfo->llroffset = (((cblen + 4) + 15) >> 4 ) * 16;
    //userInfo->betaStateLoc = (userInfo->llroffset) * 3;
    //userInfo->cblengthover8 = cblen >> 3;
    //userInfo->reserved = 0;
    TCP3D_codeBlkSeg (cblen, 2,
                        &param.sw0Length,
                        &sw0LenSel,
                        &sw1Len,
                        &sw2LenSel,
                        &numsw0);

    /* Number of SW0 and block size (Range: 39 to 8191; block size - 1) */
    reg = 0;
    CSL_FINS (reg, TCP3D_DMA_TCP3D_IC_CFG0_P0_NUM_SW0, numsw0);
    CSL_FINS (reg, TCP3D_DMA_TCP3D_IC_CFG0_P0_BLK_LN, cblen - 1);

    userInfo->TCP3D_IC_CFG0 = reg;

    /* SW0, SW1, SW2 length */
    reg = 0;
    CSL_FINS (reg, TCP3D_DMA_TCP3D_IC_CFG1_P0_SW0_LN_SEL, sw0LenSel);
    CSL_FINS (reg, TCP3D_DMA_TCP3D_IC_CFG1_P0_SW2_LN_SEL, sw2LenSel);
    CSL_FINS (reg, TCP3D_DMA_TCP3D_IC_CFG1_P0_SW1_LN, sw1Len);


    userInfo->TCP3D_IC_CFG1 = reg;

    /* Interleaver table load, Max star, output status register, 
     * swap HD bit ordering, extrinsic scaling, soft output flag, 
     * soft output format, number of iterations, SNR reporting,
     * stopping criteria, CRC selection */
    reg = 0;
    CSL_FINS (reg, TCP3D_DMA_TCP3D_IC_CFG2_P0_INTER_LOAD_SEL, CSL_TCP3D_DMA_TCP3D_IC_CFG2_P0_INTER_LOAD_SEL_SET);
    CSL_FINS (reg, TCP3D_DMA_TCP3D_IC_CFG2_P0_MAXST_EN, param.maxStar);
    CSL_FINS (reg, TCP3D_DMA_TCP3D_IC_CFG2_P0_OUT_FLAG_EN, param.outStsRead);
//#ifdef _BIG_ENDIAN
    //CSL_FINS (reg, TCP3D_DMA_TCP3D_IC_CFG2_P0_OUT_ORDER_SEL, CSL_TCP3D_DMA_TCP3D_IC_CFG2_P0_OUT_ORDER_SEL_SWAP);
//#else
    CSL_FINS (reg, TCP3D_DMA_TCP3D_IC_CFG2_P0_OUT_ORDER_SEL, CSL_TCP3D_DMA_TCP3D_IC_CFG2_P0_OUT_ORDER_SEL_NO_SWAP);
//#endif
    CSL_FINS (reg, TCP3D_DMA_TCP3D_IC_CFG2_P0_EXT_SCALE_EN, param.extScale);
    CSL_FINS (reg, TCP3D_DMA_TCP3D_IC_CFG2_P0_SOFT_OUT_FLAG_EN, param.softOutRead);
//#ifdef _BIG_ENDIAN
    //CSL_FINS (reg, TCP3D_DMA_TCP3D_IC_CFG2_P0_SOFT_OUT_ORDER_SEL, CSL_TCP3D_DMA_TCP3D_IC_CFG2_P0_SOFT_OUT_ORDER_SEL_8_BIT);
//#else
    CSL_FINS (reg, TCP3D_DMA_TCP3D_IC_CFG2_P0_SOFT_OUT_ORDER_SEL, CSL_TCP3D_DMA_TCP3D_IC_CFG2_P0_SOFT_OUT_ORDER_SEL_32_BIT);
//#endif
    CSL_FINS (reg, TCP3D_DMA_TCP3D_IC_CFG2_P0_SOFT_OUT_FMT, param.softOutFrmtSel);
    CSL_FINS (reg, TCP3D_DMA_TCP3D_IC_CFG2_P0_MIN_ITR, param.minIter);
    CSL_FINS (reg, TCP3D_DMA_TCP3D_IC_CFG2_P0_MAX_ITR, param.maxIter);
    CSL_FINS (reg, TCP3D_DMA_TCP3D_IC_CFG2_P0_SNR_VAL, param.snrVal);
    CSL_FINS (reg, TCP3D_DMA_TCP3D_IC_CFG2_P0_SNR_REP, param.snrReport);
    CSL_FINS (reg, TCP3D_DMA_TCP3D_IC_CFG2_P0_STOP_SEL, param.stopSel);
    CSL_FINS (reg, TCP3D_DMA_TCP3D_IC_CFG2_P0_CRC_ITER_PASS, param.crcIterSel);
    CSL_FINS (reg, TCP3D_DMA_TCP3D_IC_CFG2_P0_CRC_SEL, param.crcPolySel);

    userInfo->TCP3D_IC_CFG2 = reg;

    /* Max star threshold and value */
    reg = 0;
    CSL_FINS (reg, TCP3D_DMA_TCP3D_IC_CFG3_P0_MAXST_THOLD, param.maxStarThres);
    CSL_FINS (reg, TCP3D_DMA_TCP3D_IC_CFG3_P0_MAXST_VALUE, param.maxStarValue);

    userInfo->TCP3D_IC_CFG3 = reg;
    
    Kt = COMPUTE_KT(cblen);
    signChange = 1;
    /* Beta states */
    Tcp3d_betaStates(tailBits, signChange, Kt, beta0, beta1);
                    
    /*
    7:0 beta_st0_map0
    15:8 beta_st1_map0
    23:16 beta_st2_map0
    31:24 beta_st3_map0
    */
    //userInfo->TCP3D_IC_CFG4 = _rotl(_swap4(_mem4(&beta0[0])),16); // beta_st0_map0 | beta_st1_map0 | beta_st2_map0 | beta_st3_map0
    //userInfo->TCP3D_IC_CFG5 = _rotl(_swap4(_mem4(&beta0[4])),16); // beta_st4_map0 | beta_st5_map0 | beta_st6_map0 | beta_st7_map0
    //userInfo->TCP3D_IC_CFG6 = _rotl(_swap4(_mem4(&beta1[0])),16);
    //userInfo->TCP3D_IC_CFG7 = _rotl(_swap4(_mem4(&beta1[4])),16);
    userInfo->TCP3D_IC_CFG4 = _mem4(&beta0[0]);
    userInfo->TCP3D_IC_CFG5 = _mem4(&beta0[4]);
    userInfo->TCP3D_IC_CFG6 = _mem4(&beta1[0]);
    userInfo->TCP3D_IC_CFG7 = _mem4(&beta1[4]);

    
    /* Extrinsic scale value 0-3 */
    reg = 0;
    CSL_FINS (reg, TCP3D_DMA_TCP3D_IC_CFG8_P0_EXT_SCALE_0, param.extrScale_0);
    CSL_FINS (reg, TCP3D_DMA_TCP3D_IC_CFG8_P0_EXT_SCALE_1, param.extrScale_1);
    CSL_FINS (reg, TCP3D_DMA_TCP3D_IC_CFG8_P0_EXT_SCALE_2, param.extrScale_2);
    CSL_FINS (reg, TCP3D_DMA_TCP3D_IC_CFG8_P0_EXT_SCALE_3, param.extrScale_3);
    userInfo->TCP3D_IC_CFG8 = reg;
    
    /* Extrinsic scale value 4-7 */
    reg = 0;
    CSL_FINS (reg, TCP3D_DMA_TCP3D_IC_CFG9_P0_EXT_SCALE_4, param.extrScale_4);
    CSL_FINS (reg, TCP3D_DMA_TCP3D_IC_CFG9_P0_EXT_SCALE_5, param.extrScale_5);
    CSL_FINS (reg, TCP3D_DMA_TCP3D_IC_CFG9_P0_EXT_SCALE_6, param.extrScale_6);
    CSL_FINS (reg, TCP3D_DMA_TCP3D_IC_CFG9_P0_EXT_SCALE_7, param.extrScale_7);
    userInfo->TCP3D_IC_CFG9 = reg;

    /* Extrinsic scale value 8-11 */
    reg = 0;
    CSL_FINS (reg, TCP3D_DMA_TCP3D_IC_CFG10_P0_EXT_SCALE_8, param.extrScale_8);
    CSL_FINS (reg, TCP3D_DMA_TCP3D_IC_CFG10_P0_EXT_SCALE_9, param.extrScale_9);
    CSL_FINS (reg, TCP3D_DMA_TCP3D_IC_CFG10_P0_EXT_SCALE_10, param.extrScale_10);
    CSL_FINS (reg, TCP3D_DMA_TCP3D_IC_CFG10_P0_EXT_SCALE_11, param.extrScale_11);
    userInfo->TCP3D_IC_CFG10 = reg;

    /* Extrinsic scale value 12-15 */
    reg = 0;
    CSL_FINS (reg, TCP3D_DMA_TCP3D_IC_CFG11_P0_EXT_SCALE_12, param.extrScale_12);
    CSL_FINS (reg, TCP3D_DMA_TCP3D_IC_CFG11_P0_EXT_SCALE_13, param.extrScale_13);
    CSL_FINS (reg, TCP3D_DMA_TCP3D_IC_CFG11_P0_EXT_SCALE_14, param.extrScale_14);
    CSL_FINS (reg, TCP3D_DMA_TCP3D_IC_CFG11_P0_EXT_SCALE_15, param.extrScale_15);
    userInfo->TCP3D_IC_CFG11 = reg;

    /* LTE or WIMAX */
    if (mode == CSL_TCP3D_CFG_TCP3_MODE_MODE_SEL_LTE )
    {
        signed int frameLenInd = LTE_interleaver_index(cblen);

        /* ITG param 0-1 */
        reg = 0;

        CSL_FINS (reg, TCP3D_DMA_TCP3D_IC_CFG12_P0_ITG_PARAM0, 
                        (unsigned short) ((2*TCP3_LteInterleaverTable[frameLenInd][2]) % TCP3_LteInterleaverTable[frameLenInd][0]));
        CSL_FINS (reg, TCP3D_DMA_TCP3D_IC_CFG12_P0_ITG_PARAM1, TCP3_LteInterleaverTable[frameLenInd][6]);
        userInfo->TCP3D_IC_CFG12 = reg;
    
        /* ITG param 2-3 */
        reg = 0;
        CSL_FINS (reg, TCP3D_DMA_TCP3D_IC_CFG13_P0_ITG_PARAM2, TCP3_LteInterleaverTable[frameLenInd][3]);
        CSL_FINS (reg, TCP3D_DMA_TCP3D_IC_CFG13_P0_ITG_PARAM3, TCP3_LteInterleaverTable[frameLenInd][4]);
        userInfo->TCP3D_IC_CFG13 = reg;
    
        /* ITG param 4 */
        reg = 0;
        CSL_FINS (reg, TCP3D_DMA_TCP3D_IC_CFG14_P0_ITG_PARAM4, TCP3_LteInterleaverTable[frameLenInd][5]);
        userInfo->TCP3D_IC_CFG14 = reg;
    }
    else
    {
        /* ITG Params are not required for 3GPP */
        userInfo->TCP3D_IC_CFG12 = 0;
        userInfo->TCP3D_IC_CFG13 = 0;
        userInfo->TCP3D_IC_CFG14 = 0;
    }

    userInfo->TCP3D_TRIG = 1;
    return;
}

signed int TCP3D_codeBlkSeg (
            unsigned int  blockLengthK,
            unsigned char   numMAP,
            unsigned char  *sw0NomLen,
            unsigned char  *sw0LenSel,
            unsigned char  *sw1Len,
            unsigned char  *sw2LenSel,
            unsigned char  *numsw0)
{
    signed int  status = 0;
    signed int K, Kext;
    signed int numSWrem;
    signed int subFrameLen;
    signed int sw0LenSelTmp;
    signed int sw1LenTmp;
    signed int sw2LenSelTmp;
    signed int numsw0Tmp;
    signed int numSW;
    signed int shiftVal, mulVal;
    signed int sw0Len = *sw0NomLen;

    /**
     * Check the bounds based on numMAP value. frameLenTab is for the bound values
     *      numMAP  -   mode    - block length bounds
     *        1     -   3GPP    -   [40,5114]
     *        2     - LTE/WIMAX -   [40,6144]
     */
    if ( (blockLengthK < frameLenTab[numMAP-1][0]) ||
         (blockLengthK > frameLenTab[numMAP-1][1]) )
    {
        status = 1;
        return (status);
    }

    K = blockLengthK;
    Kext = ((K + 0x3)>>2)<<2;

    //Calculate sw0LenSelTmp, SW1Len, SW2LenSel, numsw0Tmp
    subFrameLen = Kext >> numMAP;   //Kext / (2*numMAP);

    sw0LenSelTmp = TAB[((sw0Len>>4)-1)&0x7];

    //Check that this holds: (reg->NumInfoBits <= 128 * sparms->tcp3_SW0_length * numMap)
    while((Kext > 128 * sw0Len * numMAP) && sw0LenSelTmp<6)
    {
        sw0LenSelTmp++;
        sw0Len = tcp3d_sw0_Tab[sw0LenSelTmp];
    }

    //numSW = subFrameLen/sw0Len;  Replaced by:
    shiftVal = shiftValTab[sw0LenSelTmp];
    mulVal   = mulValTab[sw0LenSelTmp];
    numSW = _mpysu((subFrameLen >> shiftVal), mulVal)>>15;

    numSWrem = subFrameLen - numSW*sw0Len;
    if(numSWrem)
    {
        numSW++;
    }

    if(numSW == 1)
    {
        numsw0Tmp = 0;
        sw1LenTmp = subFrameLen-1;             //stored value is (sw1_length -1)
        sw2LenSelTmp = 0;                      //SW2 is Off.
    }
    else if(numSW == 2)
    {
        numsw0Tmp = 0;
        if(subFrameLen & 0x3)
        {
            sw1LenTmp = 2*(subFrameLen>>2) + 1;    //stored value is (sw1_length -1)
            sw2LenSelTmp = 2;                      //sw1LenTmp > SW2Len
        }
        else
        {
            sw1LenTmp = (subFrameLen>>1) - 1;  //stored value is (sw1_length -1)
            sw2LenSelTmp = 1;                  //sw1LenTmp = SW2Len
        }
    }
    else if( numSWrem <= (sw0Len>>1) )
    {
        numsw0Tmp = numSW-2;
        numSWrem = subFrameLen - (numSW-2)*sw0Len;
        if((numSWrem) & 0x3)
        {
            sw1LenTmp = 2*(numSWrem>>2) + 1;   //stored value is (sw1_length -1)
            sw2LenSelTmp = 2;                  //sw1LenTmp > SW2Len
        }
        else
        {
            sw1LenTmp = (numSWrem>>1) - 1;     //stored value is (sw1_length -1)
            sw2LenSelTmp = 1;                  //sw1LenTmp = SW2Len
        }
    }
    else
    {
        numsw0Tmp = numSW-1;
        sw1LenTmp = numSWrem - 1;              //stored value is (sw1_length -1)
        sw2LenSelTmp = 0;                      //SW2 is Off.
    }


    *sw0LenSel  = (unsigned char) sw0LenSelTmp;
    *sw1Len     = (unsigned char) sw1LenTmp;
    *sw2LenSel  = (unsigned char) sw2LenSelTmp;
    *numsw0     = (unsigned char) numsw0Tmp;
    *sw0NomLen  = (unsigned char) sw0Len;

    return ( status );

}
