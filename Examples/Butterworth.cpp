#include<iostream>
#include<fstream>
#include<vector>

#include<Butterworth.hpp>

using namespace std;

int main(){

    ifstream fpin("data/Butterworth_infile");
    vector<double> p,pp;
    double tmpval;

    while (fpin >> tmpval)
        p.push_back(tmpval);
    fpin.close();

    // Output results.
    ofstream fpout;

    // Bandpass 0.1 ~ 0.4 Hz.
    pp=p;
    Butterworth(pp,0.01,0.1,0.4);

    fpout.open("data/Butterworth_bp_CPP");
    for (auto &item:pp) fpout << item << "\n";
    fpout.close();

    // Lowpass 0.4 Hz.
    pp=p;
    Butterworth(pp,0.01,-1,0.4);

    fpout.open("data/Butterworth_lp_CPP");
    for (auto &item:pp) fpout << item << "\n";
    fpout.close();

    // Highpass 0.1 Hz.
    pp=p;
    Butterworth(pp,0.01,0.1,100);

    fpout.open("data/Butterworth_hp_CPP");
    for (auto &item:pp) fpout << item << "\n";
    fpout.close();

    return 0;
}
// SAC Benchmark:
// See Butterworth_matlab.m
