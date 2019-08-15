#include<iostream>
#include<fstream>
#include<vector>

#include<GaussianBlur.hpp>

using namespace std;

int main(){

    double dt=0.025,time,amp,sigma=1.0;
    vector<double> A,T;

    ifstream fpin("data/GaussianBlur_infile");

    while (fpin >> time >> amp) {
        T.push_back(time);
        A.push_back(amp);
    }
    fpin.close();

    // Use function.
    GaussianBlur(A,dt,sigma,true);

    ofstream fpout("data/GaussianBlur_out");
    size_t i=0;
    for (const auto &item:A)
        fpout << T[i++] << " " << item << '\n';
    fpout.close();

    return 0;
}

// Matlab Benchmark:
// See data/GaussianBlur_matlab.m
