#include<iostream>
#include<fstream>

#include<ASU_tools.hpp>

using namespace std;

int main(){

    double a,b;
    vector<double> x,y;
    ifstream fpin("data/RemoveTrend_in");
    while (fpin >> a >> b) {
        x.push_back(a);
        y.push_back(b);
    }

	auto res=PolyFit(x,y,1);

	for (auto &item:res)
		cout << item << " ";
	cout << endl;
	
	return 0;
}

