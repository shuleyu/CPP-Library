#include<iostream>

#include<SortWithIndex.hpp>

using namespace std;

int main(){

    vector<double> A{1.3,2.4,1.2,3.1,5.1,6.3,4.5,6.4,5.6};
    vector<double> B{4.2,2.1,1.3,3.5,3.2,1.3,1.4,5.2,5.7};

    for (size_t i=0;i<A.size();++i)
        cout << A[i] << " " << B[i] << " Original Index: "  << i << "\n";
    cout << endl;

    auto res=SortWithIndex(A.begin(),A.end(),std::greater<double>());
    auto C=B;

    for (size_t i=0;i<B.size();++i) C[i]=B[res[i]];

    for (size_t i=0;i<A.size();++i)
        cout << A[i] << " " << C[i] << " Original Index: "  << res[i] << "\n";
    cout << endl;

    return 0;
}

