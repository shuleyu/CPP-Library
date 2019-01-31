#include<iostream>
#include<fstream>
#include<vector>

#include<GaussianSignal.hpp>

using namespace std;

int main(){

    int N=4000;
    double dt=0.025,sigma=42.91;

    // Use function.
    vector<double> A=GaussianSignal(N,dt,sigma,true);

    ofstream fpout("data/GaussianSignal_out.txt");
    for (int i=0;i<N;++i)
        fpout << (i-N/2)*dt << " " << A[i] << "\n";
    fpout.close();

    return 0;
}
