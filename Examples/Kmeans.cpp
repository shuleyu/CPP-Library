#include<iostream>
#include<ASU_tools.hpp>

using namespace std;

int main(){

	double **A=new double *[12];
	int *B=new int [12];
	for (size_t index=0;index<12;index++){
		A[index]=new double [1];
	}

	A[0][0]=1;A[4][0]=11;A[8][0]=21;
	A[1][0]=2;A[5][0]=12;A[9][0]=22;
	A[2][0]=3;A[6][0]=13;A[10][0]=23;
	A[3][0]=4;A[7][0]=14;A[11][0]=24;

	Kmeans(A,12,1,3,B);

	for (size_t index=0;index<12;index++){
		cout << A[index][0] << " ";
	}

	cout << endl <<  "=========="  << endl;

	for (size_t index=0;index<12;index++){
		cout << B[index] << " ";
	}

	cout << endl <<  "=========="  << endl;

	return 0;
}
