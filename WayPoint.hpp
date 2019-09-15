#ifndef ASU_WAYPOINT
#define ASU_WAYPOINT

#include<cmath>

#include<FindAz.hpp>
#include<Lon2180.hpp>

/**************************************************************
 * This C++ template returns the geographic location (waypiont,
 * in longitude and latitude) if one start from (lon,lat),
 * walk along the great circle path (initial azimuth is az)
 * for (dist) degree.
 *
 * Range of inputs:
 *
 *     lon : -180 ~ 180.
 *     lat : -90  ~ 90.
 *     az  : -180 ~ 180.
 *     dist:  0   ~ 180.
 *
 * input(s):
 * const double &lon   ----  longitude of start point
 * const double &lat   ----  latitude  of start point
 * const double &azi   ----  direction, azimuth centerd on the start piont.
 * const double &dist  ----  GCP length from start point to waypoint. ( deg. )
 *
 * return(s):
 * pair<double,double> ans  ----  {plon,plat}, location of the waypoint.
 *
 * Shule Yu
 * Jan 23 2018
 *
 * Key words: waypoint.
 *
 * Reference:
 *     http://en.wikipedia.org/wiki/Great-circle_navigation
**************************************************************/

std::pair<double,double> WayPoint(const double &lon, const double &lat,
                                  const double &azi, const double &dist){

    // Step 1. Calculate azimuth from event to station. (azimuth is centered on event)
    double a,az;
    az=azi*M_PI/180;
    a=lat*M_PI/180;

    // Step 2.
    // Extend great circle arc from station to event, till it cross equator at point P.
    // Calculate azimuth from P to event (or station). (azimuth is centered on P)
    double x,y;
    double az_p;
    y=sin(az)*cos(a);
    x=sqrt(pow(cos(az),2)+pow(sin(az),2)*pow(sin(a),2));
    az_p=atan2(y,x);

    // Step 3. Calculate the great circle distance from P to event.
    double delta_p_e;
    y=tan(a);
    x=cos(az);
    delta_p_e=atan2(y,x);

    // Step 4. Calculate longitude of point P.
    double lo_p;
    y=sin(az_p)*sin(delta_p_e);
    x=cos(delta_p_e);
    lo_p=lon*M_PI/180-atan2(y,x); // this may have some problem.

    // Step 5. Calculate great circle distance from point P to our destination.
    double delta;
    delta=delta_p_e+dist*M_PI/180;

    // Step 6. Calculate destination's latitude.
    double plat;
    y=cos(az_p)*sin(delta);
    x=sqrt(pow(cos(delta),2)+pow(sin(az_p),2)*pow(sin(delta),2));
    plat=atan2(y,x);
    plat*=180/M_PI;

    // Step 7. Calculate destination's longitude.
    double plon;
    y=sin(az_p)*sin(delta);
    x=cos(delta);
    plon=lo_p+atan2(y,x);
    plon*=180/M_PI;

    return {Lon2180(plon),plat};
}

std::pair<double,double> WayPoint(const double &lon1, const double &lat1,
                                  const double &lon2, const double &lat2, const double &dist){
    return WayPoint(lon1, lat1, FindAz(lon1, lat1, lon2, lat2), dist);
}

#endif
