#pragma once
#include "core/individual.h"

namespace emoc{
    double CalculateIGD_CF(Individual **pop,int pop_num,int obj_num,double* reference_point,double preferred_r);
    int* findPF(Individual **pop, int pop_num, int obj_num, bool is_consider_cons);
}