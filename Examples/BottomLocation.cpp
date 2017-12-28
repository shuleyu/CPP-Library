#include<iostream>
#include<string>

#include<ASU_tools.hpp>

using namespace std;

int main(){

	double evdp=500,depth;
	string phase="S";

	auto ans=BottomLocation(16,80,evdp,32.3,56.23,phase,depth);
	cout << ans.first << " " << ans.second << endl;
	cout << depth  << endl;

	return 0;
}

