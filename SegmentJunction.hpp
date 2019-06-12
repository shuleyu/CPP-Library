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

template<typename T1,typename T2,typename T3,typename T4,
         typename T5,typename T6,typename T7,typename T8>
std::pair<bool,std::pair<double,double>>
SegmentJunction(const std::pair<T1,T2> &p1, const std::pair<T3,T4> &q1,
                const std::pair<T5,T6> &p2, const std::pair<T7,T8> &q2, const bool &EndPoint=true){
    /*
         q2
         
     p1      q1
         
         p2

    */

    // which side is p1, relative to p2q2
    // p2q2 x p2p1
    double res1=(q2.first-p2.first)*(p1.second-p2.second)-(q2.second-p2.second)*(p1.first-p2.first);

    // which side is q1, relative to p2q2
    // p2q2 x p2q1
    double res2=(q2.first-p2.first)*(q1.second-p2.second)-(q2.second-p2.second)*(q1.first-p2.first);

    // which side is p2, relative to p1q1
    // p1q1 x p1p2
    double res3=(q1.first-p1.first)*(p2.second-p1.second)-(q1.second-p1.second)*(p2.first-p1.first);

    // which side is q2, relative to p1q1
    // p1q1 x p1q2
    double res4=(q1.first-p1.first)*(q2.second-p1.second)-(q1.second-p1.second)*(q2.first-p1.first);

    // Check whether p1q1 and p2q2 straddles each other.
    double minX1=p1.first,maxX1=q1.first,minY1=p1.second,maxY1=q1.second;
    if (minX1>maxX1) std::swap(minX1,maxX1);
    if (minY1>maxY1) std::swap(minY1,maxY1);
    double minX2=p2.first,maxX2=q2.first,minY2=p2.second,maxY2=q2.second;
    if (minX2>maxX2) std::swap(minX2,maxX2);
    if (minY2>maxY2) std::swap(minY2,maxY2);

    if (((res1>0 && res2<0) || (res1<0 && res2>0)) && ((res3>0 && res4<0) || (res3<0 && res4>0))) ;
    else if (res1==0 && minX2<=p1.first && p1.first<=maxX2 && minY2<=p1.second && p1.second<=maxY2) return {true,p1};
    else if (res2==0 && minX2<=q1.first && q1.first<=maxX2 && minY2<=q1.second && q1.second<=maxY2) return {true,q1};
    else if (res3==0 && minX1<=p2.first && p2.first<=maxX1 && minY1<=p2.second && p2.second<=maxY1) return {true,p2};
    else if (res4==0 && minX1<=q2.first && q2.first<=maxX1 && minY1<=q2.second && q2.second<=maxY1) return {true,q2};
    else return {false,{}};

    // Find the junction.
    auto res=LineJunction(p1,(p1.second-q1.second)/(p1.first-q1.first),p2,(p2.second-q2.second)/(p2.first-q2.first));
    return {true,{res.second}};
}

#endif
