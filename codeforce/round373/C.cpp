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

#define N 200500
#define Q 100
#define INF 1000000000000000

typedef long long LL;
typedef unsigned long long ULL;
using namespace std;
char c[N];
int main(){
	
	DRII(n, t);
	int point = n;
	REP(i, n){
		cin >> c[i];
		if(c[i] == '.'){
			point = i;
		}
	}
	int k = 0;
	bool check = 1;
	int back = n;
	int start = point + 1;
	while(k < t){
		if(check != 1) break;
		check = 0;
		REPP(i, start, n){
			if(i == point + 1){
				if(c[i] >= '5'){
					for(int j = i - 2; j >= 0; j--){

						c[j] = c[j] + 1;
						if(c[j] == ':'){
							c[j] = '0';
							if(j == 0){
								cout << "1";
							}
						}else{
							break;
						}
					}
					check = 1;
					REPP(j, i, back){
						c[j] = '0';
					}
					back = i;
					start = i - 1;
					break;
				}
			}else if(c[i] >= '5'){
				c[i - 1] = c[i - 1] + 1;
				check = 1;
				REPP(j, i, back){
					c[j] = '0';
				}
				back = i;
				start = i - 1;
				break;
			}
		}
		k++;
	}
	if(point != n){
		for(int i = n - 1; i >= 0; i--){
			if(c[i] == '.'){
				point = i - 1;
				break;
			}else if(c[i] != '0'){
				point = i;
				break;
			}
		}
		REP(i, point + 1){
			cout << c[i];
		}
		cout << endl;
	}else{
		REP(i, n){
			cout << c[i];
		}
		cout << endl;
	}

}