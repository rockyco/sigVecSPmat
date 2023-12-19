/**
 * This function is responsible for performing a Channel Estimation task.
 * It takes in certain parameters and returns a specific value.
 * 
 * @param param1 The first parameter of the function.
 * @param param2 The second parameter of the function.
 * @return The result of the task performed by the function.
 */

#include <stdio.h>
#include "sigVecSPmat.h"

void sigVecSPmat(const bool is_lts_1,
                 const std::complex<ap_fixed<W+2,I+2,AP_TRN,AP_SAT>> hpre_sc_rx[LEN_LTS_8],
                 std::complex<ap_fixed<W,I,AP_TRN,AP_SAT>> h_sc_rx[LEN_LTS_8]) 
{
    std::complex<ap_fixed<W+2,I+2,AP_TRN,AP_SAT>> h_sc_rx_tmp[LEN_LTS_8];
#pragma HLS ARRAY_PARTITION dim=0 type=complete variable=h_sc_rx_tmp
	ap_uint<1> SP_Mat[LEN_LTS_8][LEN_LTS_8] = {
		 { 1,  0,  1,  1,  0,  1,  0,  0},
		 { 1,  1,  0,  1,  0,  0,  1,  0},
		 { 1,  1,  1,  0,  0,  0,  0,  1},
		 { 0,  1,  1,  1,  1,  0,  0,  0},
		 { 1,  0,  1,  1,  1,  0,  1,  1},
		 { 1,  1,  0,  1,  1,  1,  0,  1},
		 { 1,  1,  1,  0,  1,  1,  1,  0},
		 { 0,  1,  1,  1,  0,  1,  1,  1}
	};
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=SP_Mat

	std::complex<ap_fixed<W+2,I+2,AP_TRN,AP_SAT>> hpre_sc_rx_dup[LEN_LTS_8][LEN_LTS_8];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=hpre_sc_rx_dup

	Init_loop:
	for (int i=0; i<LEN_LTS_8; i++) {
#pragma HLS PIPELINE II=1
		for (int j=0; j<LEN_LTS_8; j++) {
			hpre_sc_rx_dup[j][i] = hpre_sc_rx[i];
		}
		h_sc_rx_tmp[i] = 0;
	}

#ifdef PIPELINE_OPT
    Acc_outer_loop:
    for (int i=0; i<LEN_LTS_8; i++) {
#pragma HLS PIPELINE II=4
    	Acc_inner_loop:
        for (int j=0; j<LEN_LTS_8; j++) {
            if (SP_Mat[j][i] == 1) {
                h_sc_rx_tmp[j] += hpre_sc_rx_dup[j][i];
            }
            else {
                h_sc_rx_tmp[j] -= hpre_sc_rx_dup[j][i];
            }
        }
    }
#endif

#ifdef UNROLL_OPT
    Acc_outer_loop:
    for (int i=0; i<LEN_LTS_8; i++) {
#pragma HLS UNROLL factor=8
    	Acc_inner_loop:
        for (int j=0; j<LEN_LTS_8; j++) {
#pragma HLS PIPELINE II=1
            if (SP_Mat[i][j] == 1) {
                h_sc_rx_tmp[i] += hpre_sc_rx_dup[i][j];
            }
            else {
                h_sc_rx_tmp[i] -= hpre_sc_rx_dup[i][j];
            }
        }
    }
#endif

    Write_out_loop:
    for (int i=0; i<LEN_LTS_8; i++) {
#pragma HLS PIPELINE II=1
        if (is_lts_1 == 1) {
            h_sc_rx[i] = h_sc_rx_tmp[i] >> 3;
        }
        else {
            h_sc_rx[i] = (-h_sc_rx_tmp[i]) >> 3;
        }
    }
}

void get_h_per_sc_per_rx(const bool is_lts_1,
                         const std::complex<ap_fixed<W+2,I+2,AP_TRN,AP_SAT> > hpre_sc_rx[LEN_LTS_8],
                         std::complex<ap_fixed<W,I,AP_TRN,AP_SAT> >           h_sc_rx   [LEN_LTS_8]) {
// #pragma HLS ARRAY_PARTITION variable=hpre_sc_rx,h_sc_rx complete dim=1
//  #pragma HLS PIPELINE II=1

  std::complex<ap_fixed<W+2,I+2,AP_TRN,AP_SAT> > hpre_cast[LEN_LTS_8], r[LEN_LTS_8];
  std::complex<ap_fixed<W+2,I+2,AP_TRN,AP_SAT> > r_h0[LEN_LTS_8], r_h02[LEN_LTS_8],r_h1[LEN_LTS_8], r0[LEN_LTS_8];

    r_h0[0] = ( hpre_sc_rx[0] - hpre_sc_rx[1]) ;
    r_h0[1] = ( hpre_sc_rx[0] + hpre_sc_rx[1]) ;
	r_h0[2] =  r_h0[1];
	r_h0[3] = -r_h0[0];
    r_h0[4] = (( hpre_sc_rx[2] + hpre_sc_rx[3]));
    r_h0[5] = ((-hpre_sc_rx[2] + hpre_sc_rx[3]));
	r_h0[6] = -r_h0[5];
	r_h0[7] =  r_h0[4];

	r_h02[0] = ( -hpre_sc_rx[4] + hpre_sc_rx[5]) ;
    r_h02[1] = ( -hpre_sc_rx[4] - hpre_sc_rx[5]) ;
	r_h02[2] =  r_h02[1];
	r_h02[3] = -r_h02[0];
    r_h02[4] = ((-hpre_sc_rx[6] - hpre_sc_rx[7]));
    r_h02[5] = (( hpre_sc_rx[6] - hpre_sc_rx[7]));
	r_h02[6] = -r_h02[5];
	r_h02[7] =  r_h02[4];

    r_h1[0] = r_h0[0] + r_h0[4];
    r_h1[1] = r_h0[1] + r_h0[5];
    r_h1[2] = r_h0[2] + r_h0[6];
    r_h1[3] = r_h0[3] + r_h0[7];
	r_h1[4] = r_h02[0] + r_h02[4];
    r_h1[5] = r_h02[1] + r_h02[5];
    r_h1[6] = r_h02[2] + r_h02[6];
    r_h1[7] = r_h02[3] + r_h02[7];

	r0[0] = r_h1[0] + r_h1[4];
	r0[1] = r_h1[1] + r_h1[5];
	r0[2] = r_h1[2] + r_h1[6];
	r0[3] = r_h1[3] + r_h1[7];
	r0[4] = r_h1[0] - r_h1[4];
	r0[5] = r_h1[1] - r_h1[5];
	r0[6] = r_h1[2] - r_h1[6];
	r0[7] = r_h1[3] - r_h1[7];

  if (is_lts_1) {
	r[0] = r0[0];
	r[1] = r0[1];
	r[2] = r0[2];
	r[3] = r0[3];
	r[4] = r0[4];
	r[5] = r0[5];
	r[6] = r0[6];
	r[7] = r0[7];
  }
  else {
	r[0] = -r0[0];
	r[1] = -r0[1];
	r[2] = -r0[2];
	r[3] = -r0[3];
	r[4] = -r0[4];
	r[5] = -r0[5];
	r[6] = -r0[6];
	r[7] = -r0[7];
  }

  for (int i=0; i<LEN_LTS_8; ++i) {
    #pragma HLS UNROLL
//    h_sc_rx[i] = r[i] >> LOG2_LEN_LTS_8;
    h_sc_rx[i] = r[i] >> 3;

  }
}

