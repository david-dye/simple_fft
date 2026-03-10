#include <cstdint>
#include <vector>
#include <complex>

class Convolver {
public:
    using Complex_T = std::complex<float>;

    Convolver();
    ~Convolver();

    std::vector<Complex_T>& convolve(
        const Complex_T* const z1,
        const Complex_T* const z2,
        const uint64_t n_z1,
        const uint64_t n_z2
    );

private:

    

    std::vector<Complex_T> conv_in_1;
    std::vector<Complex_T> conv_in_2;
    std::vector<Complex_T> conv_out;

};