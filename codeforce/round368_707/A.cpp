#include <iostream>
#include <algorithm>
#include <cmath>

using namespace std;

int main(){
	int row, column;

	cin >> row >> column;

	/*char** matrice = new char *[row];
	for(int i = 0; i < row; i++){
		matrice[i] = new char[column];
	}*/
	char input;
	for(int i = 0; i < row; i++){
		for(int j = 0; j < column; j++){

			cin >> input;
			//cin >> matrice[i][j];
			if(input != 'W' && input != 'B' && input != 'G'){
				cout << "#Color" << endl;
				return 0;
			}
		}
	}

	cout << "#Black&White" << endl;
}