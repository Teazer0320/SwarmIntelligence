#include "Alg.h"
#include "test_function.h"
#include <vector>
#include <random>
#include <iostream>
#include <chrono>
#include <cmath>

using namespace std;

vector<double> Position_avg(vector<vector<double>> &Hawks, int D)
{
    vector<double> avg(D, 0.0);
    for (int i = 0; i < Hawks.size(); ++i)
        for (int j = 0; j < D; ++j)
            avg[j] += Hawks[i][j];
    for (int i = 0; i < D; ++i)
        avg[i] /= Hawks.size();
    return avg;
}

double random_double(double lower_bound, double upper_bound)
{
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    static mt19937 gen(seed);
    uniform_real_distribution<double> dis(lower_bound, upper_bound);
    return dis(gen);
}

void HHO::RunALG()
{
    init();
    while (MaxEval > CurEval)
    {
        // cout << "CurEval: " << CurEval << " CurRun: " << CurRun << endl;
        /* calculate fitness and set rabbit */
        for (int i = 0; i < N; ++i)
        {
            CurEval += 1;
            double fit = Fitness(&Hawks[i][0], func_num);
            // double fit = Fitness(Hawks[i], func_num);
            if (fit < rabbit_fit)
            {
                rabbit_fit = fit;
                rabbit = Hawks[i];
            }
        }
        // cout << "rabbit_fit: " << rabbit_fit << endl;

        /* Update Harris hawks locations*/
        for (int i = 0; i < N; i++)
        {
            E0 = 2 * random_double(-1.0, 1.0) - 1; // E0 = [-1, 1]
            E = 2 * E0 * (1 - CurEval / MaxEval);
            Jump_len = 2 * (1 - random_double(0.0, 1.0));
            double r = random_double(0.0, 1.0);
            if (abs(E) >= 1) /* Exploration phase */
            {
                Xavg = Position_avg(Hawks, D);
                vector<double> Xrand(D);

                Xrand = Hawks[int(random_double(0, N))];
                chance_q = random_double(0.0, 1.0);
                if (chance_q >= 0.5) // equation (1)
                    for (int j = 0; j < D; j++)
                        Hawks[i][j] = Xrand[j] - random_double(0.0, 1.0) * abs(Xrand[j] - 2 * random_double(0.0, 1.0) * Hawks[i][j]);

                else
                    for (int j = 0; j < D; j++)
                        Hawks[i][j] = rabbit[j] - Xavg[j] - random_double(0.0, 1.0) * (lb + random_double(0.0, 1.0) * (ub - lb));
            }
            else /* Exploitation phase */
            {
                r = random_double(0.0, 1.0);
                if (r >= 0.5 and abs(E) >= 0.5)
                    Besiege_Soft();
                else if (r >= 0.5 and abs(E) < 0.5)
                    Besiege_Hard();
                else if (r < 0.5 and abs(E) >= 0.5)
                    RapidDive_Soft();
                else if (r < 0.5 and abs(E) < 0.5)
                    RapidDive_Hard();
            }
        }
    }
}

void HHO::init()
{
    Hawks.resize(N, vector<double>(D));
    set_search_bound(&ub, &lb, func_num);
    CurEval = 0;
    rabbit_fit = DBL_MAX;
    E0 = 0;
    E = 0;
    Jump_len = 0;
    rabbit.clear();
    Xavg.clear();

    /* Set inintial Hawks positions */
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < D; ++j)
            Hawks[i][j] = random_double(lb, ub);

    /* output all positions */
    // Show_Hawks();
}

void HHO::Show_Hawks()
{
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < D; ++j)
            cout << Hawks[i][j] << " ";
        cout << endl;
    }
}

void HHO::Besiege_Soft()
{
    // cout << "Besiege_Soft" << endl;
    // Jump_len = 2 * (1 - random_double(0.0, 1.0));

    for (int i = 0; i < N; i++)
    {
        vector<double> Xdiff(D);
        for (int j = 0; j < D; j++)
        {
            Xdiff[j] = Hawks[i][j] - rabbit[j];                                   // Equation(5)
            Hawks[i][j] = Xdiff[j] - E * abs(Jump_len * rabbit[j] - Hawks[i][j]); // Equation(4)
        }
    }
}

void HHO::Besiege_Hard()
{
    // cout << "Besiege_Hard" << endl;
    for (int i = 0; i < N; i++)
    {
        vector<double> Xdiff(D);
        for (int j = 0; j < D; j++)
        {
            Xdiff[j] = Hawks[i][j] - rabbit[j];          // Equation(5)
            Hawks[i][j] = rabbit[j] - E * abs(Xdiff[j]); // Equation(6)
        }
    }
}

void HHO::RapidDive_Soft()
{
    // cout << "RapidDive_Soft" << endl;
    for (int i = 0; i < N; i++)
    {
        vector<double> Y(D);
        vector<double> S(D);
        vector<double> Z(D);
        /* Generate random vector S*/
        for (int j = 0; j < D; j++)
            S[j] = random_double(-1.0, 1.0);

        for (int j = 0; j < D; j++)
        {
            Y[j] = rabbit[j] - E * abs(Jump_len * rabbit[j] - Hawks[i][j]); // Equation(7)
            Z[j] = Y[j] + S[j] * LevyFlight(D);                             // Equation(8)
        }
        double cur_fit = Fitness(&Hawks[i][0], func_num);
        double Y_fit = Fitness(&Y[0], func_num);
        double Z_fit = Fitness(&Z[0], func_num);

        if (Y_fit < cur_fit) // Equation(10)
            Hawks[i] = Y;
        else if (Z_fit < cur_fit)
            Hawks[i] = Z;
    }
}

void HHO::RapidDive_Hard()
{
    // cout << "RapidDive_Hard" << endl;
    for (int i = 0; i < N; i++)
    {
        vector<double> Y(D);
        vector<double> S(D);
        vector<double> Z(D);
        /* Generate random vector S*/
        for (int j = 0; j < D; j++)
            S[j] = random_double(-1.0, 1.0);

        for (int j = 0; j < D; j++)
        {
            Y[j] = rabbit[j] - E * abs(Jump_len * rabbit[j] - Xavg[j]); // Equation(12)
            Z[j] = Y[j] + S[j] * LevyFlight(D);                         // Equation(13)
        }
        double cur_fit = Fitness(&Hawks[i][0], func_num);
        double Y_fit = Fitness(&Y[0], func_num);
        double Z_fit = Fitness(&Z[0], func_num);

        if (Y_fit < cur_fit) // Equation(10)
            Hawks[i] = Y;
        else if (Z_fit < cur_fit)
            Hawks[i] = Z;
    }
}

// double HHO::Fitness(vector<double> x, int funcNum)
double HHO::Fitness(const double *x, int funcNum)
{
    CurEval += 1;
    double f = calculate_test_function(x, D, funcNum);
    // double f = calculate_test_function(x.data(), D, funcNum);
    // if (f < 0)
    // {
    //     cout << "Fitness < 0" << endl;
    //     cout << "f: " << f << endl;
    //     cout << "x: ";
    //     for (int i = 0; i < D; i++)
    //         cout << x[i] << " ";
    //     cout << endl;
    // }
    return f;
}

double HHO::LevyFlight(int x) // Equation(9)
{
    double beta = 1.5;
    double u, v, step_size;
    u = random_double(0.0, 1.0);
    v = random_double(0.0, 1.0);

    // 計算步長
    double sigma = pow((tgamma(1.0 + beta) * sin(M_PI * beta / 2.0)) / (tgamma((1.0 + beta) / 2.0) * beta * std::pow(2.0, (beta - 1.0) / 2.0)), 1.0 / beta);
    step_size = 0.01 * u * sigma / abs(v);

    // 計算新位置
    return x + step_size;
}

double HHO::GetResult()
{
    // for (int i = 0; i < rabbit.size(); i++)
    //     cout << rabbit[i] << " ";
    // cout << endl;
    return rabbit_fit;
}

double HHO::CheckBoundary(double x)
{
    if (x > ub)
        return ub;
    else if (x < lb)
        return lb;
    else
        return x;
}