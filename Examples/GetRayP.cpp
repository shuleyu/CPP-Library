#include<iostream>
#include<string>

#include<ASU_tools.hpp>

using namespace std;

int main(){

	double dist=20,evdp=500;
	string phase="P";
	cout << GetRayP(dist,evdp,phase) << endl;

	return 0;
}

