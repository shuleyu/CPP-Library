#ifndef ASU_POINTONSEGMENT
#define ASU_POINTONSEGMENT

#include<cmath>

/**************************************************************
 * This C++ template return whether given point is on a given
 * line segment.
 *
 * input(s):
 * const pair<T1,T2> &p1   ----  {x1,y1} One end point of the line segment.
 * const pair<T3,T4> &p2   ----  {x2,y2} Another end point of the line segment.
 * const pair<T5,T6> &p    ----  Given point.
 *
 * return(s):
 * bool  ans  ----  true means the point is on the line segment.
 *
 * Shule Yu
 * Feb 02 2018
 *
 * Key words: point on line segment.
****************************************************************/

template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
bool PointOnSegment(const std::pair<T1,T2> &p1,const std::pair<T3,T4> &p2,const std::pair<T5,T6> &p){


    // First check if p lies on the straight line of p1p2.
    // p1p x p1p2
    double res1=(p.first-p1.first)*(p2.second-p1.second)-(p.second-p1.second)*(p2.first-p1.first);
    if (res1!=0) return false;

    double x1=p1.first,x2=p2.first;
    if (x1>x2) std::swap(x1,x2);

    double y1=p1.second,y2=p2.second;
    if (y1>y2) std::swap(y1,y2);

    return (x1<=p.first && p.first<=x2 && y1<=p.second && p.second<=y2);
}

#endif
