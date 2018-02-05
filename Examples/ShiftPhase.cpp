#include<iostream>
#include<fstream>
#include<vector>

#include<ASU_tools.hpp>

using namespace std;

int main(){

    double delta,sigma,gwidth,shift;

    shift=-90;
    delta=0.025;
    sigma=2;
    gwidth=50;
    int n=2*(int)ceil(gwidth/2/delta);

    // Make signal.
	vector<double>gauss=GaussianSignal(n,delta,sigma);
    Normalize(gauss);

    // Use function.
    vector<double> hilbert=ShiftPhase(gauss,shift);

    // Output.
	ofstream fpout("data/ShiftPhase_in");
    for (int i=0;i<n;++i)
        fpout << i*delta << " " << gauss[i] << endl;
    fpout.close();

    fpout.open("data/ShiftPhase_out");
    for (int i=0;i<n;++i)
        fpout << i*delta << " " << hilbert[i] << endl;
    fpout.close();

    return 0;
}
// Visualize data.
// See phase_shift_matlab.m
