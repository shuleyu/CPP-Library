#include<iostream>
#include<vector>

#include<ASU_tools.hpp>

using namespace std;

/**************************************************************
 * This C function decide weather given points on 2D plane
 * located inside one give 2D polygon using Winding Number
 * method described in the reference link. (Similar to that in c
 * functionn library)
 *
 * vector<pair<double,double>> Polygon  ----  given polygon.
 * vector<pair<double,double>> Points   ----  given points.
 *
 * return value:
 * vector<bool>                Inside   ----  result.
 *
 *	Notice: We define the inside of the polygon
 *	as the left hand side area as one
 *	walks on its edge in the given array.
 *	Points located on the left and bottom edges are decided to be inside.
 *	Points located on the right and top edges are decided to be outside.
 *
 * Shule Yu
 * Mar 14 2017
 *
 * Key words: points inside polygon, winding number.
 * Reference: http://geomalgorithms.com/a03-_inclusion.html
****************************************************************/

vector<bool> PointsInPolygon(const vector<pair<double,double>> &Polygon,const vector<pair<double,double>> &Points){

	int m=Polygon.size(),n=Points.size();
	vector<bool> Inside(n);

	int Cnt,Cnt2;

	for (Cnt=0;Cnt<n;Cnt++){

		int WN=0;

		for (Cnt2=0;Cnt2<m;Cnt2++){

			double px=Points[Cnt].first,py=Points[Cnt].second;
			double ex1,ey1,ex2,ey2;

			ex1=Polygon[Cnt2].first;
			ey1=Polygon[Cnt2].second;
			ex2=Polygon[(Cnt2+1)%m].first;
			ey2=Polygon[(Cnt2+1)%m].second;

			if (ey1<=py){
				if (py<ey2 && CrossProduct(ex1-px,ey1-py,0,ex2-px,ey2-py,0).back()>0){
					WN++;
				}
			}
			else{
				if (ey2<=py && CrossProduct(ex1-px,ey1-py,0,ex2-px,ey2-py,0).back()<0){
					WN--;
				}
			}
		}

		Inside[Cnt]=(WN!=0);
	}

	return Inside;
}
