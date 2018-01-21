#include<iostream>
#include<fstream>
#include<cmath>

#include<ASU_tools.hpp>

extern "C"{
#include<sacio.h>
#include<sac.h>
}

using namespace std;

int main(){


    double delta;

    delta=0.025;

    int n=(int)ceil(4*M_PI/delta);

    vector<double> Sin(n,0);

    for (int i=0;i<n;++i) Sin[i]=sin(i*delta);

    // Use function.
    auto ans=Envelope(Sin);

    // Use SAC library
    float *Sin_float= new float [n];
    float *ans_SAC= new float [n];
    for (int i=0;i<n;++i) Sin_float[i]=Sin[i];
    envelope(n,Sin_float,ans_SAC);

    // Output.
    ofstream fpout("data/Envelope_in");
    for (int i=0;i<n;++i) fpout << delta*i << " " << Sin[i] << '\n';
    fpout.close();

    fpout.open("data/Envelope_out");
    for (size_t i=0;i<ans.size();++i) fpout << delta*i << " " <<  ans[i] << " " << ans_SAC[i] << '\n';
    fpout.close();

    delete [] Sin_float;
    delete [] ans_SAC;

    return 0;
}
