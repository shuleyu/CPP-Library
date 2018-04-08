#include<iostream>
#include<fstream>
#include<vector>

#include<Normalize.hpp>

using namespace std;

int main(){

    ifstream fpin("data/Normalize_infile");
    vector<double> A;
    double tmpval;
    while (fpin >> tmpval) A.push_back(tmpval);
    fpin.close();

    // Use function.
//     cout << Normalize(A) << endl;
    cout << Normalize(A,{10,20}) << endl;

    // Output result.
    ofstream fpout("data/Normalize_out");
    for (auto &item:A)
        fpout << item << "\n";
    fpout.close();

    return 0;
}
