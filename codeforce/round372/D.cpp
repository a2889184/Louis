#include <bits/stdc++.h>
#define SZ(X) ((int)(X).size())
#define ALL(X) (X).begin(), (X).end()
#define REP(I, N) for (int I = 0; I < (N); ++I)
#define REPP(I, A, B) for (int I = (A); I < (B); ++I)
#define PI(I) printf("%d\n", I)
#define RI(X) scanf("%d", &(X))
#define RII(X, Y) scanf("%d%d", &(X), &(Y))
#define RIII(X, Y, Z) scanf("%d%d%d", &(X), &(Y), &(Z))
#define DRI(X) int (X); scanf("%d", &X)
#define DRII(X, Y) int X, Y; scanf("%d%d", &X, &Y)
#define DRIII(X, Y, Z) int X, Y, Z; scanf("%d%d%d", &X, &Y, &Z)
#define RS(X) scanf("%s", (X))
#define CASET int ___T, case_n = 1; scanf("%d ", &___T); while (___T-- > 0)
#define MP make_pair
#define PB push_back
#define MS0(X) memset((X), 0, sizeof((X)))
#define MS1(X) memset((X), -1, sizeof((X)))
#define LEN(X) strlen(X)
#define PII pair<int,int>
#define VI vector<int>
#define VPII vector<pair<int,int> >
#define PLL pair<long long,long long>
#define VPLL vector<pair<long long,long long> >
#define F first
#define S second

#define N 105
#define Q 100
#define INF 1000000000000000
#define MAX 1e12+3

typedef long long LL;
using namespace std;
LL path[1010][1010];
LL mpath[1010][1010];
int n, m;
LL L;
LL diff;
LL MIN;
VI zs, zd;
LL copymind[1010];
LL mind[1010], tmind[1010];
bool walk[1010];

bool dijcheck(int s, int t, LL sp){

	int dest = -1;
	LL minn = INF;
	walk[s] = 1;
	REP(i, n){
		if(walk[i] == 0){
			if(path[s][i] != INF){
				if(path[s][i] > 0){
					if((path[s][i] + sp < copymind[i])){
						copymind[i] = path[s][i] + sp;
					}
				}
			}
		}
	}

	REP(i, n){
		if(walk[i] == 0){
			if(copymind[i] < minn){
				minn = copymind[i];
				dest = i;
			}
		}
	}
	
	if(dest != -1){
		sp = copymind[dest];
		dijcheck(dest, t, sp);
	}
	
	return 0;
}

bool dij(int s, int t, LL sp){

	int dest = -1;
	LL minn = INF;
	walk[s] = 1;
	REP(i, n){
		if(walk[i] == 0){
			if(path[s][i] != INF){
				if(path[s][i] > 0){
					if((path[s][i] + sp < mind[i])){
						mind[i] = path[s][i] + sp;
					}
				}
			}
		}
	}

	REP(i, n){
		if(walk[i] == 0){
			if(mind[i] < minn){
				minn = mind[i];
				dest = i;
			}
		}
	}
	
	if(dest != -1){
		sp = mind[dest];
		dij(dest, t, sp);
	}
	
	return 0;
}
bool dijT(int s, int t, LL sp){

	int dest = -1;
	LL minn = INF;
	walk[s] = 1;
	REP(i, n){
		if(walk[i] == 0){
			if(mpath[s][i] != INF){
				if(mpath[s][i] > 0){
					if((mpath[s][i] + sp < tmind[i])){
						tmind[i] = mpath[s][i] + sp;
					}
				}
			}
		}
	}

	REP(i, n){
		if(walk[i] == 0){
			if(tmind[i] < minn){
				minn = tmind[i];
				dest = i;
			}
		}
	}
	
	if(dest != -1){
		sp = tmind[dest];
		dijT(dest, t, sp);
	}
	
	return 0;
}
bool dijS(int s, int t, LL sp){

	int dest = -1;
	LL minn = INF;
	walk[s] = 1;
	REP(i, n){
		if(walk[i] == 0){
			if(mpath[s][i] != INF){
				if(mpath[s][i] > 0){
					if(path[s][i] == 0){

						if(L - mind[s] - tmind[i] >= 1){
							path[s][i] = L - mind[s] - tmind[i];
						}else{
							path[s][i] = 1;
						}
						path[i][s] = path[s][i];
						mpath[s][i] = path[s][i];
						mpath[i][s] = path[s][i];
					}
					if((mpath[s][i] + sp < mind[i])){
						mind[i] = mpath[s][i] + sp;
					}
				}
			}
		}
	}

	REP(i, n){
		if(walk[i] == 0){
			if(mind[i] < minn){
				minn = mind[i];
				dest = i;
			}
		}
	}
	
	if(dest != -1){
		sp = mind[dest];
		dijS(dest, t, sp);
	}
	
	return 0;
}


int alfind = 0;
int parent[1010];
bool dijfindpath(int s, int t, LL sp){

	int dest = -1;
	LL minn = INF;
	walk[s] = 1;
	REP(i, n){
		if(walk[i] == 0){
			if(mpath[s][i] != INF){
				if(mpath[s][i] > 0){
					if((mpath[s][i] + sp < mind[i])){
						mind[i] = mpath[s][i] + sp;
						parent[i] = s;
					}
				}
				/*if(path[s][i] > 0 && (dest == -1 || (path[s][i] < path[s][dest]))){
					dest = i;
				}*/
			}
		}

	}

	/*REP(i, n){
		cout << mind[i] << " ";
	}
	cout << endl;*/

	REP(i, n){
		if(walk[i] == 0){
			if(mind[i] < minn){
				minn = mind[i];
				dest = i;
			}
		}
	}
	//cout << dest << endl;
	
	if(dest != -1){
		//parent[dest] = s;
		/*if(path[s][dest] == 0){
			zs.PB(s);
			zd.PB(dest);
		}*/
		sp = mind[dest];
		dijfindpath(dest, t, sp);
	}
	return 0;
}

int main(){
	REP(i, 1010){
		REP(j, 1010){
			path[i][j] = INF;
		}
	}
	REP(i, 1010){
		mind[i] = INF;
		walk[i] = 0;
	}
	int s, t;
	cin >> n >> m >> L >> s >> t;
	MIN = L + 10;
	REP(i, m){
		int u, v;
		LL w;
		cin >> u >> v >> w;
		path[u][v] = w;
		path[v][u] = w;
	}
	REP(i, 1010){
		REP(j, 1010){
			mpath[i][j] = path[i][j];
			if(mpath[i][j] == 0){
				mpath[i][j] = 1;
			}
		}
	}

	dij(s, t, 0);
	/*REP(i, n){
		cout << mind[i] << " ";
	}*/
	//cout << endl;
	//cerr << mind[t] << endl;
	if(mind[t] < L){
		cout << "NO" << endl;
		return 0;
	}
	//cerr << mind[t] << endl;
	//cerr << MIN << endl;
	REP(i, 1010){
		mind[i] = INF;
		walk[i] = 0;
		parent[i] = 100000;
		tmind[i] = INF;
	}
	tmind[t] = 0;
	dijT(t, s, 0);
	REP(i, 1010){
		walk[i] = 0;
	}
	mind[s] = 0;
	dijS(s, t, 0);
	//dijfindpath(s, t, 0);
	//cout << mind[t] << endl;
	
	if(mind[t] <= L){
		cout << "YES" << endl;
		/*int now = t;
		while(now != s){
			int parentnow = parent[now];
			if(path[parentnow][now] == 0){
				zs.PB(now);
				zd.PB(parentnow);
			}
			now = parentnow;
		}
		int num = zs.size();
		/*REP(i, zs.size()){
			cerr << zs[i] << " " << zd[i] << endl;
		}*/
			/*
		REP(i, zs.size()){
			path[zs[i]][zd[i]] = 1;
			path[zd[i]][zs[i]] = 1;
		}
		REP(i, 1010){
			REP(j, 1010){
				if(path[i][j] == 0) path[i][j] = MAX;
			}
		}
		LL diff = (L - mind[t]);
		
		if(num > 0){
			REP(i, zs.size()){
				REP(j, 1010){
					copymind[j] = mind[j];
					walk[j] = 0;
				}
				LL value = path[zs[i]][zd[i]];
				path[zs[i]][zd[i]] += diff;
				path[zd[i]][zs[i]] += diff;
				dijcheck(s, zs[i], 0);
				if(copymind[zs[i]] >= mind[zs[i]]){
					break;
				}else{
					path[zs[i]][zd[i]] -= diff;
					path[zd[i]][zs[i]] -= diff;
				}

			}
		}
		*/

	}/*else if(mind[t] == L){
		cout << "YES" << endl;
		REP(i, n){
			REPP(j, i + 1, n){
				if(mpath[i][j] != INF){
					cout << i << " " << j << " " << mpath[i][j] << endl;
				}
			}
		}
		return 0;
	}*/else{
		cout << "NO" << endl;
		return 0;
	}

	
	REP(i, n){
		REPP(j, i + 1, n){
			//if(path[i][j] == 0) path[i][j] = INF;
			if(path[i][j] < INF){
				cout << i << " " << j << " " << path[i][j] << endl;
			}
		}
		
	}

}