#include "problem/easom/easom.h"

#include <cmath>

#include "core/macro.h"
#include "core/global.h"

namespace emoc {

	Easom::Easom(int dec_num, int obj_num) :Problem(dec_num, obj_num)
	{
		for (int i = 0; i < dec_num; ++i)
		{
			lower_bound_[i] = -100.0;
			upper_bound_[i] = 100.0;
		}
	}

	Easom::~Easom()
	{

	}

	void Easom::CalObj(Individual *ind)
	{
        
        ind->obj_[0] = - cos(ind->dec_[0]) * cos(ind->dec_[1]) * exp(-pow(ind->dec_[0] - PI, 2) - pow(ind->dec_[1] - PI, 2));
	}

}