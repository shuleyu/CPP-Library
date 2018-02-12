#ifndef ASU_CROSSCORRELATION
#define ASU_CROSSCORRELATION

#include<iostream>
#include<vector>
#include<cmath>
#include<limits>
#include<numeric>

/**************************************************************************
 * This C function(s) calculate cross-correlation between x and y.
 * First we remove the mean of two signals.
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
 *                                        0     : ccc will have the larget absolute value.
 *                                        1, -1 : ccc will have the larget value.
 *                                        1     : signal y will not be flipped.
 *                                        -1    : signal y is flipped before calculation.
 * const pair<int,int> &ShiftLimit  ----  (Optional) Two parameters control the range of shift, default is no limitations.
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

template<class T1, class T2>
std::pair<std::pair<int,double>,std::vector<double>> CrossCorrelation(const std::vector<T1> &x, const std::vector<T2> &y,const bool &Dump=false,const int &Flip=0,const std::pair<int,int> &ShiftLimit={std::numeric_limits<int>::min(),std::numeric_limits<int>::max()}){

    // Check signal length.
    int m=x.size(),n=y.size();
    if (m==0 || n==0) {
        std::cerr <<  "Error in " << __func__ << ": x or y size is zero ..." << std::endl;
        return {};
    }

    // Prepare shift limit.
    if (ShiftLimit.first>ShiftLimit.second) {
        std::cerr <<  "Error in " << __func__ << ": ShiftLimit first > second ..." << std::endl;
        return {};
    }
    int ShiftLeft=std::max(ShiftLimit.first,1-n);
    int ShiftRight=std::min(ShiftLimit.second,m-1);



    // Calculate x average and y average.
    double avrx=std::accumulate(x.begin(),x.end(),0.0)/m;
    double avry=std::accumulate(y.begin(),y.end(),0.0)/n;


    // Denominator. (remove the deviation of the signals)
    double xx=0,yy=0;
    for (int i=0;i<m;++i) xx+=(x[i]-avrx)*(x[i]-avrx);
    for (int i=0;i<n;++i) yy+=(y[i]-avry)*(y[i]-avry);
    if ( xx==0 || yy==0 ){
        std::cerr <<  "Warning in " << __func__ << ": x or y is zero's ..." << std::endl;
        return {};
    }

    // Prepare cross-correlation trace.
    std::vector<double> res;
    if (Dump) res.resize(ShiftRight-ShiftLeft+1,0);

    // Shifting and create cross-correlation trace.
    double ccc=(Flip==0?0:std::numeric_limits<double>::lowest());
    int polarity=(Flip==-1?-1:1),shift;
    double energy=sqrt(xx*yy);

    for (int tau=ShiftLeft;tau<=ShiftRight;++tau){

        double R=0;

        // Equivalently:
        // 		int xBegin=tau>0?tau:0;
        // 		int xEnd=m<(n+tau)?m:(n+tau);
        //
        // 		for (int i=xBegin;i<xEnd;++i)
        // 			R+=(x[i]-avrx)*(y[i-tau]-avry)*polarity;

        int yBegin=tau>0?0:-tau;
        int yEnd=(m-tau)>n?n:m-tau;

        for (int i=yBegin;i<yEnd;++i)
            R+=(x[i+tau]-avrx)*(y[i]-avry)*polarity;

        R/=energy;

        if ( (Flip==0 && fabs(ccc)<fabs(R)) || (Flip!=0 && ccc<R) ){
            ccc=R;
            shift=tau;
        }

        if (Dump) res[tau-ShiftLeft]=R;
    }

    return {{shift,ccc},res};
}

#endif
