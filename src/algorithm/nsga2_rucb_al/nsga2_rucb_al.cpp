#include "algorithm/nsga2_rucb_al/nsga2_rucb_al.h"


#include <algorithm>
#include <iostream>
// #include <ctime>
// #include <cmath>
// #include <vector>
#include <random>

#include "core/macro.h"
#include "core/global.h"
#include "core/file.h"
#include "core/nd_sort.h"
#include "operator/tournament_selection.h"
#include "operator/polynomial_mutation.h"
#include "operator/sbx.h"
#include "random/random.h"
#include "core/utility.h"
#include "../vendor/Bandit/stdafx.h"
#include "../vendor/Bandit/RUCB-AL.h"

#include <numeric> //iota

using namespace std;

namespace emoc {

	NSGA2_RUCB_AL::NSGA2_RUCB_AL(int thread_id) :
    Algorithm(thread_id),
    pf_size(0),
	usingPref(false),
	count(0),
	weight(new double[g_GlobalSettings->obj_num_]),
	sigma(0.5),
	utl(new double[g_GlobalSettings->population_num_]),
	select_sum(10),
	compare(new size_t[select_sum]),
	best_index(0),
	KL(0),
	gamma(0.9),
	max_consultation(20),
	util_prob1(new double[g_GlobalSettings->population_num_]),
	util_prob2(new double[g_GlobalSettings->population_num_])
	{

	}

	NSGA2_RUCB_AL::~NSGA2_RUCB_AL()
	{
		// free memory for pf_data
        for (int i = 0; i < pf_size; ++i)
		{
			delete[] pf_data[i];
			pf_data[i] = nullptr;
		}
			
		for(int i = 0; i < max_consultation; i++)
		{
			delete[] recommend_point[i];
			recommend_point[i] = nullptr;
		}
			
		delete[] pf_data;
		delete[] weight;
		delete[] utl;
		delete[] compare;
		delete[] recommend_point;
		delete[] util_prob1;
		delete[] util_prob2;
		pf_data = nullptr;
		weight = nullptr;
		utl = nullptr;
		compare = nullptr;
		recommend_point = nullptr;
		util_prob1 = nullptr;
		util_prob2 = nullptr;

	}

	void NSGA2_RUCB_AL::Solve()
	{
		Initialization();
		int first_tau = 0.6 * g_GlobalSettings->max_evaluation_ / g_GlobalSettings->population_num_;
		
		while (!IsTermination())
		{
			usingPref = false;
					
			// first time to consult, after using 50% evaluation budget
			if(g_GlobalSettings->iteration_num_ >= first_tau && count == 0)
			{
				printf("the %d time to consult in generation %d \n", count + 1, g_GlobalSettings->iteration_num_);
				UtilityFunction_Gaussian(g_GlobalSettings->parent_population_.data());
				StorePreference(g_GlobalSettings->parent_population_.data());
				best_index = Consult_DM();
				printf("the recommend population index is %d\n",compare[best_index]);
				printf("the recommend population is :");
				for(int i = 0; i < g_GlobalSettings->obj_num_; i++)
				{
					cout << g_GlobalSettings->parent_population_[compare[best_index]]->obj_[i];
					recommend_point[count][i] = g_GlobalSettings->parent_population_[compare[best_index]]->obj_[i];
				}
				cout << endl;
				count++;
				// cout << "consult num:" << count << endl;
				usingPref = true;
				
			}
			// double KL = CalculateKLDivergence(g_GlobalSettings->parent_population_.data());
			// printf("KL = %f \n", KL);
			if(count && count < 10)
			{
				double KL = CalculateKLDivergence(g_GlobalSettings->parent_population_.data());
				// printf("KL = %f \n", KL);
				if(KL > 1e-3)
				{
					printf("KL = %f \n", KL);
					printf("the %d time to consult in generation %d\n", count + 1, g_GlobalSettings->iteration_num_);
					UtilityFunction_Gaussian(g_GlobalSettings->parent_population_.data());
					// StorePreference(g_GlobalSettings->parent_population_.data());
					ActiveStorePreference(g_GlobalSettings->parent_population_.data());
					best_index = Consult_DM();
					printf("the recommend population index is %d\n",compare[best_index]);
					printf("the recommend population is :");
					for(int i = 0; i < g_GlobalSettings->obj_num_; i++)
					{
						cout << g_GlobalSettings->parent_population_[compare[best_index]]->obj_[i];
						recommend_point[count][i] = g_GlobalSettings->parent_population_[compare[best_index]]->obj_[i];
					}
					cout << endl;
					count++;
					// cout << "consult num:" << count << endl;
					usingPref = true;
				}
				
			}
			

			
			// generate offspring population
			Crossover(g_GlobalSettings->parent_population_.data(), g_GlobalSettings->offspring_population_.data());
			PolynomialMutation(g_GlobalSettings->offspring_population_.data(), 2 * (real_popnum_ / 2), g_GlobalSettings->dec_lower_bound_, g_GlobalSettings->dec_upper_bound_, mutation_para_);
			EvaluatePop(g_GlobalSettings->offspring_population_.data(), 2 * (real_popnum_ / 2));
			MergePopulation(g_GlobalSettings->parent_population_.data(), real_popnum_, g_GlobalSettings->offspring_population_.data(),
				2 * (real_popnum_ / 2), g_GlobalSettings->mixed_population_.data());
			
			// select next generation's population
			bool envSelect = EnvironmentalSelection(g_GlobalSettings->parent_population_.data(), g_GlobalSettings->mixed_population_.data());
			if(!envSelect)
            {
                std::cerr<<"[ERROR] Wrong in EnvironmentalSelection!\n";
                return;
            }
			// CalculateTotalDis(weight);
		}
		printf("total consultation is %d times\n", count + 1);
		CalculateMinDis(weight);
	}

	void NSGA2_RUCB_AL::Initialization()
	{
		// initialize parent population
		g_GlobalSettings->InitializePopulation(g_GlobalSettings->parent_population_.data(), g_GlobalSettings->population_num_);
		EvaluatePop(g_GlobalSettings->parent_population_.data(), g_GlobalSettings->population_num_);
		
		// initialize the weight for DM
		weight = SetWeight(g_GlobalSettings->weight_StringType_);
		for(int i = 0; i < g_GlobalSettings->obj_num_; i ++)
		{
			printf("%f\n", weight[i]);
		}
        // initialize hv calculator
        hv_calculator_.Init(g_GlobalSettings->obj_num_, g_GlobalSettings->population_num_);
        pf_data = LoadPFData(pf_size, g_GlobalSettings->obj_num_, g_GlobalSettings->problem_name_);
        printf("pf_size:%d\n", pf_size);

		// initialize recommend point
		recommend_point = new double*[max_consultation];
		for(int i = 0; i < max_consultation; i++)
		{
			recommend_point[i] = new double[g_GlobalSettings->obj_num_];
		}

		// initialize probability1 and probability2
		for(int i = 0; i < g_GlobalSettings->population_num_; i++)
		{
			util_prob1[i] = (double)1./g_GlobalSettings->population_num_;
			util_prob2[i] = 0;
		}

		// set mutation parameter
		mutation_para_.pro = 1.0 / g_GlobalSettings->dec_num_;
		mutation_para_.index1 = 20.0;

		// set crossover parameter
		cross_para_.pro = 1.0;
		cross_para_.index1 = 20.0;
	}

	void NSGA2_RUCB_AL::Crossover(Individual **parent_pop, Individual **offspring_pop)
	{
		// generate random permutation index for tournment selection
		std::vector<int> index1(g_GlobalSettings->population_num_);
		std::vector<int> index2(g_GlobalSettings->population_num_);
		random_permutation(index1.data(), g_GlobalSettings->population_num_);
		random_permutation(index2.data(), g_GlobalSettings->population_num_);

		for (int i = 0; i < g_GlobalSettings->population_num_ / 2; ++i)
		{
			Individual *parent1 = TournamentByRank(parent_pop[index1[2 * i]], parent_pop[index1[2 * i + 1]]);
			Individual *parent2 = TournamentByRank(parent_pop[index2[2 * i]], parent_pop[index2[2 * i + 1]]);
			SBX(parent1, parent2, offspring_pop[2 * i], offspring_pop[2 * i + 1],
				g_GlobalSettings->dec_lower_bound_, g_GlobalSettings->dec_upper_bound_, cross_para_);
		}
	}
	
	void NSGA2_RUCB_AL::SetDistanceInfo(std::vector<DistanceInfo> &distanceinfo_vec, int target_index, double distance)
	{
		// search the target_index and set it's distance
		for (int i = 0; i < distanceinfo_vec.size(); ++i)
		{
			if (distanceinfo_vec[i].index == target_index)
			{
				distanceinfo_vec[i].distance += distance;
				break;
			}
		}
	}


	int NSGA2_RUCB_AL::CrowdingDistance(Individual **mixed_pop,  int pop_num, int *pop_sort, int rank_index)
	{
		int num_in_rank = 0;
		std::vector<int> sort_arr(pop_num, 0);
		std::vector<DistanceInfo> distanceinfo_vec(pop_num, { -1,0.0 });

		// find all the indviduals with rank rank_index
		for (int i = 0; i < pop_num; i++)
		{
			mixed_pop[i]->fitness_ = 0;
			if (mixed_pop[i]->rank_ == rank_index)
			{
				distanceinfo_vec[num_in_rank].index = i;
				sort_arr[num_in_rank] = i;
				num_in_rank++;
			}
		}

		for (int i = 0; i < g_GlobalSettings->obj_num_; i++)
		{
			// sort the population with i-th obj
			std::sort(sort_arr.begin(), sort_arr.begin()+num_in_rank, [=](int left, int right){
				return mixed_pop[left]->obj_[i] < mixed_pop[right]->obj_[i];
			});

			// set the first and last individual with INF fitness (crowding distance)
			mixed_pop[sort_arr[0]]->fitness_ = EMOC_INF;
			SetDistanceInfo(distanceinfo_vec, sort_arr[0], EMOC_INF);
			mixed_pop[sort_arr[num_in_rank - 1]]->fitness_ = EMOC_INF;
			SetDistanceInfo(distanceinfo_vec, sort_arr[num_in_rank - 1], EMOC_INF);

			// calculate each solution's crowding distance
			for (int j = 1; j < num_in_rank - 1; j++)
			{
				if (EMOC_INF != mixed_pop[sort_arr[j]]->fitness_)
				{
					if (mixed_pop[sort_arr[num_in_rank - 1]]->obj_[i] == mixed_pop[sort_arr[0]]->obj_[i])
					{
						mixed_pop[sort_arr[j]]->fitness_ += 0;
					}
					else
					{
						double distance = (mixed_pop[sort_arr[j + 1]]->obj_[i] - mixed_pop[sort_arr[j - 1]]->obj_[i]) /
							(mixed_pop[sort_arr[num_in_rank - 1]]->obj_[i] - mixed_pop[sort_arr[0]]->obj_[i]);
						mixed_pop[sort_arr[j]]->fitness_ += distance;
						SetDistanceInfo(distanceinfo_vec, sort_arr[j], distance);
					}
				}
			}
		}

		std::sort(distanceinfo_vec.begin(), distanceinfo_vec.begin()+num_in_rank, [](const DistanceInfo &left, const DistanceInfo &right) {
			return left.distance < right.distance;
		});

		// copy sort result
		for (int i = 0; i < num_in_rank; i++)
		{
			pop_sort[i] = distanceinfo_vec[i].index;
		}

		return num_in_rank;
	}

	bool NSGA2_RUCB_AL::EnvironmentalSelection(Individual **parent_pop, Individual **mixed_pop)
	{
		int current_popnum = 0, rank_index = 0;
		int mixed_popnum = g_GlobalSettings->population_num_ + 2 * (g_GlobalSettings->population_num_ / 2);

		NonDominatedSort(mixed_pop, mixed_popnum, g_GlobalSettings->obj_num_);

		// select individuals by rank
		while (1)
		{
			int temp_number = 0;
			for (int i = 0; i < mixed_popnum; i++)
			{
				if (mixed_pop[i]->rank_ == rank_index)
				{
					temp_number++;
				}
			}
			if (current_popnum + temp_number <= g_GlobalSettings->population_num_)
			{
				for (int i = 0; i < mixed_popnum; i++)
				{
					if (mixed_pop[i]->rank_ == rank_index)
					{
						CopyIndividual(mixed_pop[i], parent_pop[current_popnum]);
						current_popnum++;
					}
				}
				rank_index++;
			}
			else
				break;
		}

		// select individuals by crowding distance
		int sort_num = 0;
		std::vector<int> pop_sort(mixed_popnum);

		if (current_popnum < g_GlobalSettings->population_num_)
		{
			if(!usingPref)
			{
				// sort_num = CrowdingDistance(mixed_pop, mixed_popnum, pop_sort.data(), rank_index);
				if(!count)
					sort_num = CrowdingDistance(mixed_pop, mixed_popnum, pop_sort.data(), rank_index);
				else
				{
					// printf("already consult but not in this round\n");
					sort_num = FitnessViaPreference(mixed_pop, mixed_popnum, pop_sort.data(), rank_index, g_GlobalSettings->population_num_ - current_popnum);
					if (sort_num == -1)
					{
						return false;
					}
				}
			}
			else
			{
				sort_num = FitnessViaPreference(mixed_pop, mixed_popnum, pop_sort.data(), rank_index, g_GlobalSettings->population_num_ - current_popnum);
				if (sort_num == -1)
                {
                    return false;
                }
			}
				
			
			while (1)
			{
				if (current_popnum < g_GlobalSettings->population_num_)
				{
					// int index = pop_sort[sort_num - 1];
					CopyIndividual(mixed_pop[pop_sort[--sort_num]], parent_pop[current_popnum]);
					current_popnum++;
				}
				else {
					break;
				}
			}
		}

		// clear crowding distance value
		for (int i = 0; i < g_GlobalSettings->population_num_; i++)
		{
			parent_pop[i]->fitness_ = 0;
		}
		
		pop_sort.clear();

		return true;
	}


	// get the utility function value of all solutions
    void  NSGA2_RUCB_AL::UtilityFunction_Gaussian(Individual **pop)
    {
        double **f = new double*[g_GlobalSettings->population_num_];
        double *w = new double[g_GlobalSettings->obj_num_];
        double max = 0.0;
        int max_index = 0;
        // set weight for utility in range 0.4~0.6
        std::random_device rd;  // generate seed
        std::mt19937 mt(rd());  // use seed initialize generator
        std::uniform_real_distribution<double> prob(0.4,0.6);
        for(int i = 0; i < g_GlobalSettings->obj_num_; i++)
        {
            w[i] = prob(mt);
            // printf("w[%d]=%f\n",i,w[i]);
        }
        for(int i = 0; i < g_GlobalSettings->population_num_; i++)
        {
            f[i] = new double[g_GlobalSettings->obj_num_];
        }

        for(int i = 0; i < g_GlobalSettings->population_num_; i++)
        {
            for(int j = 0;j < g_GlobalSettings->obj_num_; j++)
            {
                // std::default_random_engine generator;
                // std::normal_distribution<double> distribution(weight[j], 0.5);
                // f[i][j] = pow(c[j] * pop[i]->obj_[j] - 1, 3);
                f[i][j] =  1 / (sigma * sqrt(2*PI)) * exp((double)-1/2* pow ((pop[i]->obj_[j] - weight[j]) / sigma, 2));

            }
        }

        for(int i = 0;i < g_GlobalSettings->population_num_; i++)
        {
            // cout << "i" << i << endl;
            utl[i] = 0;
            for(int j = 0;j < g_GlobalSettings->obj_num_; j++)
            {
                utl[i] += w[j] * f[i][j];
            }
            // cout << "utility" << utl[i] << endl;
            if(utl[i] > max)
            {
                max = utl[i];
                max_index = i;
                // cout << "max = " << max ;
                // cout << "max_index:" <<max_index << endl;
            }
            // if(g_GlobalSettings->obj_num_ == 3)
            //     utl[i] *= 1e6;
        }
        cout << "max = " << max ;
        cout << "max_index:" <<max_index << endl;
        printf("the best point is :\n");
        for(int i = 0; i < g_GlobalSettings->obj_num_; i++)
        {
            printf("%.2f\n", pop[max_index]->obj_[i]);   
        }

        for(int i = 0;i < g_GlobalSettings->population_num_; i++)
        {
            delete[] f[i];
            f[i] = nullptr;
        }
        delete[] f;
        delete[] w;
        f = nullptr;
        w = nullptr;
    }

	// uniformly choose 10 compare objects, and store there utility function difference in a dat file
    void NSGA2_RUCB_AL::StorePreference(Individual **pop)
    {
        
        std::vector<size_t>index(g_GlobalSettings->population_num_);
        // std::vector<size_t>compare(select_sum);
        iota(index.begin(),index.end(),0);
        std::sort(index.begin(),index.end(),
            [&pop](size_t index1, size_t index2){return pop[index1]->obj_[0] > pop[index2]->obj_[0];});
        
        // printf("store index:\n");
        

        int select_interval = ceil(g_GlobalSettings->population_num_ / select_sum);
        int selected = 0;

        for(int i = 0; i < g_GlobalSettings->population_num_; i++)
        {
            
            if(i % select_interval == 0 && selected < select_sum)
            {
                // printf("%d\n", index[i] );
                compare[selected] = i;
                selected ++;
                // printf("selected %d\n", selected);
            }
        }
        PVals = mat(select_sum, select_sum);
        for(int i = 0; i < select_sum; i++)
        {
            for(int j = 0;j < select_sum; j++)
            {
                double diff = utl[compare[i]] - utl[compare[j]];
                // printf("difference between %d and %d is %f\n", compare[i], compare[j], diff);                
                PVals(i, j) = 1 / (1 + exp(-1000* diff));
                // std::cout << PVals(i,j) << std::endl;
            }
        }
        PVals.save("compare.dat",raw_ascii);
    }

	void NSGA2_RUCB_AL::ActiveStorePreference(Individual **pop)
	{
		int selected = 0;
		std::vector<int> selected_index(count+1);
		std::vector<size_t>index(g_GlobalSettings->population_num_);
        iota(index.begin(),index.end(),0);
        std::sort(index.begin(),index.end(),
            [&pop](size_t index1, size_t index2){return pop[index1]->obj_[0] > pop[index2]->obj_[0];});
		
        bool flag[g_GlobalSettings->population_num_];
		// true denote this solution can be chosen as comparison item, false denotes already selected
        memset(flag,true,g_GlobalSettings->population_num_*sizeof(bool));
		
		// choose the least certain point
		int least_certain_index;
		double max_difference = -EMOC_INF;
		
		for(int i = 0; i < g_GlobalSettings->population_num_; i++)
		{
			if(abs(util_prob1[i] - util_prob2[i]) > max_difference)
			{
				max_difference = abs(util_prob1[i] - util_prob2[i]);
				least_certain_index = i;
				
			}
		}
		selected ++;   
		flag[index[least_certain_index]] = false;
		cout << least_certain_index << " lest certain index " << index[least_certain_index] << endl;
		compare[0] = index[least_certain_index];
		selected_index[0] = index[least_certain_index];
		std::vector<int> preferred_index(count);
		// choose the most preferred point
		double min_dist;
		int most_preferred_index;
		for(int i = 0; i < count; i ++)
		{
			min_dist = EMOC_INF;
			double temp;
			for(int j = 0; j < g_GlobalSettings->population_num_; j++)
			{
				temp = CalEuclidianDistance(g_GlobalSettings->parent_population_[index[j]]->obj_.data(), recommend_point[i], g_GlobalSettings->obj_num_);
				// cout << "j" << j ;
				// cout << " index" << index[j];
				// cout << " closest point" << g_GlobalSettings->parent_population_[index[j]]->obj_[0] << " " << g_GlobalSettings->parent_population_[index[j]]->obj_[1];
				// cout << "cur dist" << temp << endl;
				// find the closest point which hasen't been chosen yet
				if( temp < min_dist && flag[index[j]] == true)
				{
					min_dist = temp;
					most_preferred_index = j;
				}
				// cout << " min dist" << min_dist << endl;
			}
			flag[index[most_preferred_index]] = false;
			selected ++;
			preferred_index[i] = most_preferred_index;
			selected_index[i+1] = index[most_preferred_index];
			compare[i+1] = index[most_preferred_index];
			
			// cout << most_preferred_index << " select prefer index" << index[most_preferred_index] << endl;
			// cout << "closest point" << g_GlobalSettings->parent_population_[index[most_preferred_index]]->obj_[0] << g_GlobalSettings->parent_population_[index[most_preferred_index]]->obj_[1] << endl;
		}

        
        PVals = mat(selected, selected);

        for(int i = 0; i < selected; i++)
        {
            for(int j = 0;j < selected; j++)
            {
                double diff = utl[compare[i]] - utl[compare[j]];
                // printf("difference between %d and %d is %f\n", compare[i], compare[j], diff);                
                PVals(i, j) = 1 / (1 + exp(-1000 * diff));
                // std::cout << PVals(i,j) << std::endl;
            }
        }
        PVals.save("compare.dat",raw_ascii);
	}


	int NSGA2_RUCB_AL::Consult_DM()
    {
        mat PVals;
        int best;
        PVals.load("compare.dat");

        double learning_rate= 0.51;
        int T_round = 1000;
        int Budget = 40;
        double kappa = 1.0;
        // Run run(PVals, learning_rate);
        RunAL run(PVals,learning_rate, Budget,kappa);
        run.timestep(T_round);
        // for(int i = 0; i < T_round ; i++)
        // {
        //     // cout << "the " << i+1 << " round" << endl;
        //     run.timestep();

        //     // cout << run << endl;
            
        //     // best = run.winner();
            
        // }
        best = run.winner();
        printf("recommend index is %d\n", best);
        return best;
    }

	int NSGA2_RUCB_AL::FitnessViaPreference(Individual **mixed_pop, int pop_num, int *pop_sort, int rank_index, int nneed)
	{
		int num_in_rank = 0;
		std::vector<int> sort_arr(pop_num, 0);
		std::vector<DistanceInfo> distanceinfo_vec(pop_num, {-1, 0.0});

		// find all the individuals with rank rank_index
		for(int i = 0; i < pop_num; i++)
		{
			mixed_pop[i]->fitness_ = 0;
			if(mixed_pop[i]->rank_ == rank_index)
			{
				distanceinfo_vec[num_in_rank].index = i;
				sort_arr[num_in_rank] = i;
				num_in_rank ++;
			}
		}
		for(int i = 0; i < g_GlobalSettings->obj_num_; i++)
		{
			std::sort(sort_arr.begin(), sort_arr.end(),[=](int left, int right){
				return mixed_pop[left]->obj_[i] < mixed_pop[right]->obj_[i];
			});
			
			// assign the border fitness to 0 
			mixed_pop[sort_arr[0]]->fitness_ = 0;
			SetDistanceInfo(distanceinfo_vec, sort_arr[0], 0);
			mixed_pop[sort_arr[num_in_rank - 1]]->fitness_ = 0;
			SetDistanceInfo(distanceinfo_vec, sort_arr[num_in_rank - 1], 0);
			
			// assign all distance according to predicted utility distribution
			for(int j = 0; j < num_in_rank - 1; j++)
			{
				double distance = UpdateUtilityPrediction(mixed_pop[sort_arr[j]]->obj_[i], i);
				mixed_pop[sort_arr[j]]->fitness_ += distance;
				SetDistanceInfo(distanceinfo_vec, sort_arr[j], distance);
			}
		}
		std::sort(distanceinfo_vec.begin(), distanceinfo_vec.begin()+num_in_rank, [](const DistanceInfo &left, const DistanceInfo &right){
			return left.distance < right.distance;
		});

		// copy result to pop_sort
		for(int i = 0; i < pop_num; i++)
		{
			pop_sort[i] = distanceinfo_vec[i].index;
		}
		return num_in_rank;

	}

	double NSGA2_RUCB_AL::UpdateUtilityPrediction(double obj, int index)
	{
		double probability = 0.0;
		// U = gamma * U + u_c
		for(int i = 0; i < count; i ++)
		{
			double u = 0;
			// for(int j = 0; j < g_GlobalSettings->obj_num_; j ++)
			// {
			// 	 u += 1 / (sigma * sqrt(2*PI)) * exp((double)-1/2* pow ((obj - recommend_point[i][j]) / sigma, 2));
			// }
			u = 1.0 / (sigma * sqrt(2*PI)) * exp((double)-1/2* pow ((obj - recommend_point[i][index]) / sigma, 2));
			probability = probability * gamma + u;
		}
		return probability;
	}

	float NSGA2_RUCB_AL::CalculateKLDivergence(Individual **pop)
	{
		// reinitialize util_prob2 to 0
		for(int i = 0; i < g_GlobalSettings->population_num_; i++)
		{
			util_prob2[i] = 0;
		}
		// sort current population descendently according to obj0
		std::vector<size_t>index(g_GlobalSettings->population_num_);
        iota(index.begin(),index.end(),0);
        std::sort(index.begin(),index.end(),
            [&pop](size_t index1, size_t index2){return pop[index1]->obj_[0] > pop[index2]->obj_[0];});

		// calculate util_prob2 according to updated utility distribution
		for(int i = 0 ; i < g_GlobalSettings->population_num_; i++)
		{
			for(int j = 0; j < g_GlobalSettings->obj_num_; j++)
			{
				util_prob2[i] += UpdateUtilityPrediction(g_GlobalSettings->parent_population_[index[i]]->obj_[j], j);
			}
		}

		// normalize util_prob2
		double max_prob = 0;
		double min_prob = EMOC_INF;
		double sum = 0.0;
		for(int i = 0; i < g_GlobalSettings->population_num_; i++)
		{
			sum += util_prob2[i];
		}

		for(int i = 0; i < g_GlobalSettings->population_num_; i++)
		{
			util_prob2[i] = util_prob2[i] / sum;
		}
		
	

		// calculate KL divergence between util_prob1 and util_prob2
		float KLDivergence = 0.0;
		for(int i = 0; i < g_GlobalSettings->population_num_; i++)
		{
			// if(abs(util_prob2[i] - util_prob1[i]) < 0.1)
			if(util_prob2[i] == 0.0)
				util_prob2[i] += 1e-6;
			
			
			KLDivergence += util_prob2[i] * log(util_prob2[i] / util_prob1[i]);
		
		}

		// store util_prob2 value in util_prob1 for next calculation
		for(int i = 0; i < g_GlobalSettings->population_num_; i++)
		{
			util_prob1[i] = util_prob2[i];
		}

		return KLDivergence;
	}

	void NSGA2_RUCB_AL::CalculateMinDis(double *best)
    {
        double min_dis = 100;
        double dis;
        for(int i = 0; i < g_GlobalSettings->population_num_;++i)
        {
            dis = CalEuclidianDistance(best,g_GlobalSettings->parent_population_[i]->obj_.data(),g_GlobalSettings->obj_num_);
            // std::cout<<i<<":"<<dis[i]<<endl;
            // printf("current dis%f\n",dis);
            if(dis < min_dis)
            {
                min_dis = dis;
            }
        }
        printf("min dis %lf\n", min_dis);
        char output_dir[1024];
        char output_file[1024];

        std::string problem_name(g_GlobalSettings->problem_name_);
        std::string algorithm_name(g_GlobalSettings->algorithm_name_);

        sprintf(output_dir,"./MinDis/%s/",
                g_GlobalSettings->algorithm_name_.c_str()
                );
        CreateDirectory(output_dir);

        sprintf(output_file, "%s/%s_M%d_D%d.txt", 
                output_dir, 
                g_GlobalSettings->problem_name_.c_str(),
                g_GlobalSettings->obj_num_,
                g_GlobalSettings->dec_num_
                );

        FILE *fpt = fopen(output_file, "a");
		
		if(fpt == nullptr)
		{
			std::cout <<"Can not open "<< output_file << " !." << std::endl;
			std::cout << "Press enter to exit" << std::endl;
			std::cout << strerror(errno) << "\n";
			std::cin.get();
			exit(-1);
		}

		
       
        fprintf(fpt, "%lf", min_dis);
        fprintf(fpt, "\n");

		fflush(fpt);
		fclose(fpt);

    }

	void NSGA2_RUCB_AL::CalculateTotalDis(double *best)
	{
		// double min_dis = 100;
        double dis,totaldis;
		totaldis = 0.0;
        for(int i = 0; i < g_GlobalSettings->population_num_;++i)
        {
            dis = CalEuclidianDistance(best,g_GlobalSettings->parent_population_[i]->obj_.data(),g_GlobalSettings->obj_num_);
            totaldis += dis;
        }
        
		printf("total dis %lf\n", totaldis);
        char output_dir[1024];
        char output_file[1024];

        std::string problem_name(g_GlobalSettings->problem_name_);
        std::string algorithm_name(g_GlobalSettings->algorithm_name_);

        sprintf(output_dir,"./TotalDis/%s/%d/",
                g_GlobalSettings->algorithm_name_.c_str(),
				g_GlobalSettings->run_id_
                );
        CreateDirectory(output_dir);

        sprintf(output_file, "%s/%s_M%d_D%d.txt", 
                output_dir, 
                g_GlobalSettings->problem_name_.c_str(),
                g_GlobalSettings->obj_num_,
                g_GlobalSettings->dec_num_
                );

        FILE *fpt = fopen(output_file, "a");
		
		if(fpt == nullptr)
		{
			std::cout <<"Can not open "<< output_file << " !." << std::endl;
			std::cout << "Press enter to exit" << std::endl;
			std::cout << strerror(errno) << "\n";
			std::cin.get();
			exit(-1);
		}

		
       
        fprintf(fpt, "%lf", totaldis);
        fprintf(fpt, "\n");

		fflush(fpt);
		fclose(fpt);
	}

	static void PrintObjective(const char *filename, int obj_num, double *best, int pop_num)
	{
		FILE *fpt = fopen(filename, "w");
		
		if(fpt == nullptr)
		{
			std::cout <<"Can not open "<< filename << " !." << std::endl;
			std::cout << "Press enter to exit" << std::endl;
			std::cout << strerror(errno) << "\n";
			std::cin.get();
			exit(-1);
		}

		
        for(int i = 0; i < obj_num; ++i)
        {
            fprintf(fpt, "%lf\t", best[i]);
        }
        fprintf(fpt, "\n");

		fflush(fpt);
		fclose(fpt);
	}

	static void PrintDecision(const char* filename, int dec_num, double* best, int pop_num)
	{
		FILE* fpt = fopen(filename, "w");

		if (fpt == nullptr)
		{
			std::cout << "Can not open " << filename << " !." << std::endl;
			std::cout << "Press enter to exit" << std::endl;
			std::cout << strerror(errno) << "\n";
			std::cin.get();
			exit(-1);
		}

        for(int i = 0; i < dec_num; ++i)
        {
            fprintf(fpt, "%lf\t", best[i]);
        }
        fprintf(fpt, "\n");

		fflush(fpt);
		fclose(fpt);
	}

}