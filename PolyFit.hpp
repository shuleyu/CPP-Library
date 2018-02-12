#ifndef ASU_POLYFIT
#define ASU_POLYFIT

#include<boost/numeric/ublas/matrix.hpp>
#include<boost/numeric/ublas/lu.hpp>

/***************************************************************
 * This C++ template calculate the polynominal fitting for given
 * input. This is a wrapper using the LU decomposition prvided
 * by boost library.
 *
 * input(s):
 * const vector<T1> &x       ----  x series.
 * const vector<T2> &y       ----  y series.
 * const int        &Degree  ----  Degree of the polynominal fitting.
 * const vector<T3> &weight  ----  (Optional), weighting at each data point.
 *                                 default is no weighting.
 *
 * return(s):
 * const vector<double> ans  ----  Coefficients. ans[0] is the highest degree coefficient.
 *
 * Shule Yu
 * Dec 28 2017
 *
 * Key words: LU decomposition. least square.
***************************************************************/

template<class T1, class T2, class T3=double>
std::vector<double> PolyFit(const std::vector<T1> &x, const std::vector<T2> &y,
                            const int &Degree, const std::vector<T3> &weight={}){

    int n=x.size();

    // Solve for X in AX=b.
    // A[0] = (x[0]^Degree ,x[0]^ Degree-1 ,...,1).
    // A[1] = (x[1]^Degree ,x[1]^ Degree-1 ,...,1).
    // ...
    // b[0] = y[0]
    // b[1] = y[1]
    // ...

    // create A.
    boost::numeric::ublas::matrix<double> A(n,Degree+1);
    for (int i=0;i<n;++i){
        double t=1;
        for (int j=Degree;j>=0;--j){
            A(i,j)=t;
            t*=x[i];
        }
    }

    // create b.
    boost::numeric::ublas::matrix<double> b(n,1);
    for (int i=0;i<n;++i) b(i,0)=y[i];

    // creat A's transpose.
    boost::numeric::ublas::matrix<double> AT(boost::numeric::ublas::trans(A));

    // add weight
    if (!weight.empty())
        for (int i=0;i<=Degree;++i)
            for (int j=0;j<n;++j)
                AT(i,j)*=weight[j];

    // creat ATA, ATb.
    boost::numeric::ublas::matrix<double> ATA ( boost::numeric::ublas::prec_prod( AT, A ));
    boost::numeric::ublas::matrix<double> ATb ( boost::numeric::ublas::prec_prod( AT, b ));


    // lu decomposition.
    // the result is stored in ATA (lower half of ATA is l, upper half of ATA is u).
    // notice: will do a permutation of ATA before its LU decomposition.
    boost::numeric::ublas::permutation_matrix<int> pert(ATA.size1());
    if (boost::numeric::ublas::lu_factorize(ATA,pert) !=0) return {};

    // back_substitution.
    // lu_substitute call triangular solver twice.
    // result is stored in Atb.
    boost::numeric::ublas::lu_substitute(ATA,pert,ATb);

    return std::vector<double> (ATb.data().begin(),ATb.data().end());
}

#endif
