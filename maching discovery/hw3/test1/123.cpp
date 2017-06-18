#include <iostream>
#include <vector>
#include "armadillo"
#include <algorithm>


using namespace std;
using namespace arma;
double *ptr;

/*void mysort(vec &v, mat &m){
	for(int i = 0; i < v.size(); i++){
		for(int j = i - 1; j >= 0; j--){

		}
	}
}*/
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

void Mysvd(mat &U, vec &s, mat &V, mat &M, bool type /*if type == 0 decompose A_T * A, if type == 1 decompose A * A_T */){
	if(type == 0){
		mat XX = M.t() * M;
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
rowvec a(2);
vec b(2);
int main(){
	/*a << 1 << 1 << endr;
	b << 2 << endr << 2 << endr;*/
	a << 1 << 1 << endr;
	b.ones();

	double x = (a * b)(0);
	cout << x << endl;

}