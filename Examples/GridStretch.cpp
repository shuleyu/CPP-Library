#include<iostream>
#include<fstream>
#include<vector>
#include<ASU_tools.hpp>

using namespace std;

int main(){

	vector<pair<pair<double,double>,pair<double,double>>> Polygon;
	pair<double,double> Point{0.4,0.4};
	Polygon.push_back({{1,0},{1,0}});
	Polygon.push_back({{0,0},{0,0}});
	Polygon.push_back({{0,1},{0,1}});
	Polygon.push_back({{1,2},{1,1}});

    // Use function.
	auto ans=GridStretch(Polygon,Point);

	cout << ans.first << " " << ans.second << endl;

    return 0;    
}
