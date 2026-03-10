#include <complex>
#include <cstdint>

/*
Compute the DFT of a complex-valued data array with
n_data elements.

@tparam reverse_bits Whether the true DFT should be computed, or whether
it is sufficient to compute the DFT without swapping data in output indices.
It is not necessary to reverse the bits if the DFT is being used to implement
a convolution algorithm.
@param data Data array
@param n_data Number of data elements
*/
template<bool reverse_bits=true>
bool fft(
    std::complex<float>* const data,
    const uint64_t n_data
);

/*
Compute the inverse DFT of a complex-valued data array with
n_data elements.

@tparam reverse_bits Whether the true DFT should be computed, or whether
it is sufficient to compute the DFT without swapping data in output indices.
It is not necessary to reverse the bits if the DFT is being used to implement
a convolution algorithm.
@param data Data array
@param n_data Number of data elements
*/
template<bool reverse_bits=true>
bool ifft(
    std::complex<float>* const data,
    const uint64_t n_data
);

/*
Return log2(x) as an integer. Note that this incorrectly computes
log2(0) = 0.
*/
uint8_t ui_log2_ui64(const uint64_t x);

/*
Return the reverse of the n_bits least significant bits in an integer x.
For example, if:
    x = 100101001111 and:
        n_bits = 5  ==> return 11110
        n_bits = 10 ==> return 1111001010
        n_bits = 20 ==> return 11110010100100000000
*/
uint64_t get_reversed_bits(const uint64_t x, const uint8_t n_bits);