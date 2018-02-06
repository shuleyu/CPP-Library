#include<iostream>

#include<ASU_tools.hpp>

using namespace std;

int main(){

	pair<double,double> p1{0,0},q1{1,0},p2{0.75,0.25},q2{0.75,0.75};

    // Use function.
	auto ans=SegmentJunction(p1,q1,p2,q2);
	if (!ans.first) cout << "Segments don't cross paths ..." << endl;
	else cout << ans.second.first << " " << ans.second.second << endl;

    return 0;    
}
