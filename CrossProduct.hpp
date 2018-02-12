#ifndef ASU_CROSSPRODUCT
#define ASU_CROSSPRODUCT

#include<vector>

/*********************************************************
 * This C++ template take the cross product of two vectors
 * (x1,y1,z1) and (x2,y2,z2).
 *
 * input(s):
 * const T1 &x1  ----  x value of vector 1.
 * const T2 &y1  ----  y value of vector 1.
 * const T3 &z1  ----  z value of vector 1.
 * const T4 &x2  ----  x value of vector 2.
 * const T5 &y2  ----  y value of vector 2.
 * const T6 &z2  ----  z value of vector 2.
 *
 * return(s):
 * vector<double> ans  ----  {x3,y3,z3}
 *
 * Shule Yu
 * Dec 28 2017
 *
 * Key words: cross product.
*********************************************************/

template<class T1,class T2,class T3,class T4,class T5,class T6>
inline std::vector<double> CrossProduct(const T1 &x1, const T2 &y1, const T3 &z1, const T4 &x2, const T5 &y2, const T6 &z2){
    return {(1.0*y1*z2-y2*z1),(1.0*z1*x2-z2*x1),(1.0*x1*y2-x2*y1)};
}

#endif
