#include<iostream>

#include<ASU_tools.hpp>

using namespace std;

int main(){

    int n=10,k=7;

    // Use function.
	cout << "C_" << n << "^" << k << endl;
	cout << Combination(n,k) << endl;

    return 0;    
}
