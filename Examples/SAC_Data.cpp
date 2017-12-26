#include<iostream>
#include<fstream>
#include<sstream>
#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<string>

#include<ASU_tools.hpp>
#include<SAC_Data.hpp>

using namespace std;

int main(){

	string A="data/200608250044.CN.LMN.BHT.sac";
	SAC_Data tmpdata;


	if (!(A >> tmpdata)){
		cout << "Read Error !" << endl;
	}

	cout << "FileName: " << tmpdata.filename << endl;
// 	cout << "Sampling rate: " << tmpdata.delta << endl;
// 	cout << "BeginTime: " << tmpdata.BeginTime << endl;
	cout << "NPTS: " << tmpdata.NPTS << endl;

	return 0;
}
