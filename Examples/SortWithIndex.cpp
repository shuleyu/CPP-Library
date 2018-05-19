#include<iostream>
#include<cmath>

#include<SortWithIndex.hpp>

using namespace std;

int main(){

    vector<double> A{1.3,-2.4,1.2,-3.1,5.1,-6.3,4.5,-6.4,5.6};
    vector<double> B{4.2,2.1,1.3,3.5,3.2,1.3,1.4,5.2,5.7};

    // print before sort.
    for (size_t i=0;i<A.size();++i)
        cout << A[i] << " " << B[i] << " Original Index: "  << i << "\n";
    cout << endl;

    // sort A and get sorting index.

//     auto res=SortWithIndex(A.begin(),A.end());

    // or ...
//     auto res=SortWithIndex(A.begin(),A.end(),std::greater<double>());

    // or ...
    auto f=[](const double &s1, const double &s2){return fabs(s1)<fabs(s2);};
    auto res=SortWithIndex(A.begin(),A.end(),f);

    // sort B.
    auto C=B;
    for (size_t i=0;i<B.size();++i) C[i]=B[res[i]];

    // print after sort.
    for (size_t i=0;i<A.size();++i)
        cout << A[i] << " " << C[i] << " Original Index: "  << res[i] << "\n";
    cout << endl;

    return 0;
}

