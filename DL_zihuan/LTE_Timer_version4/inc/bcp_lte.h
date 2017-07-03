
#ifndef _BCP_LTE_H_
#define _BCP_LTE_H_

/* LTE Test configuration definitions */

#define MAX_CODE_BLOCK_SIZE     6144
#define MAX_BLOCK_INDEX         189

/* LTE Channel types */
#define LTE_PDSCH               0
#define LTE_PDCCH               1
#define LTE_PUSCH_SIC           2
#define LTE_PUSCH               3
#define LTE_PUCCH               4
#define LTE_PDCCHM              5
#define LTE_PUSCH_SIC_HARD      6
#define LTE_PBCH                7

typedef struct _BcpTest_LteCBParams
{
    unsigned int       numCodeBks;
    unsigned int       outputbitsNoFiller;
    unsigned int       numCodeBksKp;
	unsigned int       numCodeBksKm;
    unsigned int       codeBkSizeKp;
	unsigned int       codeBkSizeKm;
	uint16_t       numFillerBits;
	uint16_t       f1Km;
	uint16_t       f2Km;
	uint16_t       f1Kp;
	uint16_t       f2Kp;
} BcpTest_LteCBParams;

typedef struct _BcpTest_RateMatchParams
{
    unsigned int      E0;
    unsigned int      E1;
    unsigned int      Gamma;
	unsigned int      NcbKm;
	unsigned int      NcbKp;
	unsigned int      rvKm;
	unsigned int      rvKp;
	unsigned int      rvKmCol;
	unsigned int      rvKpCol;
	unsigned int      NcbKmCol;
	unsigned int      NcbKmRow;
	unsigned int      NcbKpCol;
	unsigned int      NcbKpRow;
} BcpTest_RateMatchParams;

#endif /* _BCP_LTE_H_ */

