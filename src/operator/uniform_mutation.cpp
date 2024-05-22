#include "operator/uniform_mutation.h"
#include <iostream>
#include <cmath>

#include "random/random.h"

namespace emoc {

	void UniformMutation(Individual* ind, MutationParameter& mutation_para, int upper_bound)
	{

		int dec_num = ind->dec_.size();
        int k = rnd(0, dec_num - 1);
        if(randomperc() < 1.0 / mutation_para.pro)
        {
            ind->dec_[k] = rnd(0, 19);
            std::cout << "shift decision variable in " << k << ": " << ind->dec_[k] << "\n";
        }
            
	}

}

