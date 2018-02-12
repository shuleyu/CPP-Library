#include<iostream>
#include<vector>

#include<ASU_tools.hpp>

using namespace std;

int main(){

	pair<double,double> x{2,-1},y{2,1},p{2,-1};

    // Use function.
	auto ans=PointOnSegment(x,y,p);
    cout << p.first << " " << p.second << (ans?" On (":" Off (") << x.first << "," << x.second << ") -- (" << y.first << "," << y.second << ")" << '\n';

    return 0;    
}
