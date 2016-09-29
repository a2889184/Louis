#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <time.h>  

using namespace std;

int main(){
	cout << 200000 << endl;
	srand (time(NULL));
	for(int i = 0; i < 200000; i++){
		cout << 1000;
		if(i != 199999) cout << " ";
	}
	cout << endl;
	for(int i = 0; i < 200000; i++){
		cout << 1000;
		if(i != 199999) cout << " ";
	}
	cout << endl;
}