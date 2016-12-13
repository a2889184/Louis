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

#define N 5200
#define Q 100
#define INF 1000000005

typedef long long LL;
typedef unsigned long long ULL;
using namespace std;
int road[N][N];
//int path[N][N];
int mind[N];
bool walk[N];
PII backwalk[N][N];
int cost[N];
int n, m, T;
VPII x[N];


int main(){

	cin >> n >> m >> T;
	int u, v, t;

	REP(i, N){
		REP(j, N){
			road[i][j] = INF;
		}
	}
	road[1][1] = 0;
	REP(i, m){
		cin >> u >> v >> t;
		/*road[u][v] = t;
		road[v][u] = t;*/
		x[u].PB(MP(v, t));
		x[v].PB(MP(u, t));
	}
	REPP(l, 2, n + 1){
		REPP(i, 1, n + 1){

			REP(j, x[i].size()){

				if(road[l][x[i][j].F] > road[l - 1][i] + x[i][j].S){
					road[l][x[i][j].F] = road[l - 1][i] + x[i][j].S;
					backwalk[l][x[i][j].F] = MP(l - 1, i);
				}
			}
			
			
		}
	}
	
	PII nownode;
	for(int i = n; i >= 1; i--){
		if(road[i][n] != INF && road[i][n] <= T){
			cout << road[i][n] << endl;
			nownode = MP(i, n);
			break;
		}
	}
	stack<PII> stackP;
	stackP.push(nownode);
	for(int i = nownode.F; i > 1; i--){
		nownode = backwalk[nownode.F][nownode.S];
		stackP.push(nownode);
	}

	while(!stackP.empty()){
		cout << stackP.top().S << " ";
		stackP.pop();
	}

}