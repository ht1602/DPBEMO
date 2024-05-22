

#pragma once
#include "core/individual.h"
#include "problem/problem.h"
#include <iostream>
#include <Python.h>
#include <string>

namespace emoc
{
    class RNA:public Problem
    {
    public:
        RNA(int dec_num, int obj_num);
        virtual ~RNA();
        void CalObj(Individual *ind);
        void init();
    private:
        void projection(Individual *ind);

    public:
        // std::string target_structure;
        char* target_structure;
        char* S;
    };

    
} // namespace name
