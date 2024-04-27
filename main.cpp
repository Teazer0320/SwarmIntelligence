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
        string fileName = "Output/" + string(argv[2]) + "_" + to_string(Dimension[i]) + "D.txt";
        cout << fileName << endl;
        HHO Alg(N, Dimension[i], EVAL(Dimension[i]), FuncNum(argv[2]), fileName);
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
