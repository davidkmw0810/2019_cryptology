#include <stdio.h>
#include <stdlib.h>

typedef unsigned char BYTE;

int main()
{
    int shift[16] = {0, 1, 2, 3, 5, 6, 7, 4, 10, 11, 8, 9, 15, 12, 13, 14};
    int In_shift[16] = {0, 1, 2, 3, 7, 4, 5, 6, 10, 11, 8, 9, 13, 14, 15, 12};
	
	int a[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	int b[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int i, num;

	for(i=0; i<16; i++){b[i] = a[shift[i]];}
	for(i=0; i<16; i++)
	{
		printf("%d ", b[i]);
	}
	printf("\n");
	
	for(i=0; i<16; i++){b[i] = a[In_shift[i]];}
	for(i=0; i<16; i++)
	{
		printf("%d ", b[i]);
	}
	printf("\n");
	
	return 0;
}
