#ifndef ASU_POINTSINPOLYGON
#define ASU_POINTSINPOLYGON

#include<iostream>
#include<vector>

#include<CrossProduct.hpp>
#include<PointsOnLine.hpp>

/**************************************************************
 * This C++ template decide weather given points on (x-y) plane
 * located inside one given polygon using the Winding Number
 * method.
 *
 * input(s):
 * const vector<pair<double,double>> &Polygon         ----  given polygon.
 * const vector<pair<double,double>> &Points          ----  given points.
 * bool                              IncludeBoundary  ----  (optional)
 *                                       true : points on all boundaries are counted as inside.
 *                                       false: default value:
 *                                              left and bottom edges are decided to be inside.
 *                                              right and top edges are decided to be outside.
 * return(s):
 * vector<bool>                      ans       ----  1*Points.size().
 *
 * Notice: We define the inside of the polygon as the left hand
 *	side area as one walks on its edge in the order of input.
 *
 * Shule Yu
 * Jan 18 2018
 *
 * Key words: points inside polygon, winding number.
 * Reference: http://geomalgorithms.com/a03-_inclusion.html
****************************************************************/

template<class T>
std::vector<bool> PointsInPolygon(const std::vector<std::pair<T,T>> &Polygon,const std::vector<std::pair<T,T>> &Points,bool IncludeBoundary=false){

	int m=Polygon.size(),n=Points.size();
	std::vector<bool> ans;

	for (int i=0;i<n;i++){

		int WN=0;
        double px=Points[i].first,py=Points[i].second;

		for (int j=0;j<m;j++){

			double ex1,ey1,ex2,ey2;

			ex1=Polygon[j].first;
			ey1=Polygon[j].second;
			ex2=Polygon[(j+1)%m].first;
			ey2=Polygon[(j+1)%m].second;

            if (IncludeBoundary && PointsOnLine(std::make_pair(ex1,ey1),std::make_pair(ex2,ey2),
                                                std::vector<std::pair<double,double>> {{px,py}})[0]) {
                WN=1;
                break;
            }

			if (ey1<=py && py<ey2 && CrossProduct(ex1-px,ey1-py,0,ex2-px,ey2-py,0).back()>0) ++WN;
			else if (ey2<=py && py<ey1 && CrossProduct(ex1-px,ey1-py,0,ex2-px,ey2-py,0).back()<0) --WN;
		}

		ans.push_back(WN!=0);
	}

	return ans;
}

#endif
