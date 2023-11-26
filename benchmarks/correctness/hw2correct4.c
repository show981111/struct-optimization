#include <stdio.h>
#include <stdlib.h>
typedef struct
{
	int a;
	int b;
	int e[100];
	long c;
	char d;
	long f;
	long g;
	long h;
} Test;

int main()
{
	Test *t = (Test *)malloc(sizeof(Test));

	for (int i = 0; i < 10; i++)
	{
		t->a = 10;		 // => arr_1[i]->a = 10;
		t->d = t->a + 3; // => arr_2[i]->c = arr_1[i]->a + 3;
		t->e[3] = 12;
	}

	printf("%d %d %d", t->a, t->d, t->e[3]);
	return 0;
}
