#ifndef ASU_SEGMENTJUNCTION
#define ASU_SEGMENTJUNCTION

#include<PointOnSegment.hpp>
#include<LineJunction.hpp>

/**************************************************************************************************
 * This C++ template return the junction point of two line segments.
 *
 * input(s):
 * const pair<T1,T2> &p1        ----  First end point of the first segment.
 * const pair<T3,T4> &q1        ----  Second end point of the first segment.
 * const pair<T5,T6> &p2        ----  First end point of the second segment.
 * const pair<T7,T8> &q2        ----  Second end point of the second segment.
 * const bool        &EndPoint  ----  (Optional) End points counted as a junction? Default is true.
 *                                      true : return true for junction happens at any end ponts.
 *                                      false: return false for junction happens at any end ponts.
 *
 * return(s):
 * pair<bool,pair<double,double>>  ans  ----  {flag, position of the junction point}
 *                                            If no junction point, flag is set to false.
 *
 * Shule Yu
 * Jan 19 2018
 *
 * Key words: segment junction, intercept, cross.
**************************************************************************************************/

template<class T1,class T2,class T3,class T4,class T5,class T6,class T7,class T8>
std::pair<bool,std::pair<double,double>> SegmentJunction(const std::pair<T1,T2> &p1,const std::pair<T3,T4> &q1,
                                                         const std::pair<T5,T6> &p2,const std::pair<T7,T8> &q2,
                                                         const bool &EndPoint=true){

    // Check end points.
    if (PointOnSegment(p1,q1,p2)) return {EndPoint,p2};
    if (PointOnSegment(p1,q1,q2)) return {EndPoint,q2};
    if (PointOnSegment(p2,q2,p1)) return {EndPoint,p1};
    if (PointOnSegment(p2,q2,q1)) return {EndPoint,q1};

    auto res=LineJunction(p1,(p1.second-q1.second)/(p1.first-q1.first),p2,(p2.second-q2.second)/(p2.first-q2.first));
    if (res.first && PointOnSegment(p1,q1,res.second) && PointOnSegment(p2,q2,res.second)) return {true,res.second};
    return {false,{}};
}

#endif
