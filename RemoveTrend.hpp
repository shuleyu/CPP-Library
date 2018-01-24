#ifndef ASU_REMOVETREND
#define ASU_REMOVETREND

#include<iostream>
#include<vector>

/***********************************************************
 * This C++ template remove linear trend of input signal.
 *
 * vector<T1> &p       ----  Signal.
 * const T2   &delta   ----  Sampling rate.
 * const T3   &TBegin  ----  Begin time of the signal.
 *
 * Shule Yu
 * Jan 23 2018
 *
 * Key words: remove trend.
 *
 * Reference:
 *     http://en.wikipedia.org/wiki/Simple_linear_regression
***********************************************************/

template<class T1, class T2, class T3>
void RemoveTrend(std::vector<T1> &p,const T2 &delta, const T3 &TBegin){

    if (p.size()<=1) return;

    int n=p.size();
    double slope,intercept,sumx2=0,sumy=0,sumxy=0,t=TBegin,avx=TBegin+(n-1)*delta/2.0;

    for (int i=0;i<n;++i){

        sumx2+=t*t;

        sumy+=p[i];
        sumxy+=p[i]*t;

        t+=delta;
    }

    // calculate the fitting line's slope and intercept value.
    slope=(sumxy-sumy*avx)/(sumx2-n*avx*avx);
    intercept=sumy/n-slope*avx;

    // remove the trend and average for input data points.
    t=TBegin;
    for (int i=0;i<n;++i) {
        p[i]-=(intercept+t*slope);
        t+=delta;
    }

    return;
}

#endif
