#ifndef ASU_SPH2CART
#define ASU_SPH2CART

#include<cmath>

/***********************************************************
 * This C++ template convert r, theta, phi (in rad) in
 * Spherical Cartesian coordinates to x, y, z in
 * Cartesian coordinates.
 *
 * double r      ----  r
 * double phi    ----  phi, angle from positive z direction. (rad)
 * double theta  ----  theta, angle from positive x direction.  (rad)
 * double x      ----  x
 * double y      ----  y
 * double z      ----  z
 *
 * Shule Yu
 * Dec 29 2017
 *
 * Key words: coordinates, convertion, spherical, cartesian
***********************************************************/

template<class T1,class T2,class T3>
void Sph2Cart(const T1 &r, const T2 &theta, const T3 &phi, double &x, double &y, double &z){

    x=r*cos(phi)*sin(theta);
    y=r*sin(phi)*sin(theta);
    z=r*cos(theta);

    return;
}

#endif
