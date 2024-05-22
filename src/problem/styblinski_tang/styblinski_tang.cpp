#include "problem/styblinski_tang/styblinski_tang.h"

#include <cmath>

#include "core/macro.h"
#include "core/global.h"

namespace emoc {

	StyblinskiTang::StyblinskiTang(int dec_num, int obj_num) :Problem(dec_num, obj_num)
	{
		for (int i = 0; i < dec_num; ++i)
		{
			lower_bound_[i] = -5.0;
			upper_bound_[i] = 5.0;
		}
	}

	StyblinskiTang::~StyblinskiTang()
	{

	}

	void StyblinskiTang::CalObj(Individual *ind)
	{
        double sum = 0;
        for(int i = 0 ; i < dec_num_; i++)
            sum += pow(ind->dec_[i], 4) - 16 * pow(ind->dec_[i], 2) + 5 * ind->dec_[i];

        ind->obj_[0] = sum / dec_num_;
	}

}