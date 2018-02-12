#ifndef ASU_COMBINEPOLYGONS
#define ASU_COMBINEPOLYGONS

#include<vector>

#include<CombineTwoPolygons.hpp>

/*****************************************************************
 * This C++ template use "CombineTwoPolygons" to combine a set of
 * polygons into another set of non-overlapping polygons (a union set).
 *
 * input(s):
 * const vector<vector<pair<T1,T2>>> &P  ----  Input polygons.
 *
 * Each polygon is denoted by a vecotr of pairs of coordinates (x,y).
 *
 * Return(s):
 * vector<vector<pair<double,double>>> ans  ----  Combined polygons.
 *
 * Shule Yu
 * Jan 19 2018
 *
 * Known bug: if polygon formed a circle (see Example/data/CombinePolygons.txt),
 *            the center part is included in the final result.
 *
 * Key words: polygon, union set.
*****************************************************************/

template<class T1, class T2>
std::vector<std::vector<std::pair<double,double>>> CombinePolygons(const std::vector<std::vector<std::pair<T1,T2>>> &P){

    int NP=P.size();

    std::vector<bool> V(NP,false);
    std::vector<std::vector<std::pair<double,double>>> ans;

    for (int i=0;i<NP;++i){

        if (V[i]) continue;

        std::vector<std::pair<double,double>> CurP=P[i];
        V[i]=true;

        bool flag=true;
        while (flag){                // Stop only when the current polygon is seperated from every remaining polygons.
            flag=false;
            for (int j=0;j<NP;j++){
                if (V[j]) continue;  // Only look at remaining polygons.
                auto res=CombineTwoPolygons(CurP,P[j]);
                if (res.first) {
                    V[j]=flag=true;  // If current polygon will absorb any remaining polygons, re-do examing all the remaining polygons.
                    CurP=res.second;
                }
            }
        }

        ans.push_back(CurP);
    }
    return ans;
}

#endif
