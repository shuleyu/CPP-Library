#include<iostream>
#include<fstream>
#include<iomanip>

#include<AK135.hpp>
#include<GMT.hpp>

using namespace std;

int main(){

    vector<double> d,r,p,s,q,u;
    for (double depth=0.0;depth<6371;depth+=1){
        d.push_back(depth);
        r.push_back(Drho_ak135(depth));
        p.push_back(Dvp_ak135(depth));
        s.push_back(Dvs_ak135(depth));
    }

    string outfile="AK135.ps";
    GMT::set("PS_MEDIA 6ix10i");
    GMT::BeginPlot(outfile);
    GMT::MoveReferencePoint(outfile,"-Xf1i -Yf1i");

    GMT::psbasemap(outfile,"-JX4i/-8i -R0/15/0/6371 -Bxa5f1 -Bya500f100 -BWSne -O -K");
    GMT::psxy(outfile,r,d,"-J -R -W0.5p,purple -O -K");
    GMT::psxy(outfile,p,d,"-J -R -W0.5p,blue -O -K");
    GMT::psxy(outfile,s,d,"-J -R -W0.5p,red -O -K");

    GMT::SealPlot(outfile);

    return 0;
}
