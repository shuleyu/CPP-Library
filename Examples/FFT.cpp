#include<iostream>
#include<fstream>

#include<ASU_tools.hpp>

using namespace std;

int main(){

    double delta,sigma,gwidth;

    delta=0.025;
    sigma=2;
    gwidth=50;

    int n=2*(int)ceil(gwidth/2/delta);

    vector<double> gauss=GaussianSignal(n,delta,sigma);

    Normalize(gauss);

    // Use function.
    vector<double> freq;
    vector<vector<double>> x,amp,phase;
    x.push_back(gauss);
    FFT(x,delta,freq,amp,phase);

    // Output.
    ofstream fpout("data/FFT_in");
    for (int i=0;i<n;++i) fpout << delta*i << " " << gauss[i] << '\n';
    fpout.close();

    fpout.open("data/FFT_freq_amp_phase");
    for (size_t i=0;i<freq.size();++i)
        fpout << freq[i] << " " << amp[0][i] << " " << phase[0][i] << '\n';
    fpout.close();

    return 0;
}
// Visualize data.
// See freq_amp_phase_matlab.m
