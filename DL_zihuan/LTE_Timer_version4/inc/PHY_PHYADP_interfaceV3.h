#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char

/*****以下是接口通用数据头部定义******/

typedef struct GeneralHeader12{
	//GeneralHeader12 用于Type1和Type2两种接口。因为其存放区域不同，不需要单独的标志区分Type1和Type2
	u32	NewFlag;
	u16	SFN;
	u16	SubframeN;
}__attribute__((packed))GeneralHeader12;

typedef struct GeneralHeader345
{
	//GeneralHeader345 用于Type3和Type4和Type5三种接口。因为其存放区域不同，不需要单独的标志区分
	u32	NewFlag;
	union {
		u32 CmdFlag;//按位标志 配置命令参数表标志，用于Type3,按位注明对应的配置参数表是否生效
		u32 RespFlag;//按位标志 控制响应标志，用于Type4，按位对应的命令是否或响应生效
		u32 ReportFlag;//按位标志 状态上报报告标志，用于Type5，按位标志对应的状态报告是否生效
	}setflag;
}__attribute__((packed))GeneralHeader345;


//Type1,2,3,4,5中每个参数表的可选头部，如果需要的话
typedef struct ParTableHeader{
	u16 ParTableType;
	u16 ParTableLen;
}__attribute__((packed))ParTableHeader;

/*========以下是ENB侧下行链路Type1中的参数表或参数表头部==================*/

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
	//SFN在表头有了 这里不再给出
	//如果物理层需要我们单独给SFN也是可以的
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
//在这个后面我们会直接跟信源，长度不定

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
	//u16 PDCCH_D_LEN;//2015.1.29添加，用于指示PDCCH数据源长度
	u32 dcidata0;//2015.1.29修改，DCI长度固定为64bit
	u32 dcidata1;
}__attribute__((packed))ENB_DL_TYPE1_PDCCH_D;
//在这个后面我们会直接跟信源，长度不定


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
//之后的排列顺序为PBCH的C&D PHICH的C&D PDCCH的C&D PDSCH的C&D

/*========End of ENB侧下行链路Type1中的参数表==================*/

/*========以下是ENB侧下行链路Type3中的参数表,按序存放==================*/
typedef struct ENB_DL_TYPE3_PUBLIC_C
{
	u32 PDSCHOffset;
	u32 PDCCHOffset;
}__attribute__((packed))ENB_DL_TYPE3_PUBLIC_C;

typedef struct ENB_DL_TYPE3_PDSCH_C//2015.1.30修改
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
	//这个是ENB侧下行链路Type3的总体结构
	GeneralHeader345 GHead;
	ENB_DL_TYPE3_PUBLIC_C DL_TYPE3_PUBLIC_C;
	ENB_DL_TYPE3_PDCCH_C DLType3Pdcch_C;
	ENB_DL_TYPE3_PDSCH_C DLTpye3Pdsch_C;
	
}__attribute__((packed))ENBPHYADPtoPHYType3;

/*========End of ENB侧下行链路Type3中的参数表==================*/


/*========以下是UE侧下行链路Type1中的参数表,按序存放==================*/
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
	//UE侧Type1的总体结构
	GeneralHeader12 GHead;
	UE_DL_TYPE1_PUBLIC_C DL_TYPE1_PUBLIC_C;
	UE_DL_TYPE1_PDCCH_C DL_TYPE1_PDCCH_C;  //
	UE_DL_TYPE1_PDSCH_C DL_TYPE1_PDSCH_C;
}__attribute__((packed))UEPHYADPtoPHYType1;
/*========End of UE侧下行链路Type1中的参数表==================*/


/*========以下是UE侧下行链路Type2中的参数表,按序存放==================*/
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
	//UE侧Type2的总体结构
	GeneralHeader12 GHead;
	UE_DL_TYPE2_PUBLIC_C DL_TYPE2_PUBLIC_C;
}__attribute__((packed))UEPHYADPtoPHYType2;
//之后的排列顺序为PBCH的D PHICH的D PDCCH的C&D PDSCH的C&D

/*========End of UE侧下行链路Type2中的参数表==================*/

/*========以下是UE侧下行链路Type3中的参数表,按序存放==================*/
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
	//UE侧Type3的总体结构
	GeneralHeader345 GHead;
	UE_DL_TYPE3_PUBLIC_C DL_TYPE3_PUBLIC_C;
	UE_DL_TYPE1_PDCCH_C DL_TYPE1_PDCCH_C; 
	UE_DL_TYPE1_PDSCH_C DL_TYPE1_PDSCH_C;
}__attribute__((packed))UEPHYADPtoPHYType3;


/*========End of ENB侧下行链路Type3中的参数表==================*/

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
