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

#define N 100000
#define Q 100
#define INF 1000000000000000

typedef long long LL;
using namespace std;

int input[N];



int main(){
	int MAX, MIN, MIDDLE;
	DRI(num);
	REP(i, num){
		RI(input[i]);

		if(i == 0){
			MAX = input[0];
			MIN = input[0];
		}else{
			MAX = max(MAX, input[i]);
			MIN = min(MIN, input[i]);
		}
	}

	MIDDLE = (MAX + MIN) / 2;


	REP(i, num){
		if((input[i] != MAX) && (input[i] != MIN) && (input[i] != MIDDLE)){
			cout << "NO";
			return 0;
		}else if(input[i] == MIDDLE && MIDDLE > MIN){
			if((MAX - input[i]) != input[i] - MIN) {
				cout << "NO";
				return 0;
			}
		}
	}

	cout << "YES";
}