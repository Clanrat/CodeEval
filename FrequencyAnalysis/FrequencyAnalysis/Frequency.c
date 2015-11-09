#include <math.h>
#include <complex.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


int fourierSeries(int* signal)
{
	
}

int* splitData(char* line)
{
	int splitline[1023];


}


void main(int argc, const char* argv[])
{
	FILE* file = fopen(argv[1],"r");
	char* line[1024];
	int dominatingfrequency;
	while(fgets(line,1024,file))
	{
		dominatingfrequency = fourierSeries(splitData(line));
		printf("%d\n", dominatingfrequency);
	}
}