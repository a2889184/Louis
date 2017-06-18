#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include "armadillo"
#define K 50
#define M 50000
#define N 5000
#define Top 5
using namespace std;
using namespace arma;

map<int, int> userid;
map<int, int> itemid;
map<int, int>::iterator it;
mat Guser;
mat Gitem;
mat Guserall(M, K);
mat Gitemall(N, K);
mat T1(M, M);
mat T2(N, N);
vector< pair<int, float> > *R1;
vector< pair<int, float> > *R2;
mat U1(M, K), U2(M, K);
mat D1(K, K), D2(K, K);
mat V1(N, K), V2(N, K);
//mat R1_re(M, N);

mat Matching(mat Z1, mat Z2, int i, mat*);
void FindBest(mat, mat, mat, mat, mat, vec*, vec*);
mat mattomat(mat, int);
mat vectomat(vec);
mat multi(mat, mat);


int main(){
	fstream fin1, fin2, fin3, fin4, fin5, fin6, fin7, fin8, output;
	
	fin1.open("split_50/uncoverged_50_U_1.txt", ios::in);
	fin2.open("split_50/uncoverged_50_U_2.txt", ios::in);
	fin3.open("split_50/uncoverged_50_D_1.txt", ios::in);
	fin4.open("split_50/uncoverged_50_D_2.txt", ios::in);
	fin5.open("split_50/uncoverged_50_V_1.txt", ios::in);
	fin6.open("split_50/uncoverged_50_V_2.txt", ios::in);
	fin7.open("split_train1.txt", ios::in);
	fin8.open("source.txt", ios::in);
	for (int i = 0; i < M; i++){
		for (int j = 0; j < K; j++){
			fin1 >> U1.at(i, j);
			fin2 >> U2.at(i, j);
		}
	}
	for (int i = 0; i < K; i++){
		for (int j = 0; j < K; j++){
			fin3 >> D1.at(i, j);
			fin4 >> D2.at(i, j);
		}
	}
	for (int i = 0; i < N; i++){
		for (int j = 0; j < K; j++){
			fin5 >> V1.at(i, j);
			fin6 >> V2.at(i, j);
		}
	}
	int a, b;
	float c;
	R1 = new vector< pair<int, float> > [M];
	while(fin7 >> a >> b >> c){
		R1[a].push_back(make_pair(b, c));
	}

	R2 = new vector< pair<int, float> > [M];
	while(fin8 >> a >> b >> c){
		R2[a].push_back(make_pair(b, c));

	}

	T1.zeros();
	T2.zeros();
	/*mat UD1 = multi(U1, D1);
	mat UD2 = multi(U2, D2);
	mat VD1 = multi(V1, D1);
	mat VD2 = multi(V2, D2);*/
	vec K1(M), K2(N);
	cout << "start Matching" << endl;
	int time1, time2, time3;
	for (int i = 0; i < K; i++){
		cout << "round :" << i << endl; 
		//UserMatching(U1 * sqrt(D1), U2 * sqrt(D2), i);
		//ItemMatching(V1 * sqrt(D1), V2 * sqrt(D2), i);
		time1 = time(0);
		Guser = Matching(U1*D1, U2*D2, i, &T1);
		time2 = time(0);
		cout << "Matching 1 : spent " << time2 - time1 << " s" << endl;
		Gitem = Matching(V1*D1, V2*D2, i, &T2);
		time3 = time(0);
		cout << "Matching 2 : spent " << time3 - time2 << " s" << endl;
		FindBest(U2, D2, V2.t(), Guser, Gitem, &K1, &K2);
		cout << "FindBest : spent " << time(0) - time3 << " s" << endl;
		for (int j = 0; j < M; j++)
			Guserall.at(j, i) = K1.at(j);
		for (int j = 0; j < N; j++)
			Gitemall.at(j, i) = K2.at(j);
	}
	//cout << multi(UD1, V2.t())<< endl;
	//cout <<multi(UD2, V1.t()) << endl;
	
	float min = 100;
	int finalk=0;

	for (int l = 0; l < K; l++){
		vec V_user = Guserall.col(l);
		vec V_item = Gitemall.col(l);

		//T1 = mattomat(Guserall, i);
		//T2 = mattomat(Gitemall, i);
		/*mat stop = multi(T1, U2);
		mat stop1 = multi(stop, D2);
		stop = multi(stop1, V2.t());
		multi(stop, T2.t());*/
		//R1_re = T1 * U2 * D2 * V2.t() * T2.t();
		//cout <<T1 << endl;
		float error = 0;
		
		for(int i = 0; i < M; i++){
			for(int j = 0; j < R1[i].size(); j++){
				int user_temp = V_user(i);
				int item_temp = V_item(R1[i][j].first);
				mat LA = U2.row(user_temp) * D2 * V2.row(item_temp).t();
				float value = R1[i][j].second - LA(0, 0);
				LA.clear();
				error += value * value;
	 		 	
			}
		}
		//cout << error << endl;
		if (error < min){
			finalk = l;
			min = error;
		}
	}
	
	vec V_user = Guserall.col(finalk); //final Guser
	vec V_item = Gitemall.col(finalk); //final Gitem
	//cout << V_user << endl;
	//cout << V_item << endl;
	vector<int> V2_user[M];
	vector<int> V2_item[N];

	for(int i = 0; i < M; i++){
		V2_user[(int)V_user(i)].push_back(i);
	}
	for(int i = 0; i < N; i++){
		V2_item[(int)V_item(i)].push_back(i);
	}

	for(int i = 0; i < M; i++){
		for(int p = 0; p < V2_user[i].size(); p++){
			int user_temp = V2_user[i][p];
			for(int j = 0; j < R2[i].size(); j++){
				for(int l = 0; l < V2_item[R2[i][j].first].size(); l++){
					int item_temp = V2_item[R2[i][j].first][l];
					bool flag = 0;
					for(int k = 0; k < R1[user_temp].size(); k++){
						//cout << user_temp << " " << item_temp << " " << R2[i][j].second << endl;
						if(R1[user_temp][k].first == item_temp){
							flag = 1;
							break;
						}
					}
					if(flag == 0){
						R1[user_temp].push_back(make_pair(item_temp, R2[i][j].second));
					}
				}
			}
		}

		
	}
	output.open("split_50/finaldata_test.txt", ios::out);
	for(int i = 0; i < M; i++){
		for(int j = 0; j < R1[i].size(); j++){
			output << i << " " << R1[i][j].first << " " << R1[i][j].second << endl;
		}
	}

	//T1 = mattomat(Guserall, finalk); //final Guser
	//T2 = mattomat(Gitemall, finalk); //final Gitem
	//cout << T1 << endl;
	//cout << T2 << endl;
	//mat R1_re = T1 * RRR * T2.t();
	//cout << R1_re << endl;
	/*for (int i = 0; i < T1.n_rows; i++){
		for (int j = 0; j < T1.n_cols; j++){
			if (j == i)
				T1(i, j) = 1;
			else
				T1(i, j) = 0;
		}
	}
	for (int i = 0; i < T2.n_rows; i++){
		for (int j = 0; j < T2.n_cols; j++){
			if (j == i)
				T2(i, j) = 1;
			else
				T2(i, j) = 0;
		}
	}*/
	// cout << T1 << endl;
	// cout << T2 << endl;
	/*mat stop = multi(T1, U2);
	mat stop1 = multi(stop, D2);
	stop = multi(stop1, V2.t());
	mat R1_re = multi(stop, T2.t());*/
	//R1_re = T1 * U2 * D2 * V2.t() * T2.t();
	//cout << R1_re << endl;
	//system("pause");
	return 0;
}

mat multi(mat A, mat B){
	mat result(A.n_rows, B.n_cols);
	result.zeros();
	for (int i = 0; i < A.n_rows; i++){
		for (int j = 0; j < B.n_cols; j++){
			for (int k = 0; k < A.n_cols; k++){
				result.at(i, j) += A.at(i, k) * B.at(k, j);
			}
		}
	}
	return result;
}
mat Matching(mat Z1, mat Z2, int i, mat *matrix){
	mat AA = Z1.cols(0, i);
	mat BB = Z2.cols(0, i);
	mat G1(Z1.n_rows, Top), G2(Z1.n_rows, Top);
	//vec plus(Z1.n_rows), minus(Z1.n_rows);
	vec top_plus(Top+1), top_minus(Top+1);
	vec top_indexplus(Top+1), top_indexminus(Top+1);
	int num, numb;
	float plus, minus;
	float plusmin = 0, minusmin = 0, front = 0;

	for (int k = 0; k < Z1.n_rows; k++) {
		num = 0, numb=0;
		for (int j = 0; j < Z1.n_rows; j++) {
			plus = (AA.at(k, i) - BB.at(j, i)) * (AA.at(k, i) - BB.at(j, i)) + matrix->at(j, k);
			minus = (AA.at(k, i) + BB.at(j, i)) * (AA.at(k, i) + BB.at(j, i)) + matrix->at(j, k);
		
		//	plus = (AA.at(k, i) - BB.at(j, i)) * (AA.at(k, i) - BB.at(j, i)) + front;
		//	minus = (AA.at(k, i) + BB.at(j, i)) * (AA.at(k, i) + BB.at(j, i)) + front;

			top_plus(num) = plus;
			top_indexplus(num) = j;
			for (int a = num - 1; a >= 0; a--) {
				if (top_plus(a + 1) < top_plus(a)) {
					float temp = top_plus(a + 1);
					top_plus(a + 1) = top_plus(a);
					top_plus(a) = temp;
					int tempi = top_indexplus(a + 1);
					top_indexplus(a + 1) = top_indexplus(a);
					top_indexplus(a) = tempi;
				}
			}
			if (num < Top)
				num++;

			top_minus(numb) = minus;
			top_indexminus(numb) = j;
			for (int a = numb - 1; a >= 0; a--) {
				if (top_minus(a + 1) < top_minus(a)) {
					float temp = top_minus(a + 1);
					top_minus(a + 1) = top_minus(a);
					top_minus(a) = temp;
					int tempi = top_indexminus(a + 1);
					top_indexminus(a + 1) = top_indexminus(a);
					top_indexminus(a) = tempi;
				}
			}
			if (numb < Top)
				numb++;
		}

		plusmin += top_plus(0);
		minusmin += top_minus(0);
		//cout << top_indexplus << endl;
		//cout << top_indexminus << endl;
		for (int a = 0; a < Top; a++) {
			G1(k, a) = top_indexplus(a);
			G2(k, a) = top_indexminus(a);
		}
	}

	if (plusmin < minusmin){
		for (int k = 0; k < Z1.n_rows; k++){
			for (int j = 0; j < Z1.n_rows; j++)
				matrix->at(j, k) += (AA.at(k, i) - BB.at(j, i)) * (AA.at(k, i) - BB.at(j, i));
		}
		return G1;
	}
	else{
		for (int k = 0; k < Z1.n_rows; k++){
			for (int j = 0; j < Z1.n_rows; j++)
				matrix->at(j, k) += (AA.at(k, i) + BB.at(j, i)) * (AA.at(k, i) + BB.at(j, i));
		}
		return G2;
	}

}

void FindBest(mat U, mat D, mat V_t, mat G1, mat G2, vec* V1, vec* V2){
	for (int i = 0; i < M; i++){
		V1->at(i) = G1(i, 0);
	}
	for (int i = 0; i < N; i++){
		V2->at(i) = G2(i, 0);
	}
	// mat GG1;
	// mat GG2 = vectomat(*V2);
	for (int k = 0; k < G1.n_rows; k++){
		float min = 100;
		int finali = 0;
		for (int l = 0; l < Top; l++){
			V1->at(k) = G1(k, l);
			// GG1 = vectomat(*V1);
			/*mat stop = multi(GG1, UDV2);
			R1_re = multi(stop, GG2.t());*/
			// R1_re = GG1 * UDV2 * GG2.t();
			float error = 0;

			for(int i = 0; i < M; i++){
				if(i % 2000 != 0) continue;
				for(int j = 0; j < R1[i].size(); j++){
					if(j % 800 != 0) continue;
					int user_temp = V1->at(i);
					int item_temp = V2->at(R1[i][j].first);
					mat LA = U.row(user_temp) * D * V_t.col(item_temp);
					float value = R1[i][j].second - LA(0, 0);
					LA.clear();
					error += value * value;
		 		 	
				}
			}
			// for (int a = 0; a < M; a++){
			// 	for (int b = 0; b < N; b++){
			// 		if (R1.at(a, b) != 0)
			// 			error += (R1.at(a, b) - R1_re.at(a, b)) * (R1.at(a, b) - R1_re.at(a, b));
			// 	}
			// }
			if (error < min){
				finali = l;
				min = error;
			}
		}
		V1->at(k) = G1(k, finali);
		//cout << "//////////////" << endl;
		//cout << *V1 << endl;
	}
	//GG1 = vectomat(*V1);
	for (int k = 0; k < G2.n_rows; k++){
		float min = 100;
		int finali = 0;
		for (int l = 0; l < Top; l++){
			V2->at(k) = G2(k, l);
			//GG2 = vectomat(*V2);
			/*mat stop = multi(GG1, UDV2);
			R1_re = multi(stop, GG2.t());*/
			//R1_re = GG1 * UDV2 * GG2.t();
			float error = 0;
			for(int i = 0; i < M; i++){
				if(i % 2000 != 0) continue;
				for(int j = 0; j < R1[i].size(); j++){
					if(rand() % 800 != 0) continue;
					int user_temp = V1->at(i);
					int item_temp = V2->at(R1[i][j].first);
					mat LA = U.row(user_temp) * D * V_t.col(item_temp);
					float value = R1[i][j].second - LA(0, 0);
					LA.clear();
					error += value * value;
		 		 	
				}
			}
			if (error < min){
				finali = l;
				min = error;
			}
		}
		V2->at(k) = G2(k, finali);
	}
	return;
}

mat mattomat(mat vector, int j){
	mat result(vector.n_rows, vector.n_rows);
	result.zeros();
	for (int i = 0; i < vector.n_rows; i++){
		int position = vector.at(i, j);
		result.at(i, position) = 1;
	}
	return result;
}

mat vectomat(vec vector){
	mat result(vector.n_elem, vector.n_elem);
	result.zeros();
	for (int i = 0; i < vector.n_elem; i++){
		int position = vector.at(i);
		result.at(i, position) = 1;
	}
	return result;
}