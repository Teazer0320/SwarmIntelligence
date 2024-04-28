#include <iostream>
#include <vector>
#include <float.h>
#include <string>

#ifndef ALG_H
#define ALG_H

using namespace std;
// #include "test_function.h"

// Harris Hawks Optimization Algorithm
class HHO
{
public:
    HHO(int N, int D, int MaxEval, int func_num, string fileName)
    {
        this->N = N;
        this->D = D;
        this->MaxEval = MaxEval;
        this->func_num = func_num;
        this->fileName = fileName;
    }
    // ~HHO() { cout << "HHO Destructor" << endl; }
    void RunALG();
    int N;       // Number of population
    int D;       // Dimension
    int MaxEval; // Maximum number of evaluations
    int func_num;
    string fileName; // file Name
    double GetResult();

private:
    /* Variables */
    vector<vector<double>> Hawks; // Population
    int CurEval = 0;              // Current number of evaluations
    double rabbit_fit = DBL_MAX;  // Fitness of current rabbit
    double E0, E;                 // initial Escape energy, Escaping energy
    double Jump_len;              // jump strength
    double lb, ub;                // Lower and upper bounds of the search space
    double chance_q;              // Probability of rabbit being selected as the prey
    vector<double> rabbit;        // Rabbit position
    vector<double> Xavg;

    /* Functions */
    void init();
    void Besiege_Soft();   // Soft besiege
    void Besiege_Hard();   // Hard besiege　
    void RapidDive_Soft(); // Soft besiege with progressive rapid dives
    void RapidDive_Hard(); // Hard besiege with progressive rapid dives
    void Show_Hawks();
    double LevyFlight(int x);
    // double Fitness(vector<double> x, int funcNum);
    double Fitness(const double *x, int funcNum);
    double CheckBoundary(double x);
};

#endif