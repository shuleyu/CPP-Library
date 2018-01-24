#include<iostream>

#include<ASU_tools.hpp>

using namespace std;

int main(){

	cout << FindRayP(143,13.2,500,100,12,"ScS");

	return 0;
}

//taup_time -mod prem -h 500 -ph ScS -evt 13.2 143 -sta 12 100 -rayp
