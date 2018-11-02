#ifndef ASU_LINEJUNCTION
#define ASU_LINEJUNCTION

#include<cmath>
#include<limits>

/**************************************************************
 * This C++ template return the junction point of two straigh
 * lines.
 *
 * input(s):
 * const pair<T1,T2> &p1  ----  first point.
 * const double      &s1  ----  slope through the first point.
 * const pair<T3,T4> &p2  ----  second point.
 * const double      &s2  ----  slope through the second point.
 *
 * return(s):
 * std::pair<bool,pair<double,double>>  ans
 *                       ----  {Result flag, the position of the junction point}
 *                             If lines are parallel(no junction), flag=false.
 *
 * Shule Yu
 * Jan 19 2018
 *
 * Key words: line junction, intercept, cross.
****************************************************************/

template<typename T1,typename T2,typename T3,typename T4>
std::pair<bool,std::pair<double,double>> LineJunction(const std::pair<T1,T2> &p1,const double &s1,
                                                      const std::pair<T3,T4> &p2,const double &s2){

    std::pair<double,double> ans{std::numeric_limits<double>::max(),std::numeric_limits<double>::max()};

    if (s1==s2 || (std::isinf(s1) && std::isinf(s2))) return {false,ans};

    double b1=p1.second-s1*p1.first,b2=p2.second-s2*p2.first;
    if (std::isinf(s1)){
        ans.first=p1.first;
        ans.second=s2*p1.first+b2;
    }
    else if (std::isinf(s2)){
        ans.first=p2.first;
        ans.second=s1*p2.first+b1;
    }
    else{
        ans.second=((s2*b1-s1*b2)/(s2-s1));
        if (s1!=0) ans.first=(ans.second-b1)/s1;
        else ans.first=(ans.second-b2)/s2;
    }

    return {true,ans};
}

#endif
