#include<iostream>

#include<SimpsonRule.hpp>

using namespace std;

int main(){

    double h=0.01;
    vector<double> A{1};
    for (int i=0;i<101;++i) A.push_back(A.back()+h);
    cout << SimpsonRule(A)*(h/3) << endl;

    return 0;
}
