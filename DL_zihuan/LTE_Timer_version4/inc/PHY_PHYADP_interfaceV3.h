#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char

/*****�����ǽӿ�ͨ������ͷ������******/

typedef struct GeneralHeader12{
	//GeneralHeader12 ����Type1��Type2���ֽӿڡ���Ϊ��������ͬ������Ҫ�����ı�־����Type1��Type2
	u32	NewFlag;
	u16	SFN;
	u16	SubframeN;
}__attribute__((packed))GeneralHeader12;

typedef struct GeneralHeader345
{
	//GeneralHeader345 ����Type3��Type4��Type5���ֽӿڡ���Ϊ��������ͬ������Ҫ�����ı�־����
	u32	NewFlag;
	union {
		u32 CmdFlag;//��λ��־ ��������������־������Type3,��λע����Ӧ�����ò������Ƿ���Ч
		u32 RespFlag;//��λ��־ ������Ӧ��־������Type4����λ��Ӧ�������Ƿ����Ӧ��Ч
		u32 ReportFlag;//��λ��־ ״̬�ϱ������־������Type5����λ��־��Ӧ��״̬�����Ƿ���Ч
	}setflag;
}__attribute__((packed))GeneralHeader345;


//Type1,2,3,4,5��ÿ��������Ŀ�ѡͷ���������Ҫ�Ļ�
typedef struct ParTableHeader{
	u16 ParTableType;
	u16 ParTableLen;
}__attribute__((packed))ParTableHeader;

/*========������ENB��������·Type1�еĲ�����������ͷ��==================*/

typedef struct ENB_DL_TYPE1_PUBLIC_C
{
	u16	PDSCHNum;
	u16 PDCCHNum;
	u16	NumOfUEForPHICH;
	u16 Zero;

	u32 PBCH_C_Offset;
	u32 PBCH_D_Offset;
	u32 PHICH_C_Offset;
	u32 PHICH_D_Offset;
	u32 PDCCH_C_Offset;
	u32 PDCCH_D_Offset;
	u32 PDSCH_C_Offset;
	u32 PDSCH_D_Offset;
}__attribute__((packed))ENB_DL_TYPE1_PUBLIC_C;

typedef struct ENB_DL_TYPE1_PBCH_C
{
	u16	NumPrbBw;
	u16 PhichDurtion;
	u16	GPhichNg;
	//SFN�ڱ�ͷ���� ���ﲻ�ٸ���
	//����������Ҫ���ǵ�����SFNҲ�ǿ��Ե�
	u16	spare;
}__attribute__((packed))ENB_DL_TYPE1_PBCH_C;

typedef struct ENB_DL_TYPE1_PBCH_D
{
	u32 SourcePBCH;
}__attribute__((packed))ENB_DL_TYPE1_PBCH_D;


typedef struct ENB_DL_TYPE1_PDSCH_C
{
	u16	RNTI;
	u16 NumCW;
	u16	NumPrbofUe;
	u16	UeCategory;
	u16	UeTransMod;
	u16	NumHarqPro;
	u16	RvIdx;
	u16	Modulation;
	u16	NumLayers;
	u16	DelayMod;
	u16	PA;
	u16	zero;
	u32	PDSCHTbSize;
	u32	BitMap;
}__attribute__((packed))ENB_DL_TYPE1_PDSCH_C;

typedef struct ENB_DL_TYPE1_PDSCH_D
{
	u16	RNTI;
	u16 pdsch_length;
}__attribute__((packed))ENB_DL_TYPE1_PDSCH_D;
//������������ǻ�ֱ�Ӹ���Դ�����Ȳ���

typedef struct ENB_DL_TYPE1_PDCCH_C
{
	u16	RNTI;
	u16 CommonPdcchFlag;
	u8	PdcchFormat;
	u8	DCIFormat;
	u16	DciBitLen;
}__attribute__((packed))ENB_DL_TYPE1_PDCCH_C;

typedef struct ENB_DL_TYPE1_PDCCH_D
{
	u16	RNTI;
	u16 zero;
	//u16 PDCCH_D_LEN;//2015.1.29��ӣ�����ָʾPDCCH����Դ����
	u32 dcidata0;//2015.1.29�޸ģ�DCI���ȹ̶�Ϊ64bit
	u32 dcidata1;
}__attribute__((packed))ENB_DL_TYPE1_PDCCH_D;
//������������ǻ�ֱ�Ӹ���Դ�����Ȳ���


typedef struct ENB_DL_TYPE1_PHICH_C
{
	u16	RNTI;
	u16 zero;
	u32 I_lowest_index_PRB_RA;
	u32 NDmrs;
}__attribute__((packed))ENB_DL_TYPE1_PHICH_C;

typedef struct ENB_DL_TYPE1_PHICH_D
{
	u16	RNTI;
	u16 SourcePHICH;
}__attribute__((packed))ENB_DL_TYPE1_PHICH_D;

typedef struct ENBPHYADPtoPHYType1
{
	GeneralHeader12 GHead;
	ENB_DL_TYPE1_PUBLIC_C DL_TYPE1_PUBLIC_C;
}__attribute__((packed))ENBPHYADPtoPHYType1;
//֮�������˳��ΪPBCH��C&D PHICH��C&D PDCCH��C&D PDSCH��C&D

/*========End of ENB��������·Type1�еĲ�����==================*/

/*========������ENB��������·Type3�еĲ�����,������==================*/
typedef struct ENB_DL_TYPE3_PUBLIC_C
{
	u32 PDSCHOffset;
	u32 PDCCHOffset;
}__attribute__((packed))ENB_DL_TYPE3_PUBLIC_C;

typedef struct ENB_DL_TYPE3_PDSCH_C//2015.1.30�޸�
{
	u16	CellID;
	u16 NumAntPort;
	u16	CFI;
	u16	CpType;
	u16	SFDirection;
	u16	PB;
}__attribute__((packed))ENB_DL_TYPE3_PDSCH_C;

typedef struct ENB_DL_TYPE3_PDCCH_C
{
	u16	SpecSubFrmCfg;
}__attribute__((packed))ENB_DL_TYPE3_PDCCH_C;


typedef struct ENBPHYADPtoPHYType3
{
	//�����ENB��������·Type3������ṹ
	GeneralHeader345 GHead;
	ENB_DL_TYPE3_PUBLIC_C DL_TYPE3_PUBLIC_C;
	ENB_DL_TYPE3_PDCCH_C DLType3Pdcch_C;
	ENB_DL_TYPE3_PDSCH_C DLTpye3Pdsch_C;
	
}__attribute__((packed))ENBPHYADPtoPHYType3;

/*========End of ENB��������·Type3�еĲ�����==================*/


/*========������UE��������·Type1�еĲ�����,������==================*/
typedef struct UE_DL_TYPE1_PUBLIC_C
{
	u32 PDSCHOffset;
	u32 PDCCHOffset;
}__attribute__((packed))UE_DL_TYPE1_PUBLIC_C;

typedef struct UE_DL_TYPE1_PDSCH_C
{
	u16 PA;
	u16 PB;
	u16 RNTI;
	u16 UeCategory;
	u16 DelayMod;
}__attribute__((packed))UE_DL_TYPE1_PDSCH_C;

typedef struct UE_DL_TYPE1_PDCCH_C
{
	u16 PdcchNum;
	u16 RNTI;
}__attribute__((packed))UE_DL_TYPE1_PDCCH_C;

typedef struct UEPHYADPtoPHYType1
{
	//UE��Type1������ṹ
	GeneralHeader12 GHead;
	UE_DL_TYPE1_PUBLIC_C DL_TYPE1_PUBLIC_C;
	UE_DL_TYPE1_PDCCH_C DL_TYPE1_PDCCH_C;  //
	UE_DL_TYPE1_PDSCH_C DL_TYPE1_PDSCH_C;
}__attribute__((packed))UEPHYADPtoPHYType1;
/*========End of UE��������·Type1�еĲ�����==================*/


/*========������UE��������·Type2�еĲ�����,������==================*/
typedef struct UE_DL_TYPE2_PUBLIC_C
{
	u32 PDSCHOffset;
	u32 PDCCHOffset;
}__attribute__((packed))UE_DL_TYPE2_PUBLIC_C;

typedef struct UE_DL_TYPE2_PBCH_D
{
	u32 SourcePBCH;
}__attribute__((packed))UE_DL_TYPE2_PBCH_D;

typedef struct UE_DL_TYPE2_PDSCH_C
{
	u32 CRC;
}__attribute__((packed))UE_DL_TYPE2_PDSCH_C;

/*
typedef struct UE_DL_TYPE2_PDSCH_D
{
	u32 SourcePDSCH;
}__attribute__((packed))UE_DL_TYPE2_PDSCH_D;
*/

typedef struct UE_DL_TYPE2_PDCCH_C
{
	u32 HARQprocessID;
	u32 RV;
	u16	NDI;
}__attribute__((packed))UE_DL_TYPE2_PDCCH_C;

/*
typedef struct UE_DL_TYPE2_PDCCH_D
{
	u32 DCIData;
}__attribute__((packed))UE_DL_TYPE2_PDCCH_D;
*/

typedef struct UE_DL_TYPE2_PHICH_D
{
	u32	HARQIndex;
}__attribute__((packed))UE_DL_TYPE2_PHICH_D;

typedef struct UEPHYADPtoPHYType2
{
	//UE��Type2������ṹ
	GeneralHeader12 GHead;
	UE_DL_TYPE2_PUBLIC_C DL_TYPE2_PUBLIC_C;
}__attribute__((packed))UEPHYADPtoPHYType2;
//֮�������˳��ΪPBCH��D PHICH��D PDCCH��C&D PDSCH��C&D

/*========End of UE��������·Type2�еĲ�����==================*/

/*========������UE��������·Type3�еĲ�����,������==================*/
typedef struct UE_DL_TYPE3_PUBLIC_C
{
	u32 PDSCHOffset;
	u32 PDCCHOffset;
}__attribute__((packed))UE_DL_TYPE3_PUBLIC_C;

typedef struct UE_DL_TYPE3_PDSCH_C
{
	u16	DL_Status;
}__attribute__((packed))UE_DL_TYPE3_PDSCH_C;

typedef struct UE_DL_TYPE3_PDCCH_C
{
	u16	SFDirection;
	u16	T_MODE;
	u16	PdcchNum;
	u16	RNTI;
	u16	UeCategory;
	u32	MemStart;
	u32	MemSize;
}__attribute__((packed))UE_DL_TYPE3_PDCCH_C;

typedef struct UEPHYADPtoPHYType3
{
	//UE��Type3������ṹ
	GeneralHeader345 GHead;
	UE_DL_TYPE3_PUBLIC_C DL_TYPE3_PUBLIC_C;
	UE_DL_TYPE1_PDCCH_C DL_TYPE1_PDCCH_C; 
	UE_DL_TYPE1_PDSCH_C DL_TYPE1_PDSCH_C;
}__attribute__((packed))UEPHYADPtoPHYType3;


/*========End of ENB��������·Type3�еĲ�����==================*/

typedef struct ENB_DL_DEVICE_ID
{
	u32	NewFlag;
	u16 MACID_1;
	u16 MACID_2;
	u16 DSPID_1;
	u16 DSPID_2;
	u16 FPGAID_1;
    u16 FPFAID_2;
}__attribute__((packed))ENB_DL_DEVICE_ID;
