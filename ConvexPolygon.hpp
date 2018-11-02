#ifndef ASU_CONVEXPOLYGON
#define ASU_CONVEXPOLYGON

#include<iostream>
#include<vector>

#include<PointInPolygon.hpp>

/****************************************************************
 * This C++ template judge whether the input polygon is convex.
 * This is a very naive algorithm, and can't deal with complex
 * polygons.
 *
 * input(s):
 * const vector<pair<T1,T2>> &p  ----  Given polygon.
 *
 * return(s):
 * bool ans  ----  true if it is a convex polygon.
 *
 * Shule Yu
 * Feb 02 2018
 *
 * Key words: convex polygon, incomplete.
****************************************************************/

template<typename T1,typename T2>
bool ConvexPolygon(const std::vector<std::pair<T1,T2>> &p){

    if (p.size()<=2) {
        std::cerr <<  "Error in " << __func__ << ": input polygon is array size is zero ..." << std::endl;
        return false;
    }

    for (std::size_t i=0;i<p.size();++i){
        std::vector<std::pair<double,double>> polygon;
        for (std::size_t j=0;j<p.size();++j)
            if (j!=i) polygon.push_back(p[j]);
        if (PointInPolygon(polygon,p[i],-1)) return false;
    }

    return true;
}

#endif
