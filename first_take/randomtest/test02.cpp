#include <iostream>
#include <cstdlib> /* 亂數相關函數 */
#include <ctime>   /* 時間相關函數 */
#include <string>
#include <iomanip>
using namespace std;

typedef struct man 
{
  string name;
  int id;
}man;


int main(){
  /* 固定亂數種子 */
  srand( time(NULL) );
  for (int i = 0; i < 10; i++)
  {
    /* 產生亂數 */
  double x = (double)rand() / (RAND_MAX + 1.0); // 0 <= x < 1
  cout<<fixed<<setprecision(4)<< x<<endl;
  }
  return 0;
}