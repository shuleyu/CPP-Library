#ifndef ASU_GLOBEGRIDAREA
#define ASU_GLOBEGRIDAREA

#include<cmath>
#include<algorithm>

/*************************************************
 * This C++ template returns the grid area for a
 * "square" globe grid.
 *
 * input(s):
 * const double &radius     ----  The radius the grid at. (in km.)
 * const double &centerLat  ----  The grid center latitude. (in deg, -90 ~ 90)
 * const double &lonSize    ----  The longitude size of the grid. (in deg.)
 * const double &latSize    ----  The latitude size of the grid. (in deg.)
 *                                
 *
 * output(s):
 * double ans  ----  The area of the grid (in km^2).
 *
 * Shule Yu
 * Jun 15 2019
 *
 * Key words: grid, area
*************************************************/

double GlobeGridArea(const double &radius, const double &centerLat,
                     const double &lonSize, const double &latSize){
    double lat1=std::max(centerLat-latSize/2,-90.0)*M_PI/180;
    double lat2=std::min(centerLat+latSize/2,90.0)*M_PI/180;
    double lon=std::min(360.0,fabs(lonSize))*(M_PI/180);
    return radius*radius*lon*fabs(sin(lat1)-sin(lat2));
}

#endif
