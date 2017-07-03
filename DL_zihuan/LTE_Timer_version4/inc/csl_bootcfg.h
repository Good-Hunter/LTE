
#ifndef _CSL_BOOTCFG_H_
#define _CSL_BOOTCFG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "soc.h"
#include "csl.h"
#include "cslr_bootcfg.h"

/** @defgroup CSL_BOOTCFG_API Boot Configuration
 *
 * @section Introduction
 *
 * @subsection 
 *  None
 *
 * @subsection References
 *  None
 */

/**
@defgroup CSL_BOOTCFG_SYMBOL  BOOT Configuration Symbols Defined
@ingroup  CSL_BOOTCFG_API
*/

/**
@defgroup CSL_BOOTCFG_FUNCTION  BOOT Configuration Functions
@ingroup  CSL_BOOTCFG_API
*/

/**
@addtogroup CSL_BOOTCFG_SYMBOL
@{
*/

/**
 *  Handle to access BOOTCFG registers.
 */
#define hBootCfg     ((CSL_BootcfgRegs*)CSL_BOOT_CFG_REGS)

/**
@}
*/

#ifdef __cplusplus
}
#endif

#endif /* _CSL_BOOTCFG_H_ */


