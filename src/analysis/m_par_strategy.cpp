/*
 * m_par_strategy.cpp
 *
 *  Created on: Jan 13, 2015
 *      Author: Casa
 */

#include "m_par_strategy.h"

ParStrategy::ParStrategy() {
	// TODO Auto-generated constructor stub
	int a[]={42,42,34,28,16,8,16,8,8,22,18,20,16,8,16,8,8,22,18,20};
	int b[]={6,12,8,6,12,6,22,16,20,24,20,18,16,10,10,6,10,6,8,10};
//	int a[]={3,2,1,3};
//	int b[]={2,1,2,3};

	int a_sum=0;
	int b_sum=0;

	for(int i=0; i<N; i++) {
		a_array[i]=a[i];
		b_array[i]=b[i];
		a_sum+=a_array[i];
		b_sum+=b_array[i];
	}

	a_expect=a_sum/P;
	b_expect=b_sum/P;

	for(int i=0;i<=N;i++) {
		for(int j=0;j<=P;j++) {
			state[i][j]=INT_MAX;
		}
	}
}

ParStrategy::~ParStrategy() {
	// TODO Auto-generated destructor stub
}

int ParStrategy::variance(int m, int n) {
	int ret;
	int a_i=0, b_i=0;
	for(int i=m; i<n; i++) {
		a_i+=a_array[i];
		b_i+=b_array[i];
	}
	ret=(a_i-a_expect)*(a_i-a_expect)+(b_i-b_expect)*(b_i-b_expect);
#ifdef LOGGING
	cout<<"the variance is: "<<ret<<endl;
#endif
	return ret;
}

void ParStrategy::eval() {

	int matrix[N+1][P+1]={0};

	for(int i=1; i<=N; i++) {
		matrix[i][1]=variance(0,i);
#ifdef LOGGING
		cout<<"j: "<<i<<" --k: 1"<<" --min: "<<matrix[i][1]<<endl;
#endif
		state[i][1]=matrix[i][1];
	}

	for(int k=2; k<=P; k++) {
		for(int j=2; j<=N; j++) {
			int min=INT_MAX;
			for(int l=k-1; l<j; l++) {
				if(matrix[l][k-1]+variance(l,j)<min)
					min=matrix[l][k-1]+variance(l,j);
			}
			matrix[j][k]=min;
#ifdef LOGGING
			cout<<"j: "<<j<<" --k: "<<k<<" --min: "<<min<<endl;
#endif
			state[j][k]=min;
		}
	}

	/*******************************************/
	for(int i=1; i<=P; i++) {
		for(int j=1; j<=N; j++) {
			cout<<setw(15)<<state[j][i]<<" ";
		}
		cout<<endl;
	}
	cout<<endl<<endl<<"at last: "<<matrix[N][P]<<endl<<endl<<endl;
	/*******************************************/

	/*******************************************/
	int p[P-1];
	for(int i=0; i<P-1; i++) {
		p[i]=0;
	}
	/*******************************************/
	for(int i=1; i<P; i++) {
		int min=INT_MAX;
		for(int j=1; j<=N; j++) {
			if(state[j][i]<min) {
				min=state[j][i];
				p[i-1]=j;
			}
		}
	}

	/*******************************************/
	for(int i=0; i<P-1; i++) {
		cout<<setw(10)<<p[i];
	}
	/*******************************************/
}
