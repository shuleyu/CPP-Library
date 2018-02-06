#ifndef ASU_COMBINETWOPOLYGONS
#define ASU_COMBINETWOPOLYGONS

#include<iostream>
#include<vector>
#include<cmath>
#include<map>
#include<queue>
#include<algorithm>

#include<DotDist.hpp>
#include<PointInPolygon.hpp>
#include<SegmentJunction.hpp>
#include<VectorAngle.hpp>

/*****************************************************************
 * This C++ template combine 2 polygons (Compute the union sets of
 * given two polygons).
 *
 * const vector<vector<pair<T1,T2>>> &p1  ----  Input polygon 1.
 * const vector<vector<pair<T3,T4>>> &p2  ----  Input polygon 2.
 *
 * Each polygon is denoted by a series of waypoints of pairs of
 * coordinates (x,y).
 *
 * return:
 * If the input two polygons are completely separated:
 *     ans.first=false, ans.second={};
 * Otherwise
 *     ans.first=true, ans.second=one combined polygon.
 *
 * Shule Yu
 * Jan 19 2018
 *
 * Key words: polygon, union set, gift wrapping.
*****************************************************************/

template<class T1, class T2, class T3, class T4>
std::pair<bool,std::vector<std::pair<double,double>>> CombineTwoPolygons(const std::vector<std::pair<T1,T2>> &p1,
                                                                         const std::vector<std::pair<T3,T4>> &p2){

    int m=p1.size(),n=p2.size();

    // Vertex array.
    std::vector<std::pair<double,double>> V;
    for (int i=0;i<m;++i) V.push_back(p1[i]);
    for (int i=0;i<n;++i) V.push_back(p2[i]);

    // 1. Finding new vertices.
    // M[i] store new vertices on edge i ~ i+1 (sorted according to its distance to vetex V[i])
    std::map<int,std::priority_queue<std::pair<double,int>,std::vector<std::pair<double,int>>,std::greater<std::pair<double,int>>>> M;

    for (int i=0;i<m;++i){

        for (int j=m;j<m+n;++j){

            // Check whether there's a new vertex created by segments V[i]---V[i+1] and V[j]---V[j+1]

            auto res=SegmentJunction(V[i],V[(i+1)%m],V[j],V[m+(j-m+1)%n],false);

            if (res.first) {
                int N=V.size();
                M[i].push(std::make_pair(DotDist(V[i].first,V[i].second,0,res.second.first,res.second.second,0),N));
                M[j].push(std::make_pair(DotDist(V[j].first,V[j].second,0,res.second.first,res.second.second,0),N));
                V.push_back(res.second);
            }
        }
    }

    // Judge whether the two polygons are seperated.
    int N=V.size();
    if (N==m+n) {
        int mm=0,nn=0;
        for (int i=0;i<m;++i) mm+=(PointInPolygon(p2,p1[i],1)?1:0);
        for (int i=0;i<n;++i) nn+=(PointInPolygon(p1,p2[i],1)?1:0);
        if (m==mm) return {true,p2};
        if (n==nn) return {true,p1};
        return {false,{}};
    }

    // 2. Create edges of the new graph.

    std::vector<std::vector<int>> E(N,std::vector<int> ());
    for (int i=0;i<m+n;++i) {

        // Find the index for i+1.
        int Next;
        if (i<m) Next=(i+1)%m;
        else Next=m+(i+1-m)%n;

        int Cur=i;
        while (!M[i].empty()){
            E[Cur].push_back(M[i].top().second);
            E[M[i].top().second].push_back(Cur);
            Cur=M[i].top().second;
            M[i].pop();
        }
        E[Cur].push_back(Next);
        E[Next].push_back(Cur);
    }


    // 3. Gift wrapping to find the new edges.

    // a. Find the left most point.
    auto it=min_element(V.begin(),V.end());
    int Cur=distance(V.begin(),it),Begin=Cur,Prev=-1;

    std::vector<std::pair<double,double>> ans;

    // b. Start gift-wrapping.
    std::vector<bool> F(N,false);
    std::pair<double,double> v1{0,-1},v2,v3;

    do {
        F[Cur]=true;
        ans.push_back(V[Cur]);
        int Next;
        double MinAngle=3*M_PI,Angle;
        for (auto &item:E[Cur]) {
            if (item==Prev || (F[item] && item!=Begin)) continue;
            v2={V[item].first-V[Cur].first,V[item].second-V[Cur].second};
            Angle=VectorAngle(v1,v2);
            if (Angle<=MinAngle) {
                MinAngle=Angle;
                Next=item;
                v3.first=-v2.first;
                v3.second=-v2.second;
            }
        }
        Prev=Cur;
        Cur=Next;
        v1=v3;
    } while(Cur!=Begin);

	return {true,ans};
}

#endif
