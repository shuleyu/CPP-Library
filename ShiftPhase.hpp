#ifndef ASU_SHIFTPHASE
#define ASU_SHIFTPHASE

#include<vector>
#include<cmath>

#include<fftw3.h>

/*********************************************************************
 * This C++ template runs fft on input real signal, then shift each
 * frquency with a constant phase. Then ifft back to time domain and
 * output the result.
 *
 * input(s):
 * const vector<T1> &x      ----  1-D input signal.
 * const T2         &shift  ----  Constant phase shift value. (in deg.)
 *
 * return(s):
 * vector<double> y         ----  Phase-shifted signal. (same length as x)
 *
 * Shule Yu
 * Jan 19 2018
 *
 * Dependence: sci-libs/fftw.
 *
 * Key words : fast fourier transform. phase shift.
*********************************************************************/

template<class T1, class T2>
std::vector<double> ShiftPhase(const std::vector<T1> &x,const T2 &shift){

    if (x.empty()) return {};

    int n=x.size(),N=n+(n%2);

    double *In = new double [N];
    fftw_complex *Out=(fftw_complex *)fftw_malloc((N/2+1)*sizeof(fftw_complex));;

    // Define fft transform plan.
    fftw_plan p1=fftw_plan_dft_r2c_1d(N,In,Out,FFTW_ESTIMATE);
    fftw_plan p2=fftw_plan_dft_c2r_1d(N,Out,In,FFTW_ESTIMATE);

    // Push data into the plan.
    // Pad the signal with one zero if the length of original signal is odd.
    for (int i=0;i<n;++i) In[i]=x[i];
    if (n%2==1) In[N-1]=0;

    // Run fft.
    fftw_execute(p1);

    // Shift phase.
    double amp,phase;
    for (int i=0;i<N/2+1;++i){
        amp=sqrt(pow(Out[i][0],2)+pow(Out[i][1],2));
        phase=atan2(Out[i][1],Out[i][0]);
        Out[i][0]=amp*cos(phase+shift*M_PI/180.0);
        Out[i][1]=amp*sin(phase+shift*M_PI/180.0);
    }

    // Run ifft.
    fftw_execute(p2);

    // Get result. (ignore the last point if data length is odd)
    std::vector<double> ans;
    for (int i=0;i<n;++i) ans.push_back(1.0*In[i]/n);

    fftw_destroy_plan(p1);
    fftw_destroy_plan(p2);
    fftw_free(Out);
    delete [] In;

    return ans;
}

#endif
