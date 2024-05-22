#pragma once
#include "core/individual.h"
#include "problem/problem.h"

namespace emoc {

	class StyblinskiTang:public Problem
	{
	public:
		StyblinskiTang(int dec_num, int obj_num);
		virtual ~StyblinskiTang();

		void CalObj(Individual *ind);
	};

}