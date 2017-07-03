

#ifndef _TCP3D_DRV_TYPES_H_
#define _TCP3D_DRV_TYPES_H_

/* c99 types includes */
#include <stdint.h>
#include <stdlib.h>

/** @addtogroup TCP3D_DRV_MACRO
 @{ */

/**
 * @brief   Key work used with the function definitions to represent them as 
 *          static and inline type functions.
 */
#define INLINE static inline

/**
 * @brief   Key work for restrict abstracted to support various compilers.
 */
#define RESTRICT restrict

/**
 * @brief   Key word used for indicating the argument as INPUT only.
 */
#define IN

/**
 * @brief   Key word used for indicating the argument as OUTPUT only.
 */
#define OUT

/**
 * @brief   Key word used for indicating the argument as both INPUT and OUTPUT.
 */
#define INOUT

/**
@}
*/

#endif /* _TCP3D_DRV_TYPES_H_ */
