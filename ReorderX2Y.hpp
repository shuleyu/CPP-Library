#ifndef ASU_REORDERX2Y
#define ASU_REORDERX2Y

#include<iostream>
#include<vector>

/*************************************************
 * This C++ template reorder a input 2D array.
 * Keep the dimension of the array. X direction
 * is the horizontal direction (the second index,
 * increasing along the right hand side);
 * Y direction is the vertical direction (the first
 * index, increasing along downward);
 *
 * X2Y means originally count along x direction and
 * returned array count along y direction.
 *
 * e.g.
 *
 *    1 2 3                   1 3 5
 *    4 5 6        -->        2 4 6
 *
 * const vector<vector<T>> &p  ----  Input 2D array.
 *
 * Shule Yu
 * Jan 21 2018
 *
 * Key words: reorder, x2y
*************************************************/

template <class T>
std::vector<std::vector<T>> ReorderX2Y(const std::vector<std::vector<T>> &p){

    std::vector<std::vector<T>> ans;

    // Check array size.
    if (p.empty() || p[0].empty()) return ans;


    // fill in the output array.
    int m=p.size(),n=p[0].size();
    ans.resize(m,std::vector<T> (n,0));

    for (int i=0;i<m;++i)
        for (int j=0;j<n;++j)
            ans[(i*n+j)%m][(i*n+j)/m]=p[i][j];

    return ans;
}

#endif
