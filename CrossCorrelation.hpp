#ifndef ASU_CROSSCORRELATION
#define ASU_CROSSCORRELATION

#include<iostream>
#include<vector>
#include<iterator>
#include<cmath>
#include<limits>
#include<numeric>

/**************************************************************************
 * This C function(s) calculate Zero-normalized cross-correlationbetween x
 * and y.
 *
 * numerator:
 * x*y [tau] = sum ( x[n+tau]y[n] ) over overlapping section.
 * tau is the shift between two signals.
 *
 * Negative tau indicate y move to the left.
 * Positive tau indicate y move to the right.
 *
 * denominator:
 * sqrt( (sum of x^2)*(sum of y^2) )  [Remove the standard deviaiton]
 *
 *
 * Namely,
 *
 *                 sum of x*y over overlapping section.
 *    x*y[tau] = ----------------------------------------
 *                  sqrt( (sum of x^2)*(sum of y^2) )
 *
 * input(s):
 * const vector<T1> &x              ----  Signal x.
 * const vector<T2> &y              ----  Signal y.
 * const bool       &Dump           ----  (Optional) Choose whether to return x*y[tau], default is false.
 * const int        &Flip           ----  (Optional) Flag for compare mode, default is 0.
 *                                        0     : ccc will have the largest absolute value.
 *                                        1, -1 : ccc will have the largest value.
 *                                        1     : signal y will not be flipped.
 *                                        -1    : signal y is flipped before calculation.
 * const pair<int,int> &ShiftLimit  ----  (Optional) Two parameters control the range of shift, default is no limitations.
 *
 *
 * alternatively, inputs(s):
 * const vector<T1>::iterator XBegin  ----  Signal x begin.
 * const vector<T1>::iterator XEnd    ----  Signal x end.
 * const vector<T2>::iterator YBegin  ----  Signal y begin.
 * const vector<T2>::iterator YEnd    ----  Signal y end.
 * const bool       &Dump             ----  (Optional) Choose whether to return x*y[tau], default is false.
 * const int        &Flip             ----  (Optional) Flag for compare mode, default is 0.
 *                                          0     : ccc will have the largest absolute value.
 *                                          1, -1 : ccc will have the largest value.
 *                                          1     : signal y will not be flipped.
 *                                          -1    : signal y is flipped before calculation.
 * const pair<int,int> &ShiftLimit    ----  (Optional) Two parameters control the range of shift, default is no limitations.
 *
 *
 * return(s):
 * std::pair<std::pair<int,double>,std::vector<double>> ans  ----  {{shift,ccc},Ans}
 *                                  shift: Best fit position (tau)
 *                                  ccc  : CCC at best fit position.
 *                                  Ans  : x*y[tau] array.
 *
 *
 *
 * Shule Yu
 * Dec 28 2017
 *
 * Key words: cross-correlation.
**************************************************************************/

template<typename T1, typename T2>
std::pair<std::pair<int,double>,std::vector<double>> CrossCorrelation(const T1 XBegin, const T1 XEnd, const T2 YBegin, const T2 YEnd, const bool &Dump=false, const int &Flip=0, const std::pair<int,int> &ShiftLimit={std::numeric_limits<int>::min(),std::numeric_limits<int>::max()}){

    // Check signal length.
    int m=std::distance(XBegin,XEnd),n=std::distance(YBegin,YEnd);
    if (m==0 || n==0) {
        throw std::runtime_error("Error in " + std::string(__func__) + ": x or y size is zero ...");
        return {};
    }

    // Prepare shift limit.
    if (ShiftLimit.first>ShiftLimit.second) {
        throw std::runtime_error("Error in " + std::string(__func__) + ": ShiftLimit first > second ...");
        return {};
    }
    int ShiftLeft=std::max(ShiftLimit.first,1-n);
    int ShiftRight=std::min(ShiftLimit.second,m-1);



    // Calculate x average and y average.
    double avrx=std::accumulate(XBegin,XEnd,0.0)/m;
    double avry=std::accumulate(YBegin,YEnd,0.0)/n;


    // Denominator. (remove the deviation of the signals)
    double xx=0,yy=0;
    for (int i=0;i<m;++i) xx+=(XBegin[i]-avrx)*(XBegin[i]-avrx);
    for (int i=0;i<n;++i) yy+=(YBegin[i]-avry)*(YBegin[i]-avry);
    if ( xx==0 || yy==0 ){
        std::cerr <<  "Warning in " << __func__ << ": x or y is zero's ..." << std::endl;
        return {};
    }
    double energy=sqrt(xx*yy);

    // Prepare cross-correlation trace.
    std::vector<double> res;
    if (Dump) res.resize(ShiftRight-ShiftLeft+1,0);

    // Shifting and create cross-correlation trace.
    double ccc=(Flip==0?0:std::numeric_limits<double>::lowest());
    int shift,polarity=(Flip==-1?-1:1);

    for (int tau=ShiftLeft;tau<=ShiftRight;++tau){

        double R=0;

        // Equivalently:
        //         int xBegin=tau>0?tau:0;
        //         int xEnd=m<(n+tau)?m:(n+tau);
        //
        //         for (int i=xBegin;i<xEnd;++i)
        //             R+=(XBegin[i]-avrx)*(YBegin[i-tau]-avry)*polarity;

        int yBegin=tau>0?0:-tau;
        int yEnd=(m-tau)>n?n:m-tau;

        for (int i=yBegin;i<yEnd;++i)
            R+=(XBegin[i+tau]-avrx)*(YBegin[i]-avry)*polarity;

        R/=energy;

        if ( (Flip==0 && fabs(ccc)<fabs(R)) || (Flip!=0 && ccc<R) ){
            ccc=R;
            shift=tau;
        }

        if (Dump) res[tau-ShiftLeft]=R;
    }

    return {{shift,ccc},res};
}

template<typename T1, typename T2>
std::pair<std::pair<int,double>,std::vector<double>> CrossCorrelation(const std::vector<T1> &x, const std::vector<T2> &y, const bool &Dump=false, const int &Flip=0, const std::pair<int,int> &ShiftLimit={std::numeric_limits<int>::min(),std::numeric_limits<int>::max()}){
    return CrossCorrelation(x.begin(),x.end(),y.begin(),y.end(),Dump,Flip,ShiftLimit);
}


#endif
