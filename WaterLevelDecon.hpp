#ifndef ASU_WATERLEVELDECON
#define ASU_WATERLEVELDECON

#include<iostream>
#include<vector>
#include<cmath>

#include<fftw3.h>

#include<ComplexDivide.hpp>
#include<Convolve.hpp>

/*********************************************************************
 * This C++ template function returns a 2D array that contains the
 * "peak-kept" position of water-level deconvolution results:
 * decon y (a 1D array) from x (a 2D array). Both x and y should be
 * properly tapered around the edges.
 *
 * x and y will be padded (at both ends) with zeros to be the same length
 * [2*max(y.size(),x[0].size())] to prevent wrap around artifacts.
 *
 * const vector<vector<T1>> &x      ----  2-D array x (signals).
 * const vector<T2>         &px     ----  Peak positions on signals.
 * const vector<T3>         &y      ----  1-D array y (source).
 * const T4                 &py     ----  Peak position on the source.
 * const T5                 &delta  ----  Signals and source sampling rate.
 * const T6                 &wl     ----  Water level. (percentage of the peak spectrum value)
 *
 * ?
 * vector<double>           &filled_amp      ----  (Optional) The spectrum of zero-padded source.
 * vector<double>           &filled_phase    ----  (Optional) The phase of zero-padded source.
 * vector<vector<double>>   &original_amp    ----  (Optional) The spectrum of zero-padded x.
 * vector<vector<double>>   &original_phase  ----  (Optional) The phase of zero-padded x.
 * vector<vector<double>>   &divided_amp     ----  (Optional) The spectrum of deconed x.
 * vector<vector<double>>   &divided_phase   ----  (Optional) The phase of deconed x.
 * const bool               &Dump            ----  (Optional) Wheter output the above 6 details. Default is false.
 *
 * return(s):
 * vector<vector<double>>   &ans  ----  Deconed result, 2-D.
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

template<class T1, class T2, class T3, class T4, class T5, class T6>
std::vector<std::vector<double>> WaterLevelDecon(const std::vector<std::vector<T1>> &x, const std::vector<T2> &px,
                                                 const std::vector<T3> &y, const T4 &py, const T5 &delta, const T6 &wl){

    if (x.empty()) return {};

    int m=x.size(),n=x[0].size(),N=y.size();

    if (n<=1 || N<=1){
        std::cerr <<  __func__ << "; Error: input array too small ..." << std::endl;
        return {};
    }

    // Set up fft length.
    int NPTS=2*std::max(n,N);

    // Malloc space for FFT.

    double *In = new double [NPTS];
    fftw_complex *Out=(fftw_complex *)fftw_malloc((NPTS/2+1)*sizeof(fftw_complex));;

    // Define fft transform plan.
    fftw_plan p1=fftw_plan_dft_r2c_1d(NPTS,In,Out,FFTW_MEASURE);
    fftw_plan p2=fftw_plan_dft_c2r_1d(NPTS,Out,In,FFTW_MEASURE);


    // Calculate source fft.

    // 1. Padding source (at two ends) with zeros, the peak is at center.
    for (int i=0;i<NPTS;++i) In[i]=0;
    for (int i=0;i<N;++i) In[NPTS/2-py+i]=y[i];

    // 2. Run esf fft.
    fftw_execute(p1);

    // 3. Find max amplitude and waterlevel.
    double Amp=0;
    for (int i=0;i<NPTS/2+1;++i)
        Amp=std::max(Amp,sqrt(Out[i][0]*Out[i][0]+Out[i][1]*Out[i][1]));
    double WL=wl*Amp;

    // 4. Fill with water.
    double df=1.0/delta/NPTS;
    std::vector<double> filled_real(NPTS/2+1,0),filled_imag(NPTS/2+1,0);
    for (int i=0;i<NPTS/2+1;++i){
        double amp=sqrt(Out[i][0]*Out[i][0]+Out[i][1]*Out[i][1]);
        if (amp<WL) {
            if (amp>0) {
                filled_real[i]=Out[i][0]*(WL/amp);
                filled_imag[i]=Out[i][1]*(WL/amp);
            }
            else {
                std::cerr <<  __func__ << "; Warning : amplitude = 0 @ " << df*i << " Hz ..." << std::endl;
                filled_real[i]=WL;
                filled_imag[i]=0;
            }
        }
        else {
            filled_real[i]=Out[i][0];
            filled_imag[i]=Out[i][1];
        }
    }


    // Decon every traces.

    std::vector<std::vector<double>> ans;
    std::vector<double> Tmp(NPTS,0);
    for (int i=0;i<m;++i){

        // 1. Padding signal with zeros.
        for (int j=0;j<NPTS;++j) In[j]=0;
        for (int j=0;j<n;++j) In[NPTS/2-px[i]+j]=x[i][j];

        // 2. FFT each trace.
        fftw_execute(p1);

        // 3. Division.
        for (int j=0;j<NPTS/2+1;++j){
            auto res=ComplexDivide(Out[j][0],Out[j][1],filled_real[j],filled_imag[j]);
            Out[j][0]=res.first;
            Out[j][1]=res.second;
        }

        // 4. iFFT deconed traces.
        fftw_execute(p2);

        // 5. Shift the deconed trace to the peak of signal.
        for (int j=0;j<NPTS;++j) Tmp[j]=In[(j+NPTS/2)%NPTS];
        ans.push_back(Tmp);
    }

    fftw_destroy_plan(p1);
    fftw_destroy_plan(p2);
    fftw_free(Out);
    delete [] In;

    return ans;
}

#endif
