#include<iostream>

#include<ShellExecVec.hpp>

using namespace std;

int main(){

    vector<string> s=ShellExecVec("ls");
    cout << s.size() << endl;
    cout << s[0] << endl;

    return 0;
}
