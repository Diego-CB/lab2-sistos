#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main()
{
  pid_t t1, t2, t3;
  clock_t c1, c2;
  
  c1 = clock();
  t1 = fork();

  if (t1 == 0)
  {
    t2 = fork();
    if (t2 == 0)
    {
      t3 = fork();
      if (t3 == 0)
      {
        for (int i = 0; i < 1000000; i++){printf("%d\n", i);}

        exit(0);
      } 
      else
      {
        for (int j = 0; j < 1000000; j++){printf("%d\n", j);}
        wait(NULL);
      }
      exit(0);
    }
    else 
    {
      for (int k = 0; k < 1000000; k++){printf("%d\n", k);}
      wait(NULL);
    }
    exit(0);
  }
  else
  {
    wait(NULL);
    c2 = clock();
  }

  double delta = (double) ((c2 - c1)/CLOCKS_PER_SEC);
  printf("delta %f", delta);
  return 0;
}
