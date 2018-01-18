#include<iostream>
#include<fstream>
#include<vector>

#include<ASU_tools.hpp>

using namespace std;

int main(){

	double x1=0,y1=0,s1=0;
	double x2=1,y2=1,s2=2;

    // Use function.
	auto ans=StraightLineJunction(x1,y1,s1,x2,y2,s2);
	cout << ans.first << " " << ans.second << endl;

    return 0;    
}
