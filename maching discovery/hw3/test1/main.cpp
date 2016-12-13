#include<fstream>
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<string>
#include<vector>
#include<map>
#include"armadillo"
#define K 2
#define M 4
#define N 5
using namespace std;
using namespace arma;

map<int, int> userid;
map<int, int> itemid;
map<int, int>::iterator it;
vec Guser(M);
vec Gitem(M);
mat Guserall(M, K);
mat Gitemall(N, K);
vec S(K);
mat T1(M, M);
mat T2(N, N);
vec G1(M);
vec G2(M);
vec G3(N);
vec G4(N);
mat R1(M, N);
mat R2(M, N);
mat W1(M, N);
mat U1(M, K), U2(M, K);
mat D1(K, K), D2(K, K);
mat V1(N, K), V2(N, K);

void UserMatching(mat Z1, mat Z2, int i);
void ItemMatching(mat Z1, mat Z2, int i);
float NN(mat, mat, int);
mat mattomat(mat, int);
mat vectomat(vec);

int main(){
	umat location1(2, 20);
	vec value1(20);

	/*for (int i = 0; i < 50000; i++){
		for (int j = 0; j < 5000; j++){
			R1.at(i, j) = 0;
			R2.at(i, j) = 0;
		}
	}*/
	R1.zeros();
	R2.zeros();
	fstream fin1, fin2, fin3, fin4, fin5, fin6, output;
	/*fin1.open("source_test.txt", ios::in);
	int a, b;
	float c;
	int userindex, itemindex;
	int all=0;
	while (!fin1.eof()){
		if(all == 20)
			break;
		fin1 >> a >> b >> c;
		userindex = userid.size();
		itemindex = itemid.size();
		it = userid.find(a);
		if (it != userid.end())
			userindex = it->second;
		else
			userid[a] = userid.size();

		it = itemid.find(b);
		if (it != itemid.end())
			itemindex = it->second;
		else
			itemid[b] = itemid.size();
		R1.at(userindex, itemindex) = c;
	}
	fin2.open("train_test.txt", ios::in);
	while (!fin2.eof()){
		fin2 >> a >> b >> c;
		userindex = userid.size();
		itemindex = itemid.size();
		it = userid.find(a);
		if (it != userid.end())
			userindex = it->second;
		else
			userid[a] = userid.size();

		it = itemid.find(b);
		if (it != itemid.end())
			itemindex = it->second;
		else
			itemid[b] = itemid.size();
		R2.at(userindex, itemindex) = c;
	}*/
	//sp_mat R1(location1, value1, 500, 50);
	
	fin1.open("U_1.txt", ios::in);
	fin2.open("U_2.txt", ios::in);
	fin3.open("D_1.txt", ios::in);
	fin4.open("D_2.txt", ios::in);
	fin5.open("V_1.txt", ios::in);
	fin6.open("V_2.txt", ios::in);
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
	cout << D1 << endl;
	S.ones();
	T1.zeros();
	T2.zeros();
	//mat D1 = vectomat(S1);
	//mat D2 = vectomat(S2);
	for (int i = 0; i < K; i++){
		UserMatching(U1 * sqrt(D1), U2 * sqrt(D2), i);
		ItemMatching(V1 * sqrt(D1), V2 * sqrt(D2), i);
		for (int j = 0; j < M; j++)
			Guserall.at(j, i) = Guser.at(j);
		for (int j = 0; j < N; j++)
			Gitemall.at(j, i) = Gitem.at(j);
	}
	float min = 100;
	int finalk;
	for (int i = 0; i < K; i++){

		T1 = mattomat(Guserall, i);
		T2 = mattomat(Gitemall, i);

		mat R1_my = T1 * U2 * D2 * V2.t() * trans(T2);
		float error = 0;
		for (int a = 0; a < M; a++){
			for (int b = 0; b < N; b++){
				if (W1.at(a, b) != 0)
					error += (R1.at(a, b) - R1_my.at(a, b)) * (R1.at(a, b) - R1_my.at(a, b));
			}
		}
		if (error < min){
			finalk = i;
			min = error;
		}
	}

	T1 = mattomat(Guserall, finalk); //final Guser
	T2 = mattomat(Gitemall, finalk); //final Gitem

	cout << T1 << endl;
	cout << T2 << endl;
	system("pause");
	return 0;
}
	
void UserMatching(mat Z1, mat Z2, int i){
	mat AA = Z1.cols(0, i);
	mat BB = Z2.cols(0, i);
	vec plus(M), minus(M);
	float plusmin=0, minusmin=0;
	//Z1, Z2 size M*N
	G1.zeros();
	G2.zeros();
	//float plus = NN(AA, BB, 0);
	for (int k = 0; k < M; k++){
		for (int j = 0; j < M; j++){
			plus.at(j) = (AA.at(k, i) - BB.at(j, i)) * (AA.at(k, i) - BB.at(j, i)) + T1.at(j, k);
			minus.at(j) = (AA.at(k, i) + BB.at(j, i)) * (AA.at(k, i) + BB.at(j, i)) + T1.at(j, k);
		}
		plusmin += plus.min();
		minusmin += minus.min();
		for (int j = 0; j < M; j++){
			if (plus.at(j) == plus.min())
				G1.at(k) = j;
			if (minus.at(j) == minus.min())
				G2.at(k) = j;
		}
	}
	if (plusmin < minusmin){
		for (int k = 0; k < M; k++){
			for (int j = 0; j < M; j++)
				T1.at(j, k) += (AA.at(k, i) - BB.at(j, i)) * (AA.at(k, i) - BB.at(j, i));
		}
		Guser = G1; //vector to mat
	}
	else{
		for (int k = 0; k < M; k++){
			for (int j = 0; j < M; j++)
				T1.at(j, k) += (AA.at(k, i) + BB.at(j, i)) * (AA.at(k, i) + BB.at(j, i));
		}
		Guser = G2; //vector to mat
		S.at(i) = -1;
	}
}

void ItemMatching(mat Z1, mat Z2, int i){
	mat AA = Z1.cols(0, i);
	mat BB = Z2.cols(0, i);
	vec plus(N), minus(N);
	float plusmin=0, minusmin=0;
	G3.zeros();
	G4.zeros();
	//float plus = NN(AA, BB, 0);
	for (int k = 0; k < N; k++){
		for (int j = 0; j < N; j++){
			plus.at(j) = (AA.at(k, i) - BB.at(j, i)) * (AA.at(k, i) - BB.at(j, i)) + T1.at(j, k);
			minus.at(j) = (AA.at(k, i) + BB.at(j, i)) * (AA.at(k, i) + BB.at(j, i)) + T1.at(j, k);
		}
		plusmin += plus.min();
		minusmin += minus.min();
		for (int j = 0; j < N; j++){
			if (plus.at(j) == plus.min())
				G3.at(k) = j;
			if (minus.at(j) == minus.min())
				G4.at(k) = j;
		}
	}
	if (plusmin < minusmin){
		for (int k = 0; k < N; k++){
			for (int j = 0; j < N; j++)
				T2.at(j, k) += (AA.at(k, i) - BB.at(j, i)) * (AA.at(k, i) - BB.at(j, i));
		}
		Gitem = G3; //vector to mat
	}
	else{
		for (int k = 0; k < N; k++){
			for (int j = 0; j < N; j++)
				T2.at(j, k) += (AA.at(k, i) + BB.at(j, i)) * (AA.at(k, i) + BB.at(j, i));
		}
		Gitem = G4; //vector to mat
		S.at(i) = -1;
	}
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
	for (int i = 0; i < vector.n_elem; i++)
		result.at(i, i) = vector.at(i);
	return result;
}