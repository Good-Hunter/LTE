/***********************************************/
/*Bcp_Device.c                                 */
/*功能：BCP初始化函数                          */
/***********************************************/
/* BCP types include */
#include "bcp_types.h"

/* BCP include */
#include "bcp.h"

signed int Bcp_initDevice (Bcp_LldObj* pBcpLldObj)
{
    unsigned char             destSel [BCP_MAX_NUM_TXQUEUES], prioVal [BCP_MAX_NUM_TXQUEUES];
        
	/* Setup TM MMR */
    Bcp_setCdmaHpSrcId (pBcpLldObj, 0xbc);        
    Bcp_enableTxCdmaHpReadArbPrio (pBcpLldObj);

    /* Setup the Tx queue --> PPB (Port) Mapping.
     *
     * Lets use the following configuration:
     * Tx Q0 --> PPB0   Tx Q1 --> PPB1  TX Q2 --> PPB2  Tx Q3 --> PPB3
     * Tx Q4 --> PPB0   Tx Q5 --> PPB1  TX Q6 --> PPB2  Tx Q7 --> PPB3 
     *
     * All are at the same priority for now.
     */
    destSel [0] = 0;    destSel [1] = 1;    destSel [2] = 2;    destSel [3] = 3;
    destSel [4] = 0;    destSel [5] = 1;    destSel [6] = 2;    destSel [7] = 3;
    memset (prioVal, 0, sizeof (unsigned char) * BCP_MAX_NUM_TXQUEUES);
    Bcp_setTxQfifoReadDestSelReg (pBcpLldObj, destSel, prioVal);

    /* Initialize the Encoder engine block */        
    Bcp_setEncPolyCoef1Reg (pBcpLldObj, 0x4D9413);      //WCDMA code rate 1/2
    Bcp_setEncPolyCoef2Reg (pBcpLldObj, 0xD5AA5F);      //WCDMA code rate 1/3
    Bcp_setEncPolyCoef3Reg (pBcpLldObj, 0xDDC3C0);      //LTE code rate 1/3

    Bcp_setEncScrInit0Reg (pBcpLldObj, 0x0);            //scrambler initialization for Wimax
    Bcp_setEncScrPoly0Reg (pBcpLldObj, 0x0);            //scrambler polynomial for Wimax
    Bcp_setEncCrc24Init0Reg (pBcpLldObj, 0x0);          //crc24 initialization
    Bcp_setEncCrc24Poly0Reg (pBcpLldObj, 0x800063);     //crc24 polynomial

    Bcp_setEncScrInit1Reg (pBcpLldObj, 0x6E2A);         //scrambler initialization for Wimax
    Bcp_setEncScrPoly1Reg (pBcpLldObj, 0x0006);         //scrambler polynomial for Wimax
    Bcp_setEncCrc16Init1Reg (pBcpLldObj, 0x0);          //crc16 initialization
    Bcp_setEncCrc16Poly1Reg (pBcpLldObj, 0x1021);       //crc16 polynomial

    Bcp_setEncScrInit2Reg (pBcpLldObj, 0x6E2A);         //scrambler initialization
    Bcp_setEncScrPoly2Reg (pBcpLldObj, 0x0006);         //scrambler polynomial

    /* Initialize the Correlator MMR */
    Bcp_setCorReedMullerTableColumn (pBcpLldObj, 0, 0xFFFFF);
    Bcp_setCorReedMullerTableColumn (pBcpLldObj, 1, 0x5A933);
    Bcp_setCorReedMullerTableColumn (pBcpLldObj, 2, 0x10E5A);
    Bcp_setCorReedMullerTableColumn (pBcpLldObj, 3, 0x6339C);
    Bcp_setCorReedMullerTableColumn (pBcpLldObj, 4, 0x7C3E0);
    Bcp_setCorReedMullerTableColumn (pBcpLldObj, 5, 0xFFC00);
    Bcp_setCorReedMullerTableColumn (pBcpLldObj, 6, 0xD8E64);
    Bcp_setCorReedMullerTableColumn (pBcpLldObj, 7, 0x4F5B0);
    Bcp_setCorReedMullerTableColumn (pBcpLldObj, 8, 0x218EC);
    Bcp_setCorReedMullerTableColumn (pBcpLldObj, 9, 0x1B746);
    Bcp_setCorReedMullerTableColumn (pBcpLldObj, 10, 0xFFFF);
    Bcp_setCorReedMullerTableColumn (pBcpLldObj, 11, 0x33FFF);
    Bcp_setCorReedMullerTableColumn (pBcpLldObj, 12, 0x3FFFC);
    Bcp_setCorControlReg (pBcpLldObj, 0x0, 0x1, 0x2, 0x3);

    return 0;        
}
