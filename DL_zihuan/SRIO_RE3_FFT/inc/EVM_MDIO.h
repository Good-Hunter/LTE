
#ifndef __EVM_MDIO_H__
#define __EVM_MDIO_H__


#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 


/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include <tistdtypes.h>


/****************************************************************************
 *                             Local constants                              *
 ****************************************************************************/

// Error codes
#define EVM_MDIO_OK              0
#define EVM_MDIO_READ_ERROR      1
#define EVM_MDIO_WRITE_ERROR     2
#define EVM_MDIO_REQUEST_FAILURE 3
#define EVM_MDIO_RELEASE_FAILURE 4


/****************************************************************************
 *                           Forward Declarations                           *
 ****************************************************************************/


/****************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/

/****************************************************************************
 *   NAME : EVM_MDIO_Init
 ************************************************************************//**
 *
 *   Init mdio module 
 *
 *   @return
 *      !=EVM_MDIO_OK for failure, EVM_MDIO_OK for success
 *   
 *   @ingroup EVM_MDIO
 *
 ***************************************************************************/   
 Int32 EVM_MDIO_Init();
 

/****************************************************************************
 *   NAME : EVM_MDIO_RequestBus
 ************************************************************************//**
 *
 *   Requests the MDIO control 
 *
 *   @return
 *      !=EVM_MDIO_OK for failure, EVM_MDIO_OK for success
 *   
 *   @ingroup EVM_MDIO
 *
 ***************************************************************************/   
 Int32 EVM_MDIO_RequestBus();
 

/****************************************************************************
 *   NAME : EVM_MDIO_ReleaseBus
 ************************************************************************//**
 *
 *   Releases the MDIO control 
 *
 *   @return
 *      !=EVM_MDIO_OK for failure, EVM_MDIO_OK for success
 *   
 *   @ingroup EVM_MDIO
 *
 ***************************************************************************/   
Int32 EVM_MDIO_ReleaseBus();


/****************************************************************************
 *   NAME : EVM_MDIO_PhyRead
 ************************************************************************//**
 *
 *   This function reads a register of the specified PHY
 *
 *   @param [in] PhyAddress
 *      Specified PHY    
 *   
 *   @param [in] Reg
 *      Register index to read
 *   
 *   @param [out] Value
 *      Value read from the PHY register  
 *   
 *   @return
 *      !=EVM_MDIO_OK for failure, EVM_MDIO_OK for success
 *
 ***************************************************************************/
 Int32 EVM_MDIO_PhyRead(Uint8 Reg, Uint8 PhyAddr, Uint32* Value);


/****************************************************************************
 *   NAME : EVM_MDIO_PhyWrite
 ************************************************************************//**
 *
 *   This function writes a register of the specified PHY
 *
 *   @param [in] PhyAddress
 *      Specified PHY    
 *   
 *   @param [in] Reg
 *      Register index to write
 *   
 *   @param [in] Value
 *      Value to write to the PHY register  
 *   
 *   @return
 *      !=EVM_MDIO_OK for failure, EVM_MDIO_OK for success
 *
 ***************************************************************************/
Int32 EVM_MDIO_PhyWrite(Uint8 Reg, Uint8 PhyAddr, Uint32 Value);


#ifdef __cplusplus
} 
#endif  // extern "C"

#endif //__EVM_FPGA_H__
   






































