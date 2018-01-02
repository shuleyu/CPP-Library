#include<iostream>
#include<vector>

#include<ASU_tools.hpp>

using namespace std;

int main(){

	vector<int> A{1,2,3,4,5};
	vector<double> B{1,2.5,1};

    // Use function.
    auto C=Convolve(A,B,0);
    C2=Convolve(A,B,1);

	cout << "A : ";
    for (auto &item:A) cout << item << " ";
	cout << "\n";

	cout << "B : ";
    for (auto &item:B) cout << item << " ";
	cout << "\n";

	cout << "C : ";
    for (auto &item:C) cout << item << " ";
	cout << "\n";

	cout << "C': ";
    for (auto &item:C2) cout << item << " ";
	cout << "\n";

    return 0;
}

// Matlab Benchmark:
// a=[1,1,1,1,1];
// b=[1,7,1,-3,1];
// conv(a,b)
