/*
 * m_km.cpp
 *
 *  Created on: Dec 8, 2014
 *      Author: Casa
 */

#include "m_km.h"

KM::KM() {
	lx[MAXN]={0};
	ly[MAXN]={0};
}

KM::~KM() {
	// TODO Auto-generated destructor stub
}

bool KM::search(int x) {
	visx[x]=true;
	for(int y=0;y<MAXN;y++){
		if(visy[y])
			continue;
		int t=lx[x]+ly[y]-w[x][y];
		if(t==0){
			visy[y]=true;
			if(linky[y]==-1||search(linky[y])){
				linky[y]=x;
				return	true;
			}
		}
		else if(lack>t)
			lack=t;
	}
	return false;
}

int KM::run() {
	memset(linky,-1,sizeof(linky));
	for(int i=0;i<MAXN;i++)
		for(int j=0;j<MAXN;j++)
			if(w[i][j]>lx[i])
				lx[i]=w[i][j];//初始化顶标
	for(int x=0;x<MAXN;x++){
		for(;;){
			memset(visx,0,sizeof(visx));
			memset(visy,0,sizeof(visy));
			lack=INF;
			if(search(x))
				break;
			for(int i=0;i<MAXN;i++){
				if(visx[i])
					lx[i]-=lack;
				if(visy[i])
					ly[i]+=lack;
			}
		}
	}
	return 0;
}
