#include <iostream>
#include <algorithm>
#include <cmath>

using namespace std;



int main(){
	int input;
	cin >> input;
 	long long a;
 	long long b;

	if(input % 2 == 0){
		a = (2 + (long long)input * (long long)input / 2) / 2;
		b = a - 2;
	}

	else{
		a = (1 + (long long)input * (long long)input) / 2;
		b = a - 1; 
	}
	if(b == 0) {
		cout << -1;
		return 0;
	} 
	cout << a << " " << b;

}