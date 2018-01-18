#include<iostream>

/**************************************************************
 * This C++ function returns the combination of input integers.
 *
 * int n  ----  Lower number.
 * int k  ----  Upper number.
 *
 * Return C_{n}^{k}
 *
 * Shule Yu
 * Mar 10 2015
 *
 * Key words: combination
**************************************************************/

int Combination(const int &n, const int &k){

    if ( n < 0 || k < 0 || k > n ){
		std::cerr <<  __func__ << "; Error: input doesn't make sense ..." << std::endl;
		return 0;
    }

    int ans=1;
    for (int i=1;i<=k;++i) ans*=(n-k+i);
    for (int i=1;i<=k;++i) ans/=i;

    return ans;
}
