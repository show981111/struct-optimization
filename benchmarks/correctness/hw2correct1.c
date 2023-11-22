#include <stdio.h>

typedef struct
{ //
	int a;
	long padding[1000];
	int b;
	long c;
	char d;
	long e;
} Test;

int main()
{

	Test t;
	t.c = 98765432101;
	t.b = 5;
	t.d = 'c';
	t.a = 123;
	t.e = 3123;

	int x = t.a;
	long y = t.e;
	printf("%d %ld", t.a, t.e);

	for (int i = 0; i < 100; i++)
	{
		long a = i * i;
		long b = (i + 1) * 2;
		long c = (i + 2) * 8;
		long d = (i + 3) * 32;
		long e = d * 8;
		long f = a * 321;
		printf("%d %ld", t.a++, t.e++);
	}

	printf("%d %d %ld %c %ld", t.a, t.b, t.c, t.d, t.e);
	printf("opt X");
	return 0;
}