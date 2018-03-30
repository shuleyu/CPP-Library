#include<iostream>
#include<fstream>

#include<RemoveTrend.hpp>

using namespace std;

int main(){

    vector<double> p;
    double a,b;
    ifstream fpin("data/RemoveTrend_in");
    while (fpin >> a >> b) p.push_back(b);

	auto ans=RemoveTrend(p,0.01,0);

    cout << ans.first << " " << ans.second << endl;
	
	return 0;
}

