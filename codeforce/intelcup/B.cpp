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

#define N 100010
#define Q 100
#define INF 1000000000000000

typedef long long LL;
using namespace std;

string input[150];
string correct;

int main(){
	int n, k;
	cin >> n >> k;

	REP(i, n){
		cin >> input[i];
	}

	cin >> correct;

	int smallsize1 = 0, smallsize2 = 0;
	REP(i, n){
		if((input[i]).size() < correct.size()){
			smallsize1++;
			smallsize2++;
		}else if((input[i]).size() == correct.size()){
			smallsize2++;
		}
	}

	int minval = (smallsize1 / k) * 5 + smallsize1 + 1;
	int maxval = ((smallsize2 - 1)/ k) * 5 + smallsize2;

	cout << minval << " " << maxval << endl;

}