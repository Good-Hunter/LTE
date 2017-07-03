
#ifndef _BCP_UMTS_H_
#define _BCP_UMTS_H_

typedef struct _CPLX16
{
  int16_t imag;
  int16_t real;
} cplx16_t;

/* xCDMA Channel Types */
#define WCDMAFDD_HSDPA         0
#define WCDMAFDD_HSUPA         1
#define WCDMAFDD_HSUPAPIC      2
#define WCDMAFDD_REL99DL       3
#define WCDMAFDD_REL99UL       4
#define WCDMAFDD_REL99ULPIC    5

#define TDSCDMA_HSDPA          6
#define TDSCDMA_HSUPA          7
#define TDSCDMA_HSUPAPIC       8 
#define TDSCDMA_REL99DL        9
#define TDSCDMA_REL99UL       10
#define TDSCDMA_REL99ULPIC    11

#define FDD_R99_DL_MAX_TFCS_SIZE 150
#define FDD_R99_DL_MAX_NUM_TRCH  7
#define FDD_R99_DL_MAX_NUM_TF    6
#define FDD_R99_DL_NUM_RF_TO_RUN 4

#define FDD_R99_UL_MAX_TFCS_SIZE 66
#define FDD_R99_UL_MAX_NUM_TRCH  6
#define FDD_R99_UL_MAX_NUM_TF    9
#define FDD_R99_UL_MAX_NUM_PHYCH 6

#define TDD_R99_DL_MAX_NUM_TRCH  FDD_R99_DL_MAX_NUM_TRCH

#define TDD_R99_MAX_NUM_PHY_CHAN  6

#define FDD_R99_DL_MAX_RF        8

#define TDD_R99_DL_MAX_NUM_TRCH  FDD_R99_DL_MAX_NUM_TRCH
#define TDD_R99_DL_NUM_RF_TO_RUN FDD_R99_DL_NUM_RF_TO_RUN

typedef struct _BcpTest_HsdpaParams 
{
	/* root */
	unsigned int TBsize;
	unsigned int modScheme; 
	unsigned int Nir; 
	unsigned int nPhyCh; 
	unsigned int RVindex;

	/* drived */
	unsigned int numCodeBks;
    unsigned int codeBkSizeK;
	unsigned int numFillerBits;
	unsigned int Ne;    //number of rate matching input bits
	unsigned int Nedata;//number of rate matching output bits
    unsigned int einit_sys;
    unsigned int eplus_sys;
	unsigned int eminus_sys;
	unsigned int einit1_p1;
	unsigned int eplus1_p1;
	unsigned int eminus1_p1;
	unsigned int einit1_p2;
	unsigned int eplus1_p2;
	unsigned int eminus1_p2;
	unsigned int einit_p1;
    unsigned int eplus_p1; 
	unsigned int eminus_p1; 
	unsigned int einit_p2; 
	unsigned int eplus_p2; 
	unsigned int eminus_p2; 
	unsigned int Ncol;
	unsigned int Nrow;
	unsigned int Nr;
	unsigned int Nc;
	unsigned int punctureFlag;
	unsigned int b;
	unsigned int valid;//0: not valid, 1: valid

	/* TAC format */
	unsigned int TACformatFlag;

	/* TDD use only */
	unsigned int tddSlotFormat;
	unsigned int tddNslots;
} BcpTest_HsdpaParams;

typedef struct _BcpTest_Hsupa_PhyChParams 
{
	unsigned int      SF;
	unsigned int      partialSF;
	unsigned int      SFradio;
	unsigned int      Ndata;
	uint16_t      modRms;
	double      noiseVar;
	unsigned char*      psigned intOut;//8-bit byte per binary bits
	int16_t*      pModOut;//8-bit byte per binary bits
	int16_t*      pSpreadOut;//8-bit byte per binary bits
	int16_t*      pChannelOut;//8-bit byte per binary bits
	int16_t*      pPartialDeSpreadOut;//16-bit real value
	int16_t*      pFinalDeSpreadOut;//16-bit real value
	int8_t*       pSoftSlicerOut;
	int8_t*       pRateDeMatchOut;

} BcpTest_Hsupa_PhyChParams;

typedef struct _BcpTest_Hsupa_TddSlotParams
{
	uint16_t modRms;//the effictive rms at SSL input, so the rms at modulation should be rms/SFratio  
	double noiseVar;
	int8_t     * pModIn;//8-bit byte per binary bits
	cplx16_t  * pModOut;//32-bit value, 16-bit I, 16-bit Q
	cplx16_t  * pSpreadOut;//32-bit value, 16-bit I, 16-bit Q
	cplx16_t  * pChannelOut;//32-bit value, 16-bit I, 16-bit Q
	cplx16_t  * pPartialDeSpreadOut;//32-bit value, 16-bit I, 16-bit Q
	cplx16_t  * pFinalDeSpreadOut;//32-bit value,16-bit I, 16-bit Q
	int8_t      * pSoftSlicerOut;//8-bit byte per soft bit 
} BcpTest_Hsupa_TddSlotParams;

typedef struct _BcpTest_HsupaParams 
{
	unsigned int      ttiType;//0:2msTTI, 1-10msTTI
	unsigned int      startTTIN;
	unsigned int      numTTItoRun;
	unsigned int      maxNumReTrans;

	/* root, parameter for test */
	unsigned int      TBsize;
	unsigned int      maxSet0;      
	unsigned int      Narq;
	double      PLnonmax;
	unsigned int      commonPartialSF;
	unsigned int      commonSFradio;
    
	//uint16_t    modRms;
	uint16_t      snr;
	//double    noiseVar;
    //
	/* root, paramters changed per TTI (packet)*/
	unsigned int      RSN;
	unsigned int      TTIN;
	unsigned int      awgnFlag;
    uint16_t      qFmt;

	/* derived */
	unsigned int      edchCategory;
	unsigned int      modScheme;//1:BPSK, 2:4PAM 
	unsigned int      nPhyCh;
	BcpTest_Hsupa_PhyChParams   hsupaPhyCh[4]; 
	//unsigned int    SF;
	unsigned int      RVindex;
	unsigned int      numCodeBks;
    unsigned int      codeBkSizeK;
	unsigned int      numFillerBits;
	unsigned int      Ne;    //number of rate matching input bits
	unsigned int      Nedata;//number of rate matching output bits
    unsigned int      einit_sys;
    unsigned int      eplus_sys;
	unsigned int      eminus_sys;
	/**/
	unsigned int      einit1_p1;
	unsigned int      eplus1_p1;
	unsigned int      eminus1_p1;
	unsigned int      einit1_p2;
	unsigned int      eplus1_p2;
	unsigned int      eminus1_p2;
	/**/
	unsigned int      einit_p1;
    unsigned int      eplus_p1; 
	unsigned int      eminus_p1; 
	unsigned int      einit_p2; 
	unsigned int      eplus_p2; 
	unsigned int      eminus_p2; 
	unsigned int      punctureFlag;
	unsigned int      valid;//0: not valid, 1: valid
	float**     pRateDeMatchOut1;
	float**     pRateDeMatchOut2;
	float**     pLlrHistoryTi;

	//TDD used only 
	unsigned int tddNslots;
	unsigned int Ncol;
	unsigned int Nrow;
	unsigned int Nsamplesperslot;
	unsigned int Nsymbolsperslot;
	unsigned int tddNdataPerSlot;
	unsigned int b;
	unsigned int tddSlotFormat;
	BcpTest_Hsupa_TddSlotParams hsupaTddSlot[5]; 
	unsigned int jackBit;
	int8_t * pTddSoftSlicerOut;//entire TTI
	int8_t * pTddDeConsteRearrageOut;//entire TTI
	int8_t * pTddDesigned interleaverOut;//entire TTI
	int8_t * pTddDeBitScramOut;//entire TTI
//	int8_t * pFddRateDematchIn;//entire TTI
	int8_t * pTddRateDematchOut[3];
	int8_t * pTddHistoryIn[3];
	int8_t * pTddHistoryOut[3];
	int8_t * pTddLlrCombiningOut[3];
	int16_t * pTddLlrCombining9bitOut[3];

	unsigned int SFratio;

	unsigned int Nc;
	unsigned int Nr;
	unsigned int Ntsys;
	unsigned int Ntp1;
	unsigned int Ntp2;
} BcpTest_HsupaParams;

/* R99 DL TF */
typedef struct _BcpTest_R99DlTF
{
		unsigned int TBsize;
		unsigned int numTB;
		unsigned int codeBkSizeK;
		unsigned int numCodeBks;
		unsigned int numFillerBits;

		unsigned int Niltti;//numBits in TTI before RM on TrCh i TF l	
		signed int  deltaNiltti;	

		unsigned int Nil;//only used in flexible pos
} BcpTest_R99DlTF; 

/* R99 DL TrCH */
typedef struct _BcpTest_R99DlTrch 
{
		unsigned int numTTItoRun;
		unsigned int numRadioFrmPerTti;
		unsigned int CRCsize;
		unsigned int RMattribute;
		unsigned int turboFlag;
		unsigned int turboFlagBcpConfig;
		unsigned int halfCodeRateFlag;
		
		/**/
		unsigned int TBsize;
		unsigned int numTB;
		unsigned int codeBkSizeK;
		unsigned int numCodeBks;
		unsigned int numFillerBits;
		
		/**/
		unsigned int Niltti;
		signed int  deltaNiltti;

		/**/
		unsigned int Xi;
		
		unsigned int cc_einit;
		unsigned int cc_eplus;
		unsigned int cc_eminus;

		unsigned int tc_einit_sys;
		unsigned int tc_eplus_sys;
		unsigned int tc_eminus_sys;

		unsigned int tc_einit_p1;
		unsigned int tc_eplus_p1;
		unsigned int tc_eminus_p1;

		unsigned int tc_einit_p2; 
		unsigned int tc_eplus_p2; 
		unsigned int tc_eminus_p2; 

		unsigned int punctureFlag;

		/**/
		unsigned int numBitsRmOut;             //number of real bits
		unsigned int numBits1DTXOut;           //number of real bits
		unsigned int numBits1signed intOutPerRadioFrm;//number of real bits

		/**/	
		unsigned int   TFI;
		unsigned int   numTF;
		BcpTest_R99DlTF TF[FDD_R99_DL_MAX_NUM_TF];
		
		/**/
		unsigned int Nistar;     //intermediate variable, only used in fixed pos
		signed int  deltaNistar;//intermediate variable, only used in fixed pos
		signed int  deltaNimax; //intermediate variable, only used in fixed pos
		
		/* used by DIO module write which is one TrCH per TTI operation */
		unsigned int dioAddrPerRadioFrm[FDD_R99_DL_NUM_RF_TO_RUN];//must be in 128-bit bundery
		unsigned int dioNumBytesPerRadioFrm;//number of bytes
		unsigned int dioNumBytesPerTti;//number of bytes

		/* used for TDD only */
		unsigned int tddNij;
		signed int  tdddeltaNij;

		unsigned int tddusedTF;
		unsigned char* pTddRadioFrmEqualizeOut[TDD_R99_DL_NUM_RF_TO_RUN];//per TTI, max possible number of TTI = TDD_R99_DL_NUM_RF_TO_RUN
		unsigned char* pTdd1stsigned intOut[TDD_R99_DL_NUM_RF_TO_RUN];//per TTI, max possible number of TTI = TDD_R99_DL_NUM_RF_TO_RUN

		unsigned char* pTddRadioFrmSegOut[TDD_R99_DL_NUM_RF_TO_RUN];//per radio frame, test runs 4 radio frame
		unsigned char* pTddRateMatchOut[TDD_R99_DL_NUM_RF_TO_RUN];//per radio frame, test always run 4 radio frame
		unsigned int tddNumBitsInsertRFEqualisation;
		
} BcpTest_R99DlTrch; 

/* R99 DL */
typedef struct _BcpTest_R99DlParams
{
	unsigned int     numPhyCh;
	unsigned int     slotFormat;//0:data, 1: sccpch
	unsigned int     ChType;//0:data, 1: s-ccpch
	unsigned int     dtxPosType;//0:flexible, 1:fixed
	unsigned int     Ndata;//numPhyCh*15*(Ndata1+Ndata2)
	unsigned int     numTrch; 
	BcpTest_R99DlTrch r99DlTrch[FDD_R99_DL_MAX_NUM_TRCH];
	
	unsigned int     TFCSsize;
	unsigned int     TFCSTab[FDD_R99_DL_MAX_TFCS_SIZE][FDD_R99_DL_MAX_NUM_TRCH];//TFCS
	unsigned int     totalSumTFCSTab[FDD_R99_DL_MAX_TFCS_SIZE];
	unsigned int     usedTFCI;//used TFC

	unsigned int     numBitsTrchConcatOut;//number of read bits, not include DTX, 
	
	/* used by DIO module read which is all TrCH per radio frame operation */
	unsigned int     dioAddrPerRadioFrm[FDD_R99_DL_NUM_RF_TO_RUN][6];//[RF][TrCh]

	unsigned int     TACformatFlag;

	/* used for TDD only */
	float      tddPL;
	unsigned int     tddModScheme;//2:QPSK, 3:8PSK
	unsigned int     tddNumTimeSlot;//1 to 6 for R99
	unsigned int     tddSF;
	unsigned int     tddtotalSumRMxNmj;
	unsigned int     tddradioFrmIdx;
	unsigned int     tddFrmsigned interleaverFlag;//1: frame related 2nd interleaver, 0: time slot ralated 2nd interleaver
	unsigned char*     tddTrChMultiplexOut[TDD_R99_DL_NUM_RF_TO_RUN];//per radio frame
	unsigned char*     tddBitScrambOut[TDD_R99_DL_NUM_RF_TO_RUN];//per radio frame
	unsigned char*     tddFrm2ndsigned intOut[TDD_R99_DL_NUM_RF_TO_RUN];//per radio frame
	unsigned char*     tddSlot2ndsigned intIn[TDD_R99_DL_NUM_RF_TO_RUN][6];//per radio frame per timeslot
	unsigned char*     tddSlot2ndsigned intOut[TDD_R99_DL_NUM_RF_TO_RUN][6];//per radio frame per timeslot
	unsigned char      tddValid;
	unsigned int     tddNumBitsTimeSlot[6];
	unsigned int     tddPackedOutFlag;
	
} BcpTest_R99DlParams;

/* R99 UL TF */
typedef struct _BcpTest_R99UlTF
{
		unsigned int TBsize;
		unsigned int numTB;
} BcpTest_R99UlTF; 

/* R99 UL TrCH */
typedef struct _BcpTest_R99UlTrch
{
	unsigned int numRadioFrmPerTti;
	unsigned int CRCsize;
	unsigned int RMattribute;
	unsigned int turboFlag;
	unsigned int turboFlagBcpConfig;
	unsigned int halfCodeRateFlag;
	
	unsigned int TBsize;
	unsigned int numTB;
	unsigned int codeBkSizeK;
	unsigned int numCodeBks;
	unsigned int numFillerBits;

	/**/
	unsigned int cc_einit;
	unsigned int cc_eplus;
	unsigned int cc_eminus;

	unsigned int tc_einit_sys;
	unsigned int tc_eplus_sys;
	unsigned int tc_eminus_sys;
	/**/
	unsigned int tc_einit_p1;
	unsigned int tc_eplus_p1;
	unsigned int tc_eminus_p1;
	/**/
	unsigned int tc_einit_p2; 
	unsigned int tc_eplus_p2; 
	unsigned int tc_eminus_p2; 

	unsigned int punctureFlag;

	unsigned int Xi; 

	unsigned int   numTF;
	BcpTest_R99UlTF TF[FDD_R99_UL_MAX_NUM_TF];

	unsigned int Nij;
	signed int  deltaNij;

	unsigned int usedTF;
} BcpTest_R99UlTrch; 

/* R99 UL */
typedef struct _BcpTest_R99UlParams 
{
	unsigned int      numRadioFmttoRun;
	unsigned int      radioFrmIdx;
	unsigned int      qFmt;
	float       PL;
	unsigned int      maxSet0;
	unsigned int      Ndataj;
	unsigned int      SF;

	unsigned int      totalSumRMxNmj;

	unsigned int      numTrch; 
	BcpTest_R99UlTrch  r99UlTrch[FDD_R99_UL_MAX_NUM_TRCH];

	unsigned int      nPhyCh;
	
	unsigned int      partialSF;
	unsigned int      SFradio;
	uint16_t      modRms;
	double      noiseVar;
	unsigned char     * psigned intOut;//8-bit byte per binary bits
	int16_t     * pModOut;//8-bit byte per binary bits
	int16_t     * pSpreadOut;//8-bit byte per binary bits
	int16_t     * pChannelOut;//8-bit byte per binary bits
	int16_t     * pPartialDeSpreadOut;//16-bit real value
	int16_t     * pFinalDeSpreadOut;//16-bit real value
	int8_t      * pSoftSlicerOut;
	int8_t      * pRateDeMatchOut;

	unsigned int    Ntr;

	/* used for TDD only */
	unsigned int     tddModScheme;//2:QPSK, 3:8PSK
	unsigned int     tddNumTimeSlot;//1 to 6 for R99
	unsigned int     tddFrmsigned interleaverFlag;//1: frame related 2nd interleaver, 0: time slot ralated 2nd interleaver
	unsigned char      tddValid;
	unsigned int     tddNumBitsTimeSlot[6];
	unsigned int     tddPackedOutFlag;
	unsigned int     tddJackBit;
	BcpTest_Hsupa_TddSlotParams tddSlot[6];
	uint16_t     commonModRms;
	unsigned int     awgnFlag;
	double commonSnr;

	//TDD used only 
	int8_t * pTddSoftSlicerOut;//entire TTI
	int8_t * pTddDesigned interleaverOut;//entire TTI
	int8_t * pTddDeBitScramOut;//entire TTI
} BcpTest_R99UlParams;

#endif /* _BCP_UMTS_H_ */
