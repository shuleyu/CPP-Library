#ifndef ASU_IFFT
#define ASU_IFFT
// Need sci-libs/fftw

#include<vector>
#include<cmath>

extern "C"{
#include<fftw3.h>
}

/*********************************************************************
 * This C++ template runs ifft on input amplitude and phase vector
 * (same length) which is obtained by *real* signal FFT, then return the
 * original *real* signal.
 *
 * input(s):
 * const vector<T1>  &amp    ----  FFT amplitude array.
 * const vector<T2>  &phase  ----  FFT phase array. (-PI ~ PI)
 * const double      &df     ----  df*(amp.size()-1)=1/2/delta=Nq.
 *                                 delta is the sampling rate of orignal signal.
 *
 * return(s):
 * std::vector<double> ans  ----  original signal amplitude series.
 * for original time series, create grid using dt=1/2/df/(amp.size()-1) and size of ans.
 *
 * Shule Yu
 * Aug 29 2018
 *
 * Dependence: fftw-3.
 *
 * Key words : fast fourier transform, ifft.
*********************************************************************/

template<typename T1, typename T2>
std::vector<double>
IFFT(const std::vector<T1> &amp, const std::vector<T2> &phase, const double &df){

    if (amp.empty()) return {};

    int n=amp.size(),N=2*(n-1);

    double *In = new double [N];
    fftw_complex *Out=(fftw_complex *)fftw_malloc(n*sizeof(fftw_complex));;

    // Define ifft transform plan.
    fftw_plan p=fftw_plan_dft_c2r_1d(N,Out,In,FFTW_MEASURE);

    // Push data into the plan.
    for (int i=0;i<n;++i) {
        Out[i][0]=amp[i]*cos(phase[i]);
        Out[i][1]=amp[i]*sin(phase[i]);
    }

    // Run ifft.
    fftw_execute(p);
    std::vector<double> ans;
    for (int i=0;i<N;++i) ans.push_back(In[i]);

    // free resources.
    fftw_destroy_plan(p);
    fftw_free(Out);
    delete [] In;

    return ans;
}

#endif
