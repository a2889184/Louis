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

typedef long long LL;
using namespace std;
LL dp[N][N][N];
int main(){
	DRIII(n, m, k);	
	int tree_color[N];
	
	int zero_array[N];

	REPP(i, 1, n + 1){
		RI(tree_color[i]);
	}
	LL color[N][N];
	REPP(i, 1, n + 1){
		REPP(j, 1, m + 1){
			cin >> color[i][j];
		}
	}
	REP(i, N){
		REP(j, N){
			REP(k, N){
				dp[i][j][k] = (LL)INF;
			}
		}
	}


	
	if(tree_color[1] != 0){
		dp[1][tree_color[1]][1] = 0;
	}else{
		REPP(i, 1, m + 1){
			dp[1][i][1] = color[1][i];
		}
	}
	
	REPP(i, 2, n + 1){
		REPP(j, 1, k + 1){
			if(tree_color[i] != 0){
				dp[i][tree_color[i]][j] = dp[i - 1][tree_color[i]][j];
				REPP(l, 1, m + 1){
					if(l != tree_color[i]){
						dp[i][tree_color[i]][j] = min(dp[i][tree_color[i]][j], dp[i - 1][l][j - 1]);
					}
				}
			}else{
				REPP(l, 1, m + 1){
					dp[i][l][j] = dp[i - 1][l][j] + color[i][l];
					//cout << i << j << l << " "<< dp[i][l][j] <<endl;
					REPP(s, 1, m + 1){
						if(s != l){
							dp[i][l][j] = min(dp[i][l][j], dp[i - 1][s][j - 1] + color[i][l]);
						}
					}
				}
			}
			
		}
	}
	/*
	REPP(i, 1, n + 1){
		REPP(l, 1, k + 1){
			REPP(j, 1, m + 1){
				cout << "n, k, m:  " << i << "  " << l << "  " <<  j << endl;
				if(dp[i][j][l] != INF){
					cout << setw(5) << right << dp[i][j][l];
				}
				else {
					cout << setw(5) << right << "INF";
				}

				cout << endl;
			}
			cout << endl;
		}
		cout << endl;
	}

*/
	LL MIN = INF;
	REPP(i, 1, m + 1){
		MIN = min(MIN, dp[n][i][k]);
	}
	if(MIN >= INF){
		cout << "-1";
		return 0;
	}
	cout << MIN << endl;
}
