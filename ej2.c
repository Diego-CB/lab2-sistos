#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

int main()
{
  clock_t c1, c2;
  c1 = clock();

  int i = 0;
  for (i = 0; i < 100000; i++){}
  for (i = 0; i < 100000; i++){}
  for (i = 0; i < 100000; i++){}

  c2 = clock();
  double delta = (double) c2 - c1;
  printf("delta %f", delta);
  return 0;
}
