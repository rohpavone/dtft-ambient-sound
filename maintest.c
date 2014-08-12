#include <stdlib.h>
#include <stdio.h>

#define PI 3.14159265358979323846
#define PORT_ON(port, pin) port |= ( 1 << pin )
#define PORT_OFF(port, pin) port &= ~( 1 << pin )

int dtft(float *, unsigned int, float, float *, float *);
double powers(float, float);
double factorial(int);
float cosine(float);
float sine(float);

struct complex{
	float real;
	float imag;
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

float sqrt1(float x)
{
	unsigned int i = *(unsigned int *) &x;
	i += 127 << 23;
	i >>= 1;
	return *(float *) &i;
}

float cosine(float value)
{
	if(value < 0)
	{
		value *= -1;
	}
	while (value > 2 * PI)
	{
		value -= 2*PI;
	}
	
	float result = 1;
	if(value > PI)
	{
		value -= PI;
		result = -1;
	}
	float sum = 0;
	int multiplier [4] = {1, 0, -1, 0};
	for(char i = 0; i < 15; i++)
	{
		sum += multiplier[i % 4] / (float) factorial(i) * (float) powers(value, i);
	}
	result *= sum;
	return result;
}

float sine(float value)
{
	return cosine(value - PI/2);
}

int main(void)
{
	//unsigned char * adc_value;
	float * points;
	unsigned int samples = 128;
	struct complex * sum;
	float vref = 5.04f;

	int freqs = 20; // (hz) number of frequencies examined
	int max = 16000; // (hz) highest frequency
	int min = 20; // (hz) lowest frequency
	float freqinc = (max - min)/freqs; // (hz) if linear scale
	unsigned char leds = 16;

	// ADEN : turn on ADC mode
	// ADFR: turn on free running mode (off right now)
	
	ADCSRA = (1 << ADEN);
	
	// ADPS2, ADPS1, ADPS0: set prescaler to 128
	// frequency is now 125kHz
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	
	// left bit shift (use only ADCH register instead of ADCH and ADCL) 
	ADMUX = 0x00;
	
	// allocate memory
	adc_value = (unsigned char *) malloc(sizeof(unsigned char) * samples);
	//points = (float *) malloc(sizeof(float) * samples);
	sum = (struct complex *) malloc(sizeof(struct complex) * freqs);
	
	DDRD = 1 << PD0;
	DDRC |= (1 << PC5) | (1 << PC4);
	float mags [freqs];
	int values
	while(1)
	{
		for(unsigned char i = 0; i < samples; i++)
		{
			// turn on the ADC
			ADCSRA |= (1 << ADSC);
			
			// check if finished. If ADSC is set to 0, then
			// it breaks and we can read the value
			while(ADCSRA & (1 << ADSC));
			adc_value[i] = ADCW;
		} 

		// call dtft
		for(int i = 0; i < freqs; i++)
		{
			sum[i].real = 0;
			sum[i].imag = 0;
			dtft(points, samples, min + freqinc * i, &(sum[i].real), &(sum[i].imag));
		}

		// calculate mags and such
		for(int i = 0; i < freqs; i++)
		{
			mags[i] = leds*sqrt1(powers(sum[i].real, 2) + powers(sum[i].imag, 2));
		}
		
		/*
		debug code, not important

		PORTD &= ~(1 << PD0);
		PORTC &= ~(1 << PC5);
		PORTC &= ~(1 << PC4);
		if(adc_value[0] <85)
			PORTD |= (1 << PD0);
		else if(adc_value[0] < 170)
			PORTC |= (1 << PC5);
		else
			PORTC |= (1 << PC4);*/	
	}
	
	
}

int dtft(float * function, unsigned int length, float freq, float * real, float * img)
{
	float sum = 0, isum = 0;
	for(unsigned int i = 0; i < length; i++)
	{
		*real += function[i] * cosine(-2 * PI / length * i * freq);
		*img += function[i] * sine ( -2* PI / length * i * freq);
	}
	return 0;
}