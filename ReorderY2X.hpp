#ifndef ASU_REORDERY2X
#define ASU_REORDERY2X

#include<vector>

/*************************************************
 * This C++ template reorder an input 2D array.
 *
 * X direction is the horizontal direction (the second
 * index, increasing along the right hand side);
 * Y direction is the vertical direction (the first
 * index, increasing along downward);
 *
 * Y2X here means: input is first counted along the
 * Y direction, then return an array which is in the
 * same dimension but counted along the X direction.
 *
 * e.g.
 *
 *    Input                                 Output
 *
 *    1 2 3                                 1 4 2
 *    4 5 6     -->   1 4 2 5 3 6    -->    5 3 6
 *
 * input(s):
 * const vector<vector<T>> &p  ----  Input 2D array.
 *
 * return(s):
 * vector<vector<T>> ans  ----  Reordered 2D array.
 *
 * Shule Yu
 * Jan 21 2018
 *
 * Key words: reorder, y2x
*************************************************/

template <typename T>
std::vector<std::vector<T>> ReorderY2X(const std::vector<std::vector<T>> &p){

    std::vector<std::vector<T>> ans;

    // Check array size.
    if (p.empty() || p[0].empty()) return ans;

    // fill in the output array.
    int m=p.size(),n=p[0].size();
    ans.resize(m,std::vector<T> (n,0));

    for (int i=0;i<n;++i)
        for (int j=0;j<m;++j)
            ans[(i*m+j)/n][(i*m+j)%n]=p[j][i];

    return ans;
}

#endif
