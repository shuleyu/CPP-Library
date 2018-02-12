#ifndef ASU_SPH2CART
#define ASU_SPH2CART

#include<cmath>
#include<vector>

/***********************************************************
 * This C++ template convert r, theta, phi (in rad) in
 * Spherical Cartesian coordinates to x, y, z in
 * Cartesian coordinates.
 *
 * input(s):
 * const double &r      ----  r
 * const double &theta  ----  theta, angle with positive z direction.  (rad)
 * const double &phi    ----  phi, angle couter-clock wise with positive x direction. (rad)
 *
 * return(s):
 * vector<double> ans  ----  {x,y,z} in Cartesian.
 *
 * Shule Yu
 * Dec 29 2017
 *
 * Key words: coordinates, convertion, spherical, cartesian
 *
 * Reference: https://en.wikipedia.org/wiki/Spherical_coordinate_system
***********************************************************/

template<class T1,class T2,class T3>
inline std::vector<double> Sph2Cart(const T1 &r, const T2 &theta, const T3 &phi){
    return {r*cos(phi)*sin(theta),r*sin(phi)*sin(theta),r*cos(theta)};
}

#endif
