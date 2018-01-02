#include<iostream>
#include<fstream>
#include<vector>

#include<ASU_tools.hpp>

using namespace std;

int main(){

	int N=2000;
    double dt=0.01,sigma=1.27398;
	vector<double> A;

    // Use function.
	GaussianSignal(A,N,dt,sigma);

	ofstream fpout("data/GaussianSignal_out.txt");
	for (int i=0;i<N;++i)
		fpout << (i-N/2)*dt << " " << A[i] << "\n";
	fpout.close();

    return 0;
}
