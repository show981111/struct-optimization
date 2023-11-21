#include <stdio.h>

struct myStructure {
  int myNum;
  char myLetter;
};

int main() {
  struct myStructure s1;
  s1.myNum = 1;
  s1.myLetter = 'c';
  s1.myNum = 2;
  s1.myLetter = 'd';
  printf("%u \n", s1.myNum);
  printf("%c \n", s1.myLetter);
  return 0;
} 