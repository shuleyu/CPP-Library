#include<iostream>

#include<ASU_tools.hpp>

using namespace std;

int main(){

	auto res=CrossProduct(1,0,0,0.984808,0.173648,0);

	for (auto &item: res) 
		cout << item << " ";
	cout << endl;
	
	return 0;
}

