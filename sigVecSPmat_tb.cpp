#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <iostream>

#include "sigVecSPmat.h"

int main(void)
{
	bool is_lts_1 = 1;
	std::complex<ap_fixed<W+2,I+2,AP_TRN,AP_SAT>> hpre_sc_rx[LEN_LTS_8] =
		{{-0.0428, 0.3179}, {-0.2847, 0.4941}, {0.9223, 0.0908}, {0.6368, 0.2702}, {-0.1652, 0.1066}, {-0.4568,0.0258}, {0.2257, 0.2089}, {0.4727, 0.3902}};
	std::complex<ap_fixed<W,I,AP_TRN,AP_SAT>> h_sc_rx[LEN_LTS_8];

	std::complex<ap_fixed<W+2,I+2,AP_TRN,AP_SAT>> h_sc_rx_soft[LEN_LTS_8];
	std::complex<ap_fixed<W,I,AP_TRN,AP_SAT>> h_sc_rx_ref[LEN_LTS_8];

	char SP_Mat[LEN_LTS_8][LEN_LTS_8] = {
		 { 1, -1,  1,  1, -1,  1, -1, -1},
		 { 1,  1, -1,  1, -1, -1,  1, -1},
		 { 1,  1,  1, -1, -1, -1, -1,  1},
		 {-1,  1,  1,  1,  1, -1, -1, -1},
		 { 1, -1,  1,  1,  1, -1,  1,  1},
		 { 1,  1, -1,  1,  1,  1, -1,  1},
		 { 1,  1,  1, -1,  1,  1,  1, -1},
		 {-1,  1,  1,  1, -1,  1,  1,  1}
	};

	for (int i=0; i<LEN_LTS_8; i++) {
		h_sc_rx_soft[i] = 0;
	}
	for (int i=0; i<LEN_LTS_8; i++) {
		for (int j=0; j<LEN_LTS_8; j++) {
			if (SP_Mat[i][j] == 1) {
				h_sc_rx_soft[i] += hpre_sc_rx[j];
			} else {
				h_sc_rx_soft[i] -= hpre_sc_rx[j];
			}
		}
	}
	for (int i=0; i<LEN_LTS_8; i++) {
        if (is_lts_1 == 1) {
        	h_sc_rx_soft[i] = h_sc_rx_soft[i] >> 3;
        }
        else {
        	h_sc_rx_soft[i] = (-h_sc_rx_soft[i]) >> 3;
        }
	}

	sigVecSPmat(is_lts_1, hpre_sc_rx, h_sc_rx);
	get_h_per_sc_per_rx(is_lts_1, hpre_sc_rx, h_sc_rx_ref);

	for (int i=0; i<LEN_LTS_8; i++) {
		std::cout << "Real part - hard: " << h_sc_rx[i].real() << '\n';
		std::cout << "Real part - soft: " << h_sc_rx_soft[i].real() << '\n';
		std::cout << "Real part - ref: " << h_sc_rx_ref[i].real() << '\n';
	}
}
