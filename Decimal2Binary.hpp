#ifndef ASU_DECIMAL2BINARY
#define ASU_DECIMAL2BINARY

#include<string>

/*************************************************
 * This C++ template converts integer to binary
 * represent by string. (32-bit)
 *
 * input(s):
 * const int &n  ----  Input integer.
 *
 * return(s):
 * string ans  ----  Converted binary 0/1 string.
 *
 * Shule Yu
 * Dec 19 2017
 *
 * Key words: decimal to binary.
*************************************************/

std::string Decimal2Binary(const int &n){

    // set the sign bit.
    std::string Ans(32,'0');
    Ans[0]=(n<0?'1':'0');
    int N=n;

    // clear the sign bit.
    N &= ~(1UL << 31);

    // set the rest bits.
    int i=0;
    while(N>0){
        Ans[31-i++]=('0'+N%2);
        N>>=1;
    }

    return Ans;
}

#endif
