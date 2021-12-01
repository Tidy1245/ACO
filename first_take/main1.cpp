#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iomanip>
#define CITY 51
using namespace std;

typedef struct city
{
    int x;
    int y;
} city;

typedef struct ant
{
    double total_dis;
    int seq[CITY];
    int location;
} ant;

double road[CITY][CITY];     //���������q�Z��
double road_phe[CITY][CITY]; //���q�O���X (���V�ϡA�D��ٯx�})
const int Q = 10;            //�`��
const int N = 50;            //���Ƽ�
const double u = 0.3;        //�O���X�]�o�Y��
const double p = 0.1;        //�ഫ�W�h���v
int T = 700;                 //���զ���
city cities[CITY];           //����
ant ants[N];                 //�Ƹs

void set_city() //�]�w�����y��
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
                road_phe[i][j] = Q / L;
            }
        }
    }
}
void init_ant(int i) //��l�ƿ��Ƹ�T�ó]�w��l��m
{
    ants[i].total_dis = 0;
    for (int j = 0; j < CITY; j++)
        ants[i].seq[j] = -1;
    srand(time(NULL));
    int min = 0;
    int max = CITY - 1;
    int x = rand() % (max - min + 1) + min; // 0 <= x < 1
    ants[i].seq[x] = 0;
    ants[i].location = x;
}
bool unfinished(ant ants) //�P�_���ƬO�_�������X������
{
    for (int i = 0; i < CITY; i++)
    {
        if (ants.seq[i] == -1)
            return true;
    }
    return false;
}

double get_num(int from, int to) //[�n i->j(t)]^�\*[�b i->j]^? (�ثe�\,?���O1)
{
    return road_phe[from][to] * (Q / road[from][to]);
}
int choose_road(double *acc) //��ܤU�@����
{
    srand(time(NULL));
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
        for (int j = 0; j < CITY; j++)
            road_phe[i][j] *= u; //�O���X�]�o

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
    while (step < CITY)
    {
        if (seq[i] == step)
        {
            to = i;
            road_phe[from][to] += Q / L; //�G�n i->j
            from = i;
            step++;
        }
        if (i == CITY - 1)
            i = 0;
        else
            i++;
    }
}

int main()
{
    set_city();
    set_road();
    set_phe();

    int best_ant;                  //�̦n������
    int best_seq[CITY];            //�̨θ��{
    double best_dis = 99999;       //�̵u�Z��
    double num_exp[CITY];          //[�n i->j(t)]^�\*[�b i->j]^? (i��j�������)
    double each_probability[CITY]; // P i->j  (i��j�����v)
    double acc_probability[CITY];  //�UP i->j (�ֿn)
    int t = 0;
    while (t < T)
    {
        // init_each_of_ants
        for (int i = 0; i < N; i++) init_ant(i);
        // test_each_of_ants
        for (int i = 0; i < N; i++)
        {
            int step = 0;
            //cout<<"�� "<< t + 1 <<"���^��~ / "; 
            while (unfinished(ants[i]))
            {
                //cout<<"�� "<< t + 1 <<"���^�餺 / "; 
                int f = ants[i].location; //��e��m
                double sum_exp = 0;       //�U[�n i->u(t)]^�\*[�b i->u]^? (�`�����)

                for (int j = 0; j < CITY; j++)
                    acc_probability[j] = 0;

                for (int j = 0; j < CITY; j++)
                {
                    if (ants[i].seq[j] != -1)
                    {
                        acc_probability[j] = -1;
                        num_exp[j] = 0;
                        each_probability[j] = 0;
                    }
                }

                for (int j = 0; j < CITY; j++)
                {
                    if (ants[i].seq[j] == -1)
                    {
                        num_exp[j] = get_num(f, j);
                        sum_exp += num_exp[j];
                    }
                }
                for (int j = 0; j < CITY; j++)
                {
                    if (ants[i].seq[j] == -1)
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
                step++;
                ants[i].location = choose_road(acc_probability);
                int g = ants[i].location; //�s��m
                ants[i].seq[g] = step;
                ants[i].total_dis += road[f][g];
            }
            if (best_dis > ants[i].total_dis)
            {
                best_ant = i;
                best_dis = ants[i].total_dis;
                for (int j = 0; j < CITY; j++)
                    best_seq[j] = ants[i].seq[j];
            }
        }
        t++;
        cout << "��" << t << "������" << endl;
        update_phe(best_seq, best_dis);
    }
    // cout << "�� " << best_ant + 1 << " ������" << endl;
    cout << "�Z���� " << best_dis << endl;
    int step = 0, i = 0;
    while (step < CITY)
    {
        if (best_seq[i] == step)
        {
            cout << i + 1 << " -> ";
            step++;
        }
        if (i == CITY - 1)
            i = 0;
        else
            i++;
    }
    return 0;
}