#ifndef ASU_MESHGRID
#define ASU_MESHGRID

#include<iostream>
#include<vector>

#include<CreateGrid.hpp>
#include<Enumerate.hpp>

/***********************************************************
 * This C++ template mesh input ingredients.
 *
 * For example, ingredients for x is (0,10,0.1) --- p[0]
 *              ingredients for y is (0,5,0.2)  --- p[1]
 *
 * This will results in a 2D vector (ans):
 * (0,0)
 * (0,0.2)
 * (0,0.4)
 * ...
 * ...
 * (0,4.8)
 * (0,5)
 * (0.1,0)
 * (0.1,0.2)
 * (0.1,0.4)
 * ...
 * ...
 * (10,4.6)
 * (10,4.8)
 * (10,5)
 *
 * input(s):
 * const vector<vector<T>> &p     ----  Input parameters.
 *
 *                                      p[i].size() == 3. p[i] is the ingredients for the ith dimension.
 *                                      p[i][0] indicating lowerbound
 *                                      p[i][1] indicating upperbound
 *                                      p[i][2] indicating Para (see CreateGrid.hpp)
 *
 * const int               &mode  ----  grid creating mode, see CreateGrid.hpp
 *                                      (-1 is converted to 1, -2 is converted to 2)
 *
 * return(s):
 * vector<vector<double>> ans  ----  2D vector including all the mesh points.
 *                                   each element is a vector of p.size().
 *
 * Shule Yu
 * Jan 23 2018
 *
 * Key words: mixing, mesh grid.
***********************************************************/

template<typename T>
std::vector<std::vector<double>> MeshGrid(const std::vector<std::vector<T>> &p, const int &mode){


    // check mode.
    if (abs(mode)>2){
        std::cerr <<  "Error in " << __func__ << ": mode error ..." << std::endl;
        return {};
    }

    // check input parameters.
    int n=p.size();
    if (n==0) return {};

    for (int i=0;i<n;++i) {
        if (p[i].size()!=3) {
            std::cerr <<  "Error in " << __func__ << ": some input set doesn't have 3 parameters..." << std::endl;
            return {};
        }
    }


    std::vector<std::vector<T>> grids;
    for (int i=0;i<n;++i)
        grids.push_back(CreateGrid(p[i][0],p[i][1],p[i][2],abs(mode)));
    return Enumerate(grids);
}

#endif
