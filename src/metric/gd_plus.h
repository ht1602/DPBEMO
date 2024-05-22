#pragma once
#include <string>
#include <vector>

#include "core/individual.h"

namespace emoc {

	double CalculateGDPlus(Individual** pop, int pop_num, int obj_num, double** pf_data, int pf_size);
	double CalculateGDPlus(const std::vector<std::vector<double>>& pop,int pop_num,int obj_num,std::string& problem_name);
}



