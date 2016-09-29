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

#define N 1010
#define Q 100

typedef long long LL;
using namespace std;

class node{
	
};

int main(){

	//int number = 0;
	DRIII(n, m, q);
	//int operation[N][3];
	int operation[Q][3];
	int bookshelf[Q][N][N];
	int number[Q];
	VI memory;

	REP(i, q){
		RI(operation[i][0]);
		if(operation[i][0] == 1 || operation[i][0] == 2){
			RII(operation[i][1], operation[i][2]);
		}else if(operation[i][0] == 3){
			RI(operation[i][1]);
		}else{
			RI(operation[i][1]);
			memory.PB(operation[i][1]);
		}
	}
	REP(i, q){
		//DRI(operation[i][0]);
		//operation[i][0] = operation[i][0];
		if(operation[i][0] == 1){
			//DRII(operation[i][1], operation[i][2]);
			//operation[i][1] = operation[i][1];
			//operation[i][2] = operation[i][2];
			if(bookshelf[i][operation[i][1]][operation[i][2]] == 0) {
				bookshelf[i + 1][operation[i][1]][m]++;
				bookshelf[i + 1][operation[i][1]][operation[i][2]] = 1;
				number[i + 1]++;
			}
		}else if(operation[i][0] == 2){
			//DRII(operation[i][1], operation[i][2]);
			//operation[i][1] = operation[i][1];
			//operation[i][2] = operation[i][2];
			if(bookshelf[i][operation[i][1]][operation[i][2]] == 1) {
				bookshelf[i + 1][operation[i][1]][m]--;
				bookshelf[i + 1][operation[i][1]][operation[i][2]] = 0;
				number[i + 1]--;
			}
		}else if(operation[i][0] == 3){
			//DRI(operation[i][1]);
			REP(j, m){
				bookshelf[i + 1][operation[i][1]][j] = (bookshelf[i][operation[i][1]][j] ^ 1);
			}
			bookshelf[i + 1][operation[i][1]][m] = m - bookshelf[i][operation[i][1]][m];
			number[i + 1] = number[i] + (m - 2 * bookshelf[i + 1][operation[i][1]][m]);

		}else{
			//DRI(operation[i][1]);
			REP(j, n)REP(k, m + 1){
				bookshelf[i + 1][j][k] = bookshelf[i][j][k];
			}

			number[i + 1] = number[i];
		}
	}

	REP(i, q){
		PI(number[i]);
	}

}