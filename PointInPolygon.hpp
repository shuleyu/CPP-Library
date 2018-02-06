#ifndef ASU_POINTINPOLYGON
#define ASU_POINTINPOLYGON

#include<iostream>
#include<vector>

#include<CrossProduct.hpp>
#include<PointOnSegment.hpp>

/**************************************************************
 * This C++ template decide weather given point on (x-y) plane
 * located inside one given polygon using the Winding Number
 * method.
 *
 * input(s):
 * const vector<pair<double,double>> &Polygon         ----  given polygon.
 * const pair<double,double>         &Point           ----  given point.
 * int                               BoundaryMode     ----  (optional)
 *                                       0 (default value):
 *                                          left and bottom edges are decided to be inside.
 *                                          right and top edges are decided to be outside.
 *                                       1: point on all boundaries are counted as inside.
 *                                      -1: point on all boundaries are counted as outside.
 * return(s):
 * bool  ans  ----  true means the point is inside the polygon.
 *
 * Notice: We define the inside of the polygon as the left hand
 *	side area as one walks on its edge in the order of input.
 *
 * Shule Yu
 * Jan 18 2018
 *
 * Key words: point inside polygon, winding number.
 * Reference: http://geomalgorithms.com/a03-_inclusion.html
****************************************************************/

template<class T1,class T2,class T3, class T4>
bool PointInPolygon(const std::vector<std::pair<T1,T2>> &Polygon,const std::pair<T3,T4> &Point,int BoundaryMode=0){

	int n=Polygon.size(),WN=0;
    double px=Point.first,py=Point.second;

    for (int i=0;i<n;++i){

        double ex1,ey1,ex2,ey2;

        ex1=Polygon[i].first;
        ey1=Polygon[i].second;
        ex2=Polygon[(i+1)%n].first;
        ey2=Polygon[(i+1)%n].second;

        auto flag=PointOnSegment(std::make_pair(ex1,ey1),std::make_pair(ex2,ey2),Point);

        if (BoundaryMode==1 && flag) {
            WN=1;
            break;
        }
        if (BoundaryMode==-1 && flag) {
            WN=0;
            break;
        }

        if (ey1<=py && py<ey2 && CrossProduct(ex1-px,ey1-py,0,ex2-px,ey2-py,0).back()>0) ++WN;
        else if (ey2<=py && py<ey1 && CrossProduct(ex1-px,ey1-py,0,ex2-px,ey2-py,0).back()<0) --WN;
    }

    return (WN!=0);
}

#endif
