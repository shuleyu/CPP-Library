#include<iostream>
#include<algorithm>

#include<Butterworth.hpp>
#include<CompareSignal.hpp>
#include<GaussianSignal.hpp>
#include<StretchSignal.hpp>

using namespace std;

int main(){

    int npts=1000;
    double delta=0.025;
    vector<double> p=GaussianSignal(npts,delta,1);
    Normalize(p);
    auto p2=p;
    rotate(p2.begin(),p2.end()-200,p2.end());
    for (size_t i=0;i<p.size();++i) p[i]+=0.2*p2[i];
    Normalize(p);

    auto pp=StretchSignal(p,0.95);
    auto ppp=StretchSignal(p,0.9);

    auto pppp=StretchSignal(p,0.9);
    vector<vector<double>> A{pppp};
    Butterworth(A,delta,0.03,0.3);
    pppp=A[0];
    Normalize(pppp);

    int P1=distance(p.begin(),max_element(p.begin(),p.end()));
    int P2=distance(pp.begin(),max_element(pp.begin(),pp.end()));
    int P3=distance(ppp.begin(),max_element(ppp.begin(),ppp.end()));
    int P4=distance(pppp.begin(),max_element(pppp.begin(),pppp.end()));

    // Use function.

    //
    auto res=CompareSignal(p,P1,p,P1,delta,-3,3,0.25);
    cout << ">V1 : " << res[0] << '\n';
    cout << ">V2 : " << res[1] << '\n';
    cout << ">V3 : " << res[2] << "\n\n";

    //
    res=CompareSignal(p,P1,pp,P2,delta,-3,3,0.25);
    cout << ">V1 : " << res[0] << '\n';
    cout << ">V2 : " << res[1] << '\n';
    cout << ">V3 : " << res[2] << "\n\n";

    //
    res=CompareSignal(p,P1,ppp,P3,delta,-3,3,0.25);
    cout << ">V1 : " << res[0] << '\n';
    cout << ">V2 : " << res[1] << '\n';
    cout << ">V3 : " << res[2] << "\n\n";

    //
    res=CompareSignal(p,P1,pppp,P4,delta,-3,3,0.25);
    cout << ">V1 : " << res[0] << '\n';
    cout << ">V2 : " << res[1] << '\n';
    cout << ">V3 : " << res[2] << "\n\n";

    for (size_t i=0;i<p.size();++i) cout << delta*i-delta*P1 << " " << p[i] << '\n';
    cout << ">\n";
    for (size_t i=0;i<pp.size();++i) cout << delta*i-delta*P2 << " " << pp[i] << '\n';
    cout << ">\n";
    for (size_t i=0;i<ppp.size();++i) cout << delta*i-delta*P3 << " " << ppp[i] << '\n';
    cout << ">\n";
    for (size_t i=0;i<pppp.size();++i) cout << delta*i-delta*P4 << " " << pppp[i] << '\n';
    cout << ">\n";

    return 0;
}
