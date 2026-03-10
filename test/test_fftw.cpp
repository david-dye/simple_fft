#include <fftw3.h>
#include <iostream>

int main() {
    const int N = 8;

    fftwf_complex* in  = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * N);
    fftwf_complex* out = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * N);

    fftwf_plan p = fftwf_plan_dft_1d(
        N, in, out,
        FFTW_FORWARD,
        FFTW_ESTIMATE
    );

    fftwf_execute(p);

    fftwf_destroy_plan(p);
    fftwf_free(in);
    fftwf_free(out);

    std::cout << "FFTW works.\n";
}