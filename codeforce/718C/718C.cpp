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

#define N 100000000
#define Q 100
#define INF 1000000000000000

const int M = 1e9+7;

typedef long long LL;
using namespace std;

const int num = 1 << 18;

struct mat{
	int a, b, c, d;
};

mat operator*(mat A, mat B){
	mat temp = {(A.a * B.a + A.b * B.c) % M, (A.a * B.b + A.b * B.d) % M, (A.c * B.a + A.d * B.c) % M,
		(A.c * B.b + A.d * B.d) % M};
	return temp;
}

mat power(mat A, int x){
	mat temp = {1, 0, 0, 1};

	REP(i, x){
		temp = temp * A;
	}
	return temp;
}

class node{
public:
	mat Fib_n;
	mat Fib_n_1;
}

mat coeificient = {1, 1, 1, 0};
mat initial = {1, 0, 1, 0};
LL origin[num], Fib_n[num], Fib_n_1[num];

void construct_tree(int low, int high, int pos){
	if(low >= high){
		Fib_n[pos] = origin[low];
		return;
	}
	int mid = (low + high) / 2;
	construct_tree(low, mid, pos << 1 + 1);
	construct_tree(mid + 1, high, pos << 1 + 2)
	Fib_n[pos] = Fib_n[pos << 1 + 1] + Fib_n[pos << 1 + 1];
}


int main(){
	int type;
	int l, r, x;
	int n, m;
	
	cin >> n >> m;
	REP(i, n){
		cin >> origin[i];
	}

	

	REP(i, m){
		cin >> type;
		if(type == 1){
			cin >> l >> r >> x;
		}else{
			cin >> l >> r;
		}
	}
	
}