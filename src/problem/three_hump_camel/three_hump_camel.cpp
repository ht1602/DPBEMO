#include "problem/three_hump_camel/three_hump_camel.h"

#include <cmath>

#include "core/macro.h"
#include "core/global.h"

namespace emoc {

	ThreeHumpCamel::ThreeHumpCamel(int dec_num, int obj_num) :Problem(dec_num, obj_num)
	{
		for (int i = 0; i < dec_num; ++i)
		{
			lower_bound_[i] = -5;
			upper_bound_[i] = 5;
		}
	}

	ThreeHumpCamel::~ThreeHumpCamel()
	{

	}

	void ThreeHumpCamel::CalObj(Individual* ind)
	{
		double f = 2 * pow(ind->dec_[0], 2) - 1.05 * pow(ind->dec_[0], 4) + pow(ind->dec_[0], 6) / 6 + ind->dec_[0] * ind->dec_[1] + pow(ind->dec_[1], 2);
		ind->obj_[0] = f;
	}

}