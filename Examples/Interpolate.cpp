#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>

#include<Interpolate.hpp>

using namespace std;

int main(){

//     vector<double> x,y,xx;
//     double tmpval;
//     int Cnt;
//
//     ifstream fpin;
//
//     Cnt=0;
//     fpin.open("data/Interpolate_infile");
//     while (fpin >> tmpval) {
//         x.push_back(0.01*Cnt++);
//         y.push_back(tmpval);
//         xx.push_back(x.back());
//         xx.push_back(x.back()+0.002);
//         xx.push_back(x.back()+0.004);
//         xx.push_back(x.back()+0.006);
//         xx.push_back(x.back()+0.008);
//     }
//     fpin.close();
//
//     // Use function.
//     reverse(x.begin(),x.end());
//     reverse(y.begin(),y.end());
//
//     auto ans=Interpolate(x,y,xx);
//
//     // Output result.
//     ofstream fpout;
//     fpout.open("data/Interpolate_out");
//     for (size_t i=0;i<ans.size();++i)
//         fpout << xx[i] << " " << ans[i] << "\n";
//     fpout.close();

    vector<double> x{1,2,3,4,5};
    vector<double> y{1,1,1,1,1};
    vector<double> xx{1,1.5,2,2.3,2.5,3,3.2,3.5,4.5,4.7,5};
    auto yy=Interpolate(x,y,xx);
    for (size_t i=0;i<xx.size();++i) cout  << xx[i] << " " << yy[i] << '\n';
    cout << endl;
//
//     reverse(x.begin(),x.end());
//     reverse(y.begin(),y.end());
//     yy=Interpolate(x,y,xx);
//     for (size_t i=0;i<xx.size();++i) cout  << xx[i] << " " << yy[i] << '\n';
//     cout << endl;
//
//     x={1,2.3,3.2,4.7,5.9};
//     y={1,3,-3,1,5};
//     yy=Interpolate(x,y,xx);
//     for (size_t i=0;i<xx.size();++i) cout  << xx[i] << " " << yy[i] << '\n';
//     cout << endl;
//
//     reverse(x.begin(),x.end());
//     reverse(y.begin(),y.end());
//     yy=Interpolate(x,y,xx);
//     for (size_t i=0;i<xx.size();++i) cout  << xx[i] << " " << yy[i] << '\n';
//     cout << endl;
//
//     return 0;
}
// Plots:
// See Interpolate_matlab.m
