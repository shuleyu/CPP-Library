#include<iostream>
#include<fstream>

#include<ASU_tools.hpp>

using namespace std;

int main(){

	cout << "For vectors of float:" << endl;

	vector<double> A{5.4,1.3,-5.4,-3.2,0};
	auto ansA=Amplitude(A);

	cout << ansA.first << " occurs at positions: " << endl;
	for (auto item:ansA.second)
		cout << item << endl;
	cout << endl;


	//

	cout << "For vectors of integer:" << endl;

	vector<int> B{5,3,5,8,-5,-8,0};
	auto ansB=Amplitude(B);

	cout << ansB.first << " occurs at positions: " << endl;
	for (auto item:ansB.second)
		cout << item << endl;
	cout << endl;

	//

	cout << "For test data set:" << endl;

	ifstream fpin("data/Amplitude_infile");
	vector<double> C;
	double tmpval;
	while (fpin >> tmpval) C.push_back(tmpval);
	fpin.close();

	cout << Amplitude(C).first << endl;

	return 0;
}
