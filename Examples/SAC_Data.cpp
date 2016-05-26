#include<iostream>
#include<fstream>
#include<sstream>
#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<string>
#include<SAC_Data.hpp>

using namespace std;

int main(){

	string A="200608250044.CN.LMN.BHT.sac";
	SAC_Data tmpdata;


	if (!(A >> tmpdata)){
		cout << "Read Error !" << endl;
	}

// 	cout << tmpdata.filename << endl;
// 	cout << tmpdata.delta << endl;
// 	cout << tmpdata.BeginTime << endl;
// 	cout << tmpdata.NPTS << endl;

	return 0;
}
