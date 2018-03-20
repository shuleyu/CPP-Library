#include<iostream>
#include<vector>

#include<ASU_tools.hpp>

using namespace std;

int main(){

	pair<double,double> x{22.4,3500},y{33,3500},p{30,3499.9999999998908606};

    // Use function.
	auto ans=PointOnSegment(x,y,p);
    cout << p.first << " " << p.second << (ans?" On (":" Off (") << x.first << "," << x.second << ") -- (" << y.first << "," << y.second << ")" << '\n';

    return 0;    
}
