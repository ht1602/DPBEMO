#pragma once
#include "core/individual.h"
#include "algorithm/algorithm.h"
#include "problem/problem.h"
#include "../vendor/armadillo/armadillo"

using namespace arma;

#include <vector>
 
namespace emoc {

	class MOEADRUCB : public Algorithm
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

		MOEADRUCB(int thread_id);
		virtual ~MOEADRUCB();

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
		void StorePreference(Individual **pop);
	private:
		//MOEAD parameters
		double **lambda_;                  // weight vector
		int weight_num_;                   // the number of weight vector
		int **neighbour_;	               // neighbours of each individual
		int neighbour_num_;                // the number of neighbours
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
		int tau_; 								//consultation interval
    	int first_tau;                          //  number of generations in first consecutive consultation.
    	double eta;                             //step size.
    	double sigma;                            // the width of the Gaussian function.
		
		//param for pairwise preference learning
		int replace_num_;  
		int nPromisingWeight;	//num of promising weights
		double step_size_;		//change the weight to what extent
		double *c;				//parameters of utility function
		double *utl;
		mat PVals;
		int select_sum;
		size_t *compare;
	};

}