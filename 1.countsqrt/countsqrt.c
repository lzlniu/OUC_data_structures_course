#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#include <math.h>

void main() {
	int i=1;
	double f1, f2, sum, y;
	FILE * in, * out;
	if ((in = fopen("input.txt", "r")) == NULL)
	{
		printf("Cann't open this file\n");
		//getchar();
		return;
	}
	if ((out = fopen("output.txt", "w")) == NULL)
	{
		printf("Cann't open this file\n");
		//getchar();
		return;
	}
	out = fopen("output.txt", "w");
	fprintf(out, "sets\tf1\tf2\tf1+f2\tsqrt(f1+f2)\n");
	while (!feof(in)) {
		fscanf_s(in,"%f,%f", &f1, &f2);
		sum = f1 + f2;
		if (sum < 0) {
			fprintf(out, "%d:\t%.2f\t%.2f\t%.2f\tError\n", i, f1, f2, sum);
		}
		else {
			y = sqrt(sum);
			fprintf(out, "%d:\t%.2f\t%.2f\t%.2f\t%.2f\n", i, f1, f2, sum, y);
		}
		i++;
	}
	fclose(in);
	fclose(out);
	return;
}
