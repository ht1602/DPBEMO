#include "algorithm/algorithm_head_collect.h"

#include "core/macro.h"
#include "algorithm/algorithm_factory.h"

namespace emoc {

	// Docomposition Based Algorithms
	EMOC_REGIST_ALGORITHM(Multiobjective, Docomposition Based, MOEADDYTS);
	EMOC_REGIST_ALGORITHM(Multiobjective, Docomposition Based, MOEAD);
	EMOC_REGIST_ALGORITHM(Multiobjective, Docomposition Based, MOEADDE);
	EMOC_REGIST_ALGORITHM(Multiobjective, Docomposition Based, MOEADDRA);
	EMOC_REGIST_ALGORITHM(Multiobjective, Docomposition Based, MOEADUCB);
	EMOC_REGIST_ALGORITHM(Multiobjective, Docomposition Based, MOEADFRRMAB);
	EMOC_REGIST_ALGORITHM(Multiobjective, Docomposition Based, MOEADGRA);
	EMOC_REGIST_ALGORITHM(Multiobjective, Docomposition Based, MOEADIRA);
	EMOC_REGIST_ALGORITHM(Multiobjective, Docomposition Based, ENSMOEAD);
	EMOC_REGIST_ALGORITHM(Multiobjective, Docomposition Based, MOEADCDE);
	EMOC_REGIST_ALGORITHM(Multiobjective, Docomposition Based, MOEADSTM);
	EMOC_REGIST_ALGORITHM(Multiobjective, Docomposition Based, RVEA);
	EMOC_REGIST_ALGORITHM(Multiobjective, Docomposition Based, MOEADPAS);
	EMOC_REGIST_ALGORITHM(Multiobjective, Docomposition Based, MOEADM2M);
	EMOC_REGIST_ALGORITHM(Multiobjective, Docomposition Based, MOEADD);
	EMOC_REGIST_ALGORITHM(Multiobjective, Docomposition Based, MOEAD_LTR);
	EMOC_REGIST_ALGORITHM(Multiobjective, Docomposition Based, MOEADPPL);
	// EMOC_REGIST_ALGORITHM(Multiobjective, Docomposition Based, MOEADRUCB);
	EMOC_REGIST_ALGORITHM(Multiobjective, Docomposition Based, MOEADPLVF);
	EMOC_REGIST_ALGORITHM(Multiobjective, Docomposition Based, MOEAD_RUCB_AL);
	EMOC_REGIST_ALGORITHM(Multiobjective, Docomposition Based, IEMOD);
	EMOC_REGIST_ALGORITHM(Multiobjective, Docomposition Based, MOEADPBO);
	EMOC_REGIST_ALGORITHM(Multiobjective, Docomposition Based, DPBEMO_MOEAD);


	// Dominance Based Algorithms
	EMOC_REGIST_ALGORITHM(Multiobjective,Dominance Based, NSGA2);
	EMOC_REGIST_ALGORITHM(Multiobjective,Dominance Based, NSGA3);
	EMOC_REGIST_ALGORITHM(Multiobjective,Dominance Based, SPEA2);
	EMOC_REGIST_ALGORITHM(Multiobjective,Dominance Based, SPEA2SDE);
	EMOC_REGIST_ALGORITHM(Multiobjective,Dominance Based, tDEA);
	EMOC_REGIST_ALGORITHM(Multiobjective,Dominance Based, NSGA2PPL);
	EMOC_REGIST_ALGORITHM(Multiobjective,Dominance Based, NSGA2_LTR);
	EMOC_REGIST_ALGORITHM(Multiobjective,Dominance Based, NSGA2_RUCB_AL);
	EMOC_REGIST_ALGORITHM(Multiobjective,Dominance Based, NEMO_0);
	EMOC_REGIST_ALGORITHM(Multiobjective,Dominance Based, DPBEMO_NSGA2);
	EMOC_REGIST_ALGORITHM(Multiobjective,Dominance Based, DPBEMO_DTS);
	// EMOC_REGIST_ALGORITHM(Multiobjective,Dominance Based, NSGA2_AR);

	// Indicator Based Algorithms
	EMOC_REGIST_ALGORITHM(Multiobjective,Indicator Based, IBEA);
	EMOC_REGIST_ALGORITHM(Multiobjective,Indicator Based, HypE);
	EMOC_REGIST_ALGORITHM(Multiobjective,Indicator Based, SMSEMOA);
	EMOC_REGIST_ALGORITHM(Multiobjective,Indicator Based, R2_IBEA);
	EMOC_REGIST_ALGORITHM(Multiobjective,Indicator Based, R2_IBEA_LTR);
	EMOC_REGIST_ALGORITHM(Multiobjective,Indicator Based, R2_IBEA_RUCB_AL);
	EMOC_REGIST_ALGORITHM(Multiobjective,Indicator Based, DPBEMO_R2_IBEA);

	// Constraint Algorithms
	EMOC_REGIST_ALGORITHM(Multiobjective, Constraint, CNSGA2);
	EMOC_REGIST_ALGORITHM(Multiobjective, Constraint, CMOEAD);
	EMOC_REGIST_ALGORITHM(Multiobjective, Constraint, CTAEA);

	// Single Objective Algorithms
	EMOC_REGIST_ALGORITHM(Singleobjective, Single Objective, GA);
	EMOC_REGIST_ALGORITHM(Singleobjective, Single Objective, DifferentialEvolution);
	EMOC_REGIST_ALGORITHM(Singleobjective, Single Objective, SA);
	// EMOC_REGIST_ALGORITHM(Singleobjective, Single Objective, GA_RUCB_AL);

	// multi-objective Bandit
	// EMOC_REGIST_ALGORITHM(Multiobjective, Bandit, ITS);

}