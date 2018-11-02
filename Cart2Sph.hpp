#ifndef ASU_CART2SPH
#define ASU_CART2SPH

#include<vector>
#include<cmath>

/***********************************************************
 * This C++ template convert x, y, z in Cartesian coordinate
 * to r, theta, phi ( in rad. ) in Shperical coordinates.
 *
 * input(s):
 * const double &x      ----  x
 * const double &y      ----  y
 * const double &z      ----  z
 *
 * return(s):
 * vector<double> ans  ----  {r,theta,phi}
 *                           theta, angle with positive z direction.  (rad)
 *                           phi, angle couter-clock wise with positive x direction. (rad)
 *
 * Shule Yu
 * May 15 2014
 *
 * Key words: coordinates, convertion, spherical, cartesian
 *
 * Reference: https://en.wikipedia.org/wiki/Spherical_coordinate_system
***********************************************************/

inline std::vector<double> Cart2Sph(const double &x, const double &y, const double &z){
    return{sqrt(x*x+y*y+z*z),atan2(sqrt(1.0*x*x+y*y),1.0*z),atan2(1.0*y,1.0*x)};
}

#endif
