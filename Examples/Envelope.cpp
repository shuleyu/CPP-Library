#include<iostream>
#include<fstream>
#include<cmath>

extern "C"{
#include<sac.h>
#include<sacio.h>
}

#include<ASU_tools.hpp>

using namespace std;

int main(){

    vector<double> A;
    ifstream fpin("data/200608250044.CN.LMN.BHT.txt");
    double x,y,delta=0.025;
    while (fpin >> x >> y) A.push_back(y);
    fpin.close();
    Normalize(A);

    // Use function.
    auto ans=Envelope(A);

    // Use SAC library
    int n=A.size();
    float *A_float= new float [n];
    float *ans_SAC= new float [n];
    for (int i=0;i<n;++i) A_float[i]=A[i];
    envelope(n,A_float,ans_SAC);

    // Output.
    ofstream fpout;
    fpout.open("data/Envelope_out");
    for (size_t i=0;i<A.size();++i)
        fpout << delta*i << " " <<  ans[i] << '\n';
    fpout.close();

    fpout.open("data/Envelope_SAC_out");
    for (size_t i=0;i<A.size();++i)
        fpout << delta*i << " " <<  ans_SAC[i] << '\n';
    fpout.close();

    delete [] A_float;
    delete [] ans_SAC;

    return 0;
}
