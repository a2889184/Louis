#include <iostream>
#include <algorithm>
#include <cmath>

using namespace std;
long long ** preprocess(long long *input, int number, bool condition);
long long get(long long **RMD, int from, int to, bool condition);
int lowerbound(long long **RMD_max, long long **RMD_min, int l, int n);
int upperbound(long long **RMD_max, long long **RMD_min, int l, int n);
int main(){
	int number = 0;
	cin >> number;

	long long *a = new long long[number];
	long long *b = new long long[number];

	for(int i = 0; i < number; i++){
		cin >> a[i];
	}
	for(int i = 0; i < number; i++){
		cin >> b[i];
	}
	long long **RMD_max, **RMD_min;
	RMD_max = preprocess(a, number, 1);
	RMD_min = preprocess(b, number, 0);

	long long count = 0;
	bool different = 0;
	/*
	for(int j = 0;  j < number; j++){
		if(RMD_max[0][j] != RMD_min[0][j]) {
			different = 1;
			break;
		}
	}
	

	if(different == 0){
		count = (long long)number * (long long) (number + 1) / 2;
		cout << count;
		return 0;
	}*/

/*
	for(int i = 0; i < number; i++){
		for(int j = 0; j <= i; j++){
			if(get(RMD_max, j, i, 1) == get(RMD_min, j, i, 0)) count++;
			else if(get(RMD_max, j, i, 1) < get(RMD_min, j, i, 0)) break;
		}
	}

	cout << count;*/
	int rmin = 0, rmax = 0;
	for(int i = 0; i < number; i++){//for each left end = i, enumerate rmin, rmax
		rmin = lowerbound(RMD_max, RMD_min, i, number);
		rmax = upperbound(RMD_max, RMD_min, i, number);
		if(rmin <= rmax)
            count += (rmax - rmin + 1);
		//printf("left = %d, rmin = %d, rmax = %d\n", i, rmin, rmax);
	}
	cout << count;
	/*
	
	*/	

}

long long ** preprocess(long long *input, int number, bool condition){
	int width = log2(number) + 1;
	long long **RMD = new long long *[width];
	for(int i = 0; i < width; i++){
		RMD[i] = new long long [number];
	}

	for(int i = 0; i < number; i++){
			RMD[0][i] = input[i];
	}
	for(int i = 1; i < width; i++){
		for(int j = 0; /*pow(2, i)*/(1 << i) + j - 1 < number; j++){
			if(condition == 0 /*min*/) RMD[i][j] = min(RMD[i - 1][j], RMD[i - 1][j + /*(int)pow(2, i - 1)*/(1 << (i - 1))]);
			else /*max*/ RMD[i][j] = max(RMD[i - 1][j], RMD[i - 1][j + /*(int)pow(2, i - 1)*/(1 << (i - 1))]);
		}
	}
/*
	for(int i = 0; i < width; i++){
		for(int j = 0; pow(2, i) + j - 1 < number; j++){
			cout << RMD[i][j] << " " ;
		}
		cout << endl;
	}
	cout << endl;*/
	return RMD;
}

long long get(long long **RMD, int from, int to, bool condition){
	int l = to - from + 1;
	int k = log2(l);

	if(condition == 0) return min(RMD[k][from], RMD[k][from + l - /*(int)pow(2, k)*/(1 << k)]);
	else return max(RMD[k][from], RMD[k][from + l - /*(int)pow(2, k)*/(1 << k)]);
	return 0;


}

int lowerbound(long long **RMD_max, long long **RMD_min, int l, int n){
	int lo = l, hi = n;
	int ans = n;
	while(lo < hi){
		int mi = (lo+hi)/2;
		long long qa = get(RMD_max, l, mi, 1);
		long long qb = get(RMD_min, l, mi, 0);
		if(qa > qb) hi = mi;
		else if(qa < qb) lo = mi+1;
		else{
			ans = min(ans, mi);
			hi = mi;
		}

	}
	return ans;
}

int upperbound(long long **RMD_max, long long **RMD_min, int l, int n){
	int lo = l, hi = n;
	int ans = -1;
	while(lo < hi){
		int mi = (lo+hi)/2;
		long long qa = get(RMD_max, l, mi, 1);
		long long qb = get(RMD_min, l, mi, 0);
		if(qa > qb) hi = mi;
		else if(qa < qb) lo = mi+1;
		else{
			ans = max(ans, mi);
			lo = mi+1;
		}
	}
	return ans;
}