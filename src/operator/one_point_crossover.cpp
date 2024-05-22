#include "operator/one_point_crossover.h"

#include <vector>
#include <cmath>

#include "random/random.h"

namespace emoc {

	void OnePointCrossOver(Individual *parent1, Individual *parent2, Individual *offspring1, Individual *offspring2)
	{
		int dec_num = parent1->dec_.size();
		int k = rnd(0, dec_num -1);

		for (int i = 0; i < dec_num; i++)
		{
			if (i <= k)
			{
				offspring1->dec_[i] = parent1->dec_[i];
				offspring2->dec_[i] = parent2->dec_[i];
			}
            if(i > k)
            {
                offspring1->dec_[i] = parent2->dec_[i];
                offspring2->dec_[i] = parent1->dec_[i];
            }
		}

	}
}

