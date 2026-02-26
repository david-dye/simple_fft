#include <fft.hpp>
#include <complex>
#include <cstdint>
#include <vector>
#include <random>
#include <limits>
#include <iostream>

int main() {
    uint8_t n_bits = 5;
    uint64_t n_data = UINT64_C(1) << n_bits;

    std::mt19937 gen(0);
    std::uniform_int_distribution<uint32_t> rand_u32(0, std::numeric_limits<uint32_t>::max());
    
    std::vector<std::complex<float>> x(n_data);
    for (uint64_t i = 0; i < n_data; ++i) {
        float re = static_cast<float>(rand_u32(gen)) / std::numeric_limits<uint32_t>::max();
        float im = static_cast<float>(rand_u32(gen)) / std::numeric_limits<uint32_t>::max();
        x[i] = std::complex<float>(re, im);
    }

    std::cout << "Pre-FFT:\n";
    for (uint64_t i = 0; i < n_data; ++i) {
        std::cout << x[i].real() << ", " << x[i].imag() << ", ";
    }
    std::cout << std::endl;

    bool should_reverse_bits = true;
    fft(x.data(), n_data, should_reverse_bits);

    std::cout << "Post-FFT:\n";
    for (uint64_t i = 0; i < n_data; ++i) {
        std::cout << x[i].real() << ", " << x[i].imag() << ", ";
    }
    std::cout << std::endl;

    ifft(x.data(), n_data, should_reverse_bits);
    std::cout << "Post-IFFT:\n";
    for (uint64_t i = 0; i < n_data; ++i) {
        std::cout << x[i].real() << ", " << x[i].imag() << ", ";
    }
    std::cout << std::endl;
}