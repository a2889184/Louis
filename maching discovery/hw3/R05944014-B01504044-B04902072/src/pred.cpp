#include <cstdio>
#include <assert.h>
int main()
{
	FILE* fp = fopen("../test2/fmpred.txt", "r");
	assert(fp != NULL);
	FILE* fp2 = fopen("../test2/test.txt", "r");
	assert(fp2 != NULL);
	FILE *fpw = fopen("../test2/pred.txt", "w");
	assert(fpw != NULL);
	int a, b;
	char c;
	double d;
	while(fscanf(fp2, "%d %d %c", &a, &b, &c) != EOF){
		fscanf(fp, "%lf",&d);
		fprintf(fpw, "%d %d %lf\n",a, b ,d);
	}
	fclose(fp);
	fclose(fpw);
	fclose(fp2);
}