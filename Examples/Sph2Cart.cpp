#include<iostream>

#include<ASU_tools.hpp>

using namespace std;

int main(){

	int x=0,y=0,z=-1;

    // Use function.
	cout << x << " " <<  y << " " << z << endl;
    auto ans=Cart2Sph(x,y,z);
	cout << ans[0] << " " <<  ans[1] << " " << ans[2] << endl;

    auto res=Sph2Cart(ans[0],ans[1],ans[2]);
	cout << res[0] << " " <<  res[1] << " " << res[2] << endl;

    return 0;

}
