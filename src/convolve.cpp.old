#include <iostream>
#include <vector>
#include <cstdint>
#include <cstdlib>
#include <cstring>

#include "convolve.hpp"
#include "fft.hpp"

Convolver::Convolver() {}

Convolver::~Convolver() {
    if (plan_fwd_1) {
        fftwf_destroy_plan(plan_fwd_1);
    }
    if (plan_fwd_2) {
        fftwf_destroy_plan(plan_fwd_2);
    }
    if (plan_inv) {
        fftwf_destroy_plan(plan_inv);
    }

    if (in1) {
        fftwf_free(in1);
    }
    if (in2) {
        fftwf_free(in2);
    }
    if (freq1) {
        fftwf_free(freq1);
    }
    if (freq2) {
        fftwf_free(freq2);
    }
}

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

    uint64_t out_size = n_z1 + n_z2 - 1;
    plan_convolution(n_z1, n_z2);

    memcpy((void*)in1, z1, sizeof(Complex_T) * n_z1);
    memcpy((void*)in2, z2, sizeof(Complex_T) * n_z2);
    memset((void*)(in1 + n_z1), 0, sizeof(Complex_T) * (planned_size - n_z1));
    memset((void*)(in2 + n_z2), 0, sizeof(Complex_T) * (planned_size - n_z2));

    fftwf_execute(plan_fwd_1);
    fftwf_execute(plan_fwd_2);

    for (uint64_t i = 0; i < planned_size; ++i) {
        float a = freq1[i][0];
        float b = freq1[i][1];
        float c = freq2[i][0];
        float d = freq2[i][1];

        freq1[i][0] = a*c - b*d;
        freq1[i][1] = a*d + b*c;
    }

    fftwf_execute(plan_inv);

    float scale = 1.0f / planned_size;
    conv_out.resize(out_size);

    for (uint64_t i = 0; i < out_size; ++i) {
        conv_out[i] = Complex_T(
            in1[i][0] * scale,
            in1[i][1] * scale
        );
    }

    return conv_out;

}

void Convolver::plan_convolution(uint64_t n_z1, uint64_t n_z2) {
    if (n_z1 == 0 || n_z2 == 0) {
        std::cerr << "Error: plan_convolution: data buffers must contain elements" << std::endl;
        return;
    }

    uint64_t out_size_min = n_z1 + n_z2 - 1;
    uint64_t out_size_true = out_size_min;
    uint8_t n_bits = ui_log2_ui64(out_size_min);
    if ((out_size_min & (out_size_min - 1)) != 0) {
        ++n_bits;
        out_size_true = UINT64_C(1) << n_bits; // next power of 2 above the out_size_min
    }

    if (out_size_true == planned_size) {
        return;
    }

    // Destroy old plans/buffers if they exist
    if (plan_fwd_1) {
        fftwf_destroy_plan(plan_fwd_1);
    }
    if (plan_fwd_2) {
        fftwf_destroy_plan(plan_fwd_2);
    }
    if (plan_inv) {
        fftwf_destroy_plan(plan_inv);
    }

    if (in1) {
        fftwf_free(in1);
    }
    if (in2) {
        fftwf_free(in2);
    }
    if (freq1) {
        fftwf_free(freq1);
    }
    if (freq2) {
        fftwf_free(freq2);
    }

    planned_size = out_size_true;

    in1   = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * planned_size);
    in2   = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * planned_size);
    freq1 = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * planned_size);
    freq2 = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * planned_size);

    plan_fwd_1 = fftwf_plan_dft_1d(
        planned_size, in1, freq1, FFTW_FORWARD, FFTW_ESTIMATE
    );

    plan_fwd_2 = fftwf_plan_dft_1d(
        planned_size, in2, freq2, FFTW_FORWARD, FFTW_ESTIMATE
    );

    plan_inv = fftwf_plan_dft_1d(
        planned_size, freq1, in1, FFTW_BACKWARD, FFTW_ESTIMATE
    );
}