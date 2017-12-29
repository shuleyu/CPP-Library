#include<iostream>
#include<fstream>
#include<string>
#include<algorithm>

using namespace std;

int main(int argc, char **argv){

	string filename(argv[1]),line;
	ifstream fpin(filename);
	while(getline(fpin,line)){
		auto I=string::npos;
		while ((I=line.find('\t'))!=string::npos){
			line[I]=' ';
			line.insert(I,3,' ');
		}
		cout << line << endl;
	}
	return 0;
}

