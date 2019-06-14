#ifndef ASU_CONVEXPOLYGON
#define ASU_CONVEXPOLYGON

#include<iostream>
#include<vector>

#include<PointInPolygon.hpp>

/****************************************************************
 * This C++ template judge whether the input polygon is convex.
 *
 * input(s):
 * const vector<pair<T1,T2>> &p  ----  Given polygon.
 * const bool                &mode  ----  (Optional, default is true)
 *                                        true: easy mode, assuming input polygon is simple:
 *                                              that is, the sides do not intersect.
 *                                              Time complexity: O(n).
 *                                        false: hard mode, not guarantee correct.
 *                                               do a convex wrapping, then decide if all points
 *                                               are on the wrapping, and the points are not on a single line. O(nlogn)
 *
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
        std::cerr <<  "Error in " << __func__ << ": input polygon's array size is zero ..." << std::endl;
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
