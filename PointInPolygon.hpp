#ifndef ASU_POINTINPOLYGON
#define ASU_POINTINPOLYGON

#include<vector>
#include<limits>

#include<PointOnSegment.hpp>

/**************************************************************
 * This C++ template decide weather given point on (x-y) plane
 * located inside one given polygon using the Winding Number
 * method.
 *
 * input(s):
 * const vector<pair<double,double>> &Polygon         ----  given polygon.
 * const pair<double,double>         &Point           ----  given point.
 * const int                         &BoundaryMode    ----  (Optional), default is 0
 *                                       0: left and bottom edges are decided to be inside.
 *                                          right and top edges are decided to be outside.
 *                                       1: point on all boundaries are counted as inside.
 *                                      -1: point on all boundaries are counted as outside.
 * const vector<double>              &PolygonBound    ----  (Optional), default is empty
 *                                                          The given polygon bounds (Xmin, Xmax, Ymin, Ymax).
 *                                                          If there are a lot of points to decide, it's highly
 *                                                          recommended to provide this parameter.
 *
 * return(s):
 * bool  ans  ----  true means the point is inside the polygon.
 *
 * Notice: We define the inside of the polygon as the left hand
 * side area as one walks on its edge in the order of input.
 *
 * Shule Yu
 * Jan 18 2018
 *
 * Key words: point inside polygon, winding number.
 * Reference: http://geomalgorithms.com/a03-_inclusion.html
****************************************************************/

template<typename T1,typename T2,typename T3, typename T4>
bool PointInPolygon(const std::vector<std::pair<T1,T2>> &Polygon,const std::pair<T3,T4> &Point, const int BoundaryMode=0,
                    const std::vector<double> &PolygonBound={}){

    int n=(int)Polygon.size(),WN=0;
    double px=Point.first,py=Point.second;

    // Check bounds.
    if (!PolygonBound.empty() && (px<PolygonBound[0] || px>PolygonBound[1] || py<PolygonBound[2] || py>PolygonBound[3])) return false;

    for (int i=0;i<n;++i){

        const T1 &ex1=Polygon[i].first;
        const T2 &ey1=Polygon[i].second;
        const T3 &ex2=Polygon[(i+1)%n].first;
        const T4 &ey2=Polygon[(i+1)%n].second;

        bool flag=false;
        if (BoundaryMode!=0)
            flag=PointOnSegment(std::make_pair(ex1,ey1),std::make_pair(ex2,ey2),Point);

        if (BoundaryMode==1 && flag) return true;
        if (BoundaryMode==-1 && flag) return false;

        if (ey1<=py && py<ey2 && (ex1-px)*(ey2-py)>(ey1-py)*(ex2-px)) ++WN;
        else if (ey2<=py && py<ey1 && (ex1-px)*(ey2-py)<(ey1-py)*(ex2-px)) --WN;
    }

    return (WN!=0);
}

#endif
