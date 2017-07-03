/***********************************************/
/*ifft_data.c                                  */
/*功能：ifft后的发送数据                       */
/***********************************************/
#include "system_init.h"
#pragma DATA_ALIGN (gTxBuffer, 8)
#pragma DATA_SECTION(ifft_subframe, ".testData")

#ifdef SINGLE_USER
unsigned int ifft_subframe[]=
{
    #include "ifft_subframe0_pbch_pcfich_pdcch_rnti61"
    #include "ifft_subframe1_pbch_pcfich_pdcch_rnti61"
    //#include "ifft_subframe3_pbch_pcfich_pdcch_rnti61"
	#include "ifft_subframe3_pbch_pcfich_pdcch_phich_1user"
    #include "ifft_subframe4_pbch_pcfich_pdcch_rnti61"
    #include "ifft_subframe5_pbch_pcfich_pdcch_rnti61"
    #include "ifft_subframe6_pbch_pcfich_pdcch_rnti61"
    //#include "ifft_subframe8_pbch_pcfich_pdcch_rnti61"
    #include "ifft_subframe8_pbch_pcfich_pdcch_phich_1user"
    #include "ifft_subframe9_pbch_pcfich_pdcch_rnti61"
};
#endif
#ifdef TWO_USER
unsigned int ifft_subframe[]=
{
    #include "pcfich_pbch_pdcch_2ue_ifft_data_subframe0.dat"
    #include "pcfich_pbch_pdcch_2ue_ifft_data_subframe1.dat"
    #include "pcfich_pbch_pdcch_2ue_ifft_data_subframe3.dat"
    #include "pcfich_pbch_pdcch_2ue_ifft_data_subframe4.dat"
    #include "pcfich_pbch_pdcch_2ue_ifft_data_subframe5.dat"
    #include "pcfich_pbch_pdcch_2ue_ifft_data_subframe6.dat"
    #include "pcfich_pbch_pdcch_2ue_ifft_data_subframe8.dat"
    #include "pcfich_pbch_pdcch_2ue_ifft_data_subframe9.dat"
};
#endif
//五用户修改

#ifdef FIVE_USER
unsigned int ifft_subframe[]=
{
    #include "5users_subframe0.dat"
    #include "5users_subframe1.dat"
    #include "5users_subframe3.dat"
    #include "5users_subframe4.dat"
    #include "5users_subframe5.dat"
    #include "5users_subframe6.dat"
    #include "5users_subframe8.dat"
    #include "5users_subframe9.dat"

};


#endif
#ifdef TEN_USER
unsigned int ifft_subframe[]=
{
    #include "pcfich_pbch_pdcch_10ue_ifft_data_subframe0.dat"
    #include "pcfich_pbch_pdcch_10ue_ifft_data_subframe1.dat"
    //#include "pcfich_pbch_pdcch_10ue_ifft_data_subframe3.dat"
    #include "pcfich_pbch_pdcch_phich_10ue_ifft_data_subframe3.dat"
    #include "pcfich_pbch_pdcch_10ue_ifft_data_subframe4.dat"
    #include "pcfich_pbch_pdcch_10ue_ifft_data_subframe5.dat"
    #include "pcfich_pbch_pdcch_10ue_ifft_data_subframe6.dat"
   // #include "pcfich_pbch_pdcch_10ue_ifft_data_subframe8.dat"
    #include "pcfich_pbch_pdcch_phich_10ue_ifft_data_subframe8.dat"
    #include "pcfich_pbch_pdcch_10ue_ifft_data_subframe9.dat"

};
#endif

//unsigned int ifft_subframe[SUBFRAME_LEN * NUM_MAX_ANTENNA];
unsigned int fft_output_opt_data[] =
{
#include "fft_output_opt_data.dat"
};

