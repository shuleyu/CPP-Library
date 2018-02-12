#include<iostream>

#include<ASU_tools.hpp>

using namespace std;

int main(){

	auto res=CrossProduct(0,0,0,0,-1,0);

	for (auto &item: res) 
        printf("%.12lf\n",item);
// 		cout << item << " ";
	cout << endl;
	
	return 0;
}

