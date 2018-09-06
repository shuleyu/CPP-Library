#include<iostream>
#include<fstream>

#include<IFFT.hpp>
#include<FFT.hpp>
#include<GaussianSignal.hpp>
#include<Normalize.hpp>
#include<CreateGrid.hpp>

using namespace std;

int main(){

    double delta,sigma,gwidth;

    delta=0.025;
    sigma=2;
    gwidth=50;

    int n=2*(int)ceil(gwidth/2/delta);

    vector<double> gauss=GaussianSignal(n,delta,sigma);
    Normalize(gauss);

    // Use FFT function.
    auto ans=FFT(gauss,delta);
    auto freq=CreateGrid(0,1/delta/2,ans.first.size(),0);

    auto restore=IFFT(ans.first,ans.second,freq[1]-freq[0]);
    for (auto &item: restore) item/=restore.size();

    for (size_t i=0;i<gauss.size();++i)
//         cout << delta*i << " " << restore[i] << "\n";
//         cout << delta*i << " " << gauss[i] << "\n";
        cout << gauss[i] << " " << restore[i] << " " << (restore[i]-gauss[i])/gauss[i]*100 << "% \n";

    return 0;
}
