// In development
#ifndef ASU_COMBINEPOLYGONS
#define ASU_COMBINEPOLYGONS

#include<iostream>
#include<vector>
#include<algorithm>
#include<numeric>
#include<cmath>

#include<CombineTwoPolygons.hpp>

/*****************************************************************
 * This C++ function use "CombineTwoPolygons" from ASU_tools
 * to combine a set of polygons into another set of polygons.(The
 * union sets of given polygons)
 *
 * const vector<vector<pair<double,double>>> &P   ----   Input polygons.
 *
 * Each polygon is denoted by a vecotr of pairs of coordinates (x,y).
 *
 * Return:
 * vector<vector<pair<double,double>>> Ans  ----   Output polygons.
 *
 * Shule Yu
 * Jan 19 2018
 *
 * Key words: polygon, union set.
*****************************************************************/

template<class T>
std::vector<std::vector<std::pair<T,T>>> CombinePolygons(const std::vector<std::vector<std::pair<T,T>>> &P){

	int NP=P.size();

	std::vector<bool> V(NP,false);
	std::vector<std::vector<std::pair<T,T>>> ans;

	for (int i=0;i<NP;i++){

		if (V[i]) continue;

		std::vector<std::pair<T,T>> CurP=P[i];
		V[i]=true;

        bool flag=true;
		while (flag){                // Stop only when the current polygon is seperated from every remaining polygons.
			flag=false;
			for (int j=0;j<NP;j++){
				if (V[j]) continue;  // Only look at remaining polygons.
				V[j]=CombineTwoPolygons(CurP,P[j]);
				if (V[j]) flag=true; // If current polygon will absorb any remaining polygons, keep looking.
			}
		}

		ans.push_back(CurP);
	}
    return ans;
}

#endif
