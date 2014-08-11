#include "converter.h"

double complex dtft(double * function, int length, float freq, double increase)
{
	fprintf(stdout, "DTFT\n");
	// real value only
	double complex sum = 0;
	for(unsigned int i = 0; i < length; i++)
	{
		sum += function[i] * cexp(-2 * M_PI * i * freq * I/length);
	}
	return sum;
}

int convert_wav(char * filename, int ** values)
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
    sf = sf_open("file.wav",SFM_READ,&info);
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
    *values = buf;
    sf_close(sf);
    printf("Read %d items\n",num);
    /* Write the data to filedata.out. */
    return 0;
}
