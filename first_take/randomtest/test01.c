#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main(){
  srand( time(NULL) );

  /* 產生 [0, 1) 的浮點數亂數 */
  /*double x = (double) rand() ;
    printf("x = %f\n", x);
    double y = RAND_MAX;
    printf("%f\n", y);
  x /=  (RAND_MAX + 1.0);
  printf("x = %f\n", x);*/
  int CITY = 10;
  for (int i = 1; ; i++,i %= CITY)
  {
    if(i == 5) continue;
    printf("i = %d\n", i);
    if(i == 0) break;
  }
  return 0;
}