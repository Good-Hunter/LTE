
#ifndef CSL_CPPI_H_
#define CSL_CPPI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "csl.h"

/**
@addtogroup CPPI_LLD_SYMBOL
@{
*/

/** CPPI maximum number of CPDMAs */
#define CPPI_MAX_CPDMA                      8

/**
@}
*/

/**
@addtogroup CPPI_LLD_ENUM
@{
*/

/** 
 * @brief CPPI CPDMA types
 */
typedef enum
{
    /** SRIO */
    Cppi_CpDma_SRIO_CPDMA = 0,
    /** AIF */
    Cppi_CpDma_AIF_CPDMA,
    /** FFTC A */
    Cppi_CpDma_FFTC_A_CPDMA,
    /** FFTC B */
    Cppi_CpDma_FFTC_B_CPDMA,
    /** PASS */
    Cppi_CpDma_PASS_CPDMA,
    /** QMSS */
    Cppi_CpDma_QMSS_CPDMA,
    /** FFTC C */
    Cppi_CpDma_FFTC_C_CPDMA,
    /** BCP */
    Cppi_CpDma_BCP_CPDMA
}Cppi_CpDma;

/**
@}
*/



#ifdef __cplusplus
}
#endif

#endif /* CSL_CPPI_H_ */
