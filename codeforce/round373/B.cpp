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

char c[N], c_1[N], c_2[N];


int main(){
	
	int n;
	cin >> n;
	REP(i, n){
		cin >> c[i];
		if(i % 2 == 0){
			c_1[i] = 'r';
			c_2[i] = 'b';
		}else{
			c_1[i] = 'b';
			c_2[i] = 'r';
		}
	}
	/*REP(i, n){
		cout << c_1[i];
	}
	cout << endl;
	REP(i, n){
		cout << c_2[i];
	}
	cout << endl;
	*/
	int countA1 = 0, countA2 = 0, countB1 = 0, countB2 = 0;
	REP(i, n){
		if(c_1[i] == 'r' && c[i] == 'b'){
			countA1++;
		}else if(c_1[i] == 'b' && c[i] == 'r'){
			countA2++;
		}

		if(c_2[i] == 'r' && c[i] == 'b'){
			countB1++;
		}else if(c_2[i] == 'b' && c[i] == 'r'){
			countB2++;
		}
	}

	cout << min(max(countA1, countA2), max(countB1, countB2)) << endl;
	
}