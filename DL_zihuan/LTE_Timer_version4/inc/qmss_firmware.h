


#ifndef QMSS_FW_H_
#define QMSS_FW_H_

#ifdef __cplusplus
extern "C" {
#endif

/* PDSP firmware files */
#include "acc16_le_bin.h"
#include "acc16_be_bin.h"
#include "acc32_le_bin.h"
#include "acc32_be_bin.h"
#include "acc48_le_bin.h"
#include "acc48_be_bin.h"
#include "qos_le_bin.h"
#include "qos_be_bin.h"

/** @addtogroup QMSS_LLD_DATASTRUCT
@{ 
*/        

/** 
 * @brief PDSP Firmware symbols.
 */
/** @brief 32 channel high priority accumulation little endian firmware */
extern const unsigned int acc32_le[];
/** @brief 32 channel high priority accumulation big endian firmware */
extern const unsigned int acc32_be[];
/** @brief 16 channel low priority accumulation little endian firmware */
extern const unsigned int acc16_le[];
/** @brief 16 channel low priority accumulation big endian firmware */
extern const unsigned int acc16_be[];
/** @brief 48 channel combined high and low priority accumulation little endian firmware */
extern const unsigned int acc48_le[];
/** @brief 48 channel combined high and low priority accumulation big endian firmware */
extern const unsigned int acc48_be[];
/** @brief QoS little endian firmware */
extern const unsigned int qos_le[];
/** @brief QoS big endian firmware */
extern const unsigned int qos_be[];

/** 
@} 
*/

#ifdef __cplusplus
}
#endif

#endif /* QMSS_FW_H_ */

