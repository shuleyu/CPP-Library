#include<iostream>
#include<fstream>
#include<algorithm>

#include<SNR.hpp>

using namespace std;

int main(){


    vector<vector<double>> p(5,vector<double> ());
    double a,b,c,d,e;

	ifstream fpin("data/SNR_in");

    while (fpin >> a >> b >> c >> d >> e) {
        p[0].push_back(a);
        p[1].push_back(b);
        p[2].push_back(c);
        p[3].push_back(d);
        p[4].push_back(e);
    }

    for (int i=0;i<5;++i)
        cout << SNR(p[i],126,375,625,251) << " " << SNR(p[i],126,375,625,251,1) << " " << SNR(p[i],126,375,625,251,2) << endl;

    return 0;
}
