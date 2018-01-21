// In development
#ifndef ASU_COMBINETWOPOLYGONS
#define ASU_COMBINETWOPOLYGONS

#include<iostream>
#include<vector>
#include<algorithm>
#include<numeric>
#include<cmath>

#include<PointsInPolygon.hpp>

/*****************************************************************
 * This C++ function use "PointsInPolygons" from ASU_tools
 * to combine 2 polygons(The union sets of given polygons).
 *
 * vector<vector<pair<double,double>>> &All  ----  Input polygons 1; Output combined polygon.
 * vector<vector<pair<double,double>>> New  ----  Input polygons 2.
 *
 * Each polygon is denoted by a vecotr of pairs of coordinates (x,y).
 *
 * return:
 * If the input two polygons are separated, retrun false.
 * Otherwise return true. All is modified to the combine result.
 *
 * Shule Yu
 * Jan 19 2018
 *
 * Key words: polygon, union set.
*****************************************************************/

template<class T>
bool CombineTwoPolygons(std::vector<std::pair<T,T>> &All,std::vector<std::pair<T,T>> New){

	int m=All.size(),n=New.size();

	auto res=PointsInPolygon(All,New);
	auto res2=PointsInPolygon(New,All);
	int nn=std::count(res.begin(),res.end(),true);
	int mm=std::count(res2.begin(),res2.end(),true);

	// One of the polygon contains another.
	if (nn==n) return true;
	if (mm==m){
		All=New;
		return true;
	}

	// Two polygons are seperated.
	if (mm==0 && nn==0) return false;

    // Combine this two polygons.

	// Find the New Begin/End points in All.

	std::vector<int> NB_All,NE_All;
	for (int i=0;i<m;i++){
		if (res2[i] && !res2[(i+1)%m]) NB_All.push_back((i+1)%m);
		if (!res2[i] && res2[(i+1)%m]) NE_All.push_back(i);

	}

	// Find the New Begin/End points in New.

	std::vector<int> NB_New,NE_New;
	for (int i=0;i<n;i++){
		if (res[i] && !res[(i+1)%n]) NB_New.push_back((i+1)%n);
		if (!res[i] && res[(i+1)%n]) NE_New.push_back(i);

	}

    if (NB_All.size()>NB_New.size()) {

        for (int i=0;i<n;++i) {
        }
        
    }

// 	if (flag) rotate(NE_New.begin(),NE_New.begin()+1,NE_New.end());

// std::cout  << "HERE" << std::endl;
// for  (auto item:NB_New) std::cout << New[item].first << " " << New[item].second << std::endl;

	// Bug ! Bug ! .... Pia ! Pia !
	// ignore small sections.
// 	int n1=NB_All.size(),n2=NB_New.size();
// 	if (n1!=n2){
// 		for (int i=n1-1;i>=0;i--){
// 			int x=NE_All[i]-NB_All[i];
// 			if (x<0) x+=All.size();
// 			if (x<3){
// 				NB_All.erase(NB_All.begin()+i);
// 				NE_All.erase(NE_All.begin()+i);
// 			}
// 		}
// 		for (int i=n2-1;i>=0;i--){
// 			int x=NE_New[i]-NB_New[i];
// 			if (x<0) x+=New.size();
// 			if (x<3){
// 				NB_New.erase(NB_New.begin()+i);
// 				NE_New.erase(NE_New.begin()+i);
// 			}
// 		}
// 	}


	// Bug bug.. buzz..buzz... pia !
// 	n1=NB_All.size(),n2=NB_New.size();
// 	if (n1!=n2){
// 		std::vector<int> &p=(n1>n2?NB_All:NB_New);
// 		std::vector<int> &p2=(n1>n2?NE_All:NE_New);
// 		std::vector<std::pair<double,double>> &P=(n1>n2?All:New);
// 		int Diff=abs(n1-n2),N=(n1>n2?m:n);
// 
// 		rotate(p2.begin(),p2.end()-1,p2.end());
// 		std::vector<std::pair<int,int>> pp;
// 		for (size_t i=0;i<p.size();i++){
// 			pp.push_back({p[i]-p2[i],i});
// 			if (pp.back().first<0) pp.back().first+=N;
// 		}
// 		sort(pp.begin(),pp.end());
// 
// 		std::vector<int> t;
// 		for (int i=0;i<Diff;i++){
// 			t.push_back(pp[i].second);
// 		}
// 
// 		sort(t.begin(),t.end(),std::greater<int>());
// 		for (auto item: t){
// 			for(size_t i=p2[item]+1;i<(p[item]<p2[item]?(p[item]+P.size()):p[item]);i++){
// 				int xx=(i>=P.size()?i-P.size():i);
// 				P[xx].first=std::numeric_limits<double>::quiet_NaN();
// 			}
// 			p.erase(p.begin()+item);
// 			p2.erase(p2.begin()+item);
// 		}
// 		rotate(p2.begin(),p2.begin()+1,p2.end());
// 	}

	// Find where these two shape meet.
	int N=NB_All.size();
	std::vector<int> X(N),Y(N);

	for (int i=0;i<N;++i){

		double MinDist=std::numeric_limits<double>::max();
		double MinDist2=std::numeric_limits<double>::max();

		for (int j=0;j<N;++j){

            double dx=All[NE_All[i]].first-New[NB_New[j]].first;
            double dy=All[NE_All[i]].second-New[NB_New[j]].second;
            double dist=dx*dx+dy*dy;

			if (MinDist>dist){
				MinDist=dist;
				X[i]=j;
			}

            dx=New[NE_New[i]].first-All[NB_All[j]].first;
            dy=New[NE_New[i]].second-All[NB_All[j]].second;
            dist=dx*dx+dy*dy;

			if (MinDist2>dist){
				MinDist2=dist;
				Y[i]=j;
			}
		}
	}

	// For Non-simply connected result.
	// we ignore the center parts.

	std::vector<std::vector<std::pair<double,double>>> Polygons;
	std::vector<bool> pPB(N);
	decltype(pPB.begin()) it;
	while((it=find(pPB.begin(),pPB.end(),false))!=pPB.end()){
		int PB=(it-pPB.begin());
		std::vector<std::pair<double,double>> All_New;
		while(!pPB[PB]){
			pPB[PB]=true;

			for (int i=0;i<m;i++){
				int xx=(NB_All[PB]+i)%m;
				if (!std::isnan(All[xx].first)) All_New.push_back(All[xx]);
				if (xx==NE_All[PB]) break;
			}
			for (int i=0;i<n;i++){
				int xx=(NB_New[X[PB]]+i)%n;
				if (!std::isnan(New[xx].first)) All_New.push_back(New[xx]);
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

	return true;
}

#endif
