#include<iostream>
#include<fstream>
#include<vector>

#include<Butterworth.hpp>

using namespace std;

int main(){

    ifstream fpin("data/Butterworth_infile");
	vector<vector<double>> p,pp;
	vector<double> tmpdata;
	double tmpval;

	while (fpin >> tmpval)
		tmpdata.push_back(tmpval);
    fpin.close();

	p.push_back(tmpdata);


    // Output results.
	ofstream fpout;

	// Bandpass 0.1 ~ 0.4 Hz.
	pp=p;
	Butterworth(pp,0.01,0.1,0.4);

	fpout.open("data/Butterworth_bp_CPP");
    for (auto &item:pp[0]) fpout << item << "\n";
	fpout.close();

	// Lowpass 0.4 Hz.
	pp=p;
	Butterworth(pp,0.01,-1,0.4);

	fpout.open("data/Butterworth_lp_CPP");
    for (auto &item:pp[0]) fpout << item << "\n";
	fpout.close();

	// Highpass 0.1 Hz.
	pp=p;
	Butterworth(pp,0.01,0.1,100);

	fpout.open("data/Butterworth_hp_CPP");
    for (auto &item:pp[0]) fpout << item << "\n";
	fpout.close();

    return 0;
}
// SAC Benchmark:
// See Butterworth_matlab.m
