#ifndef ASU_TSTAROPERATION
#define ASU_TSTAROPERATION

#include<iostream>
#include<vector>
#include<cmath>

#include<fftw3.h>

/*********************************************************
 * This C++ template returns impulse response of futterman
 * T-Star operator.
 *
 * input(s):
 * const T1  &ts     ----  tstar parameter.
 * const T2  &delta  ----  Sampling rate (in sec.).
 * const int &n      ----  Signal length.(suggestion: delta * n > 10 sec.)
 *
 * return(s):
 * vector<double> ans  ----  t* impulse response.
 *
 * Shule Yu
 * Jan 20 2018
 *
 * Key words: Tstar, futterman, ifft.
 * Note: Modified from Grand's code. I got the code from ed.
 *       The code is modified here such that if ts<=0,
 *       returns a delta function at t= ~1 sec.
*********************************************************/

template<class T1, class T2>
std::vector<double> TstarOperation(const T1 &ts, const T2 &delta, const int &n){

    if (n<=1){
        std::cerr <<  "Error in " << __func__ << ": n too small ..." << std::endl;
        return {};
    }

    std::vector<double> ans(n,0);

    if (ts<=0){
        ans[(int)(1/delta)]=1;
        return ans;
    }

    int N=n+(n%2);
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
        c=2*f*ts*(log(f)-M_PI*8/ts+M_PI*0.65); // Phase shift, output signal should
                                               // has peak near (8-0.65*ts) sec.
        Out[i][0]=exp(-M_PI*f*ts)*cos(c);
        Out[i][1]=exp(-M_PI*f*ts)*sin(c);
    }

    // Run ifft.
    fftw_execute(p);

    // Output.
    for (int i=0;i<n;++i) ans[i]=exp(M_PI*ts)*In[i];

    // Normalize
    fftw_free(Out);
    fftw_destroy_plan(p);
    delete [] In;

    return ans;
}

#endif
