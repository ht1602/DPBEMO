

#pragma once
#include "core/individual.h"
#include "problem/problem.h"
#include <iostream>
#include <Python.h>

namespace emoc
{
    class Swimmer:public Problem
    {
    public:
        Swimmer(int dec_num, int obj_num);
        virtual ~Swimmer();
        void CalObj(Individual *ind);
    };
    
} // namespace name
