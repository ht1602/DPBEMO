#pragma once
#include "core/global.h"
#include "core/individual.h"
#include "core/emoc_utility_structures.h"

namespace emoc {

	void UniformMutation(Individual *ind, MutationParameter &mutation_para, int upper_bound);

}