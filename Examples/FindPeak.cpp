#include<iostream>
#include<vector>
#include<random>
#include<chrono>

#include<FindPeak.hpp>

using namespace std;

int main(){

	auto seed=chrono::system_clock::now().time_since_epoch().count();
	default_random_engine e(seed);
	uniform_real_distribution<double> u(-1,1);

	vector<double> A;
	for (int i=0;i<100;++i) A.push_back(u(e));

	int Peak=50;
    // Use function.
    printf("Widow: %d ~ %d\n",Peak-10,Peak+10-1);
	Peak=FindPeak(A,Peak,-10,20);
    printf("Peak is at: %d\n",Peak);

	for (int i=35;i<65;++i){
		printf("%d\t%.4lf\n",i,A[i]);
	}

    return 0;
}
