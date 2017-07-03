
#ifndef __SGMII_H__
#define __SGMII_H__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 

#include "tistdtypes.h"


void serdes_init_RefClk (int sgmii0, int sgmii1);
void serdes_test_pattern (void);
void serdes_loopback_init (void);
void sgmii0_loopback_init (void);
void sgmii1_loopback_init (void);
void sgmii0_init_AN (void);
void sgmii1_init_AN (void);
int sgmii0_init_new (int enable, int loopback);
int sgmii1_init_new (int enable, int loopback);
void sgmii0_init_slave (void);
void sgmii1_init_slave (void);
void sgmii0_init (void);
void sgmii1_init (void);
void sgmii_status (void);
void stream_switch_init (void);
void ale_init (void);
void emac1_init (void);
void emac1_init_AN (void);
void emac2_init (void);
void emac2_init_AN (void);
void cpsw_init (void);
unsigned int LclToGbl (unsigned int LocalPtr);
unsigned int enable_module(unsigned int pdctl, unsigned int mdctl);


#ifdef __cplusplus
}
#endif //__cplusplus 
#endif //__SGMII_H__

