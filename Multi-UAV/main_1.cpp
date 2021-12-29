#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iomanip>
#include <windows.h>
#define CITY 11
#define INF 10000
using namespace std;

typedef struct city
{
    double x;
    double y;
    double weight;
    int group;
} city;

typedef struct drone
{
    double total_dis;
    double total_charge;
    int seq[CITY];
    int location;
} drone;

typedef struct ant
{
    double total_dis;
    int seq[CITY];
    int location;
    int first;
} ant;

const double max_charge = 700;  //最大電耗
const int num = 3;              //無人機數量
city cities[CITY];              //點群
city zone[num];                 //分群點
drone UAVs[num];                //無人機
double road[CITY][CITY];        //城市間路段距離
double road_phe[CITY][CITY];    //路段費洛蒙 (有向圖，非對稱矩陣)
const int Q = 1;                //常數
const int N = 25;               //螞蟻數
const double u = 0.8;           //費洛蒙蒸發係數
const double p = 0.8;           //轉換規則比率
int T = 50;                     //測試次數
ant ants[N];                    //蟻群



void set_city()
{
    cities[0].x = 0;
    cities[0].y = 0;
    cities[0].weight = 1;
    cities[1].x = 37;
    cities[1].y = 52;
    cities[1].weight = 5;
    cities[2].x = 52;
    cities[2].y = 64;
    cities[2].weight = 3;
    cities[3].x = 40;
    cities[3].y = 30;
    cities[3].weight = 5;
    cities[4].x = 17;
    cities[4].y = 63;
    cities[4].weight = 3;
    cities[5].x = 52;
    cities[5].y = 33;
    cities[5].weight = 1;
    cities[6].x = 42;
    cities[6].y = 41;
    cities[6].weight = 1;
    cities[7].x = 5;
    cities[7].y = 25;
    cities[7].weight = 4;
    cities[8].x = 36;
    cities[8].y = 16;
    cities[8].weight = 2;
    cities[9].x = 27;
    cities[9].y = 23;
    cities[9].weight = 5;
    cities[10].x = 13;
    cities[10].y = 13;
    cities[10].weight = 1;
}
void set_road()                 //設定所有路段長
{
    for (int i = 0; i < CITY; i++)
    {
        for (int j = 0; j < CITY; j++)
        {
            road[i][j] = sqrt(pow(cities[i].x - cities[j].x, 2) + pow(cities[i].y - cities[j].y, 2));
        }
    }
}
double get_dis(city a, city b)  //回傳兩城市距離
{
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}
int get_x_upper_limit()
{
    int max = 0;
    for (int i = 1; i < CITY; i++)
    {
        if (cities[i].x > max)
            max = cities[i].x;
    }
    return max;
}
int get_y_upper_limit()
{
    int max = 0;
    for (int i = 1; i < CITY; i++)
    {
        if (cities[i].y > max)
            max = cities[i].y;
    }
    return max;
}
int get_x_lower_limit()
{
    int min = INF;
    for (int i = 1; i < CITY; i++)
    {
        if (cities[i].x < min)
            min = cities[i].x;
    }
    return min;
}
int get_y_lower_limit()
{
    int min = INF;
    for (int i = 1; i < CITY; i++)
    {
        if (cities[i].y < min)
            min = cities[i].y;
    }
    return min;
}


double max_phe;
double min_phe;




void grouping(){
    for(int i=0; i < CITY; i++)
    {
        double min = INF;
        for(int j = 0; j < num; j++){
            double path = get_dis(cities[i], zone[j]);
            if(min > path){
                min = path;
                cities[i].group = j;
            }
        }
    }
}
void find_CG(){

    for(int i=0; i < num; i++)
    {
        int sum_x = 0;
        int sum_y = 0;
        int n = 0;
        for(int j=0; j<CITY; j++){
            if(cities[j].group == i){
                n++;
                sum_x += cities[j].x;
                sum_y += cities[j].y;
            }
        }
        zone[i].x = sum_x / n;
        zone[i].y = sum_y / n;
    }
}

city generate_points(int xu, int xl, int yu, int yl)
{ //產生分群點
    city point;
    point.x = rand() % (xu - xl + 1) + xl;
    point.y = rand() % (yu - yl + 1) + yl;
    return point;
}

int main()
{
    set_city();
    set_road();

    int x_lower = get_x_lower_limit();
    int y_lower = get_y_lower_limit();
    int x_upper = get_x_upper_limit();
    int y_upper = get_y_upper_limit();

    
    srand(time(NULL));
    for (int i = 0; i < num; i++)       //隨機生成num個分群點
    {
        zone[i] = generate_points(x_upper, x_lower, y_upper, y_lower);
        cout << i + 1 << " : "<< "(" << zone[i].x << "," << zone[i].y <<")"<< endl;
    }
    
    city last_time_zone[num];
    for (int i = 0; i < num; i++){
        last_time_zone[i].x = INF;
        last_time_zone[i].y = INF;
    }

    grouping();

    while (1)
    {
        find_CG();
        int counter = 0;    //若等於num則離開，表示本次重心已經沒有更新
        for(int i = 0; i < num; i++){
            if(zone[i].x == last_time_zone[i].x && zone[i].y == last_time_zone[i].y){
                counter++;
            }
        }
        if(counter == num) break;
        for(int i = 0; i < num; i++){
            last_time_zone[i].x = zone[i].x;
            last_time_zone[i].y = zone[i].y;
        }
        grouping();
        //cout<<counter<<endl;
    }
    
    
    for (int i = 0; i < num; i++)
        cout << i + 1 << " : "<< "(" << zone[i].x << "," << zone[i].y <<")"<< endl;
    return 0;
}
