#include<iostream>
#include<fstream>
#include<cmath>

#include<SACSignals.hpp>

using namespace std;

int main(){

    SACSignals A("/home/shule/x");
    A.CheckDist();
    A.Interpolate(0.01);
    A.PrintInfo();

	return 0;
}
