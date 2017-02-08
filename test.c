#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
double complex dtft(double *, int, float, double);
double complex * produce_dtft(double * points, int num_points, double sample_period, float min_freq, float max_freq, int num_frequencies);

int main(void)
{

	fprintf(stdout, "variable initialization\n");
	FILE * fout, * fpoints;
	fout = fopen("results.txt", "w");
	fpoints = fopen("points", "w");
	// variable declaration
	double * points;
	double * sums;
	int num_points = 1024; // only 128 points for now
	points = (double *) malloc(sizeof(double) * num_points*2);
	// point collection

	fprintf(stdout, "Collecting points\n");
	unsigned int counter = 0;
        double max_val = 0;
        double temp;
	for(double i = -1; i < 1; i += (double)1/num_points)
	{
		points[counter] = sin(4*M_PI*i) + 4*cos(8*M_PI*i);
                temp = sqrt(pow(points[counter], 2));
                if(temp > max_val) max_val = temp;
                fprintf(stdout, "%d: %f\n", counter,points[counter]);
		counter++;
	}
        double complex * results = produce_dtft(points, counter, (double)1/num_points, -10, 10, 4096);
}

double complex * produce_dtft(double * points, int num_points, double sample_period, float min_freq, float max_freq, int num_frequencies) {
        double complex * sums = (double complex *) malloc(sizeof(double complex) * num_frequencies);

	float freqinc = (max_freq - min_freq)/num_frequencies;

	for(int i = 0; i < num_frequencies; i ++)
	{
		sums[i] = dtft(points, num_points*2, min_freq+freqinc *i, (double)1/num_points);
	}
        return sums;
}

double complex dtft(double * function, int length, float freq, double sample_period)
{
	// real value only
	double complex sum = 0;
	for(int i = -length/2; i < length/2; i++)
	{
                // multiply to normalize
		sum += function[i+length/2] * cexp(-2 * M_PI * (i) * freq * sample_period * I) *sample_period;
	}
	return sum;
}
