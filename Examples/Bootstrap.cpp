#include<iostream>
#include<fstream>
#include<sstream>
#include<string>

#include<ASU_tools.hpp>

using namespace std;

int main(){

    // Read input.
	vector<vector<double>> A,BootDetails;
	vector<double> Tmp;
	double tmpval;
	string line;
	ifstream fpin("data/Bootstrap_in");
	while (getline(fpin,line)){
		stringstream ss(line);
		Tmp.clear();
		while (ss >> tmpval)
			Tmp.push_back(tmpval);
		A.push_back(Tmp);
	}
	fpin.close();

    // Use function.
    auto ans=Bootstrap(A,5000,BootDetails,{});

    // Output.
	ofstream fpout("data/Bootstrap_out");
	for (auto &item: BootDetails){
		for (auto &item2: item)
			fpout << item2 << " ";
		fpout << "\n";
	}
	fpout.close();

	fpout.open("data/Bootstrap_out_avr_std");
	for (size_t i=0;i<ans.first.size();++i)
		fpout << ans.first[i] << " " << ans.second[i] << "\n";
	fpout.close();

    return 0;
}
