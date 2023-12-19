#include <ap_fixed.h>
#include <hls_math.h>
#include <hls_stream.h>
#include <complex>

#define PIPELINE_OPT
#define LEN_LTS_8 8

static const int W = 16; // default bit width
static const int I = 1; // default int part width

void sigVecSPmat(const bool is_lts_1,
                 const std::complex<ap_fixed<W+2,I+2,AP_TRN,AP_SAT>> hpre_sc_rx[LEN_LTS_8],
                 std::complex<ap_fixed<W,I,AP_TRN,AP_SAT>> h_sc_rx[LEN_LTS_8]);

void get_h_per_sc_per_rx(const bool is_lts_1,
                         const std::complex<ap_fixed<W+2,I+2,AP_TRN,AP_SAT> > hpre_sc_rx[LEN_LTS_8],
                         std::complex<ap_fixed<W,I,AP_TRN,AP_SAT> >           h_sc_rx   [LEN_LTS_8]);

template <class T>
static inline std::complex<T> operator << (const std::complex<T>& cmp, const unsigned shift) {
  std::complex<T> c, r;
  c = cmp;
  r.real(c.real() << shift);
  r.imag(c.imag() << shift);
  return r;
}

template <class T>
static inline std::complex<T> operator >> (const std::complex<T>& cmp, const unsigned shift) {
  std::complex<T> c, r;
  c = cmp;
  r.real(c.real() >> shift);
  r.imag(c.imag() >> shift);
  return r;
}
