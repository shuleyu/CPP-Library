#ifndef ASU_POINTONSEGMENT
#define ASU_POINTONSEGMENT

#include<cmath>

#include<CrossProduct.hpp>

/**************************************************************
 * This C++ template return whether given point is on a given
 * straight line segment. Due to rounding error of floating
 * number, the criteria hard-wired in the code is set to be 1e-7.
 *
 * input(s):
 * const pair<T1,T2> &p1  ----  {x1,y1} One end point of the line segment.
 * const pair<T3,T4> &p2  ----  {x2,y2} Another end point of the line segment.
 * const pair<T5,T6> &p   ----  Given point.
 *
 * return(s):
 * bool  ans  ----  true means the point is on the line segment.
 *
 * Shule Yu
 * Feb 02 2018
 *
 * Key words: point on line segment.
****************************************************************/

template<class T1,class T2,class T3,class T4,class T5,class T6>
bool PointOnSegment(const std::pair<T1,T2> &p1,const std::pair<T3,T4> &p2,const std::pair<T5,T6> &p){

    double dx1=p1.first-p.first,dy1=p1.second-p.second,dx2=p2.first-p.first,dy2=p2.second-p.second;
    double EPS=fabs(CrossProduct(dx1,dy1,0,dx2,dy2,0).back());
    if (EPS!=0) EPS/=(sqrt(dx1*dx1+dy1*dy1)*sqrt(dx2*dx2+dy2*dy2));

    return (std::min(p1.first,p2.first)<=p.first && p.first<=std::max(p1.first,p2.first) &&
            std::min(p1.second,p2.second)<=p.second && p.second<=std::max(p1.second,p2.second) && EPS<1e-7);
}

#endif
