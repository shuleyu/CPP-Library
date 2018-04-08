#include<iostream>
#include<fstream>
#include<vector>

#include<HannTaper.hpp>

using namespace std;

int main(){

//     vector<double> A(101,1);
    vector<double> A(100,1);

    // Use function.

    HannTaper(A,0.5);

    ofstream fpout("data/HannTaper_out");
    for (size_t i=0;i<A.size();++i) fpout << i << " " << A[i] << '\n';
    fpout.close();

    return 0;
}
