#include<iostream>
#include<vector>

#include<ASU_tools.hpp>

using namespace std;

int main(){

    vector<float> A;

    // Use function.
//     TriangleSignal(A,20,0.2);
    TriangleSignal(A,20,2.0);

	for (auto &item:A) cout << item << "  ";
	cout << "\n";

    return 0;
}
