#include<iostream>
#include<vector>
#include<algorithm>
#include<numeric>
#include<cmath>

#include<ASU_tools.hpp>
extern "C"{
#include<ASU_tools.h>
}

using namespace std;

/*****************************************************************
 * This C++ function use "points_in_polygon.fun.c" from ASU_tools
 * to combine several polygons into another set of polygons.(The
 * union sets of given polygons)
 *
 * vector<vector<pair<double,double>>> P    ----   Input polygons.
 *
 * Each polygon is denoted by a vecotr of pairs of coordinates (x,y).
 *
 * Return value.
 * vector<vector<pair<double,double>>> Ans  ----   Output polygons.
 *
 * Shule Yu
 * Mar 14 2017
 *
 * Key words: polygon, union set.
*****************************************************************/

double CalcDist(pair<double,double> p1,pair<double,double> p2){
	return sqrt(pow(p1.first-p2.first,2)+pow(p1.second-p2.second,2));
}

bool CombineTwoPolygons(vector<pair<double,double>> &All,vector<pair<double,double>> &New){

	auto res=PointsInPolygon(All,New);
	auto res2=PointsInPolygon(New,All);
	int m=All.size(),n=New.size();

	int nn=count(res.begin(),res.end(),true);
	int mm=count(res2.begin(),res2.end(),true);

	// One of the polygon contains another.
	if (nn==n) return true;
	if (mm==m){
		All=New;
		return true;
	}

	// Two polygons are seperated.
	if (mm==0 || nn==0) return false;

	// Find the new begin/end points in All.
	bool flag=false;
	vector<int> NB_All,NE_All;
	for (int i=0;i<m;i++){
		if (res2[i] && !res2[(i+1)%m]){
			NB_All.push_back((i+1)%m);
		}
		if (!res2[i] && res2[(i+1)%m]){
			NE_All.push_back(i);
		}
		if (NB_All.size()==0 && NE_All.size()==1) flag=true;
	}
	if (flag) rotate(NE_All.begin(),NE_All.begin()+1,NE_All.end());


	// Find the new begin/end points in New.
	flag=false;
	vector<int> NB_New,NE_New;
	for (int i=0;i<n;i++){
		if (res[i] && !res[(i+1)%n]){
			NB_New.push_back((i+1)%n);
		}
		if (!res[i] && res[(i+1)%n]){
			NE_New.push_back(i);
		}
		if (NB_New.size()==0 && NE_New.size()==1) flag=true;
	}
	if (flag) rotate(NE_New.begin(),NE_New.begin()+1,NE_New.end());

	// Bug ! Bug ! .... Pia ! Pia !
	// ignore small sections.
	int n1=NB_All.size(),n2=NB_New.size();
	if (n1!=n2){
		for (int i=n1-1;i>=0;i--){
			int x=NE_All[i]-NB_All[i];
			if (x<0) x+=All.size();
			if (x<3){
				NB_All.erase(NB_All.begin()+i);
				NE_All.erase(NE_All.begin()+i);
			}
		}
		for (int i=n2-1;i>=0;i--){
			int x=NE_New[i]-NB_New[i];
			if (x<0) x+=New.size();
			if (x<3){
				NB_New.erase(NB_New.begin()+i);
				NE_New.erase(NE_New.begin()+i);
			}
		}
	}


	// Bug bug.. buzz..buzz... pia !
	n1=NB_All.size(),n2=NB_New.size();
	if (n1!=n2){
		vector<int> &p=(n1>n2?NB_All:NB_New);
		vector<int> &p2=(n1>n2?NE_All:NE_New);
		vector<pair<double,double>> &P=(n1>n2?All:New);
		int Diff=abs(n1-n2),N=(n1>n2?m:n);

		rotate(p2.begin(),p2.end()-1,p2.end());
		vector<pair<int,int>> pp;
		for (size_t i=0;i<p.size();i++){
			pp.push_back(make_pair(p[i]-p2[i],i));
			if (pp.back().first<0) pp.back().first+=N;
		}
		sort(pp.begin(),pp.end());

		vector<int> t;
		for (int i=0;i<Diff;i++){
			t.push_back(pp[i].second);
		}

		sort(t.begin(),t.end(),greater<int>());
		for (auto item: t){
			for(size_t i=p2[item]+1;i<(p[item]<p2[item]?(p[item]+P.size()):p[item]);i++){
				int xx=(i>=P.size()?i-P.size():i);
				P[xx].first=numeric_limits<double>::quiet_NaN();
			}
			p.erase(p.begin()+item);
			p2.erase(p2.begin()+item);
		}
		rotate(p2.begin(),p2.begin()+1,p2.end());
	}

	// Find where these two shape meet.
	int N=NB_All.size();
	vector<int> X(N),Y(N);

	for (int i=0;i<N;i++){

		double MinDist=numeric_limits<double>::max();
		double MinDist2=numeric_limits<double>::max();

		for (int j=0;j<N;j++){

			double dist=CalcDist(All[NE_All[i]],New[NB_New[j]]);

			if (MinDist>dist){
				MinDist=dist;
				X[i]=j;
			}

			double dist2=CalcDist(New[NE_New[i]],All[NB_All[j]]);

			if (MinDist2>dist2){
				MinDist2=dist2;
				Y[i]=j;
			}
		}
	}

	/**************
	// Method 1. Create new polygon. 
	vector<pair<double,double>> All_New;
	for (int PB=0;PB<N;PB++){
		for (int i=0;i<m;i++){
			int xx=(NB_All[PB]+i)%m;
			if (!isnan(All[xx].first)) All_New.push_back(All[xx]);
			if (xx==NE_All[PB]) break;
		}
		for (int i=0;i<n;i++){
			int xx=(NB_New[X[PB]]+i)%n;
			if (!isnan(New[xx].first)) All_New.push_back(New[xx]);
			if (xx==NE_New[X[PB]]) break;
		}
	}

	All=All_New; 

	******************/

// 	/**************

	// Method 2.
	// For Non-simply connected result.
	// we ignore the center parts.

	vector<vector<pair<double,double>>> Polygons;
	vector<bool> pPB(N);
	decltype(pPB.begin()) it;
	while((it=find(pPB.begin(),pPB.end(),false))!=pPB.end()){
		int PB=(it-pPB.begin());
		vector<pair<double,double>> All_New;
		while(!pPB[PB]){
			pPB[PB]=true;

			for (int i=0;i<m;i++){
				int xx=(NB_All[PB]+i)%m;
				if (!isnan(All[xx].first)) All_New.push_back(All[xx]);
				if (xx==NE_All[PB]) break;
			}
			for (int i=0;i<n;i++){
				int xx=(NB_New[X[PB]]+i)%n;
				if (!isnan(New[xx].first)) All_New.push_back(New[xx]);
				if (xx==NE_New[X[PB]]) break;
			}

			PB=Y[X[PB]];
		}
		Polygons.push_back(All_New);
	}

	size_t MaxL=Polygons[0].size(),j=0;
	for (size_t i=1;i<Polygons.size();i++){
		if (MaxL<Polygons[i].size()){
			MaxL=Polygons[i].size();
			j=i;
		}
	}
	All=Polygons[j];

// 	******************/

	return true;
}

vector<vector<pair<double,double>>> CombinePolygons(vector<vector<pair<double,double>>> P){

	vector<vector<pair<double,double>>> ans;

	int NP=P.size();

	vector<bool> V(NP);

	for (int i=0;i<NP;i++){

		if (V[i]) continue;
		V[i]=true;

		vector<pair<double,double>> CurP=P[i];

		int Cnt=0,Cnt_Prev=-1;
		while (Cnt!=Cnt_Prev){
			Cnt_Prev=Cnt;
			for (int j=0;j<NP;j++){
				if (V[j]) continue;
				V[j]=CombineTwoPolygons(CurP,P[j]);
				if (V[j]) Cnt++;
			}
		}

		ans.push_back(CurP);
	}

    return ans;
}

