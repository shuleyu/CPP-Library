#include<iostream>
#include<fstream>

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

// 	vector<double> x={1,2,3,4},y={-1,-2,-3,-4};

    // Use function.
	double ccc;
	int shift;
// 	for (int i=0;i<1000;++i)
	CrossCorrelation(x,y,shift,ccc);
// 	CrossCorrelation(x,y,shift,ccc,false,-1);
	cout << "Shift: " << shift << "\n";
	cout << "Shift_time: " << shift*0.025 << "\n";
	cout << "CCC: " << ccc << "\n";

	ofstream fpout;
    fpout.open("data/CrossCorrelation_out");
	fpout << shift << endl;
	fpout.close();

    return 0;
}
// matlab equivalent:
// see CrossCorrelation_matlab.m