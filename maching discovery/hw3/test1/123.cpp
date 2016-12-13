#include <iostream>
#include <vector>

using namespace std;

int main(){
	int n = 4;
	vector<int> *a;
	a = new vector<int> [n];

	a[1].push_back(3);
	a[3].push_back(5);

	for(int i = 0; i < n; i++){
		for(int j = 0; j < a[i].size(); j++){
			cout << a[i][j] << " ";
		}
		cout << endl;
	}
}