#ifndef ASU_REORDERUSEINDEX
#define ASU_REORDERUSEINDEX

#include<iterator>
#include<vector>

/*************************************************
 * This C++ template takes a pair of iterators and
 * the original array indices to reorder the input
 * vector.
 *
 * intput(s):
 * vector<T1>::iterator      Begin  ----  Reorder begin position.
 * vector<T1>::iterator      End    ----  Reorder end position.
 * const vector<std::size_t> &bi    ----  The indices of original position.
 *
 * Shule Yu
 * Aug 07 2018
 *
 * Key words: reorder, index
*************************************************/

template <typename T>
void ReorderUseIndex(T Begin, T End, const std::vector<std::size_t> &bi) {

    // Check array size.
    int n=std::distance(Begin,End);
    if (n<(int)bi.size())
        throw std::runtime_error("Error in ReorderUseIndex: input index array longer than data array.");
    if (n!=(int)bi.size())
        std::cerr <<  "Warning in " << __func__ << ": input index array size doesn't match data array size ..." << std::endl;

    std::vector<bool> flag(bi.size(),false);

    for (std::size_t i=0;i<bi.size();++i){
        if (flag[i]) continue;
        auto cache=*(Begin+i);
        std::size_t j=i;
        while (!flag[bi[j]]){
            flag[j]=true;
            *(Begin+j)=*(Begin+bi[j]);
            j=bi[j];
        }
        *(Begin+j)=cache;
    }
    return;
}

#endif
