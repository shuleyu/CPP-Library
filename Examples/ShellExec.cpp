#include<iostream>
#include<limits>
#include<ASU_tools.hpp>

using namespace std;

int main(){

	string tmp="ls";
	cout << ShellExec(tmp.c_str()) << endl;

	return 0;
}
