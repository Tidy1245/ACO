#include <iostream>
#include <math.h>
#include <limits.h>
#include <time.h>

using namespace std;

const int INF = INT_MAX;
int citys[11][3];
double adj[11][11];

void getcitys()
{
  citys[0][0] = 0; citys[0][1] = 0; citys[0][2] = 0;
  citys[1][0] = 37; citys[1][1] = 52; citys[1][2] = 5;
  citys[2][0] = 52; citys[2][1] = 64; citys[2][2] = 3;
  citys[3][0] = 40; citys[3][1] = 30; citys[3][2] = 5;
  citys[4][0] = 17; citys[4][1] = 63; citys[4][2] = 3;
  citys[5][0] = 52; citys[5][1] = 33; citys[5][2] = 1;
  citys[6][0] = 42; citys[6][1] = 41; citys[6][2] = 1;
  citys[7][0] = 5; citys[7][1] = 25; citys[7][2] = 4;
  citys[8][0] = 36; citys[8][1] = 16; citys[8][2] = 2;
  citys[9][0] = 27; citys[9][1] = 23; citys[9][2] = 5;
  citys[10][0] = 13; citys[10][1] = 13; citys[10][2] = 1;
  for(int i = 0; i < 11; i++)
  {
    for(int j = 0; j < i; j++)
    {
      adj[i][j] = sqrt(pow(citys[i][0] - citys[j][0], 2) + pow(citys[i][1] - citys[j][1], 2));
      adj[j][i] = adj[i][j];
    }
  }
}

int main() 
{
  getcitys();
  int P; 
  int min;
  int W;
  P = INF;
  min = INF;
  for(int a = 0; a < 11; a++)
  {
    for(int b = 0; b < 11; b++)
    {
      for(int c = 0; c < 11; c++)
      {
        for(int d = 0; d < 11; d++)
        {
          for(int e = 0; e < 11; e++)
          {
            for(int f = 0; f < 11; f++)
            {
              for(int g = 0; g < 11; g++)
              {
                for(int h = 0; h < 11; h++)
                {
                  for(int i = 0; i < 11; i++)
                  {
                    for(int j = 0; j < 11; j++)
                    {
                      if(a != 0 && b != 0 && c != 0 && d != 0 && e != 0 && f != 0 && g != 0 && h != 0 && i != 0 && j != 0)
                      {
                        if(a != b && a != c && a != d && a != e && a != f && a != g && a != h && a != i && a != j && b != c && b != d && b != e && b != f && b != g && b != h && b != i && b != j && c != d && c != e && c != f && c != g && c != h && c != i && c != j && d != e && d != f && d != g && d != h && d != i && d != j && e != f && e != g && e != h && e != i && e != j && f != g && f != h && f != i && f != j && g != h && g != i && g != j && h != i && h != j && i != j)
                        {
                          W = 31;
                          int tmp = adj[0][a] 
                          + adj[a][b] 
                          + adj[b][c] 
                          + adj[c][d] 
                          + adj[d][e] 
                          + adj[e][f] 
                          + adj[f][g] 
                          + adj[g][h] 
                          + adj[h][i] 
                          + adj[i][j] 
                          + adj[j][0];

                          int tmpP = adj[0][a] * W
                          + adj[a][b] * (W - citys[a][2]) 
                          + adj[b][c] * (W - citys[a][2] - citys[b][2]) 
                          + adj[c][d] * (W - citys[a][2] - citys[b][2] - citys[c][2]) 
                          + adj[d][e] * (W - citys[a][2] - citys[b][2] - citys[c][2] - citys[d][2]) 
                          + adj[e][f] * (W - citys[a][2] - citys[b][2] - citys[c][2] - citys[d][2] - citys[e][2]) 
                          + adj[f][g] * (W - citys[a][2] - citys[b][2] - citys[c][2] - citys[d][2] - citys[e][2] - citys[f][2]) 
                          + adj[g][h] * (W - citys[a][2] - citys[b][2] - citys[c][2] - citys[d][2] - citys[e][2] - citys[f][2] - citys[g][2]) 
                          + adj[h][i] * (W - citys[a][2] - citys[b][2] - citys[c][2] - citys[d][2] - citys[e][2] - citys[f][2] - citys[g][2] - citys[h][2]) 
                          + adj[i][j] * (W - citys[a][2] - citys[b][2] - citys[c][2] - citys[d][2] - citys[e][2] - citys[f][2] - citys[g][2] - citys[h][2] - citys[i][2]) 
                          + adj[j][0] * (W - citys[a][2] - citys[b][2] - citys[c][2] - citys[d][2] - citys[e][2] - citys[f][2] - citys[g][2] - citys[h][2] - citys[i][2] - citys[j][2]);
                          if(tmpP < P)
                          {
                            P = tmpP;
                            min = tmp;
                            cout << 0 << " " << a << " " << b << " " << c << " " << d << " " << e << " " << f << " " << g << " " << h << " " << i << " " << j << " " << 0 << endl;
                            cout << P << " " << min << endl;
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  cout << (double)clock() / CLOCKS_PER_SEC << "s" << endl;
} 
