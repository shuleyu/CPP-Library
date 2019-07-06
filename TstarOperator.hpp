#ifndef ASU_TSTAROPERATOR
#define ASU_TSTAROPERATOR
// Need sci-libs/fftw

#include<iostream>
#include<vector>
#include<cmath>
#include<algorithm>

extern "C"{
#include<fftw3.h>
}

#include<Normalize.hpp>

/*********************************************************
 * This C++ function returns impulse response of futterman
 * T-Star operator. Will keep increasing the signal length
 * until the minimum amplitude / peak amplitude is less than tol.
 *
 * Will normalize the output such that peak amplitude = 1.
 *
 * input(s):
 * const double &ts     ----  tstar parameter.
 * const double &delta  ----  Sampling rate (in sec.).
 * const double &tol    ----  (optional, default is 1e-3) tolerance.
 *                            This value will affect the signal length. The minimum length is 10 seconds.
 *                            The smaller the tolerance, the longer the t* operator.
 *
 * return(s):
 * pair<vector<double>,std::size_t> ans;
 *        ans.first     ----  t* response (ampitude array).
 *        ans.second    ----  t* peak position.
 *
 * Shule Yu
 * Jan 20 2018
 *
 * Key words: Tstar, futterman, ifft.
 * Note: Modified from Grand's code. I got the code from Ed.
 *       The code is modified here such that if ts<=0,
 *       returns a delta function.
*********************************************************/

std::pair<std::vector<double>,std::size_t>
TstarOperator(const double &ts, const double &delta, const double &tol=1e-3){

    // If ts<=0, return a delta function.
    if (ts<=0){
        std::vector<double> ans(100,0);
        ans[50]=1;
        return {ans,50};
    }

    // Minimum signal length.
    double len=10;

    while (1) {

        // npts of the output signal.
        int n=len/delta,N=n+(n%2);
        std::vector<double> ans(n);

        // frequency interval on spectrum.
        // Nyquist Frequency is 1/2/delta; npts after fft is (N/2+1).
        // df is 1/2/delta / (N/2+1)
        double df=0.5/delta/(N/2+1);

        // Define ifft transform plan.
        // Notice length(IN) is N, while length(Out) is N/2+1, this is a convention
        // used in package fftw3: because the spectrum of a real signal has the
        // mirror property:  H(−f)=[H(f)]*, and fftw3 choose to only store the f>0 part.
        double *In = new double [N];
        fftw_complex *Out=(fftw_complex *)fftw_malloc((N/2+1)*sizeof(fftw_complex));;
        fftw_plan p=fftw_plan_dft_c2r_1d(N,Out,In,FFTW_ESTIMATE);


        // Create the spectrum of the t-star signal (only for the f>0 part).
        Out[0][0]=1.0;
        Out[0][1]=0.0;
        for (int i=1;i<N/2+1;++i){
            double f=df*i;
            double c=2*f*ts*(log(f));

            // Out[i][0] is the real part of spectrum at frequency f.
            // Out[i][1] is the imag part of spectrum at frequency f.
            Out[i][0]=exp(-M_PI*f*ts)*cos(c);
            Out[i][1]=exp(-M_PI*f*ts)*sin(c);
        }

        // Run ifft.
        fftw_execute(p);

        // Get answer from the ifft plan.
        for (int i=0;i<n;++i) ans[i]=In[i];


        // Destroy the ifft plan.
        fftw_free(Out);
        fftw_destroy_plan(p);
        delete [] In;


        // Should we stop?
        auto MaxElement=max_element(ans.begin(),ans.end());
        auto MinElement=min_element(ans.begin(),ans.end());

        if ((*MinElement)/(*MaxElement)<tol){
            // Shift the minimum value to the begining of the signal.
            std::rotate(ans.begin(),MinElement,ans.end());
            Normalize(ans); // Before nomalize, the integral (area under the curve) is 1.
            // After normalize, the peak value is 1.
            return {ans,std::distance(ans.begin(),max_element(ans.begin(),ans.end()))};
        }
        len*=2;
    }
}

#endif
