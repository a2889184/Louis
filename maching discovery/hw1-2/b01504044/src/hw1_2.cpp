#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <utility>
#include <cmath>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>

#define PID pair<int, double>
#define VI vector<int>
#define PB push_back
#define MP make_pair
#define F first
#define S second
#define INF log(0)

using namespace std;
vector< vector<int> > t;
VI encode_possible[100];
VI decode_possible[100];
double firstword_prob[100];
double encode[100][100];
double bigram[100][100];
int space = 0;

double a[100][100];
double b[100][100];

double logC_h1_x[100];
double logC_ht_xt_x[100][100];
double logC_ht_ht1_x[100][100];

PID probable[500][100];
bool converge = 0;
double evaluate, pre_evaluate = INF;


void read(char* filename, VI *storage){
	ifstream file(filename);
	int in1, in2;
	bool f;
	while(file >> in1 >> in2 >> f){
		if(in1 > space){
			space = in1;
		}
		if(f == 1){
			storage[in1].PB(in2);
			decode_possible[in2].PB(in1);
		}
		//cout << in1 << endl;
		//cout << in2 << endl;
		//cout << f << endl;
	}

}
template <class T>
void init_bigram(T (*storage)[100]){
	for(int i = 0; i <= space; i++){
		for(int j = 0; j <= space; j++){
			storage[i][j] = log(1.0 / (double)(space + 1));
		}
	}
}

template <class T>
void init_encode(T (*storage)[100]){
	int num;
	for(int i = 0; i <= space; i++){
		int num = encode_possible[i].size();

		for(int j = 0; j < num; j++){
			encode[i][encode_possible[i][j]] = 1.0 / (double)num;
		}
	}

	for(int i = 0; i <= space; i++){
		for(int j = 0; j <= space; j++){
			encode[i][j] = log(encode[i][j]);
		}
	}

/*
	for(int i = 0; i <= space; i++){
		num = 0;
		for(int j = 0; j <= space; j++){
			if(encode_possible[i][j] == 1) num++;
		}

		for(int j = 0; j <= space; j++){
			if(encode_possible[i][j] == 0) encode[i][j] = INF;
			else encode[i][j] = log(1.0 / (double)num);
		}
	}*/
}

template <class T>
void init_first_prob(T (*storage)){
	for(int i = 0; i <= space; i++){
		storage[i] = log(1.0 / (double)(space + 1));
	}

}

void init(){
	for(int i = 0; i < 5000; i++){
		for(int j = 0; j < 40; j++){
			probable[i][j] = MP(space, INF);
		}
	}
}

void alpha(int num){
	//init
	/*for(int i = 0; i <= space; i++){
		for(int j = 0; j <= space; j++){
			a[i][j] = INF;
		}
	}*/
	int length = t[num].size();
	
	//for a1
	for(int i = 0; i <= space; i++){
		a[0][i] = firstword_prob[i] + encode[i][t[num][0]];
	}

	bool first = 1;
	int pre_decode_element;
	int now_decode_element;
	int pre_decode_length;
	int now_decode_length;
	int pre_hidden;
	int now_hidden;
	for(int i = 1; i < length; i++){
		first = 1;
		pre_decode_element = t[num][i - 1];
		now_decode_element = t[num][i];
		pre_decode_length = decode_possible[pre_decode_element].size();
		now_decode_length = decode_possible[now_decode_element].size();

		for(int j = 0; j < pre_decode_length; j++){ // i - 1
			pre_hidden = decode_possible[pre_decode_element][j];
			//double temp = INF;
			for(int k = 0; k < now_decode_length; k++){ // i 
				now_hidden = decode_possible[now_decode_element][k];
				if(first == 1){
					a[i][now_hidden] = encode[now_hidden][now_decode_element] + bigram[pre_hidden][now_hidden] + a[i - 1][pre_hidden];
				}
				//sumlog
				else {
					a[i][now_hidden] += log(1 + exp(encode[now_hidden][now_decode_element] + bigram[pre_hidden][now_hidden] 
						+ a[i - 1][pre_hidden] - a[i][now_hidden]));//a_t cal 
				}
			}

			first = 0;
		}
	}
}

void beta(int num){
	/*for(int i = 0; i <= space; i++){
		for(int j = 0; j <= space; j++){
			b[i][j] = INF;
		}
	}*/
	int length = t[num].size();
	
	//for bn
	for(int i = 0; i <= space; i++){
		b[length - 1][i] = log(1.0);
	}
	//for other b's
	bool first = 1;
	int pre_decode_element;
	int pre_decode_length;
	int pre_hidden;
	for(int i = length - 2; i >= 0; i--){
		pre_decode_element = t[num][i + 1];
		pre_decode_length = decode_possible[pre_decode_element].size();
		/*for(int k = 0; k <= space; k++){ //i
			first = 1;
			//if(encode[j][t[num][i + 1]] == INF) continue;
			//b[i][k] = INF;
			
			for(int j = 0; j < pre_decode_length; j++){ //i+1
				pre_hidden = decode_possible[pre_decode_element][j];
				if(first == 1) {
					b[i][k] = b[i + 1][pre_hidden] + encode[pre_hidden][pre_decode_element] + bigram[k][pre_hidden];
					first = 0;
				}
				
				//sumlog
				else{
					//if(b[i + 1][j] + encode[j][t[num][i + 1]] + bigram[k][j] != INF) 
					b[i][k] += log(1 + exp(b[i + 1][pre_hidden] + encode[pre_hidden][pre_decode_element] 
						+ bigram[k][pre_hidden] - b[i][k]));//b_t cal 
				}
				
			}
		}
		*/
		first = 1;
		for(int j = 0; j < pre_decode_length; j++){ //i
			
			//if(encode[j][t[num][i + 1]] == INF) continue;
			//b[i][k] = INF;
			int pre_hidden = decode_possible[pre_decode_element][j];
			for(int k = 0; k <= space; k++){ //i+1
				
				if(first == 1) {
					b[i][k] = b[i + 1][pre_hidden] + encode[pre_hidden][pre_decode_element] + bigram[k][pre_hidden];
					//first = 0;
				}
				
				//sumlog
				else{
					//if(b[i + 1][j] + encode[j][t[num][i + 1]] + bigram[k][j] != INF) 
					b[i][k] += log(1 + exp(b[i + 1][pre_hidden] + encode[pre_hidden][pre_decode_element] 
						+ bigram[k][pre_hidden] - b[i][k]));//b_t cal 
				}
				
			}
			first = 0;
		}


	}
}

void computeParameter(int num){
	double logP_x = INF;
	int length = t[num].size();
	//compute logP(x)
	int now_hidden;
	int now_decode_length;
	int now_decode_element;
	int next_decode_element;

	now_decode_element = t[num][length - 1];
	now_decode_length = decode_possible[now_decode_element].size();
	for(int i = 0; i < now_decode_length; i++){
		now_hidden =  decode_possible[now_decode_element][i];
		if(logP_x == INF) logP_x = a[length - 1][now_hidden];
		else {
			//if(a[length - 1][i] != INF)
			logP_x += log(1 + exp(a[length - 1][now_hidden] - logP_x));
		}
	}

	if(num == 0) evaluate = logP_x;
	else{
		evaluate += logP_x;
	}

	//compute logC_h1_x
	now_decode_element = t[num][0];
	now_decode_length = decode_possible[now_decode_element].size();

	for(int i = 0; i < now_decode_length; i++){
		now_hidden = decode_possible[now_decode_element][i];
		if(logC_h1_x[now_hidden] == INF) {
			logC_h1_x[now_hidden] = a[0][now_hidden] + b[0][now_hidden] - logP_x;
		}else{
			//if(a[0][i] + b[0][i] - logP_x != INF) 
			logC_h1_x[now_hidden] += log(1 + exp(a[0][now_hidden] + b[0][now_hidden] - logP_x - logC_h1_x[now_hidden]));
		}

		//cerr << "logC_h1_x   : " << logC_h1_x[i] << endl;
	}

	//compute logC_ht_xt_x

	for(int i = 0; i < length; i++){
		now_decode_element = t[num][i];
		now_decode_length = decode_possible[now_decode_element].size();

		for(int j = 0; j < now_decode_length; j++){ // ht
			//double temp = a[i][j] + b[i][j];
			now_hidden = decode_possible[now_decode_element][j];
			if(logC_ht_xt_x[now_hidden][now_decode_element] == INF){
				logC_ht_xt_x[now_hidden][now_decode_element] = a[i][now_hidden] + b[i][now_hidden] - logP_x;
			}else{
				logC_ht_xt_x[now_hidden][now_decode_element] += 
					log(1 + exp(a[i][now_hidden] + b[i][now_hidden] - logP_x - logC_ht_xt_x[now_hidden][now_decode_element]));
			}
			//cerr << i << " " << j << " " << t[num][i] << " "<< a[i][j] + b[i][j] << endl;
			//cerr << "logC_ht_xt_x  : " << logC_ht_xt_x[i][j] << endl;
		}


	}

	//compute logC_ht_ht1_x
	int next_decode_length;
	int next_hidden;
	for(int i = 0; i < length - 1; i++){
		now_decode_element = t[num][i];
		next_decode_element = t[num][i + 1];
		now_decode_length = decode_possible[now_decode_element].size();
		next_decode_length = decode_possible[next_decode_element].size();
		for(int j = 0; j < now_decode_length; j++){ // h_t
			now_hidden = decode_possible[now_decode_element][j];
			for(int k = 0; k < next_decode_length; k++){ //h_t+1

				next_hidden = decode_possible[next_decode_element][k];
				if(logC_ht_ht1_x[now_hidden][next_hidden] == INF){
					logC_ht_ht1_x[now_hidden][next_hidden] = a[i][now_hidden] + bigram[now_hidden][next_hidden] + encode[next_hidden][next_decode_element] + b[i + 1][next_hidden] - logP_x;
				}else{

					logC_ht_ht1_x[now_hidden][next_hidden] += 
						log(1 + exp(a[i][now_hidden] + bigram[now_hidden][next_hidden] + encode[next_hidden][next_decode_element] 
							+ b[i + 1][next_hidden] - logP_x - logC_ht_ht1_x[now_hidden][next_hidden]));
					
				}
				//cerr << j << " " << k << " " << a[i][j] + bigram[j][k] + encode[k][t[num][i + 1]] + b[i + 1][k] - logP_x << endl;
			}
		}
	}


}

int a_time = 0, b_time = 0, compute_time = 0;
void pab(int num);
void E_step(){
	int word_num = t.size();
	//init the count matrix
	for(int i = 0; i <= space; i++){
		for(int j = 0; j <= space; j++){
			logC_ht_ht1_x[i][j] = INF;
			logC_ht_xt_x[i][j] = INF;
		}
		logC_h1_x[i] = INF;
	}


	for(int i = 0; i < word_num; i++){
		//cout << i << endl;
		if(t[i].size() > 0){
			int pre = time(0);
			alpha(i);
			int mid = time(0);

			a_time += (mid - pre);
			beta(i);
			int last = time(0);

			b_time += (last - mid);
			//cout << " the number of word : " << i << "   -----------------------------------------------------------" << endl;
			//pab(i);
			computeParameter(i);

			compute_time += time(0) - last;
		}
	}
}

void M_step(){
	double logC_h1 = INF;
	double logC_h_equal_i[100];
	for(int i = 0; i <= space; i++){
		logC_h_equal_i[i] = INF;
	}
	//compute logC_h1
	for(int i = 0; i <= space; i++){
		if(logC_h1 == INF){
			logC_h1 = logC_h1_x[i];
		}else{
			if(logC_h1_x[i] != INF)
				logC_h1 += log(1 + exp(logC_h1_x[i] - logC_h1));
		}
	}
	//cerr << logC_h1 << endl; 
	//compute logC_h_equal_i
	for(int i = 0; i <= space; i++){ // h = i
		for(int j = 0; j <= space ; j++){
			if(logC_h_equal_i[i] == INF){
				logC_h_equal_i[i] = logC_ht_xt_x[i][j];
			}else{
				if(logC_ht_xt_x[i][j] != INF){
					logC_h_equal_i[i] += log(1 + exp(logC_ht_xt_x[i][j] - logC_h_equal_i[i]));
				}
			}
		}
	}

	//update logP_h1 = i
	for(int i = 0; i <= space; i++){
		if(logC_h1_x[i] == INF) continue;
		if(converge == 1){
			double deviation = exp(logC_h1_x[i] - logC_h1) - exp(firstword_prob[i]);
			if(fabs(deviation / exp(firstword_prob[i])) > 0.001){
				converge = 0;
			}
		}
		firstword_prob[i] = logC_h1_x[i] - logC_h1;
	}

	//update emission and transition
	for(int i = 0; i <= space; i++){ // ht
		if(logC_h_equal_i[i] == INF) continue;
		for(int j = 0; j <= space; j++){ // x or h t+1

			encode[i][j] = logC_ht_xt_x[i][j] - logC_h_equal_i[i];
			if(i != space) bigram[i][j] = logC_ht_ht1_x[i][j] - logC_h_equal_i[i];
		}
	}

}

void p(){

	cout << "firstword_prob-------------------------------" << endl;
	for(int i = 0; i <= space; i++){
		cout << i << " " << firstword_prob[i] << endl;
	}

	cout << "bigram-------------------------------------" << endl;
	for(int i = 0; i <= space; i++){ // ht
		for(int j = 0; j <= space; j++){ // x or h t+1
			cout << i << "  " << j << "  " << bigram[i][j] << endl;
		}
	}
	cout << "encode--------------------------------------" << endl;
	for(int i = 0; i <= space; i++){ // ht
		for(int j = 0; j <= space; j++){ // x or h t+1
			cout << i << "  " << j << "  " << encode[i][j] << endl;
		}
	}
}

void pab(int num){
	cout << "alpha -----------------------------------------" << endl;
	for(int i = 0; i < t[num].size(); i++){
		cout << "the output:  " << t[num][i] << endl;
		cout << i << "   ----------------------------------------" << endl;
		for(int j = 0; j <= space; j++){

			cout << "J " << j << " ";
			cout << a[i][j] << endl;
		}
	}
	cout << "beta -----------------------------------------" << endl;
	for(int i = t[num].size() - 1; i >= 0; i--){
		cout << "the output:  " << t[num][i] << endl;
		cout << i << "   ----------------------------------------" << endl;
		for(int j = 0; j <= space; j++){

			cout << "J " << j << " ";
			cout << b[i][j] << endl;
		}
	}
}
void p_log(){
	cout << "print logC_h1_x" << endl;
	for(int i = 0; i <= space; i++){
		cout << i << " " << logC_h1_x[i] << endl;
	}

	for(int i = 0; i <= space; i++){
		for(int j = 0; j <= space; j++){
			cout << i << " "  << j << " " << logC_ht_xt_x[i][j] << endl;
		}
	}

	for(int i = 0; i <= space; i++){
		for(int j = 0; j <= space; j++){
			cout << i << " "  << j << " " << logC_ht_ht1_x[i][j] << endl;
		}
	}
}

void init_probable(int length){
	for(int i = 0; i < length; i++){
		for(int j = 0; j <= space; j++){
			probable[i][j].F = -1;
			probable[i][j].S = INF;
		}
	}
}
int init_time = 0, viterbi_time = 0, trace_time = 0;;
void trace_back(int length){
	int pos = -1;
	int MAX;
	for(int i = 0; i <= space; i++){
		if(pos == -1 || (probable[length - 1][i].S > MAX)){
			pos = i;
			MAX = probable[length - 1][i].S;
		}
	}
	int now;
	VI v;
	now = pos;
	int layer = length - 1;
	while(now != -1){
		v.PB(now);
		now = probable[layer][now].F;
		layer--;
	}

	int vectorsize = v.size();
	while(!v.empty()){
		cout << v.back() << " ";
		v.pop_back();
	}

	//cout << " ";
}

void viterbi(int num){

	int length = t[num].size();
	int pre = time(0);

	init_probable(length);

	int mid = time(0);

	init_time += (mid - pre);
	for(int j = 0; j <= space; j++){
		probable[0][j].F = -1;
		probable[0][j].S = bigram[space][j] + encode[j][t[num][0]];

	}

	for(int i = 1; i < length; i++){
		for(int j = 0; j <= space; j++){
			if(probable[i - 1][j].S == INF) continue;
			for(int k = 0; k <= space; k++){
				double value = probable[i - 1][j].S + bigram[j][k] + encode[k][t[num][i]];

				if(value > probable[i][k].S){
					probable[i][k].F = j;
					probable[i][k].S = value;
				}
			}
		}
	}
	int last = time(0);
	viterbi_time += (last - mid);
	trace_back(length);

	trace_time += (time(0) - last);
	
}


int main(int argc, char **argv){
	read(argv[1], encode_possible);

	/*for(int i = 0; i <= space; i++){
		for(int j = 0; j < decode_possible[i].size(); j++){
			cout << decode_possible[i][j] << " ";
		}

		cout << endl;
	}*/

	init_bigram(bigram);
	init_encode(encode);
	init_first_prob(firstword_prob);

	for(int i = 0; i < 500; i++){
		for(int j = 0; j <= space; j++){
			probable[i][j] = MP(-1, INF);
		}
	}


/*	ifstream afile("data");
	//cerr << "123" << endl;
	int in1, in2;
	string f;
	for(int i = 0; i <= space; i++){
		//storage[i] = log(1.0 / (double)(space + 1));
		afile >> in1 >> f;
		//cerr << in1 << " " << f << endl;
		firstword_prob[in1] = stod(f);
	}
	for(int i = 0; i <= space; i++){
		for(int j = 0; j <= space; j++){
			//storage[i][j] = log(1.0 / (double)(space + 1));
			afile >> in1 >> in2 >> f;
			//cerr << in1 << " " << in2 << " " << f << endl;
			bigram[in1][in2] = stod(f);
		}
	}



	for(int i = 0; i <= space; i++){
		//num = 0;
		for(int j = 0; j <= space; j++){
			//if(encode_possible[i][j] == 1) num++;
			afile >> in1 >> in2 >> f;
			//cerr << in1 << " " << in2 << " " << f << endl;
			encode[in1][in2] = stod(f);
		}


	}

	afile.close();

*/

	//p();


	//int num;
	int count = 0;

	int input;
	int num = 0;
	bool nextline = 0;
	vector<int> *x = new vector <int>;
	t.push_back(*x);

	while(cin >> input){
		
		if(nextline == 1){
			x = new vector <int>;
			t.push_back(*x);
			num++;
			nextline = 0;
			//t[num].PB(space);
		}else if(nextline == 0){
			if(input == space){
				nextline = 1;
			}
		}

		t[num].PB(input);
	}
	//cerr << t.size() << endl;
	/*for(int i = 0; i < t.size(); i++){
		for(int j = 0; j < t[i].size(); j++){
			cout << t[i][j] << " ";
		}
		cout << endl;
	}*/

/*	for(int i = 0; i < t[0].size(); i++){

		cout << i << "   ----------------------------------------" << endl;
		for(int j = 0; j <= space; j++){

			cout << "J " << j << " ";
			cout << a[i][j] << endl;
		}
	}
	cout << "beta -----------------------------------------" << endl;
	for(int i = t[0].size() - 1; i >= 0; i--){

		cout << i << "   ----------------------------------------" << endl;
		for(int j = 0; j <= space; j++){

			cout << "J " << j << " ";
			cout << b[i][j] << endl;
		}
	}
*/


/*
	for(int i = 0; i <= space; i++){
		for(int j = 0; j <= space; j++){
			cout << i << " " << j << " first : output, second h :  "  << " " << encode_possible[j][i] << endl;
		}
		cout << endl;
	}*/

	int round = 1;

	while(converge == 0){

		cerr << "round :    " << round << endl;
		converge = 1;
		//E step
		double pre = time(0);
		//p();

		cerr << "E " << endl;
		E_step();
		//cerr << evaluate - pre_evaluate << endl;
		if(evaluate - pre_evaluate < 200) break;
		pre_evaluate = evaluate;
		double mid = time(0);
		cerr << " cost  :  " << mid - pre << " seconds" << endl;


		//cerr << "a time  :" << a_time << "  b_time :  " << b_time << " c _time : " << compute_time << endl;
		cerr << "M " << endl;
		a_time = 0;
		b_time = 0;
		compute_time = 0;

		M_step();
		double now = time(0);
		cerr << " cost  :  " << now - mid << " seconds" << endl;
		//p();
		round++;
	}
	
	/*for(int i = 0; i <= space; i++){
		for(int j = 0; j <= space; j++){
			logC_ht_ht1_x[i][j] = INF;
			logC_ht_xt_x[i][j] = INF;
		}
		logC_h1_x[i] = INF;
	}
	alpha(1);
	beta(1);
	pab(1);
	computeParameter(1);
	double logP_x = INF;
	p();


	for(int i = 0; i < t[1].size(); i++){
		logP_x = INF;
		for(int j = 0; j <= space; j++){
			//cerr << i << " " << j << " " << a[i][j] + b[i][j] << endl;
			if(encode[j][t[1][i]] == INF) continue;
			if(logP_x == INF) logP_x = a[i][j] + b[i][j];
			else {
				if(a[i][j] + b[i][j] != INF)
					logP_x += log(1 + exp(a[i][j] + b[i][j] - logP_x));
			}
		}

		cout << logP_x << endl;
	}
	p_log();
	M_step();


	cout << "final table" << endl;
	p();*/

	num = t.size();
	for(int i = 0; i < num; i++){
		viterbi(i);
	}

	//cerr << "init:  " << init_time << "viterbi  " << viterbi_time << "trace  " << trace_time << endl;

}