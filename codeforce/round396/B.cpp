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

#define N 510
#define Q 100

typedef long long LL;
using namespace std;

int main(){
	LL square[N][N];
	DRI(row);


	int n, m;
	REP(i, row){
		REP(j, row){
			cin >> square[i][j];
			if(square[i][j] == 0){
				n = i;
				m = j;
			}
		}
	}
	int goal;
	if(n == 0){
		goal = n + 1;
	}else{
		goal = n - 1;
	}
	if(row == 1){
		cout << "1";
		return 0;
	}
	LL sum  = 0;
	REP(i, row){
		sum += square[goal][i];
	}
	//cout << n << " " << m << endl;
	LL empty_sum = 0;
	REP(i, row){
		empty_sum += square[n][i];
	}
	square[n][m] = sum - empty_sum;

	if(square[n][m] <= 0){
		cout << "-1";
		return 0;
	}
	//cout << n << " " << m << endl;
	empty_sum = 0;
	REP(i, row){
		empty_sum = 0;
		REP(j, row){
			empty_sum += square[i][j];
		}
		if(empty_sum != sum){
			cout << "-1";
			return 0;
		}
	}

	REP(i, row){
		empty_sum = 0;
		REP(j, row){
			empty_sum += square[j][i];
		}
		if(empty_sum != sum){
			cout << "-1";
			return 0;
		}
	}
	
	//cout << empty_sum << " " << sum << endl;
	
	//cout << n << " " << m << endl;
	empty_sum = 0;
	{
		REP(i, row){
			empty_sum += square[i][i];
		}
		if(empty_sum != sum){
			cout << "-1";
			return 0;
		}
	}

	empty_sum = 0;
	{
		REP(i, row){
			empty_sum += square[i][row - 1 - i];
		}
		if(empty_sum != sum){
			cout << "-1";
			return 0;
		}
	}

	cout << square[n][m];
	
}

