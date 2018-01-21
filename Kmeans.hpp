#ifndef ASU_KMEANS
#define ASU_KMEANS

#include<iostream>

#include<mlpack/methods/kmeans/kmeans.hpp>
#include<mlpack/methods/kmeans/refined_start.hpp>

/***********************************************************
 * This C++ template call kmeans from mlpack, do the cluster
 * analysis on the input 2d data array.
 *
 * vector<vector<T> &data   ----  2D Array. (nptsx*nptsy)
 *
 *                                nptsx: Number of records.
 *                                nptsy: NPTS of each record.
 * const int        &CateN  ----  Required number of clusters.
 *
 * return:
 * vector<int> size: 1*nptsx, value: 1~CateN.
 *
 * Shule Yu
 * Jan 18 2018
 *
 * Key words: kmeans, cluster analysis.
***********************************************************/

template<class T>
std::vector<int> Kmeans(const std::vector<std::vector<T>> &data,const int &CateN){

    if (data.empty()) return {};

    int nptsx=data.size(),nptsy=data[0].size();

    if (nptsy<=0||CateN<=0){
        std::cerr <<  __func__ << "; Error: input array size / cluster number is zero ..." << std::endl;
        return {};
    }


	double *data_aux=new double [nptsx*nptsy];

	int k=0;
    for (int i=0;i<nptsx;++i)
		for (int j=0;j<nptsy;++j)
			data_aux[k++]=data[i][j];

	arma::mat A(data_aux,nptsy,nptsx,false,true);
	arma::Row<size_t> assignments;

	mlpack::kmeans::RefinedStart K;
	K.Percentage()=0.5;

	K.Cluster(A,CateN,assignments);

    std::vector<int> res;
    for (int i=0;i<nptsx;++i) res.push_back(1+assignments[i]);

	delete[] data_aux;

    return res;
}

#endif
