#include<iostream>
#include<fstream>
#include<vector>

#include<ASU_tools.hpp>

using namespace std;

int main(){

	vector<pair<pair<double,double>,pair<double,double>>> G1;
	G1.push_back({{1,0},{1,0}});
	G1.push_back({{0,0},{0,0}});
	G1.push_back({{0,1},{0,1}});
	G1.push_back({{1,2},{1,1}});

	vector<pair<pair<double,double>,pair<double,double>>> G2;
	G2.push_back({{1,0},{1,0}});
	G2.push_back({{0,0},{0,0}});
	G2.push_back({{0,1},{0,1}});
	G2.push_back({{1,1},{1,2}});

    // Use function.
	auto ans=GridStretch(G1,make_pair(0.5,0.75));
	cout << ans.first << " " << ans.second << endl;

	ans=GridStretch(G2,make_pair(0.5,0.5));
	cout << ans.first << " " << ans.second << endl;

    return 0;    
}
