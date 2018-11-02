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
 * const double &lon1   ----  p1's longitude.
 * const double &lat1   ----  p1's latitude.
 * const double &r1     ----  p1's distance to Earth's center.
 * const double &lon2   ----  p2's longitude.
 * const double &lat2   ----  p2's latitude.
 * const double &r2     ----  p2's distance to Earth's center.
 *
 * return(s):
 * double ans  ----  Straight line distance between these two points.
 *
 * Shule Yu
 * Dec 31 2017
 *
 * Key words: geography, coordinates, distance.
***********************************************************/

double LocDist(const double &lon1, const double &lat1, const double &r1,
               const double &lon2, const double &lat2, const double &r2){

    auto res1=LL2TP(lon1,lat1);
    auto res2=LL2TP(lon2,lat2);

    auto p1=Sph2Cart(r1,res1.first,res1.second);
    auto p2=Sph2Cart(r2,res2.first,res2.second);

    return DotDist(p1[0],p1[1],p1[2],p2[0],p2[1],p2[2]);
}

#endif
