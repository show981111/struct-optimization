#include <stdio.h>

typedef struct
{
	int a;
	int b;
	long c;
	char d;
	long e;
} Test;

typedef struct
{
	int a;
	int b;
	long c;
	char d;
	long e;
} Test1;

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
	Test1 t1;
	t.a = 123;
	t.d = 'c';
	t.e = 3123;

	int x = t.a;
	long y = t.e;
	printf("%d %ld", t.a, t.e);

	t1.a = 123;
	t1.d = 'c';
	t1.e = 3123;

	int x1 = t1.a;
	long y1 = t1.e;

	for (int i = 0; i < 100; i++)
	{
		if (i % 2 == 0)
		{
			t.a = t.e + 3;
		}
		else
		{
			t1.a = t1.e + 1;
		}
	}

	return 0;
}
