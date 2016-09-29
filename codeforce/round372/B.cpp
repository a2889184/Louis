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
int dict[26];
int num[26];

int main(){
	string s;

	cin >> s;
	if(s.size() < 26) {
		cout << "-1";
		return 0;
	}
	REP(i, s.size() - 25){
		REPP(j, i, i + 26){
			char now = s[j];
			if(now != '?'){
				if(num[(int)(now - 'A')] == 0){
					num[(int)(now - 'A')]++;
					dict[(int)(now - 'A')] = j;
				}else{
					int prepos = dict[(int)(now - 'A')];
					dict[(int)(now - 'A')] = j;
					REPP(k, i, j){
						now = s[k];
						if(now != '?'){
							num[(int)(now - 'A')]--;
						}
					}
					i = prepos;
					break;
				}
			}

			if(j == (i + 25)){
				
				REPP(k, i, j + 1){

					if(s[k] == '?'){
						REP(l, 26){
							if(num[l] == 0){
								num[l]++;
								s[k] = (l + 'A');
								break;
							}
						}
					}
				}
				REP(k, s.size()){
					if(s[k] == '?'){
						s[k] = 'A';
					}
				}
				cout << s << endl;
				return 0;
			}


		}
	}

	cout << "-1";
	
}