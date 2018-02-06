#ifndef ASU_POINTONSEGMENT
#define ASU_POINTONSEGMENT

#include<cmath>

#include<CrossProduct.hpp>

/**************************************************************
 * This C++ template return whether given points are on a give
 * straight line segment.
 *
 * const pair<T1,T2> &x  ----  One end point of the line segment.
 * const pair<T3,T4> &y  ----  Another end point of the line segment.
 * const pair<T5,T6> &p  ----  Given point.
 *
 * return value:
 * bool  ans  ----  true means the point is on the line segment.
 *
 * Shule Yu
 * Feb 02 2018
 *
 * Key words: point on line segment.
****************************************************************/

template<class T1,class T2,class T3,class T4,class T5,class T6>
bool PointOnSegment(const std::pair<T1,T2> &x,const std::pair<T3,T4> &y,const std::pair<T5,T6> &p){

    double dx1=x.first-p.first,dy1=y.first-p.first,dx2=x.second-p.second,dy2=y.second-p.second;
    double EPS=fabs(CrossProduct(dx1,dy1,0,dx2,dy2,0).back());
    EPS/=(sqrt(dx1*dx1+dx2*dx2)*sqrt(dy1*dy1+dy2*dy2));

    return (std::min(x.first,y.first)<=p.first && p.first<=std::max(x.first,y.first) &&
            std::min(x.second,y.second)<=p.second && p.second<=std::max(x.second,y.second) && EPS<1e-7);
}

#endif
