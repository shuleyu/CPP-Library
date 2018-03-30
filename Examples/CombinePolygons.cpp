#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<string>

#include<CombinePolygons.hpp>

using namespace std;

int main(){

	ifstream fpin("data/CombinePolygons.txt");

	// Read in.
	
	vector<vector<pair<double,double>>> Polygons;

	string tmpstr;
	vector<pair<double,double>> CurP;

	while(getline(fpin,tmpstr)){

		if (tmpstr[0]=='>'){
			if (CurP.size()>0) Polygons.push_back(CurP);
			CurP.clear();
			continue;
		}

		stringstream ssin(tmpstr);
		pair<double,double> p;
		ssin >> p.first >> p.second;
		CurP.push_back(p);

	}
	Polygons.push_back(CurP);

    // Use function.
	auto Ans=CombinePolygons(Polygons);

	for (auto item:Ans){
		for (auto item2:item){
			cout << item2.first << " " << item2.second << endl;
		}
		cout << ">" << endl;
	}
    
    return 0;    
}
