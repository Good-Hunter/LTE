


#ifndef __EVM_I2C_H__
#define __EVM_I2C_H__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 

/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include <tistdtypes.h>


/****************************************************************************
 *                             Public Constants                             *
 ****************************************************************************/


 /** @name I2C Error Codes
 *  I2C functions error status codes.
 */
//@{
#define EVM_I2C_OK           0         ///< No error
#define EVM_I2C_FAIL         0x2001    ///< Generic Failure
#define EVM_I2C_BUSBUSY      0x2002    ///< Problem BUS busy before writing command
#define EVM_I2C_MASTERCLRERR 0x2003    ///< Problem Master mode clear do not happened
#define EVM_I2C_READYTXERR   0x2003    ///< Problem not ready to transmit after a while
#define EVM_I2C_REGACCRDYERR 0x2004    ///< Problem waiting register access ready
#define EVM_I2C_BUSNOTBUSY   0x2005    ///< Problem BUS not busy timeout
#define EVM_I2C_NAKTIMEOUT   0x2007    ///< Problem maximum number of NAK has been reach
#define EVM_I2C_DATARDYERR   0x2008    ///< Problem waiting for data ready
#define EVM_I2C_ENABLEI2CERR 0x2009    ///< Problem cannot enable i2c controler
//@}


/****************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/

/****************************************************************************
 *   NAME : EVM_I2C_writereg
 ************************************************************************//**
 *
 *   This function writes a value (byte) to a register of the specified
 *   device using the I2C bus.
 *
 *   @param [in] devAddr
 *      Id of the device to write to (i2c 7 bits slave address)
 *
 *   @param [in] reg
 *      Register offset within the device
 *
 *   @param [in] val
 *      Value to be written
 *  
 *   @return    Error code: EVM_I2C_OK for success, !=EVM_I2C_OK if problem
 *   
 *   @ingroup EVM_I2C
 *
 ***************************************************************************/ 
Int32 EVM_I2C_writereg( Int32 devAddr, Uint8 reg, Uint8 val );


/****************************************************************************
 *   NAME : EVM_I2C_writemem
 ************************************************************************//**
 *
 *   This function writes N bytes to the specified device on i2c bus.
 *   This function is usually used for memory device type (16 bits address).
 *
 *   @param [in] devAddr
 *      Id of the device to write to (i2c 7 bits slave address)
 *
 *   @param [in] addr
 *      Address (16 bits memory offset address) 
 *
 *   @param [in] buf
 *      Buffer of data to write
 *                         
 *   @param [in] n       
 *      Number of bytes to write
 *
 *   @return    Error code: EVM_I2C_OK for success, !=EVM_I2C_OK if problem
 *   
 *   @ingroup EVM_I2C
 *
 ***************************************************************************/   
Int32 EVM_I2C_writemem( Int32 devAddr, Uint16 addr, Uint8 *buf, Int32 n );


/****************************************************************************
 *   NAME : EVM_I2C_writeregbuf
 ************************************************************************//**
 *
 *   This function writes N bytes to the specified device register from
 *   a buffer.
 *
 *   @param [in] devAddr
 *      Id of the device to write to (i2c 7 bits slave address)
 *
 *   @param [in] addr
 *      Address (8 bits register offset in the device)
 *
 *   @param [in] buf
 *      Buffer of data to write
 *                         
 *   @param [in] n       
 *      Number of bytes to write
 *
 *   @return    Error code: EVM_I2C_OK for success, !=EVM_I2C_OK if problem
 *   
 *   @ingroup EVM_I2C
 *
 ***************************************************************************/                            
Int32 EVM_I2C_writeregbuf( Int32 devAddr, Uint8 addr, Uint8 *buf, Int32 n );


/****************************************************************************
 *   NAME : EVM_I2C_readreg
 ************************************************************************//**
 *
 *   This function reads the value (byte) of a register of the specified device
 *      using the I2C bus.
 *
 *   @param [in] devAddr
 *      Id of the device to write to (i2c 7 bits slave address)
 *
 *   @param [in] reg
 *      Register offset within the device
 *
 *   @param [in] val
 *      Value read
 *  
 *   @return    Error code: EVM_I2C_OK for success, !=EVM_I2C_OK if problem
 *   
 *   @ingroup EVM_I2C
 *
 ***************************************************************************/   
Int32 EVM_I2C_readreg( Int32 devAddr, Uint8 reg, Uint8 *val );


/****************************************************************************
 *   NAME : EVM_I2C_readmem
 ************************************************************************//**
 *
 *   This function read N bytes from the specified device on i2c bus.
 *   This function is usually used for memory device type (16 bits address).
 *
 *   @param [in] devAddr
 *      Id of the device to write to (i2c 7 bits slave address)
 *
 *   @param [in] addr
 *      Address (16 bits memory offset address) 
 *
 *   @param [in] buf
 *      Buffer of data that will get the data read
 *                         
 *   @param [in] n       
 *      Number of bytes to read
 *
 *   @return    Error code: EVM_I2C_OK for success, !=EVM_I2C_OK if problem
 *   
 *   @ingroup EVM_I2C
 *
 ***************************************************************************/     
Int32 EVM_I2C_readmem( Int32 devAddr, Uint16 addr, Uint8 *buf, Int32 n );


/**************************************************************************** 
 *   NAME : EVM_I2C_readregbuf
 ************************************************************************//**
 *
 *   This function reads N bytes for register addr from the specified device.
 *   It can be used to read buffer from a 8 bits register offset.
 *
 *   @param [in] devAddr
 *      Id of the device to write to (i2c 7 bits slave address)
 *
 *   @param [in] addr
 *      Address (8 bits register offset in the device)
 *
 *   @param [in] buf
 *      BBuffer of data to get the read values
 *                         
 *   @param [in] n       
 *      Number of bytes to read
 *
 *   @return    Error code: EVM_I2C_OK for success, !=EVM_I2C_OK if problem
 *   
 *   @ingroup EVM_I2C
 *
 ***************************************************************************/   
Int32 EVM_I2C_readregbuf( Int32 devAddr, Uint8 addr, Uint8 *buf, Int32 n );


#ifdef __cplusplus
}
#endif //__cplusplus
#endif //__EVM_I2C_H__

