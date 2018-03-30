#include<iostream>
#include<vector>

#include<PNorm.hpp>

using namespace std;

int main(int argc, char **argv){

	vector<double> A{0.1,1.1,-2.1};  

	cout << PNorm(A,2) << endl;

	return 0;
}
