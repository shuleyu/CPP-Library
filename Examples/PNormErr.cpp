#include<iostream>
#include<vector>

#include<PNormErr.hpp>

using namespace std;

int main(int argc, char **argv){

    vector<double> A{0.1,1.1,2.1},B{0,1,2};  
    vector<double> C;
    for (size_t i=0;i<A.size();++i) C.push_back(A[i]-B[i]);

    cout << PNorm(C,2) / PNorm(B,2) << endl;
    cout << PNormErr(A,B,2) << endl;

    return 0;
}
