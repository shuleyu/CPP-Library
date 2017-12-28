#include<iostream>

#include<ASU_tools.hpp> 

using namespace std;

int main(){
    
    double a,b,c,d;
    a=1;
    b=2;
    c=1;
    d=1;
    
    // Use function:
    auto res=ComplexDivide(a,b,c,d);

	cout << a << " + " << b << " i" << "\n";
	cout << "----------- = " << res.first << " + " << res.second << " i" << "\n";
	cout << c << " + " << d << " i" << "\n";

    return 0;    
}
