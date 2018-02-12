#include<iostream>

#include<ASU_tools.hpp>

using namespace std;

int main(){

	pair<double,double> p1{0,0},q1{1,1},p2{0.0,1.0},q2{1,0};

    // Use function.
	auto ans=SegmentJunction(p1,q1,p2,q2);
	if (!ans.first) cout << "Segments don't cross paths ..." << endl;
	else cout << ans.second.first << " " << ans.second.second << endl;

    return 0;    
}
