#include<iostream>
#include<fstream>
#include<vector>

#include<ASU_tools.hpp>

using namespace std;

int main(){

	vector<double> x,y,xx;
	double tmpval;
	int Cnt;

	ifstream fpin;

	Cnt=0;
	fpin.open("data/Interpolate_infile");
	while (fpin >> tmpval) {
		x.push_back(0.01*Cnt++);
		y.push_back(tmpval);
		xx.push_back(x.back());
		xx.push_back(x.back()+0.002);
		xx.push_back(x.back()+0.004);
		xx.push_back(x.back()+0.006);
		xx.push_back(x.back()+0.008);
	}
	fpin.close();

// 	vector<double> x{1,2,3,4,5};
// 	vector<double> y{1,2,3,4,5};
// 	vector<double> xx{1.5,2.5,3.5};
	
    // Use function.
    auto ans=Interpolate(x,y,xx,true);

    // Output result.
	ofstream fpout;
    fpout.open("data/Interpolate_out_C");
    for (size_t i=0;i<xx.size();++i)
		fpout << xx[i] << " " << ans[i] << "\n"; 
	fpout.close();

    return 0;
}
// Plots:
// See Interpolate_matlab.m
