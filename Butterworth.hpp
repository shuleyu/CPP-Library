#ifndef ASU_BUTTERWORTH
#define ASU_BUTTERWORTH
// Need SAC

#include<iostream>
#include<vector>

extern "C"{
#include<sac.h>
#include<sacio.h>
}

/*****************************************************
 * This C++ template apply butterworth filter on data.
 * The filter is provided by SAC library.
 * This is only a wrapper.
 *
 * input(s):
 * vector<T>   &p        ----  A signal array (2D).
 * const double &delta   ----  Data sampling (in sec.)
 * const double &f1      ----  Filter left corner.
 * const double &f2      ----  Filter right corner.
 * const int    &order   ----  (optional, default=2) Number of poles.
 * const int    &passes  ----  (optional, default=2) Number of passes (forward->backward->forward ...)
 *
 * return(s):
 * vector<T> &p (in-place)
 *
 * Note:
 *       if f1>f2 return error.
 *       if f1<=0, will do a low pass filter of corner f2.
 *       if f2>=1.0/delta/2, will do a high pass filter of corner f1.
 *       if f1 and f2 are both outside of their range, do nothing.
 *
 * Shule Yu
 * Dec 25 2017
 *
 * Key words: sac, filter, butterworth.
*****************************************************/

template<class T>
void Butterworth(std::vector<T> &p, const double &delta, const double &f1, const double &f2,
                 const int &order=2, const int passes=2){

    // Check p size.
    if (p.empty()) return;

    // check corner frequencies.
    double nf=1.0/2/delta;
    if (f1>=f2 || f1>=nf || f2<=0) {
        std::cerr <<  "Error in " << __func__ << ": corner frequency range is wrong ..." << std::endl;
        return;
    }

    if (f1<=0 && f2>=nf) return;

    int N=p.size();

    // Call SAC function.
    float *rawdata= new float [N];

    // In sac.h, we have:
    char *BU = (char *)"BU";
    char *BP = (char *)"BP";
    char *HP = (char *)"HP";
    char *LP = (char *)"LP";

    for (int Cnt=0;Cnt<N;++Cnt) rawdata[Cnt]=p[Cnt];

    if (f1<=0)
        xapiir(rawdata,N,BU,0.0,0.0,order,LP,0.0,f2,delta,passes);
    else if (f2>=nf)
        xapiir(rawdata,N,BU,0.0,0.0,order,HP,f1,0.0,delta,passes);
    else
        xapiir(rawdata,N,BU,0.0,0.0,order,BP,f1,f2,delta,passes);

    for (int Cnt=0;Cnt<N;++Cnt) p[Cnt]=rawdata[Cnt];

    delete [] rawdata;

    return ;
}

#endif
