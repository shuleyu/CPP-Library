#ifndef ASU_COMBINATION
#define ASU_COMBINATION

#include<iostream>

/**************************************************************
 * This C++ template returns the combination of input integers.
 *
 * input(s):
 * const std::size_t &n  ----  Lower number.
 * const std::size_t &k  ----  Upper number.
 *
 * return(s):
 * int ans  ---- C_n^k
 *
 * Shule Yu
 * Mar 10 2015
 *
 * Key words: combination
**************************************************************/

int Combination(const std::size_t &n, const std::size_t &k){

    if ( n < 0 || k < 0 || k > n ){
        std::cerr <<  "Error in " << __func__ << ": mathematic error ..." << std::endl;
        return 0;
    }

    int ans=1;
    for (std::size_t i=1;i<=k;++i) ans*=(n-k+i);
    for (std::size_t i=1;i<=k;++i) ans/=i;

    return ans;
}

#endif
