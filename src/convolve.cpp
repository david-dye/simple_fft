#include <iostream>
#include <vector>
#include <cstdint>
#include <cstdlib>
#include <cstring>

#include "convolve.hpp"
#include "fft.hpp"

Convolver::Convolver() {}

Convolver::~Convolver() {}

std::vector<Convolver::Complex_T>& Convolver::convolve(
    const Complex_T* const z1,
    const Complex_T* const z2,
    const uint64_t n_z1,
    const uint64_t n_z2
) {
    if (z1 == nullptr || z2 == nullptr) {
        std::cerr << "Error: convolve: invalid pointer(s) to data buffers" << std::endl;
        conv_out.resize(0);
        return conv_out;
    }
    if (n_z1 == 0 || n_z2 == 0) {
        std::cerr << "Error: convolve: data buffers must contain elements" << std::endl;
        conv_out.resize(0);
        return conv_out;
    }

    uint64_t out_size_min = n_z1 + n_z2 - 1;
    uint64_t out_size_true = out_size_min;
    uint8_t n_bits = ui_log2_ui64(out_size_min);
    if ((out_size_min & (out_size_min - 1)) != 0) {
        ++n_bits;
        out_size_true = UINT64_C(1) << n_bits; // next power of 2 above the out_size_min
    }
    conv_out.resize(out_size_true);  // the full size of the convolution
    conv_in_1.resize(out_size_true);
    conv_in_2.resize(out_size_true);

    memcpy((void*)(conv_in_1.data()), z1, sizeof(Complex_T) * n_z1);
    memcpy((void*)(conv_in_2.data()), z2, sizeof(Complex_T) * n_z2);
    memset((void*)(conv_in_1.data() + n_z1), 0, sizeof(Complex_T) * (conv_in_1.size() - n_z1));
    memset((void*)(conv_in_2.data() + n_z2), 0, sizeof(Complex_T) * (conv_in_2.size() - n_z2));

    if (!fft<false>(conv_in_1.data(), out_size_true)) {
        std::cerr << "Error: convolve: FFT error." << std::endl;
        conv_out.resize(0);
        return conv_out;
    }
    if (!fft<false>(conv_in_2.data(), out_size_true)) {
        std::cerr << "Error: convolve: FFT error." << std::endl;
        conv_out.resize(0);
        return conv_out;
    }
    for (uint64_t i = 0; i < out_size_true; ++i) {
        conv_out[i] = conv_in_1[i] * conv_in_2[i];
    }
    if (!ifft<false>(conv_out.data(), out_size_true)) {
        std::cerr << "Error: convolve: IFFT error." << std::endl;
        conv_out.resize(0);
        return conv_out;
    }

    conv_out.resize(n_z1 + n_z2 - 1); // the useful size of the convolution
    return conv_out;

}