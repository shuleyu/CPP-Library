#include<iostream>
#include<vector>

#include<ShiftStack.hpp>

using namespace std;

int main(){

	vector<vector<int>> A{{1,2,3,4,5},{2,4,6,8,10},{3,6,9,12,15}};
	vector<double> W{0.1,1.0,-0.9};
	vector<int> W2{1,2,3};
	vector<int> S{1,-1,2};

    // Use function.
    auto ans=ShiftStack(A,S,W);
//     auto ans=ShiftStack(A);

	// This is ambiguous situation,
	// W2 will be treated as Shift.
//     auto ans=ShiftStack(A,W2);

	// Use it like so instead:
//     auto ans=ShiftStack(A,{},W2);

    // Print Output.
    cout << "Original 2D Array [Shift](Weight):" << endl;
	auto it1=S.begin();
	auto it2=W.begin();
	for (auto &item: A) {
		for (auto &item2: item)
			cout << item2 << " ";
		cout << "[" << *it1++ << "]";
		cout << "(" << *it2++ << ")";
		cout << endl;
	}
	cout << endl;

    cout << "Stack:" << endl;
	for (auto &item: ans.first)
		cout << item << " ";
	cout << endl << endl;

    cout << "Standard Deviation:" << endl;
	for (auto &item: ans.second)
		cout << item << " ";
	cout << endl << endl;

    return 0;
}
