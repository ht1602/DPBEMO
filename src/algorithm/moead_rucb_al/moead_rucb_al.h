#pragma once
#include "core/individual.h"
#include "algorithm/algorithm.h"
#include "problem/problem.h"
#include "../vendor/armadillo/armadillo"

using namespace arma;

#include <vector>
 
namespace emoc {

	class MOEAD_RUCB_AL : public Algorithm
	{
	public:
		typedef struct
		{
			int index;
			double distance;
		}DistanceInfo;  // store euclidian distance to the index-th weight vector

		typedef enum 
		{
			NEIGHBOUR,
			GLOBAL
		}NeighbourType;

		MOEAD_RUCB_AL(int thread_id);
		virtual ~MOEAD_RUCB_AL();

		void Solve();

	private:
		void Initialization();
		void SetNeighbours();
		void Crossover(Individual **parent_pop, int current_index, Individual *offspring);
		// use offspring to update the neighbour of current_index-th individual with specified aggregation function
		void UpdateSubproblem(Individual *offspring, int current_index,int aggregration_type);
		int Consult_DM();
		void UpdateNeighbour();
		void SetBiasedWeight(double *best);
		void UtilityFunction(Individual **pop);
		void UtilityFunction_Gaussian(Individual **pop);
		void StorePreference(Individual **pop);
		void CalculateMinMaxDis(Individual **pop);
		float CalculateKLDivergence(Individual **pop);
		double UpdateUtilityPrediction(double obj, int index);
		void ActiveStorePreference(Individual **pop);
		void CalculateMinDis(double *best);
		// void CalculateMaxDis(Individual **pop);
		
	private:
		//MOEAD parameters
		double **lambda_;                  // weight vector
		int weight_num_;                   // the number of weight vector
		int **neighbour_;	               // neighbours of each individual
		int neighbour_num_;                // the number of neighbours
		double *weight;
		double *ideal_point_;
		int aggregation_type_;
		double pbi_theta_;
		

		
		//ppl parameters
		//double *weights_obj;
		// int *permutation;
		NeighbourType neighbour_type_;
		int subproblem_id;
		double neighbour_selectpro_; // the probability of select neighbour scope
		// double *reference_point;


		/* common paramters */
    	int miu;                                //number of incumbent candidates.
		// int tau_; 								//consultation interval
    	int first_tau;                          //  number of generations in first consecutive consultation.
    	double eta;                             //step size.
    	double sigma;                            // the width of the Gaussian function.
		
		//param for pairwise preference learning
		int count; // count the number of consultation times
		int replace_num_;  
		int nPromisingWeight;	//num of promising weights
		double step_size_;		//change the weight to what extent
		double *c;				//parameters of utility function
		double *utl;
		mat PVals;
		int select_sum;
		size_t *compare;
		int best_index;
		double minDis;
		double maxDis;
		int max_consultation;
		double **recommend_point;
		double *util_prob1;
		double *util_prob2;
		double gamma;
	};

}