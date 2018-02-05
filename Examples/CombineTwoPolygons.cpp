#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<string>

#include<ASU_tools.hpp>

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
	CombineTwoPolygons(Polygons[0],Polygons[1]);

	for (auto item:Polygons[0])
        cout << item.first << " " << item.second << endl;

    return 0;    
}
