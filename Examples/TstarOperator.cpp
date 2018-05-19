#include<iostream>
#include<fstream>

#include<Normalize.hpp>
#include<TstarOperator.hpp>

using namespace std;

int main(){

    double delta=0.025,ts=0.9;

    // Use function.
    auto ans=TstarOperator(ts,delta);

    // output.
    for (size_t i=0;i<ans.first.size();++i)
    printf("%.12e\t%.12e\n",delta*i-delta*ans.second,ans.first[i]);

    return 0;
}
