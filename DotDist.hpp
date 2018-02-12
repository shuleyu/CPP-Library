#ifndef ASU_DOTDIST
#define ASU_DOTDIST

#include<cmath>

/*********************************************************
 * This C++ template return the straight line distance
 * between two Cartesian input points.
 *
 * input(s):
 * const T1 x1  ----  x position of point 1.
 * const T2 y1  ----  y position of point 1.
 * const T3 z1  ----  z position of point 1.
 * const T4 x2  ----  x position of point 2.
 * const T5 y2  ----  y position of point 2.
 * const T6 z2  ----  z position of point 2.
 *
 * return(s):
 * double ans  ----  distance between point 1 and point 2.
 *
 * Shule Yu
 * Mar 30 2017
 *
 * Key words: distance.
*********************************************************/

template<class T1, class T2, class T3, class T4, class T5, class T6>
inline double DotDist(const T1 &x1, const T2 &y1,const T3 &z1, const T4 &x2,const T5 &y2,const T6 &z2){
    return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2));
}

#endif
