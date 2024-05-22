#pragma once
#include "core/individual.h"
#include "algorithm/algorithm.h"
#include "problem/problem.h"
#include "metric/metric_head_collect.h"
#include "../vendor/armadillo/armadillo"

#include <vector>
#include <stack>
using namespace arma;

namespace emoc {

	class NSGA2_RUCB_AL : public Algorithm
	{
	public:
		typedef struct
		{
			int index;
			double distance;
		}DistanceInfo;  // store crowding distance of index-th individual

		NSGA2_RUCB_AL(int thread_id);
		virtual ~NSGA2_RUCB_AL();
		
		void Solve();

	private:
		void Initialization();
		void Crossover(Individual **parent_pop, Individual **offspring_pop);

		// set the crowding distance of given individual index
		void SetDistanceInfo(std::vector<DistanceInfo> &distanceinfo_vec, int target_index, double distance);

		// use crowding distance to sort the individuals with rank rank_index, the result is stored in pop_sort
		// return the number of indviduals of rank rank_index
		int CrowdingDistance(Individual **mixed_pop, int pop_num, int *pop_sort, int rank_index);

		// do nsga2's environment selection on mixed_pop, the result is stored in parent_pop
		bool EnvironmentalSelection(Individual **parent_pop, Individual **mixed_pop);
		
		// calculate the virtual utility function value
		void UtilityFunction_Gaussian(Individual **pop);
		void StorePreference(Individual **pop);
		void ActiveStorePreference(Individual **pop);
		int Consult_DM();
		int FitnessViaPreference(Individual **mixed_pop, int pop_num, int *pop_sort, int rank_index, int nneed);
		double UpdateUtilityPrediction(double obj, int index);
		float CalculateKLDivergence(Individual **pop);
		void CalculateMinDis(double *best);
		void CalculateTotalDis(double *best);
	
    private:
        HVCalculator hv_calculator_;
        int pf_size;
        double** pf_data;

		bool usingPref;
		int count; // count the number of consultation times

		double *weight;	// weight for DM
		double sigma; 	// param for utility function
		double *utl;	// store the utility function value
		int select_sum;
		size_t *compare;
		mat PVals;
		int best_index;

		double KL;
		double gamma;
		
		int max_consultation;
		double **recommend_point;
		double *util_prob1;
		double *util_prob2;
    
    };

}