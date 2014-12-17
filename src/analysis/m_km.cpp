/*
 * m_km.cpp
 *
 *  Created on: Dec 8, 2014
 *      Author: Casa
 */

#include "m_km.h"

KM::KM() {

}

KM::~KM() {

}

bool KM::search(int x) {
    visx[x] = 1;
    for (int y = 1;y <= ny;y ++)
    {
        if (visy[y])
            continue;
        int t = lx[x] + ly[y] - w[x][y];
        if (t == 0)       //
        {
            visy[y] = 1;
            if (link[y] == -1||search(link[y]))
            {
                link[y] = x;
                return 1;
            }
        }
        else if (slack[y] > t)
        	//不在相等子图中slack 取最小的
            slack[y] = t;
    }
    return false;
}

int KM::run() {
    int i,j;
    memset (link,-1,sizeof(link));
    memset (ly,0,sizeof(ly));
    for (i = 1;i <= nx;i ++)
    	//lx初始化为与它关联边中最大的
        for (j = 1,lx[i] = -INF;j <= ny;j ++)
            if (w[i][j] > lx[i])
                lx[i] = w[i][j];

    for (int x = 1;x <= nx;x ++)
    {
        for (i = 1;i <= ny;i ++)
            slack[i] = INF;
        while (1)
        {
            memset (visx,0,sizeof(visx));
            memset (visy,0,sizeof(visy));
            if (search(x))
            	//若成功（找到了增广轨），则该点增广完成，进入下一个点的增广
                break;
            	//若失败（没有找到增广轨），则需要改变一些点的标号，使得图中可行边的数量增加。
                //方法为：将所有在增广轨中（就是在增广过程中遍历到）的X方点的标号全部减去一个常数d，
                //所有在增广轨中的Y方点的标号全部加上一个常数d
            int d = INF;
            for (i = 1;i <= ny;i ++)
                if (!visy[i]&&d > slack[i])
                    d = slack[i];
            for (i = 1;i <= nx;i ++)
                if (visx[i])
                    lx[i] -= d;
            for (i = 1;i <= ny;i ++)  //修改顶标后，要把所有不在交错树中的Y顶点的slack值都减去d
                if (visy[i])
                    ly[i] += d;
                else
                    slack[i] -= d;
        }
    }
    int res = 0;
    for (i = 1;i <= ny;i ++) {
        if (link[i] > -1) {
            res += w[link[i]][i];
            printf("link[i]: ");
            printf("%d  --  ",link[i]);
            printf("i: ");
            printf("%d\n",i);
        }
    }
    return res;
}

int KM::eval() {
    int i,j;
    printf("input a n and input the x and y: \n");
    while (scanf ("%d",&n)!=EOF)
    {
        nx = ny = n;
        memset (w,0,sizeof(w));
        for (i = 1;i <= n;i ++)
            for (j = 1;j <= n;j ++)
                scanf ("%d",&w[i][j]);
        int ans = run();
        printf ("%d\n",ans);
    }
    return 0;
}

bool KM::find(int x) {
	int i;
	for(i=1; i<=n; i++) {
		if(map[x][i] && !vis[i]) {
			vis[i]=1;
			if(link1[i]==0 || find(link1[i])) {
				link1[i]=x;
				return true;
			}
		}
	}
	return false;
}

/*
 * largest matching.
 *  */
int KM::LM() {
	int i, j, x, y, s;
	printf("largest matching in bitgraph!\n");
    if (scanf ("%d",&n)!=EOF) {
    	nx = ny = n;
    	/* 初始化的地方 */
    	for(int l=1; l<=n; l++) {
    		scanf("%d %d", &x, &y);
    		map[x][y]=1;
    	}
    }

    for(i=1; i<=n; i++) {
    	memset(vis,0,sizeof(vis));
    	if(find(i))
    		s++;
    }

    printf("largest matching is: %d\n",s);
    for(int k=1; k<=n; k++) {
        printf("link1[k]: ");
        printf("%d  --  ",link1[k]);
        printf("k: ");
        printf("%d\n",k);
    }
	return 0;
}
