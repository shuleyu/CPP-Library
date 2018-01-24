#include<iostream>
#include<fstream>

#include<ASU_tools.hpp>

using namespace std;

int main(){

// 	vector<pair<double,double>> Data{
// 		{1,2},
// 		{3,6},
// 		{8,16},
// 		{5,10}
// 	};

    vector<pair<double,double>> Data;
    double a,b;
    ifstream fpin("data/RemoveTrend_in");
    while (fpin >> a >> b) Data.push_back({a,b});

	auto res=PolyFit(Data,1);

	for (auto &item:res)
		cout << item << " ";
	cout << endl;
	
	return 0;
}

