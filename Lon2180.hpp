#ifndef ASU_LON2180
#define ASU_LON2180

#include<ASU_tools.hpp>

/*******************************************************
 * This C++ template convert input longitude (in deg) to
 * [-180,180] deg.
 *
 * T lon  ----  Longitude.
 *
 * Shule Yu
 * Dec 29 2017
 *
 * Key words: coordinates, longitude.
*******************************************************/

template<class T>
T Lon2180(T lon){

    if (lon>=0) lon-=360*((int)(lon/360));
    else lon+=360*(1+(int)(-lon/360));

    if (lon>180) lon-=360;

    return lon;
}

#endif
