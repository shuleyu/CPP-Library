#include<iostream>

#include<ASU_tools.hpp>

int main(){

    double r,theta,phi;
	double xx,yy,zz;

	int x,y,z;
    x=0;
    y=0;
    z=-1;

    // Use function.
	cout << x << " " <<  y << " " << z << endl;
    Cart2Sph(x,y,z,r,phi,theta);
	cout << r << " " <<  phi << " " << theta << endl;

    Sph2Cart(r,theta,phi,xx,yy,zz);
	cout << xx << " " <<  yy << " " << zz << endl;

    return 0;

}
