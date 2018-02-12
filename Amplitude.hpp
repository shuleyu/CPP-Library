#ifndef ASU_AMPLITUDE
#define ASU_AMPLITUDE

#include<iostream>
#include<vector>

/*************************************************
 * This C++ template returns the amplitude and the
 * positions of that amplitude.
 *
 * input(s):
 * const vector<T> &p  ----  Input array.
 *
 * output(s):
 * pair<T,vector<size_t>> ans
 *                     ----  ans.first is the ampiltude.
 *                           ans.second is the position(s)
 *                           this amplitude occurs in p.
 *
 * Shule Yu
 * Dec 19 2017
 *
 * Key words: amplitude
*************************************************/

template <class T>
std::pair<T,std::vector<size_t>> Amplitude(const std::vector<T> &p){

    // Check array size.
    if (p.empty()) {
        std::cerr <<  "Error in " << __func__ << ": input array size is zero ..." << std::endl;
        return {};
    }

    T amp=p[0]>0?p[0]:-p[0];
    std::pair<T,std::vector<size_t>> ans{amp,{0}};

    for (size_t i=1;i<p.size();++i){
        amp=p[i]>0?p[i]:-p[i];

        if (ans.first<amp) {
            ans.first=amp;
            ans.second={i};
        }
        else if (ans.first==amp) ans.second.push_back(i);
    }

    return ans;
}

#endif
