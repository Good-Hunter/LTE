
#include "Keystone_Serdes_init.h"

void Keystone_SRIO_HyperLink_Serdes_init(
	SerdesSetup_4links * serdes_cfg, SerdesRegs * serdesRegs)
{
	int i;
	float pllMpy, serdesPllClock_GHz=12.5, rateScale;
	SerdesLinkRate linkRateScale; 
	SerdesVcoRange vcoRange;

	if(NULL== serdes_cfg)
		return;
		
	CSL_BootCfgUnlockKicker();

	/*find lowest link speed, the SERDES PLL output should be less or equal to it*/
	for(i= 0; i<4; i++)
	{
		if(serdes_cfg->linkSetup[i])
			if(serdesPllClock_GHz>serdes_cfg->linkSetup[i]->linkSpeed_GHz)
				serdesPllClock_GHz=serdes_cfg->linkSetup[i]->linkSpeed_GHz;
	}

	/*Serdes PLL output must be less than 3.125*/
	if(serdesPllClock_GHz>6.25)
		serdesPllClock_GHz /= 4;
	else if(serdesPllClock_GHz>3.125)
		serdesPllClock_GHz /= 2;

	/*set VCO range according to the PLL output speed*/
	if(serdesPllClock_GHz<2.17)
		vcoRange= SERDES_PLL_VCO_RANGE_HIGH;
	else
		vcoRange= SERDES_PLL_VCO_RANGE_LOW;

	/*calculate PLL MPY factor according to input reference clock speed*/
	pllMpy= serdesPllClock_GHz*1000/serdes_cfg->commonSetup.inputRefClock_MHz;

	/*PLL multiply factors between 4 and 60*/
	if(pllMpy<4 || pllMpy>60)
		pllMpy= 4;

	serdesRegs->CFGPLL = 1| /*ENPLL*/
		(serdes_cfg->commonSetup.loopBandwidth<<11)|
		(vcoRange<<9)|
		((Uint32)(pllMpy*4)<<1);

	for(i=0; i<4; i++)
	{
		if(serdes_cfg->linkSetup[i])
		{
			rateScale=serdes_cfg->linkSetup[i]->linkSpeed_GHz/serdesPllClock_GHz;
			if(rateScale<1)
				linkRateScale = SRIO_SERDES_LINK_RATE_div2;
			else if(rateScale<2)
				linkRateScale = SRIO_SERDES_LINK_RATE_x1;
			else if(rateScale<4)
				linkRateScale = SRIO_SERDES_LINK_RATE_x2;
			else
				linkRateScale = SRIO_SERDES_LINK_RATE_x4;
				
			serdesRegs->link[i].CFGRX= 1| /*ENRX*/
				(2<<1)| 	/*Bus width must be 010b (20 bit)*/
				(serdes_cfg->linkSetup[i]->testPattern<<25)|
				(serdes_cfg->linkSetup[i]->loopBack<<23)|
				(1<<22)| 	/*Enable offset compensation*/
				(serdes_cfg->linkSetup[i]->rxEqualizerConfig<<18)|
				(serdes_cfg->linkSetup[i]->rxCDR<<15)|
				(serdes_cfg->linkSetup[i]->rxLos<<12)|
				(serdes_cfg->linkSetup[i]->rxAlign<<10)|
				(serdes_cfg->linkSetup[i]->rxTermination<<7)|
				(serdes_cfg->linkSetup[i]->rxInvertPolarity<<6)|
				(linkRateScale<<4);

			if(i==0)
				serdesRegs->link[i].CFGTX= 1| /*ENTX*/
					(2<<1)| 	/*Bus width must be 010b (20 bit)*/
					(serdes_cfg->linkSetup[i]->testPattern<<23)|
					(serdes_cfg->linkSetup[i]->loopBack<<21)|
					(1 << 20)| 	/*Synchronization Master.*/
					(serdes_cfg->linkSetup[i]->txOutputSwing<<7)|
					(serdes_cfg->linkSetup[i]->txInvertPolarity<<6)|
					(linkRateScale<<4);
			else
				serdesRegs->link[i].CFGTX= 1| /*ENTX*/
					(2<<1)| 	/*Bus width must be 010b (20 bit)*/
					(serdes_cfg->linkSetup[i]->testPattern<<23)|
					(serdes_cfg->linkSetup[i]->loopBack<<21)|
					(0 << 20)| 	/*Synchronization Master.*/
					(serdes_cfg->linkSetup[i]->txOutputSwing<<7)|
					(serdes_cfg->linkSetup[i]->txInvertPolarity<<6)|
					(linkRateScale<<4);
		}
	}

}

void Keystone_Serdes_disable(
	SerdesRegs * serdesRegs, Uint32 uiLinkNum)
{
	int i;
	
	CSL_BootCfgUnlockKicker();

	//disable PLL
	serdesRegs->CFGPLL = 0;

	//disable TX/RX links
	for(i= 0; i< uiLinkNum; i++)
	{
		serdesRegs->link[i].CFGRX= 0;
		serdesRegs->link[i].CFGTX= 0;
	}
}

void Keystone_SGMII_Serdes_init(
	SerdesSetup_2links * serdes_cfg, SerdesRegs * serdesRegs)
{
	int i;
	float pllMpy;

	if(NULL== serdes_cfg)
		return;
		
	CSL_BootCfgUnlockKicker();

	/*calculate PLL MPY factor according to input reference clock speed*/
	pllMpy= 1250/serdes_cfg->commonSetup.inputRefClock_MHz;

	/*PLL multiply factors between 4 and 60*/
	if(pllMpy<4 || pllMpy>60)
		pllMpy= 4;

	serdesRegs->CFGPLL = 1| /*ENPLL*/
		(serdes_cfg->commonSetup.loopBandwidth<<11)|
		(SERDES_PLL_VCO_RANGE_HIGH<<9)|
		((Uint32)(pllMpy*4)<<1);

	for(i=0; i<2; i++)
	{
		if(serdes_cfg->linkSetup[i])
		{
			serdesRegs->link[i].CFGRX= 1| /*ENRX*/
				(serdes_cfg->linkSetup[i]->loopBack<<23)|
				(1<<22)| 	/*Enable offset compensation*/
				(serdes_cfg->linkSetup[i]->rxEqualizerConfig<<18)|
				(serdes_cfg->linkSetup[i]->rxCDR<<15)|
				(serdes_cfg->linkSetup[i]->rxLos<<12)|
				(serdes_cfg->linkSetup[i]->rxAlign<<10)|
				(serdes_cfg->linkSetup[i]->rxTermination<<7)|
				(serdes_cfg->linkSetup[i]->rxInvertPolarity<<6)|
				(SGMII_SERDES_LINK_RATE_x1<<4);

			serdesRegs->link[i].CFGTX= 1| /*ENTX*/
				(serdes_cfg->linkSetup[i]->loopBack<<20)|
				(0 << 16)| 	/*Synchronization Master.*/
				(serdes_cfg->linkSetup[i]->txOutputSwing<<8)|
				(1<<7)| 	/*Common mode adjustment*/
				(serdes_cfg->linkSetup[i]->txInvertPolarity<<6)|
				(SGMII_SERDES_LINK_RATE_x1<<4);
		}
	}

}


