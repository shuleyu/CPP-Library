#include<iostream>
#include<vector>

#include<ASU_tools.hpp>

using namespace std;

int main(){

	pair<double,double> x{0,1},y{1,2},p{0.5,1.5};

    // Use function.
	auto ans=PointOnSegment(x,y,p);
    cout << p.first << " " << p.second << (ans?" On":" Outof") << '\n';

    return 0;    
}
