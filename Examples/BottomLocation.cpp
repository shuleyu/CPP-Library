#include<iostream>
#include<string>

#include<BottomLocation.hpp>

using namespace std;

int main(){

	double evdp=500;
	string phase="S";

	auto ans=BottomLocation(16,80,evdp,32.3,56.23,phase);
	cout << ans[1] << " " << ans[2] << endl;
	cout << ans[0] << endl;

	return 0;
}

