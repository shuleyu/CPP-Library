#ifndef ASU_GRIDSTRETCH
#define ASU_GRIDSTRETCH

#include<iostream>
#include<vector>
#include<cmath>
#include<algorithm>

#include<ConvexPolygon.hpp>
#include<CrossProduct.hpp>
#include<DotDist.hpp>
#include<PointInPolygon.hpp>

/****************************************************************
 * This C++ template return the original position of one point in
 * a distorted grid. Input includes the location of four corners
 * of the distroted grid, and the location of a point.
 * Will return the location of the point inside a standard 1x1 grid.
 *
 * input(s):
 * const vector<pair<T1,T2>> &Polygon  ----  the coordinates of the four corners of the distorted grid.
 *                                           Polygon.size() = 4.
 *                                           The four corners can be unordered.
 *
 * const pair<T3,T4>         &P        ----  point coordinates in the distorted grid.
 *
 * return(s):
 * pair<double,double> ans  ----  {x,y} The position of the point in the 1x1 gird: 0<=x<=1, 0<=y<=1.
 *
 * Shule Yu
 * Feb 04 2018
 *
 * Key words: stretching, distorted, restore.
****************************************************************/

double klqwejrlkjqwe(const std::vector<std::pair<double,double>> &P,const std::pair<double,double> &p){

    double l=0,r=1,mid=0.5,eps=1e-6,err=1,ex1,ey1,ex2,ey2;
    while (fabs(err)>eps && l<r){
        ex1=P[1].first+(P[0].first-P[1].first)*mid;
        ey1=P[1].second+(P[0].second-P[1].second)*mid;
        ex2=P[2].first+(P[3].first-P[2].first)*mid;
        ey2=P[2].second+(P[3].second-P[2].second)*mid;
        err=CrossProduct(ex1-p.first,ey1-p.second,0,ex2-p.first,ey2-p.second,0).back();
        if (err>0) l=mid;
        else r=mid;
        mid=(l+r)/2;
    }

    double x=DotDist(ex1,ey1,0,p.first,p.second,0),y=DotDist(p.first,p.second,0,ex2,ey2,0);
    return x/(x+y);
}

template<class T1, class T2, class T3, class T4>
std::pair<double,double> GridStretch(const std::vector<std::pair<T1,T2>> &Polygon, const std::pair<T3,T4> &P){

    // Check the polygon has four corners.
    if (Polygon.size()!=4){
        std::cerr <<  "Error in " << __func__ << ": Polygon.size() != 4 ..." << std::endl;
        return {};
    }

    // Reorder the four corners.
    // upper left[0], lower left[1], lower right[2], upper right[3]
    std::vector<std::pair<double,double>> polygon=Polygon;
    auto f1=[](const std::pair<double,double> &p1, const std::pair<double,double> &p2){
        return p1.second>p2.second;
    };
    auto f2=[](const std::pair<double,double> &p1, const std::pair<double,double> &p2){
        return p1.first<p2.first;
    };
    sort(polygon.begin(),polygon.end(),f1);
    sort(polygon.begin(),polygon.begin()+2,f2);
    sort(polygon.begin()+2,polygon.end(),f2);
    swap(polygon[1],polygon[2]);
    swap(polygon[2],polygon[3]);

    // Check whether the polygon is convex.
    if (!ConvexPolygon(polygon)) {
        std::cerr <<  "Error in " << __func__ << ": Grid is not convex ..." << std::endl;
        return {};
    }

    // Check whether the point is outside.
    if (!PointInPolygon(polygon,P,1)){
        std::cerr <<  "Error in " << __func__ << ": Point is outside of input grid..." << std::endl;
        return {};
    }
    std::pair<double,double> point=P;

    // 1. Move the lower left point to (0,0);
    double x=polygon[1].first,y=polygon[1].second;
    for (auto &item:polygon){
        item.first-=x;
        item.second-=y;
    }
    point.first-=x;
    point.second-=y;


    // 2. Rotate polygon such that polygon[1] and polygon[2] overlap with the x-axis.
    double theta=atan2(-1.0*polygon[2].second,polygon[2].first);
    x=point.first;y=point.second;
    point.first=x*cos(theta)-y*sin(theta);
    point.second=x*sin(theta)+y*cos(theta);

    for (auto &item:polygon){
        x=item.first;y=item.second;
        item.first=x*cos(theta)-y*sin(theta);
        item.second=x*sin(theta)+y*cos(theta);
    }

    // 3. Find the slope of the line on current west-east direction that intercept the left and right boundaries
    // with the same ratio.
    double l=klqwejrlkjqwe(polygon,point);


    // 1*. Move the lower right point to (0,0);
    x=polygon[2].first,y=polygon[2].second;
    for (auto &item:polygon){
        item.first-=x;
        item.second-=y;
    }
    point.first-=x;
    point.second-=y;

    // 2*. Rotate polygon such that polygon[2] and polygon[3] overlap with the x-axis.
    theta=atan2(-1.0*polygon[3].second,polygon[3].first);
    x=point.first;y=point.second;
    point.first=x*cos(theta)-y*sin(theta);
    point.second=x*sin(theta)+y*cos(theta);

    for (auto &item:polygon){
        x=item.first;y=item.second;
        item.first=x*cos(theta)-y*sin(theta);
        item.second=x*sin(theta)-y*cos(theta);
    }

    std::rotate(polygon.begin(),polygon.begin()+1,polygon.end());

    // Find the slope of the line on current west-east direction that intercept the left and right boundaries
    // with the same ratio.
    double r=klqwejrlkjqwe(polygon,point);

    return {l,r};
}

#endif
