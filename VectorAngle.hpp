#ifndef ASU_VECTORANGLE
#define ASU_VECTORANGLE

#include<cmath>

#include<CrossProduct.hpp>

/*********************************************************
 * This C++ template return the angle between two vectors.
 * (0~2*pi).
 *
 * input(s):
 * const pair<T1,T2> &v1  ----  vector 1 (0,0) -> (v1)
 * const pair<T3,T4> &v2  ----  vector 2 (0,0) -> (v2)
 *
 * return(s):
 * double ans  ----  The angle (counter-clock wise) from v1 to v2.
 *
 * Shule Yu
 * Feb 05 2018
 *
 * Key words: angle, vector.
*********************************************************/

template<typename T1,typename T2,typename T3,typename T4>
double VectorAngle(const std::pair<T1,T2> &v1,const std::pair<T3,T4> &v2){

    // Do dot product.
    double ans=v1.first*v2.first+v1.second*v2.second;
    ans/=(sqrt(v1.first*v1.first+v1.second*v1.second)*sqrt(v2.first*v2.first+v2.second*v2.second));
    ans=acos(ans);

    // Check range.
    if (CrossProduct(v1.first,v1.second,0,v2.first,v2.second,0).back()<0) ans=2*M_PI-ans;

    return ans;
}

#endif
