#include <iostream>

#include "Alg.h"

#define RUN 30
#define EVAL(d) 10000 * d

const int Dimension[3] = {3, 10, 30};

int main(void)
{

    Alg algorithm;

    algorithm.RunALG(Dimension[0], RUN, EVAL(Dimension[0]));

    return 0;
}
