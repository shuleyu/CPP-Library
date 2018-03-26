#ifndef ASU_POINTONSEGMENT
#define ASU_POINTONSEGMENT

#include<cmath>

/**************************************************************
 * This C++ template return whether given point is on a given
 * straight line segment. Due to rounding error of floating
 * number, the criteria is hard-wired that if the angle between
 * the point and two end point is larger than 179.99 deg)
 * [ cos (179.99 deg) = -0.999999984769129 ], the
 * point will be deemed as on the segment.
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
    double EPS=(dx1*dx2+dy1*dy2)/(sqrt(dx1*dx1+dy1*dy1)*sqrt(dx2*dx2+dy2*dy2));
    return (EPS<-0.999999984769129 &&
            ( (std::min(p1.first,p2.first)<=p.first && p.first<=std::max(p1.first,p2.first)) ||
              (std::min(p1.second,p2.second)<=p.second && p.second<=std::max(p1.second,p2.second)) )
           );
}

#endif
