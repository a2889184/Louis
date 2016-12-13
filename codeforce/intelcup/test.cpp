#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <ctime>

using namespace std;

int main(){
	int n;
	cin >> n;
	cout << n << endl;
	srand(time(0));
	for(int i  = 0; i < n; i++){
		cout << (rand() % 13465 + 125) << " ";
	}

	cout << endl;
	int *x = new int[n];
	for(int i = 0; i < n; i++){
		x[i] = i + 1;
	}
	int mod = n;
	for(int i = 0; i < n; i++){
		int pos = rand() % mod;
		cout << x[pos] << " ";
		x[pos] = x[mod - 1];
		mod--;
	}
}