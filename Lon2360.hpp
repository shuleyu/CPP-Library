#ifndef ASU_LON2360
#define ASU_LON2360

/***********************************************************
 * This C++ template convert input longitude (in deg) to
 * [0,360] deg.
 *
 * T lon  ----  Longitude.
 *
 * Shule Yu
 * Dec 29 2017
 *
 * Key words: coordinates, longitude.
***********************************************************/

template<class T>
T Lon2360(T lon){

    if (lon>=0) lon-=360.0*((int)(lon/360));
    else lon+=360.0*(1+(int)(-lon/360));

    if (lon>=360) lon=0;

    return lon;
}

#endif
