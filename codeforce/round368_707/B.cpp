#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>

using namespace std;

class node{
public:
	node(){}
	node(int a, int b){
		name = a;
		weight = b;
	}
	int name;
	int weight;

};

int main(){
	int city, road, flour;

	cin >> city >> road >> flour;
	//cout << "-11" << endl;
	/*
	int **matrice = new int *[city + 1];

	for(int i = 0; i < city + 1; i++){
		matrice[i] = new int[city + 1];
	}
	//cout << "0" << endl;*/

	vector <node> *graph = new vector <node> [city + 1];
	/*
	for(int i = 1; i < city + 1; i++){
		for(int j = 1; j < city + 1; j++){
			if(i == j){
				matrice[i][j] = 0;
			}
			else matrice[i][j] = -1;
		}
	}
*/
	//cout << "1" << endl;
	int num1, num2, weight;
	for(int i = 0; i < road; i++){
		cin >> num1 >> num2 >> weight;

		

			
				node aa(num2, weight);
				graph[num1].push_back(aa);
			

		
		
			
				node bb(num1, weight);
				graph[num2].push_back(bb);
			

		
		/*
		if(matrice[num1][num2] > weight || matrice[num1][num2] < 0){
			matrice[num1][num2] = weight;
			matrice[num2][num1] = weight;
		}*/
			
	}
/*vector <node>:: iterator it;
	for(it = graph[num1].begin(); it != graph[num1].end(); it++){
		cout << (*it).weight << endl;
	}*/
	//cout << "2" << endl;

	int min = 1300000000;
	int *f = new int[flour];
	for(int i = 0; i < flour; i++){
		cin >> f[i];
	}

	sort(f, f + flour);
	//cout << "3" << endl;

	for(int i = 0; i < flour; i++){
		vector <node>:: iterator it;
		for(it = graph[f[i]].begin(); it != graph[f[i]].end(); it++){
			if((*it).weight < min) {

				/*int *p = find(f, f + flour, j);
				if(p == f + flour){
					min = matrice[f[i]][j];
				}*/
				int l = 0;
				int r = flour - 1;
				int m;

				while(l <= r){
					m = (l + r)/2;
					if((*it).name > f[m]){
						l = m + 1;
					}
					else if((*it).name < f[m]){
						r = m - 1;
					}
					else{
						goto T;
					}
				}
				min = (*it).weight;
T:
int k;
			}
		}
	}


	if(min != 1300000000){
		cout << min;
	}
	else cout << "-1";

}

