#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
double dtft(double *, int, float, double);

int main(void)
{

	fprintf(stdout, "variable initialization\n");
	FILE * fout, * fpoints;
	fout = fopen("results.txt", "w");
	fpoints = fopen("points", "w");
	// variable declaration
	double * points;
	double * sums;
	int num_points = 10; // only 128 points for now
	float min = 1/(2*M_PI), max = 1/(2*M_PI);
	
	int freq = 20; // 20 frequency columns

	float freqinc = (max - min)/freq;

	points = (double *) malloc(sizeof(double) * num_points);
	sums = (double *) malloc(sizeof(double) * freq);
	
	// point collection

	fprintf(stdout, "Collecting points\n");
	unsigned int counter = 0;
	for(double i = -M_PI; i < M_PI; i += M_PI/num_points)
	{
		points[counter] = sin(2*M_PI*i);
		fprintf(fpoints, "%f,", points[counter]);
		counter++;
	}
/*
	counter = 0;
	fprintf(stdout, "Fourier transform\n");
	for(int i = 0; i < freq; i ++)
	{
		sums[i] = 0;
		for(float j = min + freqinc * i; j < min + freqinc * (i + 1); j += freqinc/10)
		{
			sums[i] += dtft(points, num_points, j, M_PI/num_points);
		}
	}
	fprintf(stdout, "RESULTS:\n");
	for(int i = 0; i < freq; i++)
	{
	
		fprintf(fout, "%f Hz: %f\n", min + freqinc*i, sums[i]);
	}*/
}

double dtft(double * function, int length, float freq, double increase)
{
	fprintf(stdout, "DTFT\n");
	// real value only
	double complex sum = 0;
	for(unsigned int i = 0; i < length; i++)
	{
		sum += function[i] * cexp(-2 * M_PI * (i * increase) * freq * I) * increase;
	}
	return creal(sum);
}
