#include<iostream>
#include<vector>

#include<PointOnSegment.hpp>

using namespace std;

int main(){

    pair<double,double> x,y,p;
    bool ans;

    // Use function.
    x={25.000000000000010,3510.000000000000000};
    y={25.000000000000010,3509.500000000000000};
    p={25.000000000000000,3509.500000000000000};
    ans=PointOnSegment(x,y,p);
    printf("%.20lf\n",x.first);
    cout << p.first << " " << p.second << (ans?" On (":" Off (") << x.first << "," << x.second << ") -- (" << y.first << "," << y.second << ")" << '\n';

    return 0;
}
