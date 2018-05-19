#include<iostream>

#include<Normalize.hpp>
#include<StretchSignal.hpp>
#include<TstarOperator.hpp>

using namespace std;

int main(){

    int n=8001;
    double delta=0.025,ts=0.9;

    auto y=TstarOperator(ts,delta);
    Normalize(y.first);
    auto yy=StretchSignal(y.first,2);
//     auto yy=StretchSignal(y.first,0.5);

    // output.
    for (size_t i=0;i<y.first.size();++i) cout << i*delta << " " << y.first[i] << '\n';
    cout << ">" << '\n';
    for (size_t i=0;i<yy.size();++i) cout << i*delta << " " << yy[i] << '\n';

    return 0;
}
