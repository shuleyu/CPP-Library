#include<iostream>

#include<Normalize.hpp>
#include<StretchSignal.hpp>
#include<TstarOperation.hpp>

using namespace std;

int main(){

    int n=8001;
    double delta=0.025,ts=0.9;

    auto y=TstarOperation(ts,delta,n);
    Normalize(y);
    auto yy=StretchSignal(y,2);
//     auto yy=StretchSignal(y,0.5);

    // output.
    for (size_t i=0;i<y.size();++i) cout << i*delta << " " << y[i] << '\n';
    cout << ">" << '\n';
    for (size_t i=0;i<yy.size();++i) cout << i*delta << " " << yy[i] << '\n';

    return 0;
}
