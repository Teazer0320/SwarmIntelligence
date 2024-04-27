#include <iostream>
#include <string>
#include <iterator>
#include "Alg.h"

using namespace std;

#define EVAL(d) 10000 * d
#define ArraySize(x) (sizeof(x) / sizeof(x[0]))
int FuncNum(string funcName);

const int Dimension[3] = {2, 10, 30};

int main(int argc, char const *argv[])
{
    int N = atoi(argv[1]);
    for (int i = 0; i < ArraySize(Dimension); i++)
    {
        cout << Dimension[i] << " ";
        HHO Alg(N, Dimension[0], EVAL(Dimension[0]), FuncNum(argv[2]));
        Alg.RunALG();
    }

    return 0;
}

int FuncNum(string funcName)
{
    string funcList[] = {"Ackley", "Rastrigin", "HappyCat", "Rosenbrock", "Zakharov", "Michalewicz"};
    for (int i = 0; i < ArraySize(funcList); i++)
        if (funcName == funcList[i])
            return i + 1;
    return -1;
}
