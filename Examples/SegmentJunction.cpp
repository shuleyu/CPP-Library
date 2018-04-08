#include<iostream>

#include<SegmentJunction.hpp>

using namespace std;

int main(){

    pair<double,double> p1{26.000000000000000,3510.000000000000000},q1{25.899999999999999,3510.000000000000000},p2{25.996921245197690,3510.099999999881675},q2{25.999672640532992,3509.999999999881766};

    // Use function.
    auto ans=SegmentJunction(p1,q1,p2,q2);
    if (!ans.first) cout << "Segments don't cross paths ..." << endl;
    else printf("%.15lf,%.15lf\n",ans.second.first,ans.second.second);

    return 0;    
}
