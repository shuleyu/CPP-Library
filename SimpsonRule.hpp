#ifndef ASU_SIMPSONRULE
#define ASU_SIMPSONRULE

#include<vector>

/*************************************************************
 * This C++ template returns integral value of input 1D array,
 * using simpson's rule.
 *
 * Simpson's rule:
 *
 *     h -- interval.
 *     ans = h/3 * [ f(x0) + 4f(x1) + 2f(x2) + ... + 4f(x_n-1) + f(x_n)]
 *
 * This template will only return the summation part (h/3 is not included).
 * If x length is even, ignore the last data point.
 *
 * input(s):
 * const vector<T> &x  ----  1-D input array.
 *
 * return(s):
 * T ans ----  Integral result.
 *
 * Shule Yu
 * Jan 21 2018
 *
 * Key words: simpson rule, integral
 * Reference: https://en.wikipedia.org/wiki/Simpson%27s_rule#Composite_Simpson's_rule
*************************************************************/

template<class T>
T SimpsonRule(const std::vector<T> &x){

    if (x.size()<=1) return 0;

    if (x.size()==2)  // reduced to trapezoid rule.
        return 3.0/2.0*(x[0]+x[1]);

    T ans=x[0];

    int n=x.size();

    ans=x[0];
    for (int i=1;i<(n-1)/2*2;++i) ans+=(i%2+1)*2*x[i];
    ans+=x[(n-1)/2*2];

    return ans;
}

#endif
