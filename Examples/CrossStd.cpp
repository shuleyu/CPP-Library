#include<iostream>
#include<fstream>
#include<vector>

#include<ASU_tools.hpp>

using namespace std;

int main(){

	vector<double> x,y;
	double tmpval;
	ifstream fpin;
    fpin.open("data/CrossCorrelation_infile1");
	while (fpin >> tmpval) x.push_back(tmpval);
	fpin.close();

    fpin.open("data/CrossCorrelation_infile2");
	while (fpin >> tmpval) y.push_back(tmpval);
	fpin.close();

    // Use function.
	auto res=CrossStd(y,x);
	cout << "Shift: " << res.first << "\n";
	cout << "STD: " << res.second << "\n";

    return 0;
}
