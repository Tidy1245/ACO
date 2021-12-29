#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iomanip>
#include <windows.h>
#include <time.h>
#define CITY 11
using namespace std;

typedef struct city
{
    double x;
    double y;
    double weight;
} city;

typedef struct ant
{
    double total_dis;
    double charge;
    int seq[CITY];
    int location;
    int first;
} ant;

double road[CITY][CITY];     //���������q�Z��
double road_phe[CITY][CITY]; //���q�O���X (���V�ϡA�D��ٯx�})
const int Q = 1;             //�`��
const int N = 25;            //���Ƽ�
const double u = 0.8;        //�O���X�]�o�Y��
const double p = 0.8;        //�ഫ�W�h��v
int T = 50;                  //���զ���
city cities[CITY];           //����
ant ants[N];                 //�Ƹs

double max_phe;
double min_phe;
int change = 0;
const int W = 31;

void set_city() //�]�w�����y��
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
void set_road() //�]�w�Ҧ����q��
{
    for (int i = 0; i < CITY; i++)
    {
        for (int j = 0; j < CITY; j++)
        {
            road[i][j] = sqrt(pow(cities[i].x - cities[j].x, 2) + pow(cities[i].y - cities[j].y, 2));
        }
    }
}
double get_dis(city a, city b) //�^�Ǩ⫰���Z��
{
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}
void set_phe() //�]�w���q�O���X
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
void init_ant(int i) //��l�ƿ��Ƹ�T�ó]�w��l��m
{
    ants[i].total_dis = 0;
    ants[i].charge = 0;
    ants[i].location = 0;
    for (int j = 0; j < CITY; j++)
        ants[i].seq[j] = -1;
    ants[i].seq[0] = 0;
    ants[i].first = 0;
}

double get_num(int from, int to) //[�n i->j(t)]^�\*[�b i->j]^? (�ثe�\,?���O1)
{
    return (road_phe[from][to] * pow(Q / road[from][to], 2) * (cities[to].weight));
}
int choose_road(double *acc) //��ܤU�@����
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
            road_phe[i][j] *= u; //�O���X�]�o
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
    while (1) //�٨S�^��_�I�N�@����
    {
        if (seq[i] == step)
        {
            to = i;
            road_phe[from][to] += Q / L; //�G�n i->j

            if (road_phe[from][to] > max_phe)
                road_phe[from][to] = max_phe;

            from = i;

            if (step == 0)
                break; //������s

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
        int from, to; //�����s��
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
    while (1) //�٨S�^��_�I�N�@����
    {
        if (seq[i] == step)
        {
            to = i;
            road_phe[from][to] += Q / L; //�G�n i->j

            if (road_phe[from][to] > max_phe)
                road_phe[from][to] = max_phe;

            from = i;

            if (step == 0)
                break; //������s

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
    int best_seq[CITY];            //�̨θ��{
    double best_dis = 1500;        //�̵u�Z��
    double num_exp[CITY];          //[�n i->j(t)]^�\*[�b i->j]^? (i��j�������)
    double each_probability[CITY]; // P i->j  (i��j�����v)
    double acc_probability[CITY];  //�UP i->j (�ֿn)
    int t = 0;

    int best_charge = 100000;

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
        

        max_phe = (1 / 1 - u) * (1 / best_dis);
        min_phe = max_phe / (2 * CITY);

        int bag[N];
        for (int c = 0; c < N; c++) bag[c] = W;

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
                    ants[c].charge += (bag[c] * road[from][to]);
                    bag[c] -= cities[to].weight;
                }
                break;
            }
            // �C�����Ʋ��ʤ@�B

            for (int c = 0; c < N; c++)
            {
                int f = ants[c].location; //��e��m

                double sum_exp = 0; //�U[�n i->u(t)]^�\*[�b i->u]^? (�`�����)

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

                //����
                //�ഫ�W�h
                if (t < p * T) //����
                    ants[c].location = choose_road(acc_probability);
                else //�l�H
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

                int g = ants[c].location; //�s��m
                ants[c].seq[g] = i;       //�� i �B (step)
                ants[c].total_dis += road[f][g];
                ants[c].charge += (bag[c] * road[f][g]);
                bag[c] -= cities[g].weight;
            }
            /*for (int c = 0; c < N; c++)
            {
                cout<<c<<": "<<ants[c].total_dis<<endl;
            }*/

            //������s�O���X
            if (t < p * T)
            {
                for (int c = 0; c < N; c++)
                    update_phe_part(ants[c], i);
            }
        }
        
         //cout << t << endl;
        //�@�N���Ƨ���
        for (int c = 0; c < N; c++) //��s�̨θ��|
        {
            if (best_charge > ants[c].charge)
            {
                best_charge = ants[c].charge;
                best_dis = ants[c].total_dis;
                for (int j = 0; j < CITY; j++)
                    best_seq[j] = ants[c].seq[j];
            }
        }
        
        t++;

        //�����s�O���X
        
        if (t < 50)//���N�p�󪺱���U�A�C�����Ƴ�������O���X��s
        {
            for (int i = 0; i < CITY; i++)//�O���X�]�o
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
        else
            update_phe(best_seq, best_dis);
        
    }
    
    cout<<best_charge<<endl;
    cout<<best_dis<<endl;
 
    int step = 0, i = 0;
    while (step < CITY)
    {
        if (best_seq[i] == step)
        {
            cout << i << "  ";
            step++;
        }

        i++;
        i %= CITY;
    }
    cout <<"0"<<endl;
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
    cout <<  (end_time - star_time) << endl;
    cout << (double)clock() / CLOCKS_PER_SEC << "s" << endl;
    return 0; 
}