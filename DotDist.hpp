#ifndef ASU_DOTDIST
#define ASU_DOTDIST

#include<cmath>

/*********************************************************
 * This C++ template return the straight line distance
 * between two Cartesian input points.
 *
 * input(s):
 * const double x1  ----  x position of point 1.
 * const double y1  ----  y position of point 1.
 * const double z1  ----  z position of point 1.
 * const double x2  ----  x position of point 2.
 * const double y2  ----  y position of point 2.
 * const double z2  ----  z position of point 2.
 *
 * return(s):
 * double ans  ----  distance between point 1 and point 2.
 *
 * Shule Yu
 * Mar 30 2017
 *
 * Key words: distance.
*********************************************************/

inline double DotDist(const double &x1, const double &y1,const double &z1,
                      const double &x2, const double &y2,const double &z2){
    return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2));
}

#endif
