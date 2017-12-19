#include<boost/numeric/ublas/matrix.hpp>
#include<boost/numeric/ublas/lu.hpp>

std::vector<double> PolyFit(const std::vector<std::pair<double,double>> &data,int Degree,const std::vector<double> &weight){

	// data.first = x0
	// data.second = y0

	int n=data.size();

	// Solve Ax=b. where A = (x0^Degree ,x0^ Degree-1 ,...,1).
	//                   b = (y0)


	// create A.
	boost::numeric::ublas::matrix<double> A(n,Degree+1);
	for (int i=0;i<n;++i){
		double t=1;
		for (int j=Degree;j>=0;--j){
			A(i,j)=t;
			t*=data[i].first;
		}
	}

	// create b.
	boost::numeric::ublas::matrix<double> b(n,1);
	for (int i=0;i<n;++i) b(i,0)=data[i].second;

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
