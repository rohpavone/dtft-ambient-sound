#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <sndfile.h>
#include "functions.c"
double complex dtft(double *, int, float, double);
int convert_wav(char *, int **);
