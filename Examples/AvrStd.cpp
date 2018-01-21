#include<iostream>
#include<fstream>

#include<ASU_tools.hpp>

using namespace std;

int main(){


	ifstream fpin("data/AvrStd_infile");
	vector<double> A,B;
	vector<float> C;
	vector<int> D;

	double tmpval,tmpweight;
	while (fpin >> tmpval >> tmpweight) {
		A.push_back(tmpval);
		B.push_back(tmpweight);
	}
	fpin.close();

// 	A={1,2,3,4};
// 	B={2,1,1,1};
// 	B={0.11188,0.33388,0.77788,0.99988};
// 	B={0.11111,0.33333,0.77777,0.99999};

// 	A={1,2,3,4,1};
// 	B={1,1,1,1,1};


	for (auto item:B) {
		C.push_back((float)item);
		D.push_back((int)floor(1000*item));
	}

	//

	cout << "For no weight:" << endl;

	auto ans1=AvrStd(A);
	printf("Mean: %.12lf\n",ans1.first);
	printf("Std : %.12lf\n",ans1.second);
	cout << endl;

	//

	cout << "For weighted by double precision:" << endl;

	auto ans2=AvrStd(A,B);
	printf("Mean: %.12lf\n",ans2.first);
	printf("Std : %.12lf\n",ans2.second);
	cout << endl;

	//

	cout << "For weighted by float precision:" << endl;

	auto ans3=AvrStd(A,C);
	printf("Mean: %.12lf\n",ans3.first);
	printf("Std : %.12lf\n",ans3.second);
	cout << endl;

	//

	cout << "For weighted by 3-digit precision:" << endl;

	auto ans4=AvrStd(A,D);
	printf("Mean: %.12lf\n",ans4.first);
	printf("Std : %.12lf\n",ans4.second);
	
	return 0;
}

