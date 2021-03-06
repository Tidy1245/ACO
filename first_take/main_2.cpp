#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iomanip>
#include <windows.h>
#define CITY 51
using namespace std;

typedef struct city
{
    double x;
    double y;
} city;

typedef struct ant
{
    double total_dis;
    int seq[CITY];
    int location;
    int first;
} ant;

double road[CITY][CITY];     //城市間路段距離
double road_phe[CITY][CITY]; //路段費洛蒙 (有向圖，非對稱矩陣)
const int Q = 1;             //常數
const int N = 25;            //螞蟻數
const double u = 0.8;        //費洛蒙蒸發係數
const double p = 0.8;        //轉換規則比率
int T = 50;                  //測試次數
city cities[CITY];           //城市
ant ants[N];                 //蟻群

double max_phe;
double min_phe;
int change = 0;

void set_city() //設定城市座標
{
    cities[0].x = 37;
    cities[0].y = 52;
    cities[1].x = 49;
    cities[1].y = 49;
    cities[2].x = 52;
    cities[2].y = 64;
    cities[3].x = 20;
    cities[3].y = 26;
    cities[4].x = 40;
    cities[4].y = 30;
    cities[5].x = 21;
    cities[5].y = 47;
    cities[6].x = 17;
    cities[6].y = 63;
    cities[7].x = 31;
    cities[7].y = 62;
    cities[8].x = 52;
    cities[8].y = 33;
    cities[9].x = 51;
    cities[9].y = 21;
    cities[10].x = 42;
    cities[10].y = 41;
    cities[11].x = 31;
    cities[11].y = 32;
    cities[12].x = 5;
    cities[12].y = 25;
    cities[13].x = 12;
    cities[13].y = 42;
    cities[14].x = 36;
    cities[14].y = 16;
    cities[15].x = 52;
    cities[15].y = 41;
    cities[16].x = 27;
    cities[16].y = 23;
    cities[17].x = 17;
    cities[17].y = 33;
    cities[18].x = 13;
    cities[18].y = 13;
    cities[19].x = 57;
    cities[19].y = 58;
    cities[20].x = 62;
    cities[20].y = 42;
    cities[21].x = 42;
    cities[21].y = 57;
    cities[22].x = 16;
    cities[22].y = 57;
    cities[23].x = 8;
    cities[23].y = 52;
    cities[24].x = 7;
    cities[24].y = 38;
    cities[25].x = 27;
    cities[25].y = 68;
    cities[26].x = 30;
    cities[26].y = 48;
    cities[27].x = 43;
    cities[27].y = 67;
    cities[28].x = 58;
    cities[28].y = 48;
    cities[29].x = 58;
    cities[29].y = 27;
    cities[30].x = 37;
    cities[30].y = 69;
    cities[31].x = 38;
    cities[31].y = 46;
    cities[32].x = 46;
    cities[32].y = 10;
    cities[33].x = 61;
    cities[33].y = 33;
    cities[34].x = 62;
    cities[34].y = 63;
    cities[35].x = 63;
    cities[35].y = 69;
    cities[36].x = 32;
    cities[36].y = 22;
    cities[37].x = 45;
    cities[37].y = 35;
    cities[38].x = 59;
    cities[38].y = 15;
    cities[39].x = 5;
    cities[39].y = 6;
    cities[40].x = 10;
    cities[40].y = 17;
    cities[41].x = 21;
    cities[41].y = 10;
    cities[42].x = 5;
    cities[42].y = 64;
    cities[43].x = 30;
    cities[43].y = 15;
    cities[44].x = 39;
    cities[44].y = 10;
    cities[45].x = 32;
    cities[45].y = 39;
    cities[46].x = 25;
    cities[46].y = 32;
    cities[47].x = 25;
    cities[47].y = 55;
    cities[48].x = 48;
    cities[48].y = 28;
    cities[49].x = 56;
    cities[49].y = 37;
    cities[50].x = 30;
    cities[50].y = 40;
}
void set_road() //設定所有路段長
{
    for (int i = 0; i < CITY; i++)
    {
        for (int j = 0; j < CITY; j++)
        {
            road[i][j] = sqrt(pow(cities[i].x - cities[j].x, 2) + pow(cities[i].y - cities[j].y, 2));
        }
    }
}
double get_dis(city a, city b) //回傳兩城市距離
{
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}
void set_phe() //設定路段費洛蒙
{
    for (int i = 0; i < CITY; i++)
    {
        for (int j = 0; j < CITY; j++)
        {
            double L = get_dis(cities[i], cities[j]);
            if (i == j)
            {
                road_phe[i][j] = 0;
            }
            else
            {
                road_phe[i][j] = max_phe;
            }
        }
    }
}
void init_ant(int i) //初始化螞蟻資訊
{
    ants[i].total_dis = 0;
    for (int j = 0; j < CITY; j++)
        ants[i].seq[j] = -1;
}
bool unfinished(ant ants) //判斷螞蟻是否有未拜訪的城市
{
    for (int i = 0; i < CITY; i++)
    {
        if (ants.seq[i] == -1)
            return true;
    }
    return false;
}

double get_num(int from, int to) //[τ i->j(t)]^α*[η i->j]^ß (目前α,ß都是1)
{
    return road_phe[from][to] * pow(Q / road[from][to], 2);
}
int choose_road(double *acc) //選擇下一條路
{
    // srand(time(NULL));
    double x = (double)rand() / (RAND_MAX + 1.0); // 0 <= x < 1
    for (int i = 0; i < CITY; i++)
    {
        if (x < acc[i])
            return i;
    }
    return -1;
}
void update_phe(int *seq, double L)
{
    for (int i = 0; i < CITY; i++)
    {
        for (int j = 0; j < CITY; j++)
        {
            road_phe[i][j] *= u; //費洛蒙蒸發
            if (road_phe[i][j] < min_phe)
                road_phe[i][j] = min_phe;
        }
    }
        

    int step = 0, from, to, i;
    for (i = 0; i < CITY; i++)
    {
        if (seq[i] == step)
        {
            from = i;
            break;
        }
    }
    i = 0;
    step++;
    while (1) //還沒回到起點就一直做
    {
        if (seq[i] == step)
        {
            to = i;
            road_phe[from][to] += Q / L; //Δτ i->j

            if (road_phe[from][to] > max_phe)
                road_phe[from][to] = max_phe;

            from = i;

            if (step == 0)
                break; //完成更新

            step++;
            step %= CITY;
        }

        i++;
        i %= CITY;
    }
}
void update_phe_part(ant a, int step)
{
    for (int i = 1; i <= step; i++)
    {
        int from, to; //城市編號
        for (int j = 0; j < CITY; j++)
        {
            if (a.seq[j] == i)
                to = j;
            if (a.seq[j] == i - 1)
                from = j;
        }
        road_phe[from][to] *= u;
        road_phe[from][to] += (1 - u) * (1 / road[from][to]);

        if (road_phe[from][to] < min_phe)
            road_phe[from][to] = min_phe;
        // road_phe[from][to] += Q / ants.total_dis;
    }
}
void update_phe_all(int *seq, double L)
{
    int step = 0, from, to, i;
    for (i = 0; i < CITY; i++)
    {
        if (seq[i] == step)
        {
            from = i;
            break;
        }
    }
    i = 0;
    step++;
    while (1) //還沒回到起點就一直做
    {
        if (seq[i] == step)
        {
            to = i;
            road_phe[from][to] += Q / L; //Δτ i->j

            if (road_phe[from][to] > max_phe)
                road_phe[from][to] = max_phe;

            from = i;

            if (step == 0)
                break; //完成更新

            step++;
            step %= CITY;
        }

        i++;
        i %= CITY;
    }
}

int main()
{
    DWORD star_time = GetTickCount();
    int best_seq[CITY];            //最佳路程
    double best_dis = 1500;        //最短距離
    double num_exp[CITY];          //[τ i->j(t)]^α*[η i->j]^ß (i到j的期望值)
    double each_probability[CITY]; // P i->j  (i到j的機率)
    double acc_probability[CITY];  //ΣP i->j (累積)
    int t = 0;

    max_phe = (1 / 1 - u) * (1 / best_dis);
    set_city();
    set_road();
    set_phe();

    srand(time(NULL));
    while (t < T)
    {
        // init_each_of_ants
        for (int c = 0; c < N; c++)
            init_ant(c);

        for (int c = 0; c < N; c++)
        {
            int min = 0;
            int max = CITY - 1;
            int x = rand() % (max - min + 1) + min; //
            ants[c].seq[x] = 0;
            ants[c].location = x;
            ants[c].first = x;
        }

        max_phe = (1 / 1 - u) * (1 / best_dis);
        min_phe = max_phe / (2 * CITY);

        // test_each_of_steps
        for (int i = 1;; i++, i %= CITY)
        {
            if (i == 0)
            {
                for (int c = 0; c < N; c++)
                {
                    int to = ants[c].first;
                    int from = ants[c].location;
                    ants[c].total_dis += road[from][to];
                }
                break;
            }
            // 每隻螞蟻移動一步

            for (int c = 0; c < N; c++)
            {

                int f = ants[c].location; //當前位置

                double sum_exp = 0; //Σ[τ i->u(t)]^α*[η i->u]^ß (總期望值)

                for (int j = 0; j < CITY; j++)
                    acc_probability[j] = 0;

                for (int j = 0; j < CITY; j++)
                {
                    if (ants[c].seq[j] != -1)
                    {
                        acc_probability[j] = -1;
                        num_exp[j] = 0;
                        each_probability[j] = 0;
                    }
                }

                for (int j = 0; j < CITY; j++)
                {
                    if (ants[c].seq[j] == -1)
                    {
                        num_exp[j] = get_num(f, j);
                        sum_exp += num_exp[j];
                    }
                }
                for (int j = 0; j < CITY; j++)
                {
                    if (ants[c].seq[j] == -1)
                    {
                        each_probability[j] = num_exp[j] / sum_exp;
                        for (int k = j; k < CITY; k++)
                        {
                            if (acc_probability[k] != -1)
                                acc_probability[k] += each_probability[j];
                        }
                    }
                }

                //移動
                //轉換規則
                if (t < p * T) //探索
                    ants[c].location = choose_road(acc_probability);
                else //追隨
                {
                    double max_probability = 0;
                    int max_location;
                    for (int k = 0; k < CITY; k++)
                    {
                        if (acc_probability[k] > max_probability)
                        {
                            max_location = k;
                            max_probability = acc_probability[k];
                        }
                    }

                    ants[c].location = max_location;
                }

                int g = ants[c].location; //新位置
                ants[c].seq[g] = i;       //第 i 步 (step)
                ants[c].total_dis += road[f][g];
            }
            /*for (int c = 0; c < N; c++)
            {
                cout<<c<<": "<<ants[c].total_dis<<endl;
            }*/

            //局部更新費洛蒙
            if (t < p * T)
            {
                for (int c = 0; c < N; c++)
                    update_phe_part(ants[c], i);
            }
        }
        // cout << "OK" << endl;
        //一代螞蟻完成
        for (int c = 0; c < N; c++) //更新最佳路徑
        {
            if (best_dis > ants[c].total_dis)
            {
                change++;
                best_dis = ants[c].total_dis;
                for (int j = 0; j < CITY; j++)
                    best_seq[j] = ants[c].seq[j];
            }
        }
        t++;

        //整體更新費洛蒙

        if (t < 50)//迭代小於的條件下，每隻螞蟻都做整體費洛蒙更新
        {
            for (int i = 0; i < CITY; i++)//費洛蒙蒸發
            {
                for (int j = 0; j < CITY; j++)
                {
                    road_phe[i][j] *= u;
                    if (road_phe[i][j] < min_phe)
                        road_phe[i][j] = min_phe;
                }
            }

            for (int c = 0; c < N; c++)
            {
                update_phe_all(ants[c].seq, ants[c].total_dis);
            }
        }
        /*else if(t >=25 && t < 75){
            if(t % 5 == 0)
                update_phe(best_seq, best_dis);
        }
        else if(t >= 75 && t < 150)
        {
            if(t % 3 == 0)
                update_phe(best_seq, best_dis);
        }
        else if(t >= 150 && t < 300)
        {
            if(t % 2 == 0)
                update_phe(best_seq, best_dis);
        }*/
        else
            update_phe(best_seq, best_dis);

        cout << "第" << t << "次完成" << endl;
    }

    // cout << "第 " << best_ant + 1 << " 隻螞蟻" << endl;
    cout << "距離為 " << best_dis << endl;
    cout << "改變了 " << change << " 次" << endl;
    int step = 0, i = 0;
    while (step < CITY)
    {
        if (best_seq[i] == step)
        {
            cout << i + 1 << "  ";
            step++;
        }

        i++;
        i %= CITY;
    }
    cout << endl;
    cout << "max_phe = " << max_phe << endl;
    cout << "min_phe = " << min_phe << endl;

    /*for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            cout << fixed << setprecision(9) << road_phe[i][j] << "  ";
        }
        cout << endl;
    }*/
    DWORD end_time = GetTickCount();
    cout << "這個程式執行時間為 " << (end_time - star_time) << "ms." << endl;
    return 0; 
}