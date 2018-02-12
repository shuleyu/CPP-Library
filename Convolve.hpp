#ifndef ASU_CONVOLVE
#define ASU_CONVOLVE

#include<iostream>
#include<vector>

/***********************************************************
 * This C++ template returns convolution result of two input
 * array.
 *
 * input(s):
 * vector<T1> &x    ----  Array x.
 * vector<T2> &y    ----  Array y.
 * const bool &Cut  ----  (Optional), default is false.
 *                        false, means return the full convlve
 *                               result. Return size = x.size()+y.size()-1
 *                        true, means only return the center
 *                              part. Return size = x.size().
 *                              Will remove y.size()/2 elements
 *                              in the begining, (y.size()-1)/2
 *                              elements at the end.
 *
 * return(s):
 * vector<double> ans  ----  Convolve result.
 *
 *
 * Shule Yu
 * Dec 29 2017
 *
 * Key words: convolution.
***********************************************************/

template<class T1, class T2>
std::vector<double> Convolve(const std::vector<T1> &x, const std::vector<T2> &y, const bool &Cut=false){

    if (x.empty() || y.empty()){
        std::cerr <<  "Error in " << __func__ << ": input array size is zero ..." << std::endl;
        return {};
    }

    int m=x.size(),n=y.size();
    int Front=0,Size=m+n-1;
    if (Cut) {
        Front=n/2;
        Size=Front+m;
    }

    std::vector<double> ans;
    for (int i=Front;i<Size;++i){
        ans.push_back(0);
        int Begin=std::max(i-n+1,0);
        int End=std::min(i+1,m);
        for (int j=Begin;j<End;++j) ans.back()+=x[j]*y[i-j];
    }

    return ans;
}

#endif
