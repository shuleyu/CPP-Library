#include<iostream>

#include<ASU_tools.hpp>

using namespace std;

int main(){

	double x1=1,y1=1,s1=3;
	double x2=1,y2=1,s2=1.0/0.0;

    // Use function.
	auto ans=LineJunction(x1,y1,s1,x2,y2,s2);
	cout << ans.first << " " << ans.second << endl;

    return 0;    
}
