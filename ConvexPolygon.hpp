#ifndef ASU_CONVEXPOLYGON
#define ASU_CONVEXPOLYGON

#include<iostream>
#include<vector>

#include<PointInPolygon.hpp>

/****************************************************************
 * This C++ template judge whether the input polygon is convex.
 * This is a very naive algorithm, which can't deal with complex
 * polygons.
 *
 *
 * const vector<pair<T1,T2>> &p  ----  Given polygon.
 *
 * return:
 * bool ans  ----  true if it is a convex polygon.
 *
 * Shule Yu
 * Feb 02 2018
 *
 * Key words: convex polygon, incomplete.
****************************************************************/

template<class T1,class T2>
bool ConvexPolygon(const std::vector<std::pair<T1,T2>> &p){

    if (p.size()<=2) {
        std::cerr <<  __func__ << "; Error: input polygon is array size is zero ..." << std::endl;
        return false;
    }

	for (size_t i=0;i<p.size();++i){
		std::vector<std::pair<double,double>> polygon;
		for (size_t j=0;j<p.size();++j)
            if (j!=i) polygon.push_back(p[j]);
		if (PointInPolygon(polygon,p[i],-1)) return false;
	}

    return true;
}

#endif
