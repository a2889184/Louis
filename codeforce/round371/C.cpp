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

char* decode(LL num){
	char* out = new char[18];
	REP(i, 18){
		out[i] = (char)((int)(num % 2) + '0');
		num /= 10;
	}
	return out;
}

class node{
public:
	node(){
		count = 0;
		child = NULL;
	}


	LL count;
	node *child;

};

class nodeControl{
public:
	nodeControl(){
		head = new node[2];
	}
	void add(LL num){
		char *in = decode(num);
		
		node *now;
		
		if(in[0] == '0') {
			now = &(head[0]);
		}else{
			now = &(head[1]);
		}

		REPP(i, 1, 18){
			if(now->child == NULL){
				now->child = new node[2];
			}
			if(in[i] == '0'){
				now = &(now->child[0]);
			}else{
				now = &(now->child[1]);
			}
			if(i == 17){
				now->count += 1;
			}
		}

	}

	void sub(LL num){
		char *in = decode(num);
		node *now;
		if(in[0] == '0') {
			now = &(head[0]);
		}else{
			now = &(head[1]);
		}
		//cerr << (now.child == NULL) << endl;
		REPP(i, 1, 18){
			//cerr << i << endl;
			if(in[i] == '0'){
				now = &(now->child[0]);
			}else{
				now = &(now->child[1]);
			}
			if(i == 17){
				now->count -= 1;
			}
		}
	}

	LL ques(string s){
		char in[18];
		fill(in, in + 18, '0');
		REP(i, s.length()){
			in[i] = s[s.length() - 1 - i];
		}
		node *now;
		if(in[0] == '0') {
			now = &(head[0]);
		}else{
			now = &(head[1]);
		}
		REPP(i, 1, 18){
			//cerr << i << endl;
			if(now->child == NULL) return 0;
			if(in[i] == '0'){
				now = &(now->child[0]);
			}else{
				now = &(now->child[1]);
			}
			if(i == 17){
				return now->count;
			}
		}
	}
	node *head;
};




int main(){
	DRI(t);
	char c;
	LL v;
	string s;
	nodeControl nc;
	REP(i, t){
		cin >> c >> v;
		if(c == '+'){
			nc.add(v);
		}else if(c == '-'){
			nc.sub(v);
		}else{
			s = to_string(v);
			cout << nc.ques(s) << endl;
		}
	}
}

