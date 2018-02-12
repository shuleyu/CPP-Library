#ifndef ASU_INTERPOLATE
#define ASU_INTERPOLATE

#include<iostream>
#include<vector>
#include<algorithm>

/****************************************************************
 * This c++ template is only a wrapper for the Wiginterp function
 * included in same library. I did some modification: now whether
 * x is ascending or descending doesn't matter. The only
 * restriction on x is stric monotonic. The SAC source code is in
 * Wiginterp.fun.cpp.
 *
 * input(s):
 * const vector<T1> &x       ----  Time of original signal.
 * const vector<T2> &y       ----  Amplitude of original signal.
 * const vector<T3> &xx      ----  Time of interpolated signal.
 * const bool       &IsEven  ----  Is input signal constantly sampled? default is true.
 *
 * return(s):
 * vector<double> ans  ----  Interpolated signal.
 * Shule Yu
 * Nov 14 2017
 *
 * Key words: interpolate, wiggins
****************************************************************/

template<class T1, class T2, class T3>
std::vector<double> Interpolate(const std::vector<T1> &x, const std::vector<T2> &y,
                                const std::vector<T3> &xx, const bool &IsEven=true){

    // Check array size.
    if (x.size()!=y.size()) {
        std::cerr <<  __func__ << "; Error: input array size is zero ..." << std::endl;
        return {};
    }

    // Check x is strictly sorted.

    auto cmp=[](const T1 &x, const T1 &y){
        return x<=y;
    };

    if (!std::is_sorted(x.begin(),x.end(),cmp) && !std::is_sorted(x.rbegin(),x.rend(),cmp)) {
        std::cerr <<  __func__ << "; Error: input x is either not sorted or has repeating value ..." << std::endl;
        return {};
    }

    double *X=new double [x.size()];
    double *Y=new double [y.size()];
    double *XX=new double [xx.size()];
    double *YY=new double [xx.size()];
    std::vector<double> ans(xx.size());

    if (x[0]<x.back()) {
        for (size_t i=0;i<x.size();++i) {
            X[i]=x[i];
            Y[i]=y[i];
        }
    }
    else {
        for (size_t i=0;i<x.size();++i) {
            X[i]=x[x.size()-1-i];
            Y[i]=y[x.size()-1-i];
        }
    }


    for (size_t i=0;i<xx.size();++i) XX[i]=xx[i];

    Wiginterp(X,Y,x.size(),XX,YY,xx.size(),IsEven);

    for (size_t i=0;i<xx.size();++i) ans[i]=YY[i];

    delete [] X;
    delete [] Y;
    delete [] XX;
    delete [] YY;

    return ans;
}

#endif
