#ifndef ASU_LL2TP
#define ASU_LL2TP

#include<iostream>
#include<cmath>

#include<Lon2180.hpp>

/*****************************************************
 * This C++ template convert Longitude, Latitude ( in
 * deg. ) to Theta, Phi ( in rad. ).
 *
 * The Cartesian coordinate x,y,z axis is as follows:
 * x axis: lon=0 ,lat=0
 * y axis: lon=90,lat=0.
 * z axis: lon=  ,lat=90.
 *
 * Theta : angle with z, 0 <= theta <= PI.
 * Phi   : angle with x, counter-clockwise.
 *
 * Longitude: -180 < lon <= 180. (Lon=0 at poles.)
 * Latitude : -90 <= lat <= 90.
 *
 * T1 Lon  ----  Longitude.
 * T2 Lat  ----  Latitude.
 *
 * Shule Yu
 * Dec 29 2017
 *
 * Key words: geography, coordinates, convertion.
*****************************************************/

template<class T1, class T2>
std::pair<double,double> LL2TP(T1 lon, T2 lat){

    // Deal with latitude, theta.
	// Check latitude.
    if ( lat < -90 || lat > 90 ){
		std::cerr <<  __func__ << "; Error: input latitude need to be -90 ~ 90 ..." << std::endl;
		return {};
	}

    if (lat==90) return {0.0,0.0};
    else if (lat==-90) return {M_PI,0.0};

    return {M_PI/2-lat*M_PI/180,Lon2180(lon)*M_PI/180};
}

#endif
