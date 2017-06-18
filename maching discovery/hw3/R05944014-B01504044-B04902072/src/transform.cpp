
#include <cstdio>
#include <assert.h>
int main()
{
	FILE* fp = fopen("../test2/test.txt", "r");
	assert(fp != NULL);
	FILE *fpw = fopen("../test2/test.trans", "w");
	assert(fpw != NULL);
	int a, b;
	char c;
	while(fscanf(fp, "%d %d %c", &a, &b, &c) != EOF){
		fprintf(fpw, "%d %d 0\n",a, b );
	}
	fclose(fp);
	fclose(fpw);
}