#ifndef ASU_COMPARESIGNAL
#define ASU_COMPARESIGNAL

#include<iostream>
#include<vector>
#include<cmath>

#include<CrossCorrelation.hpp>
#include<Normalize.hpp>

/***************************************************************************
 * This C++ template compares two input signals by returning 3 measurements:
 *
 * 1. Two measurements:
 *    A. Find the best fitting position using cross-correlation
 *       between the parts above input amplitude level (comparing portion).
 *    B. Amp_Diff, Calculate average |s1-s2| between overlapping part of the comparing portion.
 *    C. Amp_WinDiff, Calculate average |s1-s2| between overlapping part of original t1 ~ t2.
 *
 * 2. One measurement:
 *    A. Find the best fitting position using cross-correlation between t1 ~ t2 (comparison portion)
 *    B. Win_Diff, Calculate average |s1-s2| between overlapping part of the comparing portion (original t1 ~ t2).
 *
 * input(s):
 * const vector<T1> &s1        ----  Signal 1.
 * const double     &p1        ----  Signal 1 peak position.
 * const vector<T2> &s2        ----  Signal 2.
 * const double     &p2        ----  Signal 2 peak position.
 * const double     &delta     ----  Sampling rate of signal 1 & 2.
 * const double     &t1        ----  Time window begin (relative to their peaks, in sec.).
 * const double     &t2        ----  Time window end (relative to their peaks, in sec.).
 * const double     &AmpLevel  ----  CC compare part. (compare the part above this amplitude value)
 *
 * return(s):
 * SignalCompareResults ans  ----  Defined as above.
 *
 * Shule Yu
 * Jan 30 2018
 *
 * Note: The two signals should be the same sampling rate delta.
 *       The two signals should be pre-normalized and have the same polarity: s1[p1]==1 && s2[p2]==1
 *
 * Key words: compare signals
***************************************************************************/

struct SignalCompareResults{
    double Amp_CCC,Amp_Diff,Amp_WinDiff,Win_CCC,Win_Diff;
    int Amp_Shift,Win_Shift;
    SignalCompareResults () =default;
};
std::ostream &operator<<(std::ostream &os, const SignalCompareResults &item){
    os << "Compare results after shifted to the cross-correlation position"
          "using the part above AmpLevel: " << '\n';
    os << "    Shifting points: " << item.Amp_Shift << '\n';
    os << "    CCC: " << item.Amp_CCC << '\n';
    os << "    Difference between the overlapping part above AmpLevel: " << item.Amp_Diff << '\n';
    os << "    Difference between t1 ~ t2: " << item.Amp_WinDiff << '\n' << '\n';

    os << "Compare results after shifted to the cross-correlation position"
          "using the part between t1 ~ t2: " << '\n';
    os << "    Shifting points: " << item.Win_Shift << '\n';
    os << "    CCC: " << item.Win_CCC << '\n';
    os << "    Difference between t1 ~ t2: " << item.Win_Diff;
    return os;
}

template<typename T1,typename T2>
SignalCompareResults CompareSignal(const std::vector<T1> &s1, const std::size_t &p1,
                                   const std::vector<T2> &s2, const std::size_t &p2,
                                   const double &delta, const double &t1, const double &t2,
                                   const double &AmpLevel) {

    // check input.
    int P1=p1,P2=p2,n1=s1.size(),n2=s2.size();
    if (P1<0 || P1+1>=n1 || P2<0 || P2+1>=n2){
        std::cerr <<  "Error in " << __func__ << ": peak position error ..." << std::endl;
        return {};
    }

    int W1=(int)ceil(t1/delta),W2=(int)ceil(t2/delta);
    if (W1>=W2) {
        std::cerr <<  "Error in " << __func__ << ": Time window error ..." << std::endl;
        return {};
    }
    if(P1+W1<0 || P1+W1>=n1 || P1+W2<0 || P1+W2>=n1){
        std::cerr <<  "Error in " << __func__ << ": Time window error on signal 1 ..." << std::endl;
        return {};
    }
    if(P2+W1<0 || P2+W1>=n2 || P2+W2<0 || P2+W2>=n2){
        std::cerr <<  "Error in " << __func__ << ": Time window error on signal 2 ..." << std::endl;
        return {};
    }

    if (s1[P1]<0.95 || s2[P2]<0.95){
        std::cerr <<  "Error in " << __func__ << ": s1 or s2 not pre-normalized ..." << std::endl;
        return {};
    }

    // Find the begin/end of the part above AmpLevel on s1.
    int X_B,X_E;
    for (X_B=P1;X_B>=0;--X_B){
        if (s1[X_B]<AmpLevel){
            ++X_B;
            break;
        }
    }
    for (X_E=P1;X_E<n1;++X_E){
        if (s1[X_E]<AmpLevel) break;
    }

    // Find the begin/end of the part above AmpLevel on s2.
    int Y_B,Y_E;
    for (Y_B=P2;Y_B>=0;--Y_B){
        if (s2[Y_B]<AmpLevel){
            ++Y_B;
            break;
        }
    }
    for (Y_E=P2;Y_E<n2;++Y_E){
        if (s2[Y_E]<AmpLevel) break;
    }

    // Get rid of the square below AmpLevel, then normalize peak to 1.
    std::vector<double> C1(s1.begin()+X_B,s1.begin()+X_E),C2(s2.begin()+Y_B,s2.begin()+Y_E);
    for (auto &item:C1) item-=AmpLevel;
    for (auto &item:C2) item-=AmpLevel;
    Normalize(C1);
    Normalize(C2);

    // Do cross-correlation finding the best fitting position.
    SignalCompareResults ans;
    auto res=CrossCorrelation(C1,C2);
    int shift=res.first.first;
    ans.Amp_CCC=res.first.second;


    // Find the overlapping part.
    int X,Y,Len;
    if(shift<0){
        X=0;
        Y=-shift;
    }
    else{
        X=shift;
        Y=0;
    }
    Len=std::min(X_E-X_B-X,Y_E-Y_B-Y);
    X+=X_B;
    Y+=Y_B;

    // Calculate Amp_Diff.
    ans.Amp_Diff=0;
    for (int i=0;i<Len;++i) ans.Amp_Diff+=fabs(s1[X+i]-s2[Y+i]);
    ans.Amp_Diff/=Len;


    // Correct shift to originally align with peaks.
    shift-=((P1-X_B)-(P2-Y_B));
    ans.Amp_Shift=shift;


    // Calculate Amp_WinDiff.
    ans.Amp_WinDiff=0;
    for (int i=W1;i<W2;++i) ans.Amp_WinDiff+=fabs(s1[P1+i]-s2[P2+i-shift]);
    ans.Amp_WinDiff/=(W2-W1);


    // Calculate shift (by cross-correlation using t1 ~ t2).
    X_B=P1+W1;X_E=P1+W2;
    Y_B=P2+W1;Y_E=P2+W2;
    C1=std::vector<double>(s1.begin()+X_B,s1.begin()+X_E);
    C2=std::vector<double>(s2.begin()+Y_B,s2.begin()+Y_E);
    res=CrossCorrelation(C1,C2);
    shift=res.first.first;
    ans.Win_CCC=res.first.second;
    ans.Win_Shift=res.first.first;

    // Find the overlapping part.
    if(shift<0){
        X=0;
        Y=-shift;
    }
    else{
        X=shift;
        Y=0;
    }
    Len=std::min(X_E-X_B-X,Y_E-Y_B-Y);
    X+=X_B;
    Y+=Y_B;

    ans.Win_Diff=0;
    for (int i=0;i<Len;++i) ans.Win_Diff+=fabs(s1[X+i]-s2[Y+i]);
    ans.Win_Diff/=Len;

    return ans;
}

#endif
