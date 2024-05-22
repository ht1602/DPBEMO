#pragma once
#include "core/individual.h"

namespace emoc {

	double CalculateSpacing(Individual **pop, int pop_num, int obj_num);
	double CalculateSpacing(const std::vector<std::vector<double>>& pop, int pop_num, int obj_num);

}