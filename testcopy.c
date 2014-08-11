#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <sndfile.h>
#define PI 3.14159265359

double complex dtft(int *, int, float);
int convert_wav(char *, int **, int *);
int fake_dtft(int *, int, float, double *, double *);
double powers(float, float);
double factorial(int);
double cosine(float);
double sine(float);

struct cplx{
	double real;
	double imag;
};

double powers(float base, float power)
{
	if(power == 1)
	{
		return base;
	}
	else if(power == 0)
	{
		return 1;
	}
	else
	{
		return powers(base, power - 1) * base;
	}
}


double factorial(int factor)
{
	if(factor <= 1)
	{
		return 1;
	}
	else
	{
		return factorial(factor - 1) * factor;
	}
}

double cosine(float value)
{
	if(value < 0)
	{
		value *= -1;
	}
	while (value > 2 * PI)
	{
		value -= 2*PI;
	}
	
	double result = 1;
	if(value > PI)
	{
		value -= PI;
		result = -1;
	}
	double sum = 0;
	int multiplier [4] = {1, 0, -1, 0};
	for(char i = 0; i < 15; i++)
	{
		sum += multiplier[i % 4] / factorial(i) * powers(value, i);
	}
	result *= sum;
	return result;
}

double sine(float value)
{
	return cosine(value - PI/2);
}

int main(void)
{

	fprintf(stdout, "variable initialization\n");
	FILE * fout, * fpoints;
	fout = fopen("results.txt", "w");
	fpoints = fopen("points", "w");
	// variable declaration
	int * points;
	struct cplx * sums;
	int num_points = 128; // only 128 points for now
	float min = 1/(2*PI), max = 16000;
	
	int freq = 20; // 20 frequency columns

	float freqinc = (max - min)/freq;

//	points = (double *) malloc(sizeof(double) * num_points);
	sums = (struct cplx *) malloc(sizeof(struct cplx) * freq);
	
	// point collection

	/*fprintf(stdout, "Collecting points\n");
	unsigned int counter = 0;
	for(double i = -PI; i < PI; i += PI/num_points)
	{
		points[counter] = cosine(4*PI*i) + 10*sine(5*PI*i);
		fprintf(fpoints, "%f,", points[counter]);
		counter++;
	}*/
	unsigned int num =0;
	num = num_points;
	convert_wav("piece.wav", &points, &num_points);
	fprintf(stdout, "Fourier transform\n");
	for(int i = 1; i < freq + 1; i ++)
	{
		sums[i].real = 0;
		sums[i].imag = 0;
		fake_dtft(points, num, i, &(sums[i].real), &(sums[i].imag));
		//sums[i] += dtft(points, num_points, i);
	}
	fprintf(stdout, "RESULTS:\n");
	double * mags = (double *) malloc(sizeof(double) *freq);
	double maxi = 0;
	for (int i = 0; i < freq; i++){
		mags[i] = sqrt (pow (sums[i].real,2) + pow( sums[i].imag, 2));
		if(mags[i] > maxi)
			maxi = mags[i];
	}
	fprintf(stdout, "SCALE: %f\n", maxi/20);
	for(int i = 0; i < freq; i++)
		mags[i] = mags[i]/maxi*20;
	for(int i = 0; i < freq; i++)
	{
		fprintf(stdout, "%f \t|", i*freqinc + min);
		for( int j = 0; j < mags[i]; j++)
		{
			fprintf(stdout, "#");
		}
		fprintf(stdout, "\n");
		fprintf(fout, "%f Hz: %f\n", min + i*freqinc, mags[i]);
	}
	fclose(fout);
	fclose(fpoints);
	free(sums);
	sums = NULL;
	free(points);
	points = NULL;
}


double complex dtft(int * function, int length, float freq)
{
	fprintf(stdout, "DTFT\n");
	// real value only
	double complex sum = 0;
	for(unsigned int i = 0; i < length; i++)
	{
		sum += function[i] * cexp(-2 * PI * i * freq * I/length);
	}
	return sum;
}

int fake_dtft(int * function, int length, float freq, double * real, double * img)
{
	fprintf(stdout, "FAKE DTFT\n");
	double sum = 0, isum = 0;
	for(unsigned int i = 0; i < length; i++)
	{
		sum += function[i] * cosine(-2 * PI / length * i * freq);
		isum += function[i] * sine ( -2* PI / length * i * freq);
	}
	*real = sum;
	*img = isum;
	return 0;
}

int convert_wav(char * filename, int ** values, int * number)
{
    SNDFILE *sf;
    SF_INFO info;
    int num_channels;
    int num, num_items;
    int *buf;
    int f,sr,c;
    int i,j;
    FILE *out;
    
    /* Open the WAV file. */
    info.format = 0;
    sf = sf_open(filename,SFM_READ,&info);
   if (sf == NULL)
        {
        printf("Failed to open the file.\n");
        return(-1);
        }
    /* Print some of the info, and figure out how much data to read. */
    f = info.frames;
    sr = info.samplerate;
    c = info.channels;
    printf("frames=%d\n",f);
    printf("samplerate=%d\n",sr);
    printf("channels=%d\n",c);
    num_items = f*c;
    printf("num_items=%d\n",num_items);
    /* Allocate space for the data to be read, then read it. */
    buf = (int *) malloc(num_items*sizeof(int));
    num = sf_read_int(sf,buf,num_items);
	int * new_buf = (int *) malloc(f * sizeof(int));
	for(int i = 0; i < num_items; i+= c)
	{ // read one channel only
		new_buf[i/c] = buf[i];
	}
    *values = new_buf;
    *number = f;
	free(buf);
    sf_close(sf);
    printf("Read %d items\n",num);
    /* Write the data to filedata.out. */
    return 0;
}




