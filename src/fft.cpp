#include <iostream>
#include <cstdint>
#include <complex>
#include <cmath>
#include <array>

#include <constants.hpp>
#include <fft.hpp>

constexpr std::size_t MAX_STAGES = 64;

constexpr std::array<std::complex<float>, MAX_STAGES> make_twiddles(const int factor) {
    std::array<std::complex<float>, MAX_STAGES> table{};

    for (std::size_t i = 1; i <= MAX_STAGES; ++i) {
        float angle = factor * F_PI / (1ULL << (i - 1));
        table[i - 1] = {
            std::cos(angle),
            std::sin(angle)
        };
    }

    return table;
}

static constexpr auto TWIDDLES_POS = make_twiddles(1);
static constexpr auto TWIDDLES_NEG = make_twiddles(-1);

uint8_t ui_log2_ui64(const uint64_t x) {
    uint64_t x_copy = x;
    uint8_t ret = 0;
    while (x_copy >>= 1) {
        ++ret;
    }
    return ret;
}

uint64_t get_reversed_bits(const uint64_t x, const uint8_t n_bits) {
    uint64_t r = 0;
    for (size_t i = 0; i < n_bits; ++i) {
        r <<= UINT64_C(1);
        uint64_t bit = (x >> i) & 1;
        r |= bit;
    }
    return r;
}


template<bool reverse_bits>
bool fft(
    std::complex<float>* const data,
    const uint64_t n_data
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
    
    for (uint8_t i = n_data_log2; i >= 1; --i) {
        uint64_t half_stride = UINT64_C(1) << (i - 1);
        std::complex<float> twiddle = TWIDDLES_NEG[i - 1];
        for (uint64_t j = 0; j < n_data; j += 2 * half_stride) {
            std::complex<float> exp_term = std::complex<float>(1.f, 0.f);
            for (uint64_t k = 0; k < half_stride; ++k) {
                std::complex<float> u = data[j + k];
                std::complex<float> t = data[j + k + half_stride];
                data[j + k] = u + t;
                data[j + k + half_stride] = exp_term * (u - t);
                exp_term *= twiddle;
            }
        }
    }

    if constexpr (reverse_bits) {
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

    return true;
}

template<bool reverse_bits>
bool ifft(
    std::complex<float>* const data,
    const uint64_t n_data
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

    if constexpr (reverse_bits) {
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
        std::complex<float> twiddle = TWIDDLES_POS[i - 1];
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

    const float n_data_inverse = 1.f/n_data;
    for (uint64_t i = 0; i < n_data; ++i) {
        data[i] *= n_data_inverse;
    }

    return true;
}


template bool fft<true>(
    std::complex<float>* const data,
    const uint64_t n_data
);

template bool fft<false>(
    std::complex<float>* const data,
    const uint64_t n_data
);

template bool ifft<true>(
    std::complex<float>* const data,
    const uint64_t n_data
);

template bool ifft<false>(
    std::complex<float>* const data,
    const uint64_t n_data
);