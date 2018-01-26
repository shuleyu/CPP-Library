#ifndef ASU_CURVEJUNCTION
#define ASU_CURVEJUNCTION

#include<vector>
#include<cmath>
#include<algorithm>

#include<CreateGrid.hpp>
#include<Interpolate.hpp>

/***********************************************************
 * This C++ template returns the junction piont of two input
 * signals s1 and s2 (X|Y).
 *
 * x1 and x2 should be sorted.
 *
 * The algorithm is simple: use interpolate to create another
 * two signals s1*,s2* with the same X value. Then find
 * the cloest (Y value) point within tolerance.
 *
 * vector<T1> &x1   ----  Signal 1, X.
 * vector<T2> &y1   ----  Signal 1, Y.
 * vector<T3> &x2   ----  Signal 2, X.
 * vector<T4> &y2   ----  Signal 2, Y.
 * const T5   &tol  ----  tolerance of Y (0<tol<1).
 *                        e.g. tol=0.01 and y1,y2 intercept at 99,
 *                        then when |NewY1-NewY2| < 99*tol = 0.99
 *                        the search terminates.
 *
 * return:
 * pair<double,double> ans  ----  junction point.
 *
 * Shule Yu
 * Jan 24 2018
 *
 * Key words: curve junction.
***********************************************************/

template<class T1,class T2,class T3,class T4,class T5>
std::pair<double,double> CurveJunction(const std::vector<T1> &x1, const std::vector<T2> &y1,
                                       const std::vector<T3> &x2, const std::vector<T4> &y2, const T5 &tol){

    // check sorted.
    auto cmp1=[](const T1 &x, const T1 &y){
        return x<=y;
    };
    auto cmp2=[](const T3 &x, const T3 &y){
        return x<=y;
    };

    if (!std::is_sorted(x1.begin(),x1.end(),cmp1) && !std::is_sorted(x1.rbegin(),x1.rend(),cmp1)) {
        std::cerr <<  __func__ << "; Error: input x1 is not sorted/has repeated values ..." << std::endl;
        return {};
    }
    if (!std::is_sorted(x2.begin(),x2.end(),cmp2) && !std::is_sorted(x2.rbegin(),x2.rend(),cmp2)) {
        std::cerr <<  __func__ << "; Error: input x2 is not sorted/has repeated values ..." << std::endl;
        return {};
    }

    std::pair<double,double> ans{std::numeric_limits<double>::max(),std::numeric_limits<double>::max()};

    // check time.
    double l1=std::min(x1[0],x1.back()),u1=std::max(x1[0],x1.back());
    double l2=std::min(x2[0],x2.back()),u2=std::max(x2[0],x2.back());
    if (u1<=l2 || u2<=l1) return ans;

    // find interpolate begin/end.
    int npts=500;
    double err=1,ansX,ansY;
    double lower=std::max(l1,l2),upper=std::min(u1,u2);
    while (err>tol && npts<1e6){

        auto NewX=CreateGrid(lower,upper,npts,0);
        auto NewY1=Interpolate(x1,y1,NewX,false);
        auto NewY2=Interpolate(x2,y2,NewX,false);

        // find junction point.
        int P;
        err=std::numeric_limits<double>::max();
        for (int i=0;i<npts;++i){
            double diff=fabs(NewY1[i]-NewY2[i])/std::min(fabs(NewY1[i]),fabs(NewY2[i]));
            if (err>diff){
                err=diff;
                P=i;
            }
        }
        if (err<tol) {
            ansX=NewX[P];
            ansY=(NewY1[P]+NewY2[P])/2;
        }

        npts*=2;
    }

    if (err<tol) ans={ansX,ansY};
    return ans;
}

#endif
