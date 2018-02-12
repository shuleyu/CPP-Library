#include<fstream>
#include<iomanip>

#include<ASU_tools.hpp>

using namespace std;

int main(){

	// Make bins.
	double BinInc=5;
	int Nb=40/BinInc+1;
	vector<pair<double,double>> b;
	for (int i=0;i<Nb;++i)
		b.push_back({0,-20+i*BinInc});
	vector<double> r(b.size(),2);
	

	// Make points.
	double PIncx=0.5,PIncy=0.5;
	int Npx=20/PIncx+1;
	int Npy=50/PIncy+1;
	vector<pair<double,double>> p;
	for (int i=0;i<Npy;++i)
		for (int j=0;j<Npx;++j)
			p.push_back({-10+PIncx*j,-25+PIncy*i});

	// Use function.
	auto ans=CircleBin(p,b,r);

	// Output.
	ofstream fpout;

    fpout.open("data/CircleBin_index");
	for (int i=0;i<Npy;++i){
		for (int j=0;j<Npx;++j){
			bool flag=true;
			for (int k=0;k<Nb;++k){
				if (ans.second[(Npy-1-i)*Npx+j][k]){
					fpout << setw(3) << left << k+1;
					flag=false;
					break;
				}
			}
			if (flag) fpout << "0  ";
		}
		fpout << "\n";
	}
	fpout.close();

    fpout.open("data/CircleBin_bincenter");
	for (int i=0;i<Nb;++i)
		fpout << ans.first[i].first << " " << ans.first[i].second << "\n";
	fpout.close();

    return 0;
}
