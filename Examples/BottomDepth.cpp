#include<iostream>
#include<string>

#include<ASU_tools.hpp>

using namespace std;

int main(){

	double dist=120,evdp=500,depth;
	string phase="S";
	BottomDepth(dist,evdp,phase,depth);
	cout << depth  << endl;

	return 0;
}

