#ifndef ASU_MESHGRID
#define ASU_MESHGRID

#include<iostream>
#include<vector>

#include<CreateGrid.hpp>

/***********************************************************
 * This C++ template returns the meshing result in form of
 * of a vector.
 *
 * const vector<vector<T>> &p     ----  Input mesh parameters.
 *
 *                                       items in p are vectors of size 3,
 *                                       indicating lowerbound, upperbound and Para
 *                                       for this dimension.
 *
 * int                     mode  ----  grid creating mode, see CreateGrid.hpp
 *                                       (-1 or -2 indicate 1 or 2)
 *
 * return:
 * vector<vector<double>> ans  ----  1D vector including all the mesh points.
 *                                   each element is a vector of p.size().
 *
 * Shule Yu
 * Jan 23 2018
 *
 * Key words: mixing, mesh grid.
***********************************************************/

template<class T>
std::vector<std::vector<double>> MeshGrid(const std::vector<std::vector<T>> &p, int mode){

    // check mode.
	if (abs(mode)>2){
        std::cerr <<  __func__ << "; Error: mode error ..." << std::endl;
		return {};
	}
    mode=abs(mode);

    // check input parameters.
    int n=p.size();

    for (int i=0;i<n;++i) {
        if (p[i].size()!=3) {
            std::cerr <<  __func__ << "; Error: some input set doesn't have 3 parameters..." << std::endl;
            return {};
        }
    }

    std::vector<std::vector<double>> ans;

    for (int i=0;i<n;++i) {

        int N=ans.size();
        auto grid=CreateGrid(p[i][0],p[i][1],p[i][2],mode);

        for (auto &item:grid){
            if (N==0) ans.push_back({item});
            else {
                for (int j=0;j<N;++j){
                    ans.push_back(ans[j]);
                    ans.back().push_back(item);
                }
            }
        }
        ans.erase(ans.begin(),ans.begin()+N);
    }

	return ans;
}

#endif
