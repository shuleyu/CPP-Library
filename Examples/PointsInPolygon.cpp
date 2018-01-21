#include<iostream>
#include<fstream>
#include<vector>

#include<ASU_tools.hpp>

using namespace std;

int main(){


	// Read in.
	vector<pair<double,double>> Points,Polygon;
	pair<double,double> p;

	ifstream fpin("data/PointsInPolygon_polygon");
	while(fpin >> p.first >> p.second) Polygon.push_back(p);
    fpin.close();

	fpin.open("data/PointsInPolygon_points");
	while(fpin >> p.first >> p.second) Points.push_back(p);
    fpin.close();

    // Use function.
	auto res=PointsInPolygon(Polygon,Points);

	ofstream fpout("data/PointsInPolygon_result");
	for (size_t i=0;i<Points.size();++i){
		fpout << (res[i]?"*":"o");
        if (i%51==50) fpout << '\n';
    }
    fpout.close();
    
    return 0;    
}
