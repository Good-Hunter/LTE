
#ifndef _SRIO_INTERNAL_LOOPBACK_TEST_H_
#define _SRIO_INTERNAL_LOOPBACK_TEST_H_

/*test SRIO transfer through single port*/
extern void srio_signle_port_internal_loopback_test(SRIO_Loopback_Mode loopback_mode);

/*test SRIO transfer through multiple ports in parallel*/
extern void srio_multiple_ports_internal_loopback_test();

#endif

