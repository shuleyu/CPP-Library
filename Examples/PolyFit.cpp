#include<iostream>

#include<ASU_tools.hpp>

using namespace std;

int main(){

	vector<pair<double,double>> Data{
		{1,2},
		{3,6},
		{8,16},
		{5,10}
	};

	auto res=PolyFit(Data,1);

	for (auto &item:res)
		cout << item << " ";
	cout << endl;
	
	return 0;
}

