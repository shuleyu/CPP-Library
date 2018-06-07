#ifndef ASU_WATERLEVELDECON
#define ASU_WATERLEVELDECON
// Need sci-libs/fftw

#include<iostream>
#include<vector>
#include<cmath>
#include<complex>

extern "C"{
#include<fftw3.h>
}

/*********************************************************************
 * This C++ template function returns a 1D array that contains the
 * "peak-kept" position of water-level deconvolution results:
 * decon y (a 1D array) from x (a 1D array). Both x and y should be
 * properly tapered around the edges and with DC removed.
 *
 * x and y will be padded (at both ends) with zeros to be the same length
 * [2*max(y.size(),x.size())] to prevent wrap around artifacts.
 * Therefore signals need to be removed trend and tappered to avoid effects
 * from the edges due to the padding.
 *
 * input(s):
 * const vector<T1> &x      ----  1-D array x (signal).
 * const size_t     &px     ----  Peak positions on the signal.
 * const vector<T2> &y      ----  1-D array y (source).
 * const size_t     &py     ----  Peak position on the source.
 * const double     &delta  ----  Signals and source sampling rate.
 * const double     &wl     ----  Water level. (percentage of the peak spectrum value)
 *
 * return(s):
 * vector<double>   &ans  ----  Deconed trace (length=2*max(x.size(),y.size())).
 *
 * Note: Amplitudes of the deconed traces is not normalized (probably will do some post-process with normalization later).
 *       The peak position is tricky: the results should have original peaks (px) near their center.
 *
 * Shule Yu
 * Jan 30 2018
 *
 * Dependence: fftw-3.
 *
 * Key words : deconvolution, water-level
*********************************************************************/

template<class T1, class T2>
std::vector<double> WaterLevelDecon(std::vector<T1> &x, const std::size_t &px,
                                    std::vector<T2> &y, const std::size_t &py,
                                    const double &delta, const double &wl) {

    if (x.empty()) return {};

    int n=x.size(),N=y.size();

    if (n<=1 || N<=1){
        std::cerr <<  "Error in " << __func__ << ": input signals too small ..." << std::endl;
        return {};
    }

    // Set up fft length.
    int NPTS=2*std::max(n,N);

    // Malloc space for FFT.
    double *In = new double [NPTS];
    fftw_complex *Out=(fftw_complex *)fftw_malloc((NPTS/2+1)*sizeof(fftw_complex));;

    // Define fft and ifft transform plan.
    fftw_plan p1=fftw_plan_dft_r2c_1d(NPTS,In,Out,FFTW_MEASURE);
    fftw_plan p2=fftw_plan_dft_c2r_1d(NPTS,Out,In,FFTW_MEASURE);


    // Step1. Calculate source fft.

    // 1. Padding source (at two ends) with zeros, the peak is at center.
    for (int i=0;i<NPTS;++i) In[i]=0;
    for (int i=0;i<N;++i) In[NPTS/2-py+i]=y[i];

    // 2. Run esf fft.
    fftw_execute(p1);

    // Step2. Water-level filled.

    // 1. Find waterlevel.
    double Amp=0;
    for (int i=0;i<NPTS/2+1;++i)
        Amp=std::max(Amp,sqrt(Out[i][0]*Out[i][0]+Out[i][1]*Out[i][1]));
    double WL=wl*Amp;

    // 2. Fill with water.
    double df=1.0/delta/NPTS;
    std::vector<std::complex<double>> Filled(NPTS/2+1,0);
    for (int i=0;i<NPTS/2+1;++i){
        double amp=sqrt(Out[i][0]*Out[i][0]+Out[i][1]*Out[i][1]);
        if (amp<WL) {
            if (amp>0) Filled[i]={Out[i][0]*(WL/amp),Out[i][1]*(WL/amp)};
            else {
                std::cerr <<  "Warning in " << __func__ << ": amplitude = 0 @ " << df*i << " Hz ..." << std::endl;
                Filled[i]={WL,0};
            }
        }
        else Filled[i]={Out[i][0],Out[i][1]};
    }

    // Step3. Decon.

    // 1. Padding signal with zeros.
    for (int i=0;i<NPTS;++i) In[i]=0;
    for (int i=0;i<n;++i) In[NPTS/2-px+i]=x[i];

    // 2. FFT signal.
    fftw_execute(p1);

    // 3. Division.
    for (int i=0;i<NPTS/2+1;++i){
        std::complex<double> A={Out[i][0],Out[i][1]};
        auto res=A/Filled[i];
        Out[i][0]=res.real();
        Out[i][1]=res.imag();
    }

    // 4. iFFT deconed traces.
    fftw_execute(p2);

    // 5. Rotate the deconed trace so that the peak stays around the center.
    std::vector<double> ans(NPTS,0);
    for (int i=0;i<NPTS;++i) ans[i]=In[(i+NPTS/2)%NPTS];

    // Free spaces.
    fftw_destroy_plan(p1);
    fftw_destroy_plan(p2);
    fftw_free(Out);
    delete [] In;

    return ans;
}

#endif
