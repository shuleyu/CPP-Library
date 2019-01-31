#include<iostream>

#include<ShellExecVec.hpp>

using namespace std;

int main(){

    vector<string> s=ShellExecVec("taup_path -h 1 -ph S -mod prem -sta 30.23 23.145 -evt -50 -24.3 -o stdout | grep -v '>'");
    cout << s.size() << endl;
    cout << "END OF OUTPUT" << endl;

    return 0;
}
