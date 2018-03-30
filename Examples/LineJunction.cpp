#include<iostream>

#include<LineJunction.hpp>

using namespace std;

int main(){

	pair<double,double> p1{0,0},p2{1,1};
    double s1=3,s2=3;

    // Use function.
	auto ans=LineJunction(p1,s1,p2,s2);
	if (!ans.first) cout << "Lines don't cross paths..." << endl;
	else cout << ans.second.first << " " << ans.second.second << endl;

    return 0;    
}
