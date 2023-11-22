#include <stdio.h>

typedef struct
{
	int a;
	int b;
	long c;
	char d;
} Test;

/*
typedef struct
{
	int a;
	char d;
} Test_1;

typedef struct
{
	int b;
	long c;
} Test_2;
*/

int main()
{
	Test arr[10];
	// => Test_1 arr_1[10];
	// => Test_2 arr_2[10];

	for (int i = 0; i < 10; i++)
	{
		arr[i].a = 10;			 // => arr_1[i].a = 10;
		arr[i].d = arr[i].a + 3; // => arr_2[i].c = arr_1[i].a + 3;
	}

	return 0;
}
