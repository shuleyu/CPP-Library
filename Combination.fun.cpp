#include<iostream>

#include<ASU_tools.hpp>

/**************************************************************
 * This C++ function returns the combination of input integers.
 *
 * input(s):
 * int n  ----  Lower number.
 * int k  ----  Upper number.
 *
 * return(s):
 * int ans  ---- C_n^k
 *
 * Shule Yu
 * Mar 10 2015
 *
 * Key words: combination
**************************************************************/

int Combination(const int &n, const int &k){

    if ( n < 0 || k < 0 || k > n ){
        std::cerr <<  "Error in " << __func__ << ": mathematic error ..." << std::endl;
        return 0;
    }

    int ans=1;
    for (int i=1;i<=k;++i) ans*=(n-k+i);
    for (int i=1;i<=k;++i) ans/=i;

    return ans;
}
