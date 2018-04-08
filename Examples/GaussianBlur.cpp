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

//     Normalize(A);

    // Use function.
    vector<vector<double>> B;
    B.push_back(A);
    GaussianBlur(B,dt,sigma);
//     Normalize(B[0]);

    ofstream fpout("data/GaussianBlur_out");
    for (size_t i=0;i<B[0].size();++i)
        fpout << B[0][i] << '\n';
    fpout.close();

    return 0;
}

// Matlab Benchmark:
// See data/GaussianBlur_matlab.m
