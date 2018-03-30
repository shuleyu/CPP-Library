#include<iostream>
#include<vector>

#include<ReorderX2Y.hpp>

using namespace std;

int main(){

    vector<vector<int>> A{{1,2,3},{4,5,6}};
    auto B=ReorderX2Y(A);
    for (auto &item:B) {
        for (auto item2:item)
            cout << item2 << " ";
        cout << endl;
    }

	return 0;
}

