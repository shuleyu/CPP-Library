#include<cstdio>
#include<iostream>
#include<vector>
#include<cmath>
#include<algorithm>
#include<numeric>

#include<ASU_tools.hpp>

using namespace std;

/**************************************************************
 * This C function calculate the cross-difference of given two
 * vectors. Note how we customized it: for the overlaping parts,
 *
 * s2 are the shifting signal.
 * s2 are only shifting within s1.
 *
 * 1. we subtract the mean of the overlapping part.
 * 2. then calculate the averge value of the sum of the absolute 
 * difference.
 *
 * (for STD2CC:)
 * same concept as STDCC, only the input are two matrices.
 *
 * Shule Yu
 * Mar 30 2017
 *
 * Key words: cross-difference.
****************************************************************/

void STDCC(vector<double> &s1, vector<double> &s2, int &shift, double &STD){

	int xlen=s1.size(),ylen=s2.size();

    // If signals are too short, return an error flag.
    if (xlen<=1 || ylen<=1 || xlen<ylen){
		cout << "In " << __func__ << ": X/Y Sequence length error !" << endl;
        return;
	}

    // Shifting and evaluating STD at every delay value.

	STD=numeric_limits<double>::max();
    for (int delay=0;delay<xlen-ylen+1;delay++){

		// calculate difference.
		vector<double> Tmp;
        for (int i=0;i<ylen;i++){
			Tmp.push_back(s1[i+delay]-s2[i]);
		}

		// calculate std.
		double avg=accumulate(Tmp.begin(),Tmp.end(),0.0)/Tmp.size();

		double R=0.0;
        for (size_t i=0;i<Tmp.size();i++){
			R+=pow((Tmp[i]-avg),2);
		}
		R=sqrt(R/(Tmp.size()-1));

        if (STD>R){
            STD=R;
            shift=delay;
        }
    }
    return;
}

void STD2CC(vector<vector<double>> &s1, vector<vector<double>> &s2, int &mi, int &mj, double &STD){

	int xi=s1.size(),yi=s2.size(),xj=s1[0].size(),yj=s2[0].size();

    // If signals are too short, return an error flag.
    if (xi<=1 || xj<=1 || yi<=1 || yj<=1 || xi<yi || xj<yj){
		cout << "In " << __func__ << ": X/Y Matrices size error !" << endl;
        return;
	}

    // Shifting and evaluating STD at every shift pair.

	STD=numeric_limits<double>::max();

    for (int ti=0;ti<xi-yi+1;ti++){

		for (int tj=0;tj<xj-yj+1;tj++){

			vector<double> Tmp;


			// calculate difference.
			for (int i=0;i<yi;i++){
				for (int j=0;j<yj;j++){
					Tmp.push_back(s1[i+ti][j+tj]-s2[i][j]);
				}
			}

			// calculate std.
			double avg=accumulate(Tmp.begin(),Tmp.end(),0.0)/Tmp.size();

			double R=0.0;
			for (size_t i=0;i<Tmp.size();i++){
				R+=pow((Tmp[i]-avg),2);
			}
			R=sqrt(R/(Tmp.size()-1));

			if (STD>R){
				STD=R;
				mi=ti;
				mj=tj;
			}
		}
    }
    return;
}
