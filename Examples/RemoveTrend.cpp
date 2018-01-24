#include<iostream>
#include<fstream>

#include<ASU_tools.hpp>

using namespace std;

int main(){

    vector<double> p;
    double a,b;
    ifstream fpin("data/RemoveTrend_in");
    while (fpin >> a >> b) p.push_back(b);

	RemoveTrend(p,0.01,0);
	
	return 0;
}

