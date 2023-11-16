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
	Test t;
} Hello;

int main()
{
	Test t;
	t.a = 1;
	Hello hello;
	hello.t = t;
	hello.t.a = 1;
	hello.t.b = hello.t.a;
	return 0;
}
