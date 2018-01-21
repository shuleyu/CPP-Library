#ifndef ASU_FFT
#define ASU_FFT

#include<vector>
#include<cmath>

#include<fftw3.h>

/*********************************************************************
 * This C++ template runs fft on input real signals (same length, same
 * sampling rate) and return the amplitudes and phase informations.
 *
 * Note: for amplitude, power normalization is not done. (not divided by NPTS)
 *       for signal length is odd number (npts is odd), a 0 is padded.
 *
 * vector<vector<T1>>     &x      ----  input signal array. (NPTS is signal length)
 * const T2               &delta  ----  Sampling rate (for all signals).
 * vector<double>         &freq   ----  frequency array (for all signals). length = NPTS/2+1
 * vector<vector<double>> &amp    ----  amplitudes for each signal at each frequency (not normalize by NPTS).
 * vector<vector<double>> &phase  ----  phases for each signal at each frequency.
 *
 * Shule Yu
 * Jan 20 2018
 *
 * Dependence: fftw-3.
 *
 * Key words : fast fourier transform, fft.
***********************************************************/

template<class T1, class T2>
void FFT(const std::vector<std::vector<T1>> &x, const T2 &delta, std::vector<double> &freq, std::vector<std::vector<double>> &amp, std::vector<std::vector<double>> &phase){

    freq.clear();
    amp.clear();
    phase.clear();

    if (x.empty() || x[0].empty()) return;

    int n=x[0].size(),N=n+(n%2);

    double *In = new double [N];
    fftw_complex *Out=(fftw_complex *)fftw_malloc((N/2+1)*sizeof(fftw_complex));;

    // Define fft transform plan.
    fftw_plan p=fftw_plan_dft_r2c_1d(N,In,Out,FFTW_ESTIMATE);

    // for each record, fft and create return value.

    for (size_t i=0;i<x.size();++i) {

        // Push data into the plan.
        // Pad the signal with one zero if the length of original signal is odd.
        for (int j=0;j<n;++j) In[j]=x[i][j];
        if (n%2==1) In[N-1]=0;

        // Run fft.
        fftw_execute(p);

        std::vector<double> TmpAmp,TmpPhase;

        // Get result for each frequency.
        for (int j=0;j<N/2+1;++j){
            TmpAmp.push_back(sqrt(pow(Out[j][0],2)+pow(Out[j][1],2)));
            TmpPhase.push_back(atan2(Out[j][1],Out[j][0]));
        }

        amp.push_back(TmpAmp);
        phase.push_back(TmpPhase);
        
    }

    // create frequency array.
    double df=1.0/delta/N;
    for (int i=0;i<N/2+1;++i) freq.push_back(df*i);

    // free resources.
    fftw_destroy_plan(p);
    fftw_free(Out);
    delete [] In;

    return;
}

#endif
