#ifndef ASU_FFT
#define ASU_FFT
// Need sci-libs/fftw

#include<vector>
#include<cmath>

extern "C"{
#include<fftw3.h>
}

/*********************************************************************
 * This C++ template runs fft on input real signals (same length, same
 * sampling rate) and return the amplitudes and phases.
 *
 * Note: For amplitude, power normalization is not included. (not divided by NPTS)
 *       If signal length is odd number (npts is odd), a 0 is padded.
 *
 * input(s):
 * const vector<T>  &x      ----  input signal array. (NPTS is its length)
 * const double     &delta  ----  Sampling rate (for all signals).
 *
 * return(s):
 * pair<std::vector<double>,std::vector<double>> ans. {amp,phase}
 * amp:   amplitudes for each signal at each frequency (not normalize by NPTS).
 * phase: phases for each signal at each frequency.
 * (for frequencies, create grid between 0 and 1/2/delta with size of amp)
 *
 * Shule Yu
 * Jan 20 2018
 *
 * Dependence: fftw-3.
 *
 * Key words : fast fourier transform, fft.
*********************************************************************/

template<class T>
std::pair<std::vector<double>,std::vector<double>> FFT(const std::vector<T> &x, const double &delta){

    if (x.empty()) return {};

    int n=x.size(),N=n+(n%2);

    double *In = new double [N];
    fftw_complex *Out=(fftw_complex *)fftw_malloc((N/2+1)*sizeof(fftw_complex));;

    // Define fft transform plan.
    fftw_plan p=fftw_plan_dft_r2c_1d(N,In,Out,FFTW_ESTIMATE);

    // Push data into the plan.
    // Pad the signal with one zero if the length of original signal is odd.
    for (int i=0;i<n;++i) In[i]=x[i];
    if (n%2==1) In[N-1]=0;

    // Run fft.
    fftw_execute(p);

    std::vector<double> amp,phase;

    // Get result for each frequency.
    for (int i=0;i<N/2+1;++i){
        amp.push_back(sqrt(pow(Out[i][0],2)+pow(Out[i][1],2)));
        phase.push_back(atan2(Out[i][1],Out[i][0]));
    }

    // free resources.
    fftw_destroy_plan(p);
    fftw_free(Out);
    delete [] In;

    return {amp,phase};
}

#endif
