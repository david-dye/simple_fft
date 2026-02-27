#include <convolve.hpp>
#include <complex>
#include <cstdint>
#include <vector>
#include <random>
#include <limits>
#include <iostream>

int main() {
    uint64_t n_data = 20;

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

    std::cout << "z1:\n";
    for (uint64_t i = 0; i < n_data; ++i) {
        std::cout << z1[i].real() << ", " << z1[i].imag() << ", ";
    }
    std::cout << std::endl;

    std::cout << "z2:\n";
    for (uint64_t i = 0; i < n_data; ++i) {
        std::cout << z2[i].real() << ", " << z2[i].imag() << ", ";
    }
    std::cout << std::endl;

    Convolver convolver;
    std::vector<std::complex<float>> conv_out = convolver.convolve(z1.data(), z2.data(), z1.size(), z2.size());

    std::cout << "conv_out:\n";
    for (uint64_t i = 0; i < conv_out.size(); ++i) {
        std::cout << conv_out[i].real() << ", " << conv_out[i].imag() << ", ";
    }
    std::cout << std::endl;
}