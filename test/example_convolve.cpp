#include <convolve.hpp>
#include <complex>
#include <cstdint>
#include <vector>
#include <random>
#include <limits>
#include <iostream>
#include <thread>
#include <chrono>

// #define PRINT_DEBUG

int main() {
    uint64_t n_data = 100000;

    std::mt19937 gen(0);
    std::uniform_int_distribution<uint32_t> rand_u32(0, std::numeric_limits<uint32_t>::max());
    
    std::vector<std::complex<float>> z1(n_data);
    for (uint64_t i = 0; i < n_data; ++i) {
        float re = static_cast<float>(rand_u32(gen)) / std::numeric_limits<uint32_t>::max();
        float im = static_cast<float>(rand_u32(gen)) / std::numeric_limits<uint32_t>::max();
        z1[i] = std::complex<float>(re, im);
    }

    std::vector<std::complex<float>> z2(n_data);
    for (uint64_t i = 0; i < n_data; ++i) {
        float re = static_cast<float>(rand_u32(gen)) / std::numeric_limits<uint32_t>::max();
        float im = static_cast<float>(rand_u32(gen)) / std::numeric_limits<uint32_t>::max();
        z2[i] = std::complex<float>(re, im);
    }

#ifdef PRINT_DEBUG
    std::cout << "z1:\n";
    for (uint64_t i = 0; i < n_data; ++i) {
        std::cout << z1[i].real() << ", " << z1[i].imag() << ", ";
    }
    std::cout << std::endl;
#endif

#ifdef PRINT_DEBUG
    std::cout << "z2:\n";
    for (uint64_t i = 0; i < n_data; ++i) {
        std::cout << z2[i].real() << ", " << z2[i].imag() << ", ";
    }
    std::cout << std::endl;
#endif

    Convolver convolver;

#ifdef FFTW3_H
    fftwf_init_threads();
    fftwf_plan_with_nthreads(std::thread::hardware_concurrency());
    convolver.plan_convolution(z1.size(), z2.size());
    std::cout << "Made plan." << std::endl;
#endif

    std::chrono::steady_clock::time_point tp1 = std::chrono::steady_clock::now();
    std::vector<std::complex<float>> conv_out = convolver.convolve(z1.data(), z2.data(), z1.size(), z2.size());
    std::chrono::steady_clock::time_point tp2 = std::chrono::steady_clock::now();

#ifdef PRINT_DEBUG
    std::cout << "conv_out:\n";
    for (uint64_t i = 0; i < conv_out.size(); ++i) {
        std::cout << conv_out[i].real() << ", " << conv_out[i].imag() << ", ";
    }
    std::cout << std::endl;
#endif


    std::cout << "Computed convolution in " << (double)(tp2 - tp1).count() / 1e9 << " seconds" << std::endl;
}