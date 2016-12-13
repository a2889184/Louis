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

const int num = 1 << 18;
int arr[1<<17];
int tree[num];
int n;

void construct_tree(int low, int high, int pos){
	if(low >= high){
		tree[pos] = arr[low];
		return;
	}
	int mid = (low + high) / 2;
	construct_tree(low, mid, pos * 2 + 1);
	construct_tree(mid + 1, high, pos * 2 + 2);
	tree[pos] = tree[pos * 2 + 1] + tree[pos * 2 + 2];
}

int returnval(int qlow, int qhigh, int low, int high, int pos){
	/*if(qlow <= low && qhigh >= high){
		return tree[pos];
	}
	if(qlow > high || qhigh < low){
		return INF;
	}*/
	if(low == high){
		return pos;
	}
	int mid = (low + high) / 2; 
	if(mid >= qlow){
		returnval(qlow, qhigh, low, mid, 2 * pos + 1);
	}else if(qlow > mid){
		returnval(qlow, qhigh, mid + 1, high, 2 * pos + 2);
	}
}

int get(int qlow, int qhigh, int low, int high, int pos){
	if(qlow <= low && qhigh >= high){
		return tree[pos];
	}
	if(qlow > high || qhigh < low){
		return 0;
	}
	int mid = (low + high) / 2;
	return (get(qlow, qhigh, low, mid, 2 * pos + 1) + get(qlow, qhigh, mid + 1, high, 2 * pos + 2));
}

void update(int pos){
	int newpos = returnval(pos, pos, 0, n - 1, 0);
	int val = tree[newpos];
	//cout << "val:   " << val << endl;
	tree[newpos] -= val;
	/*cout << "tree" << endl;
	REP(i, 17){
		cout << tree[i] << endl;
	}*/
	while(newpos != 0){
		if(newpos % 2 == 0){
			newpos = (newpos - 2) / 2;
		}else{
			newpos = (newpos - 1) / 2;
		}
		tree[newpos] -= val;
	}
}

int main(){

	set<int> set_q;
	cin >> n;
	REP(i, n){
		cin >> arr[i];
	}
	construct_tree(0, n - 1, 0);
	//tree[0] = max(tree[1], tree[2]);
	REP(i, n){
		int query;
		cin >> query;
		update(query - 1);
		set_q.insert(query - 1);
		int pre = 0;
		int now = 0;
		int big = 0;
		for(set<int>::iterator it = set_q.begin(); it != set_q.end(); it++){
			pre = now;
			now = *it;
			big = max(big, get(pre, now, 0, n - 1, 0));
		}
		big = max(big, get(now, n- 1, 0, n - 1, 0));
		//tree[0] = max(tree[1], tree[2]);
		cout << big << endl;
	}
}