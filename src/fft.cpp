#include <iostream>
#include <cstdint>
#include <complex>
#include <cmath>
#include <constants.hpp>
#include <fft.hpp>

/*
Return log2(x) as an integer. Note that this incorrectly computes
log2(0) = 0.
*/
uint8_t ui_log2_ui64(const uint64_t x) {
    uint64_t x_copy = x;
    uint8_t ret = 0;
    while (x_copy >>= 1) {
        ++ret;
    }
    return ret;
}

/*
Return the reverse of the n_bits least significant bits in an integer x.
For example, if:
    x = 100101001111 and:
        n_bits = 5  ==> return 11110
        n_bits = 10 ==> return 1111001010
        n_bits = 20 ==> return 11110010100100000000
*/
uint64_t get_reversed_bits(const uint64_t x, const uint8_t n_bits) {
    uint64_t r = 0;
    for (size_t i = 0; i < n_bits; ++i) {
        r <<= UINT64_C(1);
        uint64_t bit = (x >> i) & 1;
        r |= bit;
    }
    return r;
}

/*
Implements the FFT and IFFT
*/
bool fft_impl(
    std::complex<float>* const data,
    const uint64_t n_data,
    const bool reverse_bits,
    const bool inverse
) {
    if (data == nullptr) {
        std::cerr << "Error: FFT: Invalid data buffer." << std::endl; 
        return false;
    }
    if (n_data == 0 || (n_data & (n_data - 1)) != 0) {
        std::cerr << "Error: FFT: n_data must be a power of 2." << std::endl;
        return false;
    }

    uint8_t n_data_log2 = ui_log2_ui64(n_data);

    if (reverse_bits) {
        /*
        Needed to correctly implement true DFT, but is not required for
        some algorithms using both the FFT and IFFT, such as convolution.
        Reversing the bits each iteration makes this loop O(N * log(N)).
        */
        for (uint64_t i = 0; i < n_data; ++i) {
            uint64_t r = get_reversed_bits(i, n_data_log2);
            if (i >= r) {
                continue; // don't swap twice
            }
            std::complex<float> tmp = data[i];
            data[i] = data[r];
            data[r] = tmp;
        }
    }
    
    for (uint8_t i = 1; i <= n_data_log2; ++i) {
        uint64_t half_stride = UINT64_C(1) << (i - 1);
        std::complex<float> twiddle = std::exp(std::complex<float>(0.f, -F_PI / half_stride));
        if (inverse) {
            twiddle.imag(-twiddle.imag());
        }
        for (uint64_t j = 0; j < n_data; j += 2 * half_stride) {
            std::complex<float> exp_term = std::complex<float>(1.f, 0.f);
            for (uint64_t k = 0; k < half_stride; ++k) {
                std::complex<float> u = data[j + k];
                std::complex<float> t = exp_term * data[j + k + half_stride];
                data[j + k] = u + t;
                data[j + k + half_stride] = u - t;
                exp_term *= twiddle;
            }
        }
    }

    if (inverse) {
        const float n_data_inverse = 1.f/n_data;
        for (uint64_t i = 0; i < n_data; ++i) {
            data[i] *= n_data_inverse;
        }
    }

    return true;
}

bool fft(
    std::complex<float>* const data,
    const uint64_t n_data,
    const bool reverse_bits
) {
    return fft_impl(data, n_data, reverse_bits, false);
}

bool ifft(
    std::complex<float>* const data,
    const uint64_t n_data,
    const bool reverse_bits
) {
    return fft_impl(data, n_data, reverse_bits, true);
}