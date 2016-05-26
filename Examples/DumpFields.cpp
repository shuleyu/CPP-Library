#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<algorithm>

// This program is designed to be used with PDFs:
// i)  only contains checkboxs.
// ii) edited only by adobe acrobat (not even adobe reader).

using namespace std;

int main(int argc, char *argv[]){

	if (argc<2){
		cout << "Usage: ./executable [infile(s)]" << endl;
		return 1;
	}

	ifstream       infile;
	ofstream       outfile_unchecked,outfile_checked;
	string         key1("/Subtype/Widget/T"),key2("/Type/Annot");
	string         tmpstr,outfilename,tmp1,tmp2,item;
	size_t         found,found2,found3;
	int            flag,cnt;
	vector<string> List_checked,List_unchecked;

	for (cnt=1;cnt<argc;cnt++){

		outfilename=argv[cnt];

		// Open infile.
		infile.open(argv[cnt],ios::binary|ios::binary);
		if (!infile.is_open()){
			continue;
		}

		// Read from pdf and find the key words.
		while (infile >> tmpstr){

			found=tmpstr.find(key1.c_str());
			found2=tmpstr.find(key2.c_str());
			found3=tmpstr.find("/V");

			if (found!=string::npos && found2!=string::npos){

				item=tmpstr.substr(found+key1.size()+1,found2-found-key1.size()-2);

				if (found3!=string::npos){

					// Check the "uncheck" List, remove this item form it.
					for (auto &i: List_unchecked){
						if (i==item){
							i="";
						}
					}

					// Check the "check" List for duplicated item.
					flag=0;
					for (auto &i: List_checked){
						if (i==item){
							flag=1;
							break;
						}
					}

					if (flag==0){
						List_checked.push_back(item);
					}

				}
				else{
					// Check the "check" List, remove this item form it.
					for (auto &i: List_checked){
						if (i==item){
							i="";
						}
					}

					// Check the "uncheck" List for duplicated item.
					flag=0;
					for (auto &i: List_unchecked){
						if (i==item){
							flag=1;
							break;
						}
					}

					if (flag==0){
						List_unchecked.push_back(item);
					}
				}
			}
		}

		infile.close();

		sort(List_unchecked.begin(),List_unchecked.end());
		sort(List_checked.begin(),List_checked.end());

		// Output the unchecked boxes.
		tmp1=outfilename+".unchecked";
		outfile_unchecked.open(tmp1.c_str(),ios::out);
		for (auto i: List_unchecked){
			if (!i.empty()){
				outfile_unchecked << i << endl;
			}
		}
		List_unchecked.clear();
		outfile_unchecked.close();

		// Output the checked boxes.
		tmp2=outfilename+".checked";
		outfile_checked.open(tmp2.c_str(),ios::out);
		for (auto i: List_checked){
			if (!i.empty()){
				outfile_checked << i << endl;
			}
		}
		List_checked.clear();
		outfile_checked.close();


	}

	return 0;
}
