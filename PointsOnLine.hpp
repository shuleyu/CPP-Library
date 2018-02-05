#ifndef ASU_POINTSONLINE
#define ASU_POINTSONLINE

#include<iostream>
#include<vector>

#include<CrossProduct.hpp>

/**************************************************************
 * This C++ template return whether given points are on a give
 * straight line segment.
 *
 * const pair<T1,T1> &x          ----  One end point of the line segment.
 * const pair<T2,T2> &y          ----  Another end point of the line segment.
 * const vector<pair<T3,T3>> &p  ----  Given points.
 *
 * return value:
 * vector<bool>  ans  ----  true means the point is on the line segment.
 *
 * Shule Yu
 * Feb 02 2018
 *
 * Key words: points on line segment.
****************************************************************/

template<class T1,class T2,class T3>
std::vector<bool> PointsOnLine(const std::pair<T1,T1> &x,const std::pair<T2,T2> &y,
                               const std::vector<std::pair<T3,T3>> &p){

    int n=p.size();
    std::vector<bool> ans;

    if (p.empty()) return ans;

    for (int i=0;i<n;++i) {
        ans.push_back(std::min(x.first,y.first)<=p[i].first && p[i].first<=std::max(x.first,y.first) &&
                      std::min(x.second,y.second)<=p[i].second && p[i].second<=std::max(x.second,y.second) &&
                      CrossProduct(x.first-p[i].first,y.first-p[i].first,0,x.second-p[i].second,y.second-p[i].second,0).back()==0);
    }

	return ans;
}

#endif
