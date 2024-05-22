#pragma once
#include "core/individual.h"
#include "problem/problem.h"

namespace emoc {

	class Easom:public Problem
	{
	public:
		Easom(int dec_num, int obj_num);
		virtual ~Easom();

		void CalObj(Individual *ind);
	};

}