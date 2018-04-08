#include<iostream>
#include<vector>

#include<ReorderY2X.hpp>

using namespace std;

int main(){

//     vector<vector<int>> A{{1,3,5},{2,4,6}};
    vector<vector<int>> A{{1,2,3},{4,5,6}};
    auto B=ReorderY2X(A);
    for (auto &item:B) {
        for (auto item2:item)
            cout << item2 << " ";
        cout << endl;
    }

    return 0;
}

