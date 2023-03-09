#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

int main()
{
  clock_t c1, c2;
  c1 = clock();

  int i = 0;
  for (i = 0; i < 1000000; i++){printf("%d\n", i);}
  for (i = 0; i < 1000000; i++){printf("%d\n", i);}
  for (i = 0; i < 1000000; i++){printf("%d\n", i);}

  c2 = clock();
  double delta = (double) ((c2 - c1)/CLOCKS_PER_SEC);
  printf("delta %f", delta);
  return 0;
}
