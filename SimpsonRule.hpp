#ifndef ASU_SIMPSONRULE
#define ASU_SIMPSONRULE

#include<iterator>
#include<vector>

/*******************************************************************************************
 * This C++ template returns integral value of input 1D array, using Simpson's rule.
 *
 * Simpson's first rule:
 *
 *     ans = 1/3 * h * [ f(x1) + 4f(x1) + 2f(x2) + ... + 4f(x_n-1) + f(x_n) ]
 *
 * Simpson's second rule (the 3/8 rule):
 *
 *     h -- interval.
 *     ans = 3/8 * h * [ f(x1) + 3f(x2) + 3f(x3) + 2f(x4)
 *                             + 3f(x5) + 3f(x6) + 2f(x7) + ... + f(x_n) ]
 *
 * Lf n is odd, use Simpson's first rule on all the points.
 * If n is even, use the first rule on points x_1 ~ x_n-3, then use the second rule on
 * points x_n-3 ~ x_n. To be specific:
 *
 *     ans =                                     0                                 (if n<=1),
 *
 *         =   1/2 * h * [ f(x1) + f(x2) ]                                         (if n==2),
 *
 *         =   1/3 * h * [ f(x1) + 4f(x2) + 2f(x3) + ... + 4f(x_n-1) + f(x_n) ]    (if n is odd),
 *
 *         =   3/8 * h * [ f(x1) + 3f(x2) + 3f(x3) + f(x4) ]                       (if n==4),
 *
 *         =   1/3 * h * [ f(x1) + 4f(x2) + 2f(x3) + ... + 4f(x_n-4) + f(x_n-3) ]
 *           + 3/8 * h * [ f(x_n-3)  +  3f(x_n-2)  +  3f(x_n-1)  +  f(x_n)      ]  (if n>4 and is even).
 *
 *
 * input(s):
 * const vector<T1>::iterator Begin  ----  x.begin()
 * const vector<T1>::iterator End    ----  x.end()
 * const T2                   &h     ----  Interval.
 * T3                         oper   ----  Unary operator.
 *
 * return(s):
 * double ans  ----  Integral result.
 *
 * Shule Yu
 * Jan 21 2018
 *
 * Key words: Simpson rule, integral
 * Reference: https://en.wikipedia.org/wiki/Simpson%27s_rule#Composite_Simpson's_rule
*******************************************************************************************/

namespace jlkqweokajsdlf{
    template <class T> struct NoChanges: std::unary_function <T,T> {
        T operator() (const T& x) const {return x;}
    };
}

template <class T1, class T2, class T3=jlkqweokajsdlf::NoChanges<class std::iterator_traits<T1>::value_type>>
double SimpsonRule(T1 Begin, T1 End, T2 h, T3 oper=T3()) {

    size_t n=std::distance(Begin,End);
    if (n<=1) return 0;
    if (n==2) return (oper(*Begin)+oper(*(Begin+1)))*h/2.0; // Reduced to trapezoid rule.
    if (n==4) return (oper(*Begin)+3*oper(*(Begin+1))+3*oper(*(Begin+2))+oper(*(Begin+3)))*h*3.0/8.0; // 3/8 rule.

    if (n%2==1) {
        double ans=oper(*Begin);
        int i=1;
        for (auto it=Begin+1;it!=std::prev(End);++it,++i)
            ans+=(i%2+1)*2*oper(*it);
        ans+=oper(*std::prev(End));
        return ans*h/3.0;
    }
    else {
        return SimpsonRule(Begin,std::prev(End,3),h,oper)+SimpsonRule(std::prev(End,4),End,h,oper);
//         // A similar modification:
//         return SimpsonRule(Begin,std::prev(End),h,oper)+SimpsonRule(std::prev(End,2),End,h,oper);
    }
}

#endif
