#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main(){
  srand( time(NULL) );

  /* 產生 [0, 1) 的浮點數亂數 */
  double x = (double) rand() ;
    printf("x = %f\n", x);
    double y = RAND_MAX;
    printf("%f\n", y);
  x /=  (RAND_MAX + 1.0);
  printf("x = %f\n", x);
  //double y = rand();
  //printf("y = %f\n", y);
  //123
  return 0;
}