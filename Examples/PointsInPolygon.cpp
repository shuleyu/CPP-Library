#include<iostream>
#include<fstream>
#include<vector>

#include<ASU_tools.hpp>

using namespace std;

int main(){

	vector<pair<double,double>> Points,Polygon;
	pair<double,double> p;

	ifstream fpin("data/PointsInPolygon_polygon");
	while(fpin >> p.first >> p.second) Polygon.push_back(p);
    fpin.close();

	fpin.open("data/PointsInPolygon_points");
	while(fpin >> p.first >> p.second) Points.push_back(p);
    fpin.close();

    // Use function.
	auto res=PointsInPolygon(Polygon,Points,true);

	ofstream fpout("data/PointsInPolygon_result");
	for (size_t i=0;i<Points.size();++i){
		fpout << (res[i]?"*":"o");
        if (i%51==50) fpout << '\n';
    }
    fpout.close();

    // By default, left and upper boundaries are excluded.
	res=PointsInPolygon(Polygon,Points);

	fpout.open("data/PointsInPolygon_result2");
	for (size_t i=0;i<Points.size();++i){
		fpout << (res[i]?"*":"o");
        if (i%51==50) fpout << '\n';
    }
    fpout.close();

    Points={{1,1}};
    Polygon={{0,0},{0,2}};
	res=PointsInPolygon(Polygon,Points);
    for (size_t i=0;i<Points.size();++i)
        cout << Points[i].first << " " <<  Points[i].second << (res[i]?" In":" Out") << endl;

    return 0;    
}
