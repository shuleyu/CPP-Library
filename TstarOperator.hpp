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
 * This C++ template returns impulse response of futterman
 * T-Star operator.
 *
 * input(s):
 * const T1  &ts     ----  tstar parameter.
 * const T2  &delta  ----  Sampling rate (in sec.).
 * const T3  &tol    ----  (optional) tolerance. After the
 *                         t* operator is normalized to peak=1,
 *                         upper bound of the amplitude of the first point.
 *                         default value is 1e-3.
 *                         This value will affect the signal length (minimum is 10 seconds).
 *                         The smaller the tolerance, the longer the t* operator.
 *
 * return(s):
 * vector<double> ans  ----  t* impulse response (t* operator).
 *
 * Shule Yu
 * Jan 20 2018
 *
 * Key words: Tstar, futterman, ifft.
 * Note: Modified from Grand's code. I got the code from ed.
 *       The code is modified here such that if ts<=0,
 *       returns a delta function at t= ~1 sec.
*********************************************************/

template<class T1, class T2, class T3=double>
std::pair<std::vector<double>,std::size_t> TstarOperator(const T1 &ts, const T2 &delta, const T3 &tol=1e-3){

    if (ts<=0){
        std::vector<double> ans(100,0);
        ans[50]=1;
        return {ans,50};
    }

    double len=10;

    while (1){


        int n=len/delta,N=n+(n%2);
        std::vector<double> ans(n);

        double df=1.0/delta/n;

        double *In = new double [N];
        fftw_complex *Out=(fftw_complex *)fftw_malloc((N/2+1)*sizeof(fftw_complex));;

        // Define ifft transform plan.
        fftw_plan p=fftw_plan_dft_c2r_1d(N,Out,In,FFTW_ESTIMATE);

        double c,f;

        // Put attenuation scheme into the ifft plan.
        Out[0][0]=1.0;
        Out[0][1]=0.0;
        for (int i=1;i<N/2+1;++i){
            f=df*i;
            c=2*f*ts*(log(f));
            Out[i][0]=exp(-M_PI*f*ts)*cos(c);
            Out[i][1]=exp(-M_PI*f*ts)*sin(c);
        }

        // Run ifft.
        fftw_execute(p);

        // Output.
        for (int i=0;i<n;++i) ans[i]=exp(M_PI*ts)*In[i];

        fftw_free(Out);
        fftw_destroy_plan(p);
        delete [] In;

        // Stop judge.
        auto MaxVal=max_element(ans.begin(),ans.end());
        auto MinVal=min_element(ans.begin(),ans.end());

        if ((*MinVal)/(*MaxVal)<tol){
            // Shift the minimum value to the begining of the signal.
            std::rotate(ans.begin(),MinVal,ans.end());
            Normalize(ans);
            return {ans,std::distance(ans.begin(),max_element(ans.begin(),ans.end()))};
        }
        len*=2;
    }
}

#endif
