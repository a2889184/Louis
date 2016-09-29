#include <iostream>
#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>

using namespace std;
typedef long long LL ;

int main(){
	int n;
	LL a;
	cin >> a >> n;
	cout << (a / n) << endl;
	/*
	int n;
	cin >> n;
	cout << n << " " << (n * 18) << " " << (354284621) << " " << 0 << " " << (n - 1) << endl;
	srand(time(0));
	for(int i = 0; i < n; i++){
		for(int j = i + 1; j < i + 19; j++){
			if(j >= n){
				cout << i << " " << (j - n) << " " <<  154251351 * (rand() % 4) << endl;
			}
			else cout << i << " " << j << " " << 154548132 * (rand() % 4) << endl;
		}
	}
	*/

}