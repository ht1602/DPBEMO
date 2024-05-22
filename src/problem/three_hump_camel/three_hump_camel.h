#pragma once
#include "core/individual.h"
#include "problem/problem.h"

namespace emoc {

	class ThreeHumpCamel :public Problem
	{
	public:
		ThreeHumpCamel(int dec_num, int obj_num);
		virtual ~ThreeHumpCamel();

		void CalObj(Individual* ind);
	};

}