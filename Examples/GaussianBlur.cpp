#include<iostream>
#include<fstream>
#include<vector>

#include<GaussianBlur.hpp>

using namespace std;

int main(){

    double dt=0.025,time,amp,sigma=1.0;
    vector<double> A;

    ifstream fpin("data/GaussianBlur_infile");

    while (fpin >> time >> amp)
        A.push_back(amp);
    fpin.close();

    // Use function.
    GaussianBlur(A,dt,sigma);

    ofstream fpout("data/GaussianBlur_out");
    for (const auto &item:A)
        fpout << item << '\n';
    fpout.close();

    return 0;
}

// Matlab Benchmark:
// See data/GaussianBlur_matlab.m
