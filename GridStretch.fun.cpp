#include<iostream>
#include<vector>
#include<algorithm>
#include<tuple>
#include<cmath>

#include<ASU_tools.hpp>

using namespace std;

/**************************************************************
 * This C function return the after-restoration position of a 
 * point inside the square.
 *
 * vector<pair<pair<double,double>,pair<double,double>>> Polygon  
 *                             ----  given stretched square and its original coordinates(grids).
 *
 * 	                           Polygon.size() should = 4, indicating
 * 	                           the four corners of the stretched/original square.
 * 	                           Polygon[0].first has the coordinates of the stretched
 * 	                           four corners; Polygon[0].second has the coordinates
 * 	                           of the original grid four corners.
 *                             The four corners can be unordered.
 *
 * vector<pair<double,double>> Point    ----  given points position.
 *                                            in the stretched coordinate.
 *
 * return value:
 * pair<double,double>         ans  ----  The position of the point inside
 *                                        the original gird square.
 *
 * Shule Yu
 * Mar 30 2017
 *
 * Key words: stretching.
****************************************************************/

double DotDist(double x1,double y1,double x2,double y2){
	return sqrt(pow((x1-x2),2)+pow((y1-y2),2));
}

pair<double,double> GridStretch(vector<pair<pair<double,double>,pair<double,double>>> &Polygon,pair<double,double> P){


	pair<double,double> ans;

	// Check polygon has four corners.
	if (Polygon.size()!=4){
		cout << "Error in " <<  __func__ << " : Polygon.size() != 4 ..." << endl;
		return ans;
	}

	// Check if it's a convex polygon.
	for (int i=0;i<4;i++){
		vector<pair<double,double>> points,polygon;
		points.push_back(Polygon[i].first);
		for (int j=0;j<4;j++) if (j!=i) polygon.push_back(Polygon[j].first);
		auto x=PointsInPolygon(polygon,points);
		if (x[0]) {
			cout << "Error in " <<  __func__ << " : Polygon isn't convex ..." << endl;
			return ans;
		}
	}


	// Reorder the four corners.
	// start from upper left [0] in counter-close wise order.
	auto f1=[](pair<pair<double,double>,pair<double,double>> &p1,pair<pair<double,double>,pair<double,double>> &p2){
		return p1.first.second>p2.first.second;
	};
	auto f2=[](pair<pair<double,double>,pair<double,double>> &p1,pair<pair<double,double>,pair<double,double>> &p2){
		return p1.first.first<p2.first.first;
	};
	sort(Polygon.begin(),Polygon.end(),f1);
	sort(Polygon.begin(),Polygon.begin()+2,f2);
	sort(Polygon.begin()+2,Polygon.end(),f2);
	swap(Polygon[1],Polygon[2]);
	swap(Polygon[2],Polygon[3]);

	// Check whether the point is outside the polygon.
	vector<pair<double,double>> points,polygon;
	polygon.push_back(Polygon[0].first);
	polygon.push_back(Polygon[1].first);
	polygon.push_back(Polygon[2].first);
	polygon.push_back(Polygon[3].first);
	points.push_back(P);
	auto xx=PointsInPolygon(polygon,points);
	if (!xx[0]) {
		cout << "Error in " <<  __func__ << " : Point is outside of polygon ..." << endl;
		return ans;
	}

	// Move the reference point to Polygon[1].first;
	for (int i=0;i<4;i++){
		if (i!=1){
			Polygon[i].first.first-=Polygon[1].first.first;
			Polygon[i].first.second-=Polygon[1].first.second;
		}
	}
	P.first-=Polygon[1].first.first;
	P.second-=Polygon[1].first.second;
	Polygon[1].first.first=Polygon[1].first.second=0;


	// Rotate Polygon such that Polygon[1] and Polygon[2] becomes horizontal to x-axis.
	double theta=atan2(-1.0*Polygon[2].first.second,Polygon[2].first.first);
	for (int i=0;i<4;i++){
		if (i!=1){
			double x=Polygon[i].first.first,y=Polygon[i].first.second;
			Polygon[i].first.first=x*cos(theta)-y*sin(theta);
			Polygon[i].first.second=x*sin(theta)+y*cos(theta);
		}
	}

	// Make a mirrorred image if needed.
	bool flipit=false;
	double s_top=(Polygon[3].first.second-Polygon[0].first.second)/(Polygon[3].first.first-Polygon[0].first.first);
	if (s_top<0){
		flipit=true;

		for (int i=0;i<4;i++){
			if (i!=2){
				Polygon[i].first.first=Polygon[2].first.first-Polygon[i].first.first;
			}
		}
		P.first=Polygon[2].first.first-P.first;
		Polygon[2].first.first=0;
		s_top*=-1;
		reverse(Polygon.begin(),Polygon.end());
	}
// cout << Polygon[1].first.first << " " << Polygon[1].first.second << " " << P.first << " " << P.second << endl;

	// Stretch the upper edge to meet the parallel line right foot .

	double xleft,yleft,xbot,ybot,xright,yright;
	double s_left=(Polygon[0].first.second-Polygon[1].first.second)/(Polygon[0].first.first-Polygon[1].first.first);
	double s_right=(Polygon[3].first.second-Polygon[2].first.second)/(Polygon[3].first.first-Polygon[2].first.first);
	tie(xleft,yleft)=StraightLineJunction(Polygon[1].first.first,Polygon[1].first.second,s_left,
										  P.first,P.second,s_top);
	tie(xbot,ybot)=StraightLineJunction(Polygon[1].first.first,Polygon[1].first.second,0,
										  P.first,P.second,s_top);
	tie(xright,yright)=StraightLineJunction(Polygon[2].first.first,Polygon[2].first.second,s_right,
										  P.first,P.second,s_top);
	
// cout << xleft << " " << yleft << endl;
// cout << xbot << " " << ybot << endl;

	double lr=DotDist(P.first,P.second,xright,yright);
	double ll=min(DotDist(P.first,P.second,xleft,yleft),DotDist(P.first,P.second,xbot,ybot));
	double ratio1=ll/(lr+ll);

	double px,py;
	if (DotDist(P.first,P.second,xleft,yleft)<DotDist(P.first,P.second,xbot,ybot)){
		px=xleft;
		py=yleft;
	}
	else{
		px=xbot;
		py=ybot;
	}
// cout << px << " " << py << endl;

	double xrr,yrr;
	tie(xrr,yrr)=StraightLineJunction(Polygon[2].first.first,Polygon[2].first.second,s_left,
									  P.first,P.second,s_top);

// cout << xrr << " " << yrr << endl;

	double NewPx,NewPy;
	NewPx=px+(xrr-px)*ratio1;
	NewPy=py+(yrr-py)*ratio1;
// cout << NewPy << " " << py << " " << yrr << " " << ratio1 << endl;

	double fxr,fyr,fxl,fyl,fxt,fyt,fxb,fyb;
	tie(fxr,fyr)=StraightLineJunction(Polygon[2].first.first,Polygon[2].first.second,s_left,
									  NewPx,NewPy,0);
	tie(fxl,fyl)=StraightLineJunction(Polygon[1].first.first,Polygon[1].first.second,s_left,
									  NewPx,NewPy,0);
	tie(fxt,fyt)=StraightLineJunction(Polygon[0].first.first,Polygon[0].first.second,s_top,
									  NewPx,NewPy,s_left);
	tie(fxb,fyb)=StraightLineJunction(Polygon[1].first.first,Polygon[1].first.second,0,
									  NewPx,NewPy,s_left);

// cout << fxl << " " << fxr <<" " << NewPx << " " << NewPy << endl;
// cout << fxt << " " << fxb <<" " << NewPx << " " << NewPy << endl;
	ans.first=(NewPx-fxl)/(fxr-fxl);
	ans.second=(NewPy-fyb)/(fyt-fyb);

	if (flipit){
		ans.first=1-ans.first;
		reverse(Polygon.begin(),Polygon.end());
	}

	ans.first=Polygon[1].second.first+(Polygon[3].second.first-Polygon[1].second.first)*ans.first;
	ans.second=Polygon[1].second.second+(Polygon[3].second.second-Polygon[1].second.second)*ans.second;
	return ans;
}

