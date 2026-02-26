#include <complex>
#include <cstdint>

/*
Compute the DFT of a complex-valued data array with
n_data elements.

@param data Data array
@param n_data Number of data elements
@param reverse_bits Whether the true DFT should be computed, or whether
it is sufficient to compute the DFT without swapping data in output indices.
It is not necessary to reverse the bits if the DFT is being used to implement
a convolution algorithm.
*/
bool fft(
    std::complex<float>* const data,
    const uint64_t n_data,
    const bool reverse_bits=true
);

/*
Compute the inverse DFT of a complex-valued data array with
n_data elements.

@param data Data array
@param n_data Number of data elements
@param reverse_bits Whether the true DFT should be computed, or whether
it is sufficient to compute the DFT without swapping data in output indices.
It is not necessary to reverse the bits if the DFT is being used to implement
a convolution algorithm.
*/
bool ifft(
    std::complex<float>* const data,
    const uint64_t n_data,
    const bool reverse_bits=true
);