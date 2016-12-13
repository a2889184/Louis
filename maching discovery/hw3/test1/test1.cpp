#include <iostream>
#include "armadillo"
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <utility>
#include <vector>

using namespace std;
using namespace arma;

int n1 = 50000;
int m1 = 5000;
int n2 = 50000;
int m2 = 5000;
int PQ = 5;
vector< pair<int, double> > *m_1;
vector< pair<int, double> > *m_2;
// mat m_1(n1, m1);
// mat W_1(n1, m1);
mat P_1(n1, PQ);
mat Q_1(PQ, m1);
double B_total_1;
vec B_User_1(n1);
vec B_Item_1(m1);
double B_total_2;
vec B_User_2(n2);
vec B_Item_2(m2);

// mat m_2(n2, m2);
// mat W_2(n2, m2);
mat P_2(n2, PQ);
mat Q_2(PQ, m2);


vector< pair<int, double> > *T;
double cost = 0;
double LR = 0.005;
double LD_P = 0.1, LD_Q = 0.1;
double LD_B = 0.1;
void init(vector< pair<int, double> > *m, mat &P, mat &Q, double &B_total, vec &B_Item, vec &B_User, int row_len){
	/*****************************************
	* W, m set zeros
	* P, Q set rand numbers
	*****************************************/
	//m = new vector< pair<int, double> > [row_len];
	//T = new vector< pair<int, double> > [row_len];

	P.randu();
	Q.randu();
	B_Item.randu();
	B_User.randu();
	srand(time(0));
	B_total = (double)(rand () % 100) / 100.0;

}



void read(char *filename, vector< pair<int, double> > *m){
	int a, b;
	double c;
	ifstream file(filename);
	while(file >> a >> b >> c){
		//cout << a << " " << b << " " <<  c << endl;
		// m(a, b) = c;
		// W(a, b) = 1;
		//cerr << a << " " << b << " " << c << endl;
		m[a].push_back(make_pair(b, c));

		T[a].push_back(make_pair(b, 0));

	}
	cerr << "read " << filename << " over" << endl;
}
double computeCost(vector< pair<int, double> > *m, mat &P, mat &Q, double &B_total, vec &B_Item, vec &B_User, int row_len){
	cost = 0;

	for(int i = 0; i < row_len; i++){
		for(int j = 0; j < m[i].size(); j++){
			double value = T[i][j].second - m[i][j].second;

			cost += value * value;
		}
	}
	cost += LD_P * sum(sum(P % P));
	cost += LD_Q * sum(sum(Q % Q));
	cost += LD_B * sum(B_User % B_User);
	cost += LD_B * sum(B_Item % B_Item);
	cost += LD_B * B_total * B_total;

	cost /= 2;

	return cost;
}

void updatePQ(vector< pair<int, double> > *m, mat &P, mat &Q, double &B_total, vec &B_Item, vec &B_User, int row_len){

	for(int i = 0; i < row_len; i++){

		for(int j = 0; j < m[i].size(); j++){
			double value = T[i][j].second - m[i][j].second;
			P.row(i) -= (LR * (value * trans(Q.col(m[i][j].first)) + LD_P * P.row(i)));
			B_User(i) -= (LR * (value + LD_B * B_User(i)));
			B_total -= (LR * (value + LD_B * B_total));

			Q.col(m[i][j].first) -= (LR * (value * trans(P.row(i)) + LD_Q * Q.col(m[i][j].first)));
			B_Item(m[i][j].first) -= (LR * (value + LD_B * B_Item(m[i][j].first)));
			
			/*P.row(i) -= LR * (value + LD_P * (P.row(i)));
			Q.col(j) -= LR * (value + LD_Q * (Q.col(j)));*/
		}

		
	}

	/*for(int j = 0; j < col_len; j++){

		for(int i = 0; i < row_len; i++){
			Q.col(j) -= (LR * (D(i, j) * trans(P.row(i)) + LD_Q * Q.col(j)));
			//B_Item(j) -= LR * (D(i, j) + LD_B * B_Item(j));
			//B_total -= LR * (D(i, j) + LD_B * B_total);
		}
		
	}*/
	
}

void multiply_Factorized_Matrix(vector< pair<int, double> > *m, mat &P, mat &Q, double &B_total, 
	vec &B_Item, vec &B_User, int row_len){
	for(int i = 0; i < row_len; i++){
		for(int j = 0; j < m[i].size(); j++){
			/*T[i][j].second = sum((P.row(i) + B_User.row(i) + B_total.first) % 
				trans(Q.col(m[i][j].first) + B_Item.col(m[i][j].first) + B_total.second));*/
 		 	//T[i][j].second = sum(P.row(i) % trans(Q.col(m[i][j].first)));
 		 	T[i][j].second = sum(P.row(i) % trans(Q.col(m[i][j].first))) + B_User(i) + B_Item(m[i][j].first) + B_total;
		}
	}
}

void MF_stochastic(vector< pair<int, double> > *m, mat &P, mat &Q, double &B_total, vec &B_Item, vec &B_User, 
	int row_len){
	cerr << "MF start" << endl;
	double pre = 0;
	double now = 0;
	int dd = 0;
	while(now < pre || pre == 0){
		dd++;
		
		multiply_Factorized_Matrix(m, P, Q, B_total, B_Item, B_User, row_len);
		// if (dd % 4 == 0){
		// 	cout << "multi: " << endl;
		// 	cout << T << endl;
		// }
		pre = now;
		//cout << now << endl;
		now = computeCost(m, P, Q, B_total, B_Item, B_User, row_len);
		cout << now << endl;
		updatePQ(m, P, Q, B_total, B_Item, B_User, row_len);
	}
	cerr << dd << endl;
	cerr << "MF over" << endl;
}
/*
void ALS(mat &W, mat &m, mat &P, mat &Q, int row_len, int col_len){
	int dd = 0;
	mat TempP(row_len, PQ);
	mat IP(PQ, PQ);
	mat SP(PQ, 1);
	mat IQ(PQ, PQ);
	mat SQ(1, PQ);
	mat TempQ(PQ, col_len);
	while(dd < 50){
		dd++;
		T = P * Q;

		mat D = T - m;
		for(int i = 0; i < row_len; i++){
			IP.zeros();
			SP.zeros();
			for(int j = 0; j < col_len; j++){
				IP += Q.col(j) * trans(Q.col(j)) + LD_P * eye<mat>(PQ, PQ);
				SP += D(i, j) * Q.col(j);
			}
			TempP.row(i) = trans(inv(IP) * SP);
		}

		for(int j = 0; j < col_len; j++){
			IQ.zeros();
			SQ.zeros();
			for(int i = 0; i < row_len; i++){
				IQ += trans(P.row(i)) * P.row(i) + LD_Q * eye<mat>(PQ, PQ);
				SQ += D(i, j) * P.row(i);
			}
			TempQ.col(j) = inv(IQ) * trans(SQ);
		}
		P = TempP;
		Q = TempQ;
	}
	
}*/
mat rebuilt_diag(int row, int col, vec &s){
	mat s_m(row, col);
	s_m.zeros();
	for(int i = 0; i < s.size(); i++){
		s_m(i, i) = s(i);
	}

	return s_m;
}

void SVD_modify(mat &U, mat &D, mat &V, mat &P, mat &Q){
	mat U_P, D_P, V_P, U_Q, D_Q, V_Q, U_X, D_X, V_X;
	vec s_P, s_Q, s_X;

	svd(U_P, s_P, V_P, P);

	svd(U_Q, s_Q, V_Q, Q.t());

	svd(U_X, s_X, V_X, diagmat(s_P) * trans(V_P.cols(0, s_P.size() - 1)) * V_Q.cols(0, s_Q.size() - 1) * trans(diagmat(s_Q)));
	
	/*mat X1 = U_Q * rebuilt_diag(U_Q.n_cols, V_Q.n_cols, s_Q) * V_Q.t();
	mat X2 = U_P * rebuilt_diag(U_P.n_cols, V_P.n_cols, s_P) * V_P.t();
	mat X3 = X2 * (X1.t());
	for(int i = 0; i < n1; i++){
		for(int j = 0; j < m1; j++){
			cout << X3(i, j) << " ";
		}
		cout << endl;
	}*/


	U = U_P.cols(0, s_P.size() - 1) * U_X.cols(0, s_X.size() - 1);
	D = diagmat(s_X);

	V = U_Q.cols(0, s_Q.size() - 1) * V_X.cols(0, s_X.size() - 1);
}

void print_mat(mat &matt, char *name){
	fstream out;
	out.open(name, ios::out);
	for(int i = 0; i < matt.n_rows; i++){
		for(int j = 0; j < matt.n_cols; j++){
			out << matt(i, j) << " ";
		}
		out << endl;
	}
}


int main(int argc, char **argv){
	/*P_init.fi
	B_User_init.randu();
	B_Item_init.randu();
	P_init *= 2;
	P_init -= 1;
	Q_init *= 2;
	Q_init -= 1;*/
 	init(m_1, P_1, Q_1, B_total_1, B_Item_1, B_User_1, n1);
 	m_1 = new vector< pair<int, double> > [n1];
	T = new vector< pair<int, double> > [n1];
	
 	read(argv[1], m_1);
 
// /*
// 	for(int i = 0; i < n1; i++){
// 		for(int j = 0; j < m1; j++){
// 			cout << P(i, j) << " ";
// 		}
// 		cout << endl;
// 	}

// 	*/

	/*vec xx = {1, 0.5, 0.3, 0.1, 0.08, 0.05, 0};
	int min_i, min_j, min_k;
	double minmin = 100;
	for(int i = 0; i < xx.size(); i++){
		LD_P = xx(i);
		for(int j = 0; j < xx.size(); j++){
			LD_Q = xx(j);
			for(int k = 0; k < xx.size(); k++){
				init(m_1, P_1, Q_1, B_total_1, B_Item_1, B_User_1, n1);
				m_1 = new vector< pair<int, double> > [n1];
				T = new vector< pair<int, double> > [n1];
				read(argv[1], m_1);
				LD_B = xx(k);
				MF_stochastic(m_1, P_1, Q_1, B_total_1, B_Item_1, B_User_1, n1);
				double aaa = computeCost(m_1, P_1, Q_1, B_total_1, B_Item_1, B_User_1, n1);

				//cout << i << " " << j << " " << k << " " << aaa << endl;
				if(aaa < minmin){
					min_i = i;
					min_j = j;
					min_k = k;
					minmin = aaa;
				}

				for(int i = 0; i < n1; i++){
					T[i].clear();
					m_1[i].clear();
				}


			}
		}
	}*/

 	// cout << min_i << " " << min_j << " " << min_k << endl;
 	// cout << minmin << endl;
// 	cout << "m" << endl;
// 	cout << m_1 << endl;

 	MF_stochastic(m_1, P_1, Q_1, B_total_1, B_Item_1, B_User_1, n1);
 	/*for(int i = 0; i < n1; i++){
 		for(int j = 0; j < m_1[i].size(); j++){
 			cout << i << "th row " << m_1[i][j].first << " col: " << "origin value " << m_1[i][j].second << " pred value " << T[i][j].second << endl;
 		}
 	}*/
	/*mat XX;
	XX = P_1  * Q_1;
	for(int i = 0; i < n1; i++){
		XX.row(i) += B_User_1(i);
	}
	for(int i = 0; i < m1; i++){
		XX.col(i) += B_Item_1(i);
	}
	XX += B_total_1;
	cout << XX << endl;*/
	for(int i = 0; i < n1; i++){
		T[i].clear();
		m_1[i].clear();
	}
	delete []T;
	delete []m_1;
	//start do m_2

	init(m_2, P_2, Q_2, B_total_2, B_Item_2, B_User_2, n2);

	m_2 = new vector< pair<int, double> > [n2];
	T = new vector< pair<int, double> > [n2];

	read(argv[2], m_2);
	MF_stochastic(m_2, P_2, Q_2, B_total_2, B_Item_2, B_User_2, n2);
	for(int i = 0; i < n2; i++){
		T[i].clear();
		m_2[i].clear();
	}
	delete []T;
	delete []m_2;
	/*mat XX;
	XX = P_2  * Q_2;
	for(int i = 0; i < n2; i++){
		XX.row(i) += B_User_2(i);
	}
	for(int i = 0; i < m2; i++){
		XX.col(i) += B_Item_2(i);
	}
	XX += B_total_2;
	cout << XX << endl;*/

	mat U_1, V_1, U_2, V_2;
	mat D_1, D_2;
	SVD_modify(U_1, D_1, V_1, P_1, Q_1);
	SVD_modify(U_2, D_2, V_2, P_2, Q_2);
	print_mat(U_1, "U_1.txt");
	print_mat(V_1, "V_1.txt");
	print_mat(D_1, "D_1.txt");
	print_mat(U_2, "U_2.txt");
	print_mat(V_2, "V_2.txt");
	print_mat(D_2, "D_2.txt");
	
	/*mat u, v;
	vec s;
	svd(u, s, v, a);
	cout << a << endl;
	cout << "u" << endl;
	cout << u.cols(0, s.size() - 1) << endl;
	cout << "s" << endl;
	cout << diagmat(s) << endl;
	cout << "v" << endl;
	cout << v.cols(0, s.size() - 1).t() << endl;
	cout << "T" << endl;
	cout << u.cols(0, s.size() - 1) * diagmat(s) * v.cols(0, s.size() - 1).t() << endl;*/

}