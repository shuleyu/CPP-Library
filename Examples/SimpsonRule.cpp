#include<iostream>
#include<cmath>

#include<SimpsonRule.hpp>
#include<CreateGrid.hpp>

using namespace std;

int main(){

    double h=0.01;
    const vector<double> A{1,-1.01};

    vector<double> B{1};
    for (int i=0;i<3;++i) B.push_back(B.back()+h);

    vector<double> C{1};
    for (int i=0;i<10001;++i) C.push_back(C.back()+h);

//     cout << SimpsonRule(A.begin(),A.end(),h) << endl;
//     cout << SimpsonRule(B.begin(),B.end(),h) << endl;

//     auto f=[](const double &s1){return (s1>0?s1:-s1);};
//     cout << SimpsonRule(C.begin(),C.end(),h,f) << endl;

    //

    for (double x=0.5;x<10;x+=0.5){
        double h=min(0.01,x/10000);
        auto xx=CreateGrid(0,x,h,1);
        cout << xx.size() << endl;
        auto f=[](const double &t){return exp(t);};
        cout << "Error: " << (exp(x)-1-SimpsonRule(xx.begin(),xx.end(),h,f))/exp(x)*100 << "%\n";
    }

    return 0;
}
