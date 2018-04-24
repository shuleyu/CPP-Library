#ifndef ASU_COMBINATION
#define ASU_COMBINATION

#include<iostream>

/**************************************************************
 * This C++ template returns the combination of input integers.
 *
 * input(s):
 * const T &n  ----  Lower number.
 * const T &k  ----  Upper number. T should be non-negativee integer.
 *
 * return(s):
 * int ans  ---- C_n^k
 *
 * Shule Yu
 * Mar 10 2015
 *
 * Key words: combination
**************************************************************/

template<class T>
int Combination(const T &n, const T &k){

    if ( n < 0 || k < 0 || k > n ){
        std::cerr <<  "Error in " << __func__ << ": mathematic error ..." << std::endl;
        return 0;
    }

    int ans=1;
    for (int i=1;i<=k;++i) ans*=(n-k+i);
    for (int i=1;i<=k;++i) ans/=i;

    return ans;
}

#endif
