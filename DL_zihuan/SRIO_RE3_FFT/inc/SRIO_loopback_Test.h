
#ifndef _SRIO_LOOPBACK_TEST_H_
#define _SRIO_LOOPBACK_TEST_H_

extern SRIO_Packet_Type packet_type[];

extern Uint32 directIO_src_dest[][2];

extern Uint32 message_src_dest[][2];

/*test SRIO transfer through single port*/
extern void srio_signle_port_loopback_test(Uint32 uiPortNum,
	SRIO_Loopback_Mode loopback_mode);

/*transfer through multiple SRIO ports in parallel*/
extern void srio_multiple_port_transfer(SRIO_Multiple_Test_Config * test_cfg);

#endif

