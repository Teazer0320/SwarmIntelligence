#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iterator>
#include "Alg.h"

using namespace std;

#define EVAL(d) 10000 * d
#define ArraySize(x) (sizeof(x) / sizeof(x[0]))
#define RUN 30

int FuncNum(string funcName);
void WriteFile(string fileName, vector<double> data);

const int Dimension[3] = {2, 10, 30};
int main(int argc, char const *argv[])
{
    int N = atoi(argv[1]);

    for (int i = 0; i < ArraySize(Dimension); i++)
    {
        string fileName = "Output/" + string(argv[2]) + "_" + to_string(Dimension[i]) + "D.txt";
        vector<double> result;
        cout << fileName << endl;
        HHO Alg(N, Dimension[i], EVAL(Dimension[i]), FuncNum(argv[2]), fileName);

        for (int r = 0; r < RUN; r++)
        {
            cout << "Run " << r + 1 << " : ";
            Alg.RunALG();
            result.push_back(Alg.GetResult());
            cout << "fitness " << result.back() << endl;
        }
        WriteFile(fileName, result);
        result.clear();
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

void WriteFile(string fileName, vector<double> data)
{
    /* Write output file */
    ofstream f;
    f.open(fileName);
    for (int i = 0; i < data.size(); i++)
    {
        f << data[i] << endl;
    }
    f.close();
}