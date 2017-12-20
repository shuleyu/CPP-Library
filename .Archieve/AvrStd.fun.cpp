#include<cmath>
#include<vector>
#include<utility>
#include<numeric>

#include<ASU_tools.hpp>

using namespace std;

pair<double,double> AvrStd(const vector<double> &V,const vector<double> &W){

	if (W.empty()){

		if (V.empty()) return {};
		if (V.size()==1) return {V[0],0};

		// Calculate x average.
		double avr=accumulate(V.begin(),V.end(),0.0)/V.size();

		// Calculate summation.
		double SUM=0;
		for (auto item:V) SUM+=pow((item-avr),2);

		// Return average and standard deviation.
		return {avr,sqrt(SUM/(V.size()-1))};
	}
	else {

		// Calculate x average.
		// Count weight!=0 terms.
		// Sum weights.
		int N=0;
		double SumW=0,avr=0;
		for (size_t i=0;i<V.size();++i){
			SumW+=fabs(W[i]);
			avr+=V[i]*W[i];
			if (W[i]!=0) ++N;
		}
		avr/=SumW;

		if (N==0) return {};
		if (N==1) return {avr,0};

		// Calculate summation.
		double SUM=0;
		for (size_t i=0;i<V.size();++i){
			double flag=(W[i]>0?1:-1);
			SUM+=fabs(W[i])*pow(flag*V[i]-avr,2);
		}

		// Return standard deviation.
		return {avr,sqrt(SUM*N/(N-1)/SumW)};
	}
}
