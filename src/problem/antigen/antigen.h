#pragma once
#include <vector>

#include "core/individual.h"
#include "problem/problem.h"
#include <string>

namespace emoc {

	class Antigen :public Problem
	{
	public:
		Antigen(int dec_num, int obj_num);
		virtual ~Antigen();

		void init();

		void CalObj(Individual* ind);
		// void CalCon(Individual* ind);

	private:
		void projection(Individual* ind);
		std::vector<std::string> split(std::string& str, std::string pattern);

	private:
		std::string protein_name;
		std::string tool;
		char temp_file[1024];
		char result_file[1024];
		char curr_dir[1024];
	// 	void Init();

		// std::vector<int> profit_;
		// std::vector<int> weight_;
	};

}