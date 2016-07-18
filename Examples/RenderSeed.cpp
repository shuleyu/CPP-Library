#include<iostream>
#include<ASU_tools.hpp>

using namespace std;

int main(){

	for (int index=0;index<10;index++){
		cout << RenderSeed() << endl;
	}

	return 0;
}
