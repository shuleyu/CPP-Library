#ifndef ASU_AMPLITUDE
#define ASU_AMPLITUDE

#include<iostream>
#include<iterator>
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
std::pair<class std::iterator_traits<T>::value_type,std::vector<T>> Amplitude(const T PBegin, const T PEnd){

    // Check array size.
    if (PBegin==PEnd) {
        std::cerr <<  "Error in " << __func__ << ": input array size is zero ..." << std::endl;
        return {};
    }

    typedef typename std::iterator_traits<T>::value_type DataType;

    DataType amp=(*PBegin)>0?(*PBegin):-(*PBegin);
    std::pair<DataType,std::vector<T>> ans{amp,{PBegin}};

    for (auto it=next(PBegin);it!=PEnd;++it){
        amp=(*it)>0?(*it):-(*it);

        if (ans.first<amp) {
            ans.first=amp;
            ans.second={it};
        }
        else if (ans.first==amp) ans.second.push_back(it);
    }

    return ans;
}

template <class T>
std::pair<T,std::vector<std::size_t>> Amplitude(const std::vector<T> &p){
    auto res=Amplitude(p.begin(),p.end());
    std::vector<std::size_t> ans;
    for (const auto &item:res.second) ans.push_back(std::distance(p.begin(),item));
    return {res.first,ans};
}

#endif
