#ifndef ASU_LOCDIST
#define ASU_LOCDIST

#include<DotDist.hpp>
#include<LL2TP.hpp>
#include<Sph2Cart.hpp>

/***********************************************************
 * This C++ template caculate the straight line distance
 * between two geographic points.
 *
 * input(s):
 * const T1 &lon1   ----  p1's longitude.
 * const T2 &lat1   ----  p1's latitude.
 * const T3 &r1     ----  p1's distance to Earth's center.
 * const T4 &lon2   ----  p2's longitude.
 * const T5 &lat2   ----  p2's latitude.
 * const T6 &r2     ----  p2's distance to Earth's center.
 *
 * return(s):
 * double ans  ----  Straight line distance between these two points.
 *
 * Shule Yu
 * Dec 31 2017
 *
 * Key words: geography, coordinates, distance.
***********************************************************/

template <class T1,class T2,class T3,class T4,class T5,class T6>
double LocDist(const T1 &lon1, const T2 &lat1, const T3 &r1, const T4 &lon2, const T5 &lat2, const T6 &r2){

    auto res1=LL2TP(lon1,lat1);
    auto res2=LL2TP(lon2,lat2);

    auto p1=Sph2Cart(r1,res1.first,res1.second);
    auto p2=Sph2Cart(r2,res2.first,res2.second);

    return DotDist(p1[0],p1[1],p1[2],p2[0],p2[1],p2[2]);
}

#endif
