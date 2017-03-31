#include<iostream>
#include<mlpack/methods/kmeans/kmeans.hpp>
#include<mlpack/methods/kmeans/refined_start.hpp>

#include<ASU_tools.hpp>

/***********************************************************
 * This C++ function call kmeans from mlpack, do the cluster
 * analysis on the input 2d data array.
 *
 * double **data       ----  2D Array pointer. (nptsx*nptsy)
 * int    nptsx        ----  Number of records.
 * int    nptsy        ----  NPTS of each records.
 * int    CateN        ----  Assigned number of clusters.
 * int    *assignment  ----  Clustering result (1*nptsx)
 * 						         Value from 1~CateN.
 *
 * Shule Yu
 * Apr 25 2016
 *
 * Key words: kmeans, cluster analysis.
***********************************************************/

void Kmeans(double **data,int nptsx,int nptsy,int CateN,int *assignment){

    if (nptsx<=0||nptsy<=0||CateN<=0){
        std::cout << "In " << __func__ << ": NPTS error ..." << std::endl;
        return;
    }


	double *data_aux=new double [nptsx*nptsy];

	int index3=0;
    for (int index1=0;index1<nptsx;index1++){
		for (int index2=0;index2<nptsy;index2++){
			data_aux[index3]=data[index1][index2];
			index3++;
		}
    }

	arma::mat A(data_aux,nptsy,nptsx,false,true);
	arma::Row<size_t> assignments;

// 	mlpack::kmeans::KMeans<> k;
	mlpack::kmeans::RefinedStart k;
	k.Percentage()=0.5;

	k.Cluster(A,CateN,assignments);

	index3=0;
    for (int index1=0;index1<nptsx;index1++){
		assignment[index1]=assignments(index3);
		index3++;
    }

	delete[] data_aux;

    return;
}
