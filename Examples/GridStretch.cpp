#include<iostream>
#include<fstream>
#include<vector>

#include<ASU_tools.hpp>

using namespace std;

int main(){

	vector<pair<double,double>> G;
	G.push_back({1,0});
	G.push_back({0,0});
	G.push_back({1,2});
	G.push_back({0,1});

    // Use function.
	auto ans=GridStretch(G,make_pair(0.5,1.5));
	cout << ans.first << " " << ans.second << endl;

    return 0;    
}
