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
int PQ = 100;
vector< pair<int, double> > *m_1;
vector< pair<int, double> > *m_2;
// mat m_1(n1, m1);
// mat W_1(n1, m1);
mat P_1(n1, PQ);
mat Q_1(PQ, m1);
double B_total_1;
vec B_User_1(1);
vec B_Item_1(1);
double B_total_2;
vec B_User_2(1);
vec B_Item_2(1);

// mat m_2(n2, m2);
// mat W_2(n2, m2);
mat P_2(n2, PQ);
mat Q_2(PQ, m2);


vector< pair<int, double> > *T;
double cost = 0;
double LR = 0.00001;
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
	//B_Item.randu();
	//B_User.randu();
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
	/*cost += LD_B * sum(B_User % B_User);
	cost += LD_B * sum(B_Item % B_Item);
	cost += LD_B * B_total * B_total;*/

	cost /= 2;

	return cost;
}

void updatePQ(vector< pair<int, double> > *m, mat &P, mat &Q, double &B_total, vec &B_Item, vec &B_User, int row_len){

	for(int i = 0; i < row_len; i++){

		for(int j = 0; j < m[i].size(); j++){
			double value = T[i][j].second - m[i][j].second;
			P.row(i) -= (LR * (value * trans(Q.col(m[i][j].first)) + LD_P * P.row(i)));
			/*B_User(i) -= (LR * (value + LD_B * B_User(i)));
			B_total -= (LR * (value + LD_B * B_total));*/

			Q.col(m[i][j].first) -= (LR * (value * trans(P.row(i)) + LD_Q * Q.col(m[i][j].first)));
			//B_Item(m[i][j].first) -= (LR * (value + LD_B * B_Item(m[i][j].first)));
			
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
 		 	T[i][j].second = sum(P.row(i) % trans(Q.col(m[i][j].first)))/* + B_User(i) + B_Item(m[i][j].first) + B_total*/;
 		 	//cout << T[i][j].second << endl;
		}
	}
}

void MF_stochastic(vector< pair<int, double> > *m, mat &P, mat &Q, double &B_total, vec &B_Item, vec &B_User, 
	int row_len){
	//cerr << "MF start" << endl;
	double pre = 0;
	double now = 0;
	//int dd = 0;
	while(now < pre || pre == 0){
		//dd++;
		
		multiply_Factorized_Matrix(m, P, Q, B_total, B_Item, B_User, row_len);
		// if (dd % 4 == 0){
		// 	cout << "multi: " << endl;
		// 	cout << T << endl;
		// }
		pre = now;
		//cout << now << endl;
		now = computeCost(m, P, Q, B_total, B_Item, B_User, row_len);
		double diff = fabs((pre - now) / pre);
		cout << diff << endl;
		updatePQ(m, P, Q, B_total, B_Item, B_User, row_len);
		if(diff < 0.0001){
			break;
		}
	}
	//cerr << dd << endl;
	//cerr << "MF over" << endl;
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

struct Range {
	int start, end;
	Range(int s = 0, int e = 0) {start = s, end = e;}
};

void quick_sort(vec &v, mat &m,const int len) {
	if (len <= 0) return; //避免len等於負值時宣告堆疊陣列當機
	//r[]模擬堆疊,p為數量,r[p++]為push,r[--p]為pop且取得元素
	Range r[len]; int p = 0;
	r[p++] = Range(0, len - 1);
	while (p) {
		Range range = r[--p];
		if(range.start >= range.end) continue;
		double mid = v(range.end);
		int left = range.start, right = range.end - 1;
		while (left < right) {
			while (v(left) > mid && left < right) left++;
			while (v(right) <= mid && left < right) right--;
			swap(v(left), v(right));
			vec temp = m.col(left);
			m.col(left) = m.col(right);
			m.col(right) = temp;
			temp.clear();
			//swap(m.col(left), m.col(right));
		}
		if (v(left) <= v(range.end)){
			swap(v(left), v(range.end));
			vec temp = m.col(left);
			m.col(left) = m.col(range.end);
			m.col(range.end) = temp;
			temp.clear();
		}
		else
			left++;
		r[p++] = Range(range.start, left - 1);
		r[p++] = Range(left + 1, range.end);
	}
}

void Mysvd(mat &U, vec &s, mat &V, mat M, bool type /*if type == 0 decompose A_T * A, if type == 1 decompose A * A_T */){
	if(type == 0){
		mat XX = M.t() * M;
		cx_vec ev;
		cx_mat em;
		eig_gen(ev, em, XX);
		vec v = real(ev);
		//cout << v << endl;

		mat m = real(em);
		//cout << m << endl;
		ev.clear();
		em.clear();
		int eigen_non_zero = 0;

		quick_sort(v, m, v.size());
		for(int i = 0; i < v.size(); i++){
			if(v(i) != 0){
				eigen_non_zero++;
			}else{
				break;
			}
		}

		//set V
		V = m.cols(0, eigen_non_zero - 1);
		s.zeros(eigen_non_zero);

		//set s
		for(int i = 0; i < eigen_non_zero; i++){
			s(i) = sqrt(v(i));
		}

		//caculate U

		U = zeros(M.n_rows, eigen_non_zero);

		for(int i = 0; i < eigen_non_zero; i++){
			U.col(i) = M * V.col(i) / s(i);
		}
		XX.clear();
		m.clear();
		v.clear();
	}else{
		mat XX = M * M.t();
		//cout << XX << endl;
		cx_vec ev;
		cx_mat em;
		eig_gen(ev, em, XX);

		vec v = real(ev);
		//cout << v << endl;

		mat m = real(em);
		//cout << m << endl;
		ev.clear();
		em.clear();
		int eigen_non_zero = 0;

		quick_sort(v, m, v.size());
		for(int i = 0; i < v.size(); i++){
			if(v(i) != 0){
				eigen_non_zero++;
			}else{
				break;
			}
		}


		U = m.cols(0, eigen_non_zero - 1);

		s.zeros(eigen_non_zero);
		//set s
		for(int i = 0; i < eigen_non_zero; i++){
			s(i) = sqrt(v(i));
		}

		V = zeros(M.n_cols, eigen_non_zero);

		for(int i = 0; i < eigen_non_zero; i++){
			V.col(i) = M.t() * U.col(i) / s(i);
		}
		XX.clear();
		m.clear();
		v.clear();

	}
}

void SVD_modify(mat &U, mat &D, mat &V, mat &P, mat &Q){
	mat U_P, D_P, V_P, U_Q, D_Q, V_Q, U_X, D_X, V_X;
	vec s_P, s_Q, s_X;
	if(P.n_rows > P.n_cols){

		Mysvd(U_P, s_P, V_P, P, 0);
	}else{

		Mysvd(U_P, s_P, V_P, P, 1);
	}
	if(Q.n_rows > Q.n_cols){
		Mysvd(U_Q, s_Q, V_Q, Q.t(), 1);
	}else{

		Mysvd(U_Q, s_Q, V_Q, Q.t(), 0);
	}
	mat temp = diagmat(s_P) * trans(V_P.cols(0, s_P.size() - 1)) * V_Q.cols(0, s_Q.size() - 1) * trans(diagmat(s_Q));
	if(temp.n_rows > temp.n_cols){
		Mysvd(U_X, s_X, V_X, temp, 0);
	}else{

		Mysvd(U_X, s_X, V_X, temp, 1);
	}
	//Mysvd(U_X, s_X, V_X, diagmat(s_P) * trans(V_P.cols(0, s_P.size() - 1)) * V_Q.cols(0, s_Q.size() - 1) * trans(diagmat(s_Q)));
	temp.clear();
	/*mat X1 = U_Q * rebuilt_diag(U_Q.n_cols, V_Q.n_cols, s_Q) * V_Q.t();
	mat X2 = U_P * rebuilt_diag(U_P.n_cols, V_P.n_cols, s_P) * V_P.t();
	mat X3 = X2 * (X1.t());
	for(int i = 0; i < n1; i++){
		for(int j = 0; j < m1; j++){
			cout << X3(i, j) << " ";
		}
		cout << endl;
	}*/


	U = U_P * U_X;
	D = diagmat(s_X);

	V = U_Q * V_X;
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
	// cout << "P" << endl;
	// cout << P_1 << endl;

	// cout << "Q" << endl;
	//  cout << Q_1 << endl;
 	MF_stochastic(m_1, P_1, Q_1, B_total_1, B_Item_1, B_User_1, n1);
 // 	cout << "P" << endl;
	// cout << P_1 << endl;

	// cout << "Q" << endl;
	//  cout << Q_1 << endl;
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
	print_mat(U_1, "split_100/uncoverged_100_U_1.txt");
	print_mat(V_1, "split_100/uncoverged_100_V_1.txt");
	print_mat(D_1, "split_100/uncoverged_100_D_1.txt");
	print_mat(U_2, "split_100/uncoverged_100_U_2.txt");
	print_mat(V_2, "split_100/uncoverged_100_V_2.txt");
	print_mat(D_2, "split_100/uncoverged_100_D_2.txt");

	/*for(int i = 0; i < n1; i++){
		XX.row(i) += B_User_1(i);
	}
	for(int i = 0; i < m1; i++){
		XX.col(i) += B_Item_1(i);
	}
	XX += B_total_1;*/
	// cout << P_1  * Q_1 << endl;

	// cout << U_1 * D_1 * V_1.t() << endl;


	/*for(int i = 0; i < n2; i++){
		XX2.row(i) += B_User_2(i);
	}
	for(int i = 0; i < m2; i++){
		XX2.col(i) += B_Item_2(i);
	}
	XX2 += B_total_2;*/
	// cout << P_2  * Q_2 << endl;

	// cout << U_2 * D_2 * V_2.t() << endl;
	
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