#ifndef ASU_CART2SPH
#define ASU_CART2SPH

#include<cmath>

/***********************************************************
 * This C++ template convert x, y, z in Cartesian coordinate
 * to r, theta, phi ( in rad. ) in Shperical coordinates.
 *
 * double x      ----  x
 * double y      ----  y
 * double z      ----  z
 * double r      ----  r
 * double phi    ----  phi, angle from positive z direction. (rad)
 * double theta  ----  theta, angle from positive x direction.  (rad)
 *
 * Shule Yu
 * May 15 2014
 *
 * Key words: coordinates, convertion, spherical, cartesian
***********************************************************/

template<class T1,class T2,class T3>
void Cart2Sph(const T1 &x, const T2 &y, const T3 &z, double &r, double &phi, double &theta){

    r=sqrt(x*x+y*y+z*z);
    phi=atan2(1.0*y,1.0*x);
    theta=atan2(sqrt(1.0*x*x+y*y),1.0*z);

    return;
}

#endif
