#include<iostream>

#include<ASU_tools.hpp>

using namespace std;

int main(){

	pair<double,double> p1{0,0},p2{1,1};
	cout << DotDist(p1.first,p1.second,p2.first,p2.second) << endl;

	return 0;
}
