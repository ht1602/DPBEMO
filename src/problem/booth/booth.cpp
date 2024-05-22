#include "problem/booth/booth.h"

#include <cmath>

#include "core/macro.h"
#include "core/global.h"

namespace emoc {

	Booth::Booth(int dec_num, int obj_num) :Problem(dec_num, obj_num)
	{
		for (int i = 0; i < dec_num; ++i)
		{
			lower_bound_[i] = -10.0;
			upper_bound_[i] = 10.0;
		}
	}

	Booth::~Booth()
	{

	}

	void Booth::CalObj(Individual *ind)
	{
        ind->obj_[0] = pow(ind->dec_[0] + 2 * ind->dec_[1] - 7, 2) + pow(2 * ind->dec_[0] + ind->dec_[1] - 5, 2);
	}

}