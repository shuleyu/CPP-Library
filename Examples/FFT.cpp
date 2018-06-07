#include<iostream>
#include<fstream>

#include<FFT.hpp>
#include<GaussianSignal.hpp>
#include<Normalize.hpp>
#include<CreateGrid.hpp>

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
    auto ans=FFT(gauss,delta);
    auto freq=CreateGrid(0,1/delta/2,ans.first.size(),0);


    // Output.
    ofstream fpout("data/FFT_in");
    for (int i=0;i<n;++i) fpout << delta*i << " " << gauss[i] << '\n';
    fpout.close();

    fpout.open("data/FFT_freq_amp_phase");
    for (size_t i=0;i<ans.first.size();++i)
        fpout << freq[i] << " " << ans.first[i] << " " << ans.second[i] << '\n';
    fpout.close();

    return 0;
}
// Visualize data.
// See freq_amp_phase_matlab.m
