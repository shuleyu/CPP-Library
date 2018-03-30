#include<iostream>
#include<fstream>

#include<CrossCorrelation.hpp>

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

// 	vector<double> x={1,2,3,4},y={-1,-2,-3,-4};

    // Use function.
// 	for (int i=0;i<1000;++i)
// 	auto ans=CrossCorrelation(x,y);
	auto ans=CrossCorrelation(x,y,true);
// 	CrossCorrelation(x,y,false,-1);
	cout << "Shift: " << ans.first.first << "\n";
	cout << "Shift_time: " << ans.first.first*0.025 << "\n";
	cout << "CCC: " << ans.first.second << "\n";

	ofstream fpout;
    fpout.open("data/CrossCorrelation_out");
    for (auto &item:ans.second) fpout << item << '\n';
	fpout.close();

    return 0;
}
// matlab equivalent:
// see CrossCorrelation_matlab.m
