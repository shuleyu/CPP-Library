#include<iostream>
#include<vector>

#include<PointOnSegment.hpp>

using namespace std;

int main(){

	pair<double,double> x{25.999672640532992,3509.999999999881766},y{25.996921245197690,3510.099999999881675},p{25.999672640529727,3510.000000000000000};

    // Use function.
	auto ans=PointOnSegment(x,y,p);
    cout << p.first << " " << p.second << (ans?" On (":" Off (") << x.first << "," << x.second << ") -- (" << y.first << "," << y.second << ")" << '\n';

    return 0;
}
