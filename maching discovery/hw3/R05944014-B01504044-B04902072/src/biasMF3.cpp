#include <cstdio>
#include <assert.h>
#include <cstdlib>
#include <string>
#include <random>
#include <iostream>
#include <math.h>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;

/////////////parameter
int FM = 0;
int n_feature = 5;
int Maxiter = 4000; 
int n_user = 500; // 50000; // 
int n_item = 1000;
int valid_fold = 1; // 1: no valid
int write_test = 1;
int PRITER = 1;
double LR_U = 0.001;//0.001;
double LR_V = 0.001;//0.001;
double LR_B = 0.009;//0.009;
double LR_C = 0.005;//0.005;
double LR_MU = 0.00225;//0.00225;
double LD_U = 0.001;
double LD_V = 0.001;
double LD_B = 1.101;
double LD_C = 0.001;
double LD_MU = 0.004;
/////////////
typedef struct Tuple{
	int user;
	int item;
	double rate;
}Tuple;

void err_sys(const char* x) 
{ 
    perror(x); 
    exit(1); 
}
typedef struct ItemRate{
	//for rating usage
	int item;
	double rate;
}ItemRate;
typedef struct Hyper{
	double u;
	double v;
	double b;
	double c;
	double mu;
}Hyper;
Hyper learn_rate, lamda;
double getdotentry(int i, int j, vector<vector<double> > &U, vector<vector<double> > &V)
{
	double sum = 0.0;
	assert(U[i].size() == V[j].size());
	for(int p = 0;p < U[i].size();p++){
		sum += U[i][p] * V[j][p]; 
	}
	if(isnan(sum)) err_sys("getdotentry error");
	return sum;
}
void scalarrow(double coeff, vector<double> &a, vector<double> &s)
{
	for(int p = 0;p < a.size();p++){
		s[p] = a[p] * coeff;
	}
	return;
}
void add(vector<double> &a, vector<double> &b, vector<double> &s)// assume 1d
{
	assert(a.size() == b.size());
	assert(s.size() == b.size());
	for(int p = 0;p < a.size();p++){
		s[p] = a[p] + b[p];
	}
	return;
}
void printpara(int i,int j,vector<vector<double> > &U, vector<vector<double> > &V, \
	vector<double> &u_bias, vector<double> &i_bias, double &mu)
{
	printf("U:\n");
	for(int p = 0;p < U[i].size();p++){
		printf("%e ",U[i][p]);
	}
	printf("\n");
	printf("V:\n");
	for(int p = 0;p < V[j].size();p++){
		printf("%e ",V[j][p]);
	}
	printf("\n");
	printf("u_bias:\n");
	printf("%e ",u_bias[i]);
	printf("\n");
	printf("i_bias:\n");
	printf("%e ",i_bias[j]);
	printf("\n");
	printf("mu:%e\n",mu);
	return;
}
void updatepara(int i, int j, vector<vector<double> > &U, vector<vector<double> > &V, \
	vector<double> &u_bias, vector<double> &i_bias, double &mu, double r)
{
	assert(r >= 0);
	double coeff = getdotentry(i, j, U, V) + u_bias[i] + i_bias[j] + mu - r;
	vector<double> tmp(n_feature, 0.0);
	vector<double> tmp2(n_feature, 0.0);
	vector<double> grad(n_feature, 0.0);// for U, V only
	//u
	scalarrow(coeff, V[j], tmp);// assign to tmp
	scalarrow(lamda.u, U[i], tmp2);// assing to tmp2
	add(tmp, tmp2, grad);// assign to grad ;
	scalarrow(-1 * learn_rate.u, grad, grad); //assign
	add(U[i], grad, U[i]); //assign
	//v
	scalarrow(coeff, U[i], tmp);// modify V_j
	scalarrow(lamda.v, V[j], tmp2);// assing to tmp2
	add(tmp, tmp2, grad);// assign to grad ;
	scalarrow(-1 * learn_rate.v, grad, grad); //assign
	add(V[j], grad, V[j]); //assign
	//u_bias
	u_bias[i] -= learn_rate.b * (coeff + lamda.b * u_bias[i]);
	//i_bias
	i_bias[j] -= learn_rate.c * (coeff + lamda.c * i_bias[j]);
	//mu
	mu -= learn_rate.mu * (coeff + lamda.mu * mu);
	if(isnan(mu)) err_sys("mu error:");
#ifdef DEBUG
	for(int k = 0;k < n_feature;k++){
		if(isnan(U[i][k])) err_sys("U error:");
		if(isnan(V[j][k])) err_sys("V error:");	
	}
#endif
	if(isnan(u_bias[i])) err_sys("u_bias error:");
	if(isnan(i_bias[j])) err_sys("i_bias error:");
#ifdef DEBUG
	printf("uid:%d iid: %d r: %f\n",i, j, r);
	printpara(i,j,U, V, u_bias, i_bias, mu);
#endif
	return;

}
double computeloss(vector<vector<ItemRate> > &R, vector<vector<double> >&U,\
vector<vector<double> >&V, vector<double> &u_bias, vector<double>&i_bias, double &mu)
{
	//apporxiamate;
	int user_window = n_user / 10;
	int item_window = n_item / 10;
	double loss[3];
// for(int rd = 0;rd < 3;rd++){
	int user_lower = 0;//rand() % (n_user - user_window + 1); // 0.. n_user - userwindow
	int item_lower = 0;//rand() % (n_item - item_window + 1); // 0.. n_item - itemwindow
	// int maxuid = n_user;//test3 
	// int maxiid = n_item;
	double sum = 0.0;
	for(int i = user_lower;i < user_lower + user_window;i++){
		for(int p = 0;p < R[i].size();p++){
			int j = R[i][p].item;
			double r = R[i][p].rate;
			sum += pow((getdotentry(i, j, U, V) + u_bias[i] + i_bias[j] + mu - r), 2);
		}
	}
	for(int i = user_lower;i < user_lower + user_window;i++){
		for(int k = 0;k < n_feature;k++){
			sum += U[i][k] * U[i][k];
		}
	}
	for(int j = item_lower;j < item_lower + item_window;j++){
		for(int k = 0;k < n_feature;k++){
			sum += V[j][k] * V[j][k];
		}
	}
	for(int i = user_lower;i < user_lower + user_window;i++){
		sum += u_bias[i] * u_bias[i];
	}
	for(int j = item_lower;j < item_lower + item_window;j++){
		sum += i_bias[j] * i_bias[j];
	}
	if(isnan(sum)) err_sys("compute loss error");
	// loss[rd] = sum;
	return sum;
// }
	
	// return (loss[0] + loss[1] + loss[2]) / 3;
}
void MF(vector<vector<ItemRate> > &R, vector<vector<double> > &U, vector<vector<double> > &V,\
	vector<double>& u_bias, vector<double>& i_bias, double &mu, vector<Tuple>& valid)
{
	//return U V;
	
	int iter = 0;
	double best_loss = 1.0e8;
	double last_loss = 1.0e8;
	double loss = 0.0;
	int best_iter = 0;
	int diff = 1;
	vector<vector<ItemRate> > R_copy(R);
	vector<vector<double> > U_copy(U);
	vector<vector<double> > V_copy(V);
	vector<double> u_bias_copy(u_bias);
	vector<double> i_bias_copy(i_bias);
	double mu_copy = mu;
	double best_rmse = 100.0;
	double rmse = 0.0;
	int valid_window = 500;
	int lower_valid;
	//note the definition of best loss depends on rmese
	while(iter < Maxiter && diff > 0){
		if(iter % 100 == 0)
			printf("iter:%d\n", iter);

		for(int i = 0;i < R_copy.size();i++){
			for(int p = 0;p < R_copy[i].size();p++){
				int j = R_copy[i][p].item;
				double r = R_copy[i][p].rate;
				updatepara(i, j, U_copy, V_copy, u_bias_copy, i_bias_copy, mu_copy, r);
				// err_sys("OK:");
			}
		}
		if(iter % PRITER == 0){
			
		}
		/*if(loss < best_loss){
			best_loss = loss;
			best_iter = iter;
			R = R_copy;
			U = U_copy;
			V = V_copy;
			u_bias = u_bias_copy;
			i_bias = i_bias_copy;
			mu = mu_copy;
		}*/
		// if(iter % 100 == 0){
			loss = computeloss(R_copy, U_copy, V_copy, u_bias_copy, i_bias_copy, mu_copy);
			// lower_valid = rand() % (valid.size() - valid_window + 1); // 0...num_valid - valid_window
			double sum = 0.0;
			/*int num = 0;
			for(int i = lower_valid;i < lower_valid + valid_window;i++){
				double pred = getdotentry(valid[i].user, valid[i].item, U_copy, V_copy)\
				 + u_bias_copy[valid[i].user] + i_bias_copy[valid[i].item] + mu_copy;
				sum += (pred - valid[i].rate) * (pred - valid[i].rate);
				num++;
			}
			rmse = sqrt(sum / num);*/
			// printf("iter: %d rmse:%f best_rmse:%f best_iter:%d loss:%f best_loss:%f\n",\
				// iter, rmse, best_rmse, best_iter, loss, best_loss);
			if(loss < last_loss){
				best_rmse = rmse;
				best_iter = iter;
				best_loss = loss;
				R = R_copy;
				U = U_copy;
				V = V_copy;
				u_bias = u_bias_copy;
				i_bias = i_bias_copy;
				mu = mu_copy;
			}
			else{
				break;
			}
			last_loss = loss;
		// }
		iter++;
	}
	fprintf(stderr,"n_feature:%d LR_U:%f LR_V:%f LR_B:%f LR_C:%f LR_MU:%f\n",n_feature,\
		LR_U, LR_V, LR_B, LR_C, LR_MU);
	fprintf(stderr,"LD_U:%f LD_V:%f LD_B:%f LD_C:%f LD_MU:%f\n",\
		LD_U, LD_V, LD_B, LD_C, LD_MU);
	fprintf(stderr,"best_iter:%d best_loss:%f\n", best_iter, best_loss);
	return;
}
void initializeUV(vector<vector<double> >&U, vector<vector<double> >&V, vector<double>& u_bias,\
	vector<double>& i_bias, double &mu)
{

	std::default_random_engine generator;
	// std::uniform_real_distribution<double> distribution(0.0, 1.0);
  	std::uniform_real_distribution<double> distribution1(-0.001, 0.001);//if test3 0.0 to 5.0
  	std::uniform_real_distribution<double> distribution2(-0.1, 0.1);
  	for(int i = 0;i < U.size();i++){
  		for(int j = 0;j < U[i].size();j++){
  			U[i][j] = distribution1(generator);
  		}
  	}
  	for(int i = 0;i < V.size();i++){
  		for(int j = 0;j < V[i].size();j++){
  			V[i][j] = distribution1(generator);
  		}
  	}
  	for(int i = 0;i < u_bias.size();i++){
  		u_bias[i] = distribution2(generator);
  	}
  	for(int i = 0;i < i_bias.size();i++){
  		i_bias[i] = distribution2(generator);
  	}
  	mu = distribution2(generator);
  	return;
}
bool sortbyitem(ItemRate &a, ItemRate &b){
	return a.item < b.item;
} 
void initializeHyper()
{
	learn_rate.u = LR_U;
	learn_rate.v = LR_V;
	learn_rate.b = LR_B;
	learn_rate.c = LR_C;
	learn_rate.mu = LR_MU;
	lamda.u = LD_U;
	lamda.v = LD_V;
	lamda.b = LD_B;
	lamda.c = LD_C;
	lamda.mu = LD_MU;
	return;
}
void initializevec(vector<vector<ItemRate> >&R, vector<vector<double> >&U, \
	vector<vector<double> >&V, vector<double> &u_bias, vector<double> &i_bias, double &mu)
{
	R.clear();
	R.resize(n_user);
	U.clear();
	U.resize(n_user);
	for(int i = 0;i < U.size();i++){
		U[i].resize(n_feature);
	}
	V.clear();
	V.resize(n_item);
	for(int i = 0;i < V.size();i++){
		V[i].resize(n_feature);
	}
	u_bias.clear();
	u_bias.resize(n_user);
	// for(int i = 0;i < u_bias.size();i++){
		// u_bias[i] = 0.01;
	// }
	i_bias.clear();
	i_bias.resize(n_item);
	// for(int i = 0;i < i_bias.size();i++){
		// i_bias[i] = 0.01;
	// }
	// mu = 0.01;
	return;
}
void run()
{
	initializeHyper();
	string path1 = "../test3/train.txt";
	string path2 = "../test3/test.txt";
	string pathw = "../test3/pred.txt";
	vector<vector<ItemRate> > R(n_user);
	vector<vector<double> > U(n_user, vector<double>(n_feature));
	vector<vector<double> > V(n_item, vector<double>(n_feature));
	vector<double> u_bias(n_user, 0.01);
	vector<double> i_bias(n_item, 0.01);
	double mu = 0.01;
	FILE *fp = fopen(path1.c_str(), "r");
	assert(fp != NULL);
	int a,b;
	double c;
	int maxuid = 0;
	int maxiid = 0;
	int num_train = 0;
	int num_valid = 0;
	
	while(fscanf(fp,"%d %d %lf", &a, &b, &c) != EOF){
		num_train ++;
	}
	
	if(valid_fold > 1){
		num_valid = num_train / valid_fold;
		num_train -= num_valid;
	}
	vector<double> test_rmse;
	
	for(int cv = 1; cv <= valid_fold; cv++){
		vector<Tuple> valid, train;
		initializevec(R, U, V, u_bias, i_bias, mu);
		printf("cv: %d\n", cv);
		int count = 0;
		fseek(fp, 0, SEEK_SET);
		while(fscanf(fp,"%d %d %lf", &a, &b, &c) != EOF){
			if(count >= (cv-1) * num_valid && count < cv * num_valid){
				Tuple tmp1;
				tmp1.user = a;
				tmp1.item = b;
				tmp1.rate = c;
				valid.push_back(tmp1);
			}
			else{
				ItemRate tmp2;
				tmp2.item = b;
				tmp2.rate = c;
				R[a].push_back(tmp2);
				//for fm usage
				if(FM == 1){
					Tuple tmp3;
					tmp3.user = a;
					tmp3.item = b;
					tmp3.rate = c;
					train.push_back(tmp3);
				}
			}
			if(maxuid < a) maxuid = a;
			if(maxiid < b) maxiid = b;
			count++;
		}
		//for fm usage
		if(FM == 1){
			string path1 = "../test3/train" + std::to_string(cv);
			string path2 = "../test3/valid" + std::to_string(cv);
			FILE *fp = fopen(path1.c_str(), "w");
			assert(fp != NULL);
			for(int i = 0;i < train.size();i++){
				fprintf(fp, "%d\t%d\t%.1f\n", train[i].user, train[i].item, train[i].rate );
			}
			fp = fopen(path2.c_str(), "w");
			assert(fp != NULL);
			for(int i = 0;i < valid.size();i++){
				fprintf(fp, "%d\t%d\t%.1f\n", valid[i].user, valid[i].item, valid[i].rate );
			}
			fclose(fp);
			continue;
		}
		assert(maxuid + 1 <= n_user);
		assert(maxiid + 1 <= n_item);
		//sort
		for(int i = 0;i < R.size();i++){
			sort(R[i].begin(), R[i].end(), sortbyitem);
		}
		//split valid test
		/*int num_test = num_valid / 5;
		for(int i = 0;i < num_test;i++){
			assert(valid.size()- 1 - i >= 0);
			test.push_back(valid[valid.size()- 1 - i]);
		}*/
		initializeUV(U, V, u_bias, i_bias, mu);
		MF(R, U, V, u_bias, i_bias, mu, valid);
		//left is for valid
		if(valid_fold > 1){
			int num = 0;
			double sum = 0.0;
			for(int i = 0;i < valid.size();i++){
				a = valid[i].user;
				b = valid[i].item;
				c = valid[i].rate;
				double pred = getdotentry(a, b, U, V) + u_bias[a] + i_bias[b] + mu;
				// fprintf(stderr, "u:%d i:%d r:%f pred:%f\n", a, b, c, pred);
				sum += (pred - c) * (pred - c);
				num++;
			}
			sum = sqrt(sum / num);
			fprintf(stderr,"cv:%d test_rmse:%f num_test:%d\n",cv , sum, num);
			test_rmse.push_back(sum);
		}
	}
	fseek(fp,0 , SEEK_SET);
	FILE *fp_tw = fopen("../test3/train.pred","w");
	assert(fp_tw != NULL);
	while(fscanf(fp,"%d %d %lf", &a, &b, &c) != EOF){
		double pred = getdotentry(a, b, U, V) + u_bias[a] + i_bias[b] + mu;
		fprintf(fp_tw, "%d %d %f\n", a,b,pred );
	}
	fclose(fp_tw);
	fclose(fp);
	fprintf(stderr, "=======================\n");
	// for(int i = 0;i < 4;i++){
		// for(int j = 0;j < 5;j++){
			// double pred = getdotentry(i, j, U, V) + u_bias[i] + i_bias[j] + mu;
			// fprintf(stderr, "user:%d item:%d pred:%f\n",i, j, pred );
		// }
	// }
	//compute average rmsel
	if(valid_fold > 1){
		double sum = 0.0;
		for(int i = 0;i < test_rmse.size();i++){
			sum += test_rmse[i];
			fprintf(stderr, "cv:%d rmse:%lf\n", i+1, test_rmse[i]);
		}
		fprintf(stderr, "aveg rmse:%lf\n", sum / test_rmse.size());
	}
	if(write_test == 1){
		fp = fopen(path2.c_str(), "r");
		assert(fp != NULL);
		FILE *fpw = fopen(pathw.c_str(), "w");
		assert(fpw != NULL);
		char d;
		while(fscanf(fp,"%d %d %c", &a, &b, &d) != EOF){
			double pred = getdotentry(a, b, U, V) + u_bias[a] + i_bias[b] + mu;
			fprintf(fpw, "%d %d %f\n", a, b, pred);
		}
		fclose(fp);
		fclose(fpw);
	}
}
int main(int argc, char const *argv[])
{
	stderr = fopen("../test3/log.txt","w");
	// for(int i = 1;i < 7;i += 1){// 10 to 30 for test1
		// printf("rd:%d\n",i);
		// n_feature = i;
		run();
	// }
	return 0;
}