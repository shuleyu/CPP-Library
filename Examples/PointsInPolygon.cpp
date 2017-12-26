#include<iostream>
#include<fstream>
#include<vector>

#include<ASU_tools.hpp>

using namespace std;

int main(){

	ifstream fpin_polygon("data/PointsInPolygon_polygon");
	ifstream fpin_points("data/PointsInPolygon_points");

	// Read in.
	
	vector<pair<double,double>> Points,Polygon;
	pair<double,double> p;
	while(fpin_points >> p.first >> p.second) Points.push_back(p);
	while(fpin_polygon >> p.first >> p.second) Polygon.push_back(p);
	fpin_polygon.close();
	fpin_points.close();

    // Use function.
	auto Flag=PointsInPolygon(Polygon,Points);

	int n=Flag.size();
	for (int i=0;i<n;i++){
		cout << Points[i].first << " " << Points[i].second << " is " 
			 << (Flag[i]?"inside":"outside") << " polygon" << endl;
	}
    
    return 0;    
}
