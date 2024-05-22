#pragma once
#include "core/individual.h"
#include "problem/problem.h"

namespace emoc {

	class Booth:public Problem
	{
	public:
		Booth(int dec_num, int obj_num);
		virtual ~Booth();

		void CalObj(Individual *ind);
	};

}