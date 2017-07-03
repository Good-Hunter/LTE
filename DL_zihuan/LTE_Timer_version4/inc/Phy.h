
#ifndef __PHY_H__
#define __PHY_H__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 

#include "tistdtypes.h"

#define PHY0 0
#define PHY1 1
#define PHY2 2

#define PHYSLAVE  0
#define PHYMASTER 1

#define PHYENABLE  1
#define PHYDISABLE 0

#define PORT0 0x10
#define PORT1 0x11
#define PORT2 0x12
#define PORT3 0x13
#define PORT4 0x14
#define PORT5 0x15
#define PORT6 0x16
#define PORT7 0x17
/****************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/
Int32 Phy_Init(int Phy, int Serdes);
Int32 Phy_Reset(int Reset);
Int32 Phy_readSpeed(Uint8 PhyAddress);
Int32 Phy_ReadMasterSlaveState(Uint8 PhyAddress);
Int32 Phy_CheckLinkStatus(Uint8 PhyAddress);

#ifdef __cplusplus
}
#endif //__cplusplus 
#endif //__PHY_H__

























