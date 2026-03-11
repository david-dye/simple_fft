#include <fft.hpp>
#include <complex>
#include <cstdint>
#include <vector>
#include <random>
#include <limits>
#include <iostream>
#include <chrono>

int main() {
    uint8_t n_bits = 20;
    uint64_t n_data = UINT64_C(1) << n_bits;
    std::cout << "Number of points: " << n_data << std::endl;

    std::mt19937 gen(0);
    std::uniform_int_distribution<uint32_t> rand_u32(0, std::numeric_limits<uint32_t>::max());
    
    std::vector<std::complex<float>> x(n_data);
    for (uint64_t i = 0; i < n_data; ++i) {
        float re = static_cast<float>(rand_u32(gen)) / std::numeric_limits<uint32_t>::max();
        float im = static_cast<float>(rand_u32(gen)) / std::numeric_limits<uint32_t>::max();
        x[i] = std::complex<float>(re, im);
    }

#ifdef PRINT_DEBUG
    std::cout << "Pre-FFT:\n";
    for (uint64_t i = 0; i < n_data; ++i) {
        std::cout << x[i].real() << ", " << x[i].imag() << ", ";
    }
    std::cout << std::endl;
#endif

    std::chrono::steady_clock::time_point tp1 = std::chrono::steady_clock::now();
    fft<false>(x.data(), n_data);
    std::chrono::steady_clock::time_point tp2 = std::chrono::steady_clock::now();
    std::cout << "Computed FFT in " << (double)(tp2 - tp1).count() / 1e9 << " seconds" << std::endl;

#ifdef PRINT_DEBUG
    std::cout << "Post-FFT:\n";
    for (uint64_t i = 0; i < n_data; ++i) {
        std::cout << x[i].real() << ", " << x[i].imag() << ", ";
    }
    std::cout << std::endl;
#endif

    tp1 = std::chrono::steady_clock::now();
    ifft<false>(x.data(), n_data);
    tp2 = std::chrono::steady_clock::now();
    std::cout << "Computed IFFT in " << (double)(tp2 - tp1).count() / 1e9 << " seconds" << std::endl;

#ifdef PRINT_DEBUG
    std::cout << "Post-IFFT:\n";
    for (uint64_t i = 0; i < n_data; ++i) {
        std::cout << x[i].real() << ", " << x[i].imag() << ", ";
    }
    std::cout << std::endl;
#endif

}