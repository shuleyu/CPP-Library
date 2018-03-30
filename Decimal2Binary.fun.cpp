#include<string>

#include<ASU_tools.hpp>

/*************************************************
 * This C++ function converts integer to binary
 * represent by string. (32-bit)
 *
 * input(s):
 * int n  ----  Input integer.
 *
 * return(s):
 * string ans  ----  Converted binary 0/1 string.
 *
 * Shule Yu
 * Dec 19 2017
 *
 * Key words: decimal to binary.
*************************************************/

std::string Decimal2Binary(int n){

    // set the sign bit.
    std::string Ans(32,'0');
    Ans[0]=(n<0?'1':'0');

    // clear the sign bit.
    n &= ~(1UL << 31);

    // set the rest bits.
    int i=0;
    while(n>0){
        Ans[31-i++]=('0'+n%2);
        n>>=1;
    }

    return Ans;
}
