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

#define N 100005
#define Q 100
#define INF 1000000005

typedef long long LL;
typedef unsigned long long ULL;
using namespace std;

LL arr[N];
int pos[N];
int r[N];
int l[N];
LL ans[N];
int n;

int read(){
	int x=0;char c;
	while((c=getchar())<'0'||c>'9');
	for(;c>='0'&&c<='9';c=getchar())x=x*10+c-'0';
	return x;
}
inline int find(int *f, int a, int &most){
	if(f[a] != 0){
		find(f, f[a], most);
	}else{
		most = a;
		return a;
	}
	//return (f[a] != 0) ? find(f, f[a]): a;
}

int main(){
	n = read();
	REP(i, n){
		if(i == 0){
			arr[i] = read();
		}else{
			arr[i] = arr[i - 1] + read();
		}
	}
	REP(i, n){
		//scanf("%d", pos + i);
		pos[i] = read();
	}
	LL Max = 0;
	for(int i = n - 1; i >= 0; i--){
		LL temp;
		ans[i] = Max;
		int rmost, lmost;
		temp = arr[find(r, pos[i], rmost) - 1] - arr[find(l, pos[i], lmost) - 2];
		if(temp > Max) Max = temp;
		r[pos[i] - 1] = rmost;
		l[pos[i] + 1] = lmost;
	}

	REP(i, n){
		printf("%I64d\n", ans[i]);
	}

}