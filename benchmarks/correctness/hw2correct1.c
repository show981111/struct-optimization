#include <stdio.h>

typedef struct
{
	int a;
	int b;
	long c;
	char d;
	long e;
} Test;

int main()
{
	// int A[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	// int B[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	// int i, j;
	// j = 0;
	// for(i = 0; i < 10; i++) {
	// 	B[i] = A[j] * 11 + i;
	// 	if(i < 8)
	// 		j = i;
	// 	printf("%d\n", B[i]);
	// }

	Test t;
	t.b = 5; 
	t.d = 'c';
	t.a = 123;
	t.e = 3123;

	int x = t.a;
	long y = t.e;
	printf("%d %ld", t.a, t.e);

	return 0;
}
