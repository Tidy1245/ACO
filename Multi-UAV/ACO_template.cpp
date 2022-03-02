#include <iostream>
#include <cstdlib>
#include <cmath>
#include <time.h>
#include <limits.h>
#define nodes 11
using namespace std;

typedef struct city
{
    double x;
    double y;
    double charge;
} city;

typedef struct ant
{
    double total_dis;
    double power;
    int location;
    int seq[nodes];
} drone;

double road[nodes][nodes];     //路段距離
double road_phe[nodes][nodes]; //路段費洛蒙(有向)


const int T = 50;         //迭代次數
const int N = 25;         //迭代螞蟻數
const int max_power = 31; //最大電量
const double u = 0.8;     //費洛蒙蒸發係數
const double p = 0.8;     //轉換選路規則的比率
double max_phe;           //費洛蒙上限
double min_phe;           //費洛蒙下限

city cities[nodes];
drone ants[N];

int best_seq[nodes];            //儲存最佳路程
double num_exp[nodes];          //[τ i->j(t)]*[η i->j] (i到j的期望值)
double each_probability[nodes]; // P i->j  (i到j的機率)
double acc_probability[nodes];  //ΣP i->j (累積)
void init_array(int *seq)
{
    for (int i = 0; i < nodes; i++)
        acc_probability[i] = 0;

    for (int i = 0; i < nodes; i++)
    {
        if (seq[i] != -1)
        {
            acc_probability[i] = -1;
            num_exp[i] = 0;
            each_probability[i] = 0;
        }
    }
}

int battery[N];
void init_battery()
{
    for (int i = 0; i < N; i++)
        battery[i] = max_power;
}

void set_city() //設定城市座標
{
    cities[0].x = 0;
    cities[0].y = 0;
    cities[0].charge = 1;
    cities[1].x = 37;
    cities[1].y = 52;
    cities[1].charge = 5;
    cities[2].x = 52;
    cities[2].y = 64;
    cities[2].charge = 3;
    cities[3].x = 40;
    cities[3].y = 30;
    cities[3].charge = 5;
    cities[4].x = 17;
    cities[4].y = 63;
    cities[4].charge = 3;
    cities[5].x = 52;
    cities[5].y = 33;
    cities[5].charge = 1;
    cities[6].x = 42;
    cities[6].y = 41;
    cities[6].charge = 1;
    cities[7].x = 5;
    cities[7].y = 25;
    cities[7].charge = 4;
    cities[8].x = 36;
    cities[8].y = 16;
    cities[8].charge = 2;
    cities[9].x = 27;
    cities[9].y = 23;
    cities[9].charge = 5;
    cities[10].x = 13;
    cities[10].y = 13;
    cities[10].charge = 1;
}
void set_road() //設定所有路段長
{
    for (int i = 0; i < nodes; i++)
        for (int j = 0; j < nodes; j++)
            road[i][j] = sqrt(pow(cities[i].x - cities[j].x, 2) + pow(cities[i].y - cities[j].y, 2));
}
double get_dis(city a, city b) //回傳兩城市距離
{
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

void set_phe() //設定路段費洛蒙
{
    for (int i = 0; i < nodes; i++)
    {
        for (int j = 0; j < nodes; j++)
        {
            if (i == j)
                road_phe[i][j] = 0;
            else
                road_phe[i][j] = max_phe;
        }
    }
}
void init_ant()
{
    for (int i = 0; i < N; i++)
    {
        ants[i].total_dis = 0;
        ants[i].power = 0;
        ants[i].location = 0;
        for (int j = 0; j < nodes; j++)
            ants[i].seq[j] = -1;
        ants[i].seq[0] = 0;
    }
}

double get_num(int from, int to) //[τ i->j(t)]*[η i->j]
{
    return (road_phe[from][to] * pow(1 / road[from][to], 2) * (cities[to].charge));
}
int choose_road(double *acc) //選擇下一條路
{
    double x = (double)rand() / (RAND_MAX + 1.0); // 0 <= x < 1
    for (int i = 0; i < nodes; i++)
    {
        if (x < acc[i])
            return i;
    }
    return -1;
}
void update_phe_part(int step)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 1; j <= step; j++)
        {
            int from, to;
            for (int k = 0; k < nodes; k++)
            {
                if (ants[i].seq[k] == j)
                    to = k;
                if (ants[i].seq[k] == j - 1)
                    from = k;
            }
            road_phe[from][to] *= u;
            road_phe[from][to] += (1 - u) * (1 / road[from][to]);

            if (road_phe[from][to] < min_phe)
                road_phe[from][to] = min_phe;
        }
    }
}
void update_phe_all()
{
    int from, to;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < nodes; j++)
        {
            if (ants[i].seq[j] == 0)
            {
                from = j;
                break;
            }
        }
        int j = 0, step = 1;
        while (1)
        {
            if (ants[i].seq[j] == step)
            {
                to = j;
                road_phe[from][to] += 1 / ants[i].total_dis;

                if (road_phe[from][to] > max_phe)
                    road_phe[from][to] = max_phe;
                from = j;

                if (step == 0)
                    break;

                step++;
                step %= nodes;
            }

            j++;
            j %= nodes;
        }
    }
}

int main()
{
    int t = 0;
    double best_dis = 1000; //最短距離
    int best_charge = 100000;
    max_phe = (1 / 1 - u) * (1 / best_dis);

    set_city();
    set_road();
    set_phe();

    srand(time(NULL));
    while (t < T)
    {
        init_ant();
        init_battery();

        max_phe = (1 / 1 - u) * (1 / best_dis);
        min_phe = max_phe / (2 * nodes);

        for (int i = 1;; i++, i %= nodes)
        {
            if (i == 0)
            {
                for (int j = 0; j < N; j++)
                {
                    int from = ants[j].location;
                    int to = 0;
                    ants[j].total_dis += road[from][to];
                    ants[j].power += (battery[j] * road[from][to]);
                    battery[j] -= cities[to].charge;
                }
                break;
            }

            // 每隻螞蟻移動一步
            for (int j = 0; j < N; j++)
            {
                int f = ants[j].location; //當前位置
                double sum_exp = 0;       //Σ[τ i->u(t)]*[η i->u] (總期望值)

                init_array(ants[j].seq);

                for (int k = 0; k < nodes; k++)
                {
                    if (ants[j].seq[k] == -1)
                    {
                        num_exp[k] = get_num(f, k);
                        sum_exp += num_exp[k];
                    }
                }
                for (int k = 0; k < nodes; k++)
                {
                    if (ants[j].seq[k] == -1)
                    {
                        each_probability[k] = num_exp[k] / sum_exp;
                        for (int g = k; g < nodes; g++)
                        {
                            if (acc_probability[g] != -1)
                                acc_probability[g] += each_probability[k];
                        }
                    }
                }

                //移動，選路規則
                if (t < p * T) //探索
                    ants[j].location = choose_road(acc_probability);
                else //追隨
                {
                    double max_probability = 0;
                    int next_location;
                    for (int k = 0; k < nodes; k++)
                    {
                        if (acc_probability[k] > max_probability)
                        {
                            next_location = k;
                            max_probability = acc_probability[k];
                        }
                    }

                    ants[j].location = next_location;
                }

                int h = ants[j].location; //新位置
                ants[j].seq[h] = i;
                ants[j].total_dis += road[f][h];
                ants[j].power += (battery[j] * road[f][h]);
                battery[j] -= cities[h].charge;
            }

            if (t < p * T)
            {
                update_phe_part(i);
            }
        }

        //一代螞蟻完成
        //更新最佳路徑
        for (int j = 0; j < N; j++)
        {
            if (best_charge > ants[j].power)
            {
                best_charge = ants[j].power;
                best_dis = ants[j].total_dis;
                for (int k = 0; k < nodes; k++)
                {
                    best_seq[k] = ants[j].seq[k];
                }
            }
        }

        t++;

        //整體更新費洛蒙
        for (int i = 0; i < nodes; i++) //費洛蒙蒸發
        {
            for (int j = 0; j < nodes; j++)
            {
                road_phe[i][j] *= u;
                if (road_phe[i][j] < min_phe)
                    road_phe[i][j] = min_phe;
            }
        }
        update_phe_all();
    }

    cout<<best_charge<<endl;
    cout<<best_dis<<endl;

    int step = 0, i = 0;
    while (step < nodes)
    {
        if (best_seq[i] == step)
        {
            cout << i << "  ";
            step++;
        }

        i++;
        i %= nodes;
    }

    cout <<"0"<<endl;
    cout << "max_phe = " << max_phe << endl;
    cout << "min_phe = " << min_phe << endl;
    cout << (double)clock() / CLOCKS_PER_SEC << "s" << endl;
    return 0;
}
