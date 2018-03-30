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
 * vector<vector<T1>> &p      ----  A signal array (2D).
 * const T2           &delta  ----  Data sampling (in sec.)
 * const T3           &f1     ----  Filter left corner.
 * const T4           &f2     ----  Filter right corner.
 *
 * return(s):
 * vector<vector<T1>> &p (in-place)
 *
 * Note: use order=2, passes=2 (forward and backward)
 * 		 if f1>f2 return error.
 *       if f1<=0, will do a low pass filter of corner f2.
 *       if f2>=1.0/delta/2, will do a high pass filter of corner f1.
 *       if f1 and f2 are both outside of their range, do nothing.
 *
 * Shule Yu
 * Dec 25 2017
 *
 * Key words: sac, filter, butterworth.
*****************************************************/

template<class T1, class T2, class T3, class T4>
void Butterworth(std::vector<std::vector<T1>> &p, const T2 &delta, const T3 &f1, const T4 &f2){

    double nf=1.0/2/delta;
    int order=2,passes=2;

    // check corner frequencies.
    if (f1>=f2 || f1>=nf || f2<=0) {
        std::cerr <<  "Error in " << __func__ << ": corner frequency range is wrong ..." << std::endl;
        return;
    }

    if (f1<=0 && f2>=nf) return;


    // Check p size.
    if (p.empty()) return;

    size_t N=p[0].size();
    for (auto &item:p)
        if (item.size()!=N) {
            std::cerr <<  "Error in " << __func__ << ": input 2D array size error ..." << std::endl;
            return;
        }

    // Call SAC function.
    float *rawdata= new float [N];

    // In sac.h, we have:
    char *BU = (char *)"BU";
    char *BP = (char *)"BP";
    char *HP = (char *)"HP";
    char *LP = (char *)"LP";

    for (size_t Cnt=0;Cnt<p.size();++Cnt){

        for (size_t Cnt2=0;Cnt2<N;++Cnt2) rawdata[Cnt2]=p[Cnt][Cnt2];

        if (f1<=0)
            xapiir(rawdata,(int)N,BU,0.0,0.0,order,LP,0.0,f2,delta,passes);
        else if (f2>=nf)
            xapiir(rawdata,(int)N,BU,0.0,0.0,order,HP,f1,0.0,delta,passes);
        else
            xapiir(rawdata,(int)N,BU,0.0,0.0,order,BP,f1,f2,delta,passes);

        for (size_t Cnt2=0;Cnt2<N;++Cnt2) p[Cnt][Cnt2]=rawdata[Cnt2];
    }

    delete [] rawdata;

    return ;
}

#endif
