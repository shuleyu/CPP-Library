#ifndef ASU_CONVEXWRAP
#define ASU_CONVEXWRAP

#include<iostream>
#include<iterator>
#include<vector>
#include<set>
#include<algorithm>

/*************************************************
 * This C++ template returns the convex wrapping
 * result of input points. (a series of coordinates)
 *
 * input(s):
 * const vector<vector<T>> &p  ----  Input array.
 *
 * output(s):
 * vector<double> ans  ----  ans is also a series of points
 *                      (ans is a subset of input p)
 *
 * Shule Yu
 * Mar 06 2019
 *
 * Key words: convex, wrapping
 *************************************************/

std::vector<std::vector<double>> ConvexWrap(const std::vector<std::vector<double>> &p) {


    if (p.size()<=2) return p;

    // check dimension.
    if (p[0].size()!=2)
        throw std::runtime_error("Currently this function only support 2D inputs.");
    for (auto &item:p) 
        if (item.size()!=p[0].size())
            throw std::runtime_error("Input dimension doesn't match!");

    std::vector<std::vector<double>> ans;

    std::set<std::pair<double,double>> S;
    for (const auto &item:p)
        S.insert(std::make_pair(item[0],item[1]));

    std::pair<double,double> cur{std::numeric_limits<double>::max(),std::numeric_limits<double>::max()};
    for (const auto &item:S)
        if ((cur.first>item.first) || (cur.first==item.first && cur.second>item.second))
            cur=item;
    S.erase(cur);
    ans.push_back({cur.first,cur.second});

    while (true) {

        std::pair<double,double> nextPoint{ans[0][0],ans[0][1]};
        if (ans.size()==1) nextPoint.second=std::numeric_limits<double>::max();

        double dl=std::numeric_limits<double>::max();
        bool flag=false;

        for (const auto &item:S) {

            double dx1=nextPoint.first-item.first,dy1=nextPoint.second-item.second;
            double dy2=cur.second-item.second,dx2=cur.first-item.first;
            double Sign=dx1*dy2-dx2*dy1;
            double curdl=sqrt(dy2*dy2+dx2*dx2);

            if (Sign>0 || (Sign==0 && curdl<dl)) {
                nextPoint=item;
                dl=curdl;
                flag=true;
            }
        }
        S.erase(nextPoint);

        if (!flag) break;
        else ans.push_back({nextPoint.first,nextPoint.second});
        cur=nextPoint;
    }
    return ans;
}
#endif
