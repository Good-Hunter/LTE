

#ifndef _CSL_MDIO_H_
#define _CSL_MDIO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "soc.h"
#include "csl.h"
#include "cslr_mdio.h"

/**
@defgroup CSL_MDIO_SYMBOL  MDIO Symbols Defined
@ingroup CSL_MDIO_API
*/
/**
@defgroup CSL_MDIO_DATASTRUCT  MDIO Data Structures
@ingroup CSL_MDIO_API
*/
/**
@defgroup CSL_MDIO_FUNCTION  MDIO Functions
@ingroup CSL_MDIO_API
*/
/**
@defgroup CSL_MDIO_ENUM MDIO Enumerated Data Types
@ingroup CSL_MDIO_API
*/
    
/**
@addtogroup CSL_MDIO_SYMBOL
@{
*/

/**
@}
*/

/** @addtogroup CSL_MDIO_DATASTRUCT
 @{ */

/** Constants for passing parameters to the functions.
 */
/** @brief Pointer to the MDIO overlay registers */
#define     hMdioRegs           ((CSL_Mdio_Regs *) (CSL_PA_SS_CFG_REGS + 0x00090300))

/** @brief      
 *
 *  Holds the MDIO peripheral's version info. 
 */        
typedef struct {
	/**  Minor revision value */	
	Uint32      revMin;

	/**  Major revision value */	
	Uint32      revMaj;

	/**  Identifies the type of peripheral */	
	Uint32      modId;
}CSL_MDIO_VERSION;

/** @brief      
 *
 *  Holds the MDIO User Access Register
 *  contents.
 */        
typedef struct {
	/**  Data bits */	
	Uint16      data;

	/**  Phy Device Address */	
	Uint32      phyAddr;

	/**  Register Address */	
	Uint32      regAddr;

	/**  Acknowledgment bit */	
	Uint32      ack;

	/**  GO bit */	
	Uint32      go;
}CSL_MDIO_USERACCESS;

/** @brief      
 *
 *  Holds the MDIO User Phy Select Register
 *  contents.
 */        
typedef struct {
	/**  Link Status determination select bit. Always reads '0' */	
	Uint16      linkSel;

	/**  Link Status Change Interrupt Enable bit */	
	Uint32      bLinkIntEnable;

	/**  Phy Device Address */	
	Uint32      phyAddr;

}CSL_MDIO_USERPHYSEL;
        

/**
@}
*/


/** @addtogroup CSL_MDIO_FUNCTION
 @{ */

/**
@}
*/

#ifdef __cplusplus
}
#endif

#endif /* _CSL_MDIO_H */                        
