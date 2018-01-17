#ifndef ASU_CIRCLEBIN
#define ASU_CIRCLEBIN

#include<GcpDistance.hpp>

/*********************************************************
 * This C++ template compute circle geographyic binning
 * results. Update the bin center to the average position.
 *
 * const vector<pair<T1,T1>> &p    ----  Sampling lon/lat.
 * vector<pair<T2,T2>>       &b    ----  Bin center lon/lat.
 * const vector<T3>          &r    ----  Bin radius (in deg).
 * vector<vector<bool>>      &ans  ----  Result matrix.
 *
 * Shule Yu
 * Dec 26 2017
 *
 * Note: original bin center locations destroyed.
 * Key words: geographic bin, circle, update.
***********************************************************/

template <class T1, class T2, class T3>
void CircleBin(const std::vector<std::pair<T1,T1>> &p, std::vector<std::pair<T2,T2>> &b, const std::vector<T3> &r, std::vector<std::vector<bool>> &ans){

	// Check array size.
	if (b.size()!=r.size()) {
		std::cerr <<  __func__ << "; Error: bins and radius size don't match ..." << std::endl;
		return;
	}

	// Prepare output.
	size_t Np=p.size(),Nb=b.size();
	ans.clear();
	ans.resize(Np,std::vector<bool>(Nb,false));

    // For each bin, search which points are inside them.
	// Then update the bin center.
	for (size_t i=0;i<Nb;++i){

		T2 NewLon=0,NewLat=0;
		int Cnt=0;

		for (size_t j=0;j<Np;++j){
            if (GcpDistance(b[i].first,b[i].second,p[j].first,p[j].second)<=r[i]){
                ans[j][i]=true;
				NewLon+=p[j].first;
				NewLat+=p[j].second;
				++Cnt;
			}
        }
		b[i].first=NewLon/Cnt;
		b[i].second=NewLat/Cnt;
    }

    return;
}

#endif
