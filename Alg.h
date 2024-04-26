#ifndef ALG_H
#define ALG_H
#include "test_function.h"
#include <iostream>
#include <vector>

class Alg
{
public:
    void RunALG(int dimension, int run, int eval_num);

private:
    double upper_bound, lower_bound;
};

#endif