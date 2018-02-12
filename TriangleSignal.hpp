#ifndef ASU_TRIANGLESIGNAL
#define ASU_TRIANGLESIGNAL

#include<vector>

/***********************************************************
 * This C++ template returns a triangle signal.
 * The signal has peak at its center, amplitude is 1.
 *
 * input(s):
 * const int &N     ----  request signal length.
 * const T   &frac  ----  request fraction of the non-zero part
 *                           (explaned below)
 *
 *                      *
 *                     * *
 *                    *   *
 *                   *     *
 *        00000000000       00000000000
 *
 *        frac = 7/N = 7/29 = 0.24137931
 *
 * return(s):
 * vector<double> ans  ----  the triangle signal.
 *
 * Shule Yu
 * Dec 31 2017
 *
 * Key words: triangle signal.
***********************************************************/

template<class T>
std::vector<double> TriangleSignal(const int &N,const T &frac){

    // Check requested size.
    if (N<=0) return {};

    // Check signal width.
    double Frac=(frac<0?0:frac);; // gives an impulse if frac<=0.

    int L=N*Frac;
    double Inc=-2.0/(L+1);
    std::vector<double> ans(N,0);
    ans[N/2]=1;

    for (int i=N/2+1;i<N;i++){
        ans[i]=ans[i-1]+Inc;
        if (ans[i]<0) ans[i]=0;
        ans[N/2-(i-N/2)]=ans[i];
    }
    ans[0]=ans[1]+Inc;
    if (ans[0]<0) ans[0]=0;

    return ans;
}

#endif
