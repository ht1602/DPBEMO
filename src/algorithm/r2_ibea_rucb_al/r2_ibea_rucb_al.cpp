#include "algorithm/r2_ibea_rucb_al/r2_ibea_rucb_al.h"

#include <cmath>
#include <ctime>
#include <iostream>
#include <algorithm>

#include "core/file.h"
#include "core/global.h"
#include "operator/tournament_selection.h"
#include "operator/sbx.h"
#include "operator/polynomial_mutation.h"
#include "random/random.h"
#include "core/utility.h"
#include "core/uniform_point.h"

#include <numeric>  //iota
#include "../vendor/Bandit/stdafx.h"
#include "../vendor/Bandit/RUCB-AL.h"
using namespace std;

namespace emoc {


	R2_IBEA_RUCB_AL::R2_IBEA_RUCB_AL(int thread_id) :
    Algorithm(thread_id),
    kappa(0.05),
    ideal_point(new double[g_GlobalSettings->obj_num_]),
    lambda_(nullptr),
    weight(nullptr),
    step_size_(0.2),
    retention_rate_(0.2),
    score_RankNet_(new double[g_GlobalSettings->population_num_]),
    sigma(0.5),
    utl(new double[g_GlobalSettings->population_num_]),
    select_sum(10),
    compare(new size_t[select_sum]),
    max_consultation(10),
    nPromisingWeight(10),
    util_prob1(new double[g_GlobalSettings->population_num_]),
	util_prob2(new double[g_GlobalSettings->population_num_]),
    gamma(0.9),
    best_index(0),
    count(0)
	{
		real_popnum_ = g_GlobalSettings->population_num_;
	}

	R2_IBEA_RUCB_AL::~R2_IBEA_RUCB_AL()
	{
        for(int i = 0; i < g_GlobalSettings->population_num_; i++)
        {
            delete[] lambda_[i];
            lambda_[i] = nullptr;
        }
        for(int i = 0; i < max_consultation; ++i)
        {
            delete[] recommend_point[i];
            recommend_point[i] = nullptr;
        }

        delete[] lambda_;
        delete[] ideal_point;
        delete[] score_RankNet_;
        delete[] weight;
        delete[] utl;
        delete[] compare;
        delete[] recommend_point;
        delete[] util_prob1;
        delete[] util_prob2;
        lambda_ = nullptr;
        ideal_point = nullptr;
        score_RankNet_ = nullptr;
        weight = nullptr;
        utl = nullptr;
        compare = nullptr;
        recommend_point = nullptr;
        util_prob1 = nullptr;
        util_prob2 = nullptr;

        while (winners_.size())
        {
            delete[] winners_.front();
            winners_.pop();
            delete[] losers_.front();
            losers_.pop();
        }
        

	}

	void R2_IBEA_RUCB_AL::Solve()
	{
		Initialization();
        int first_tau = 0.5 * g_GlobalSettings->max_evaluation_ / g_GlobalSettings->population_num_;
        char usingPref = false;

		while (!IsTermination())
		{
            if(count == 0 && g_GlobalSettings->iteration_num_ >= first_tau)
            {
                printf("the %d time to consult in generation %d \n", ++count, g_GlobalSettings->iteration_num_);
                UtilityFunction_Gaussian(g_GlobalSettings->parent_population_.data());
                StorePreference(g_GlobalSettings->parent_population_.data());

                best_index = Consult_DM();
                printf("the recommend population index is %d\n", compare[best_index]);
                printf("the recommend population is :");
                for(int i = 0; i < g_GlobalSettings->obj_num_; i++)
                {
                    cout << g_GlobalSettings->parent_population_[compare[best_index]]->obj_[i];
					recommend_point[count][i] = g_GlobalSettings->parent_population_[compare[best_index]]->obj_[i];
                }
                cout << endl;
				// count++;
                usingPref = true;

                SetBiasedWeight(g_GlobalSettings->parent_population_[compare[best_index]]->obj_.data());
                // cout << "lambda" << endl;
                // for(int i = 0; i < weight_num_; i++)
                // {
                //     for(int j = 0; j < g_GlobalSettings->obj_num_; j++)
                //     {
                //         cout << lambda_[i][j];
                //     }
                //     cout << endl;

                // }
            }
            if(count && count < max_consultation)
            {
                double KL = CalculateKLDivergence(g_GlobalSettings->parent_population_.data());
                if(KL > 1e-2)
                {
                    printf("KL = %f \n", KL);
					printf("the %d time to consult in generation %d\n", ++count, g_GlobalSettings->iteration_num_);
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
					// count++;
                    // cout << "recommend weight" << endl;
                    // cout << lambda_[compare[best_index]][0] << lambda_[compare[best_index]][1] << endl;
                    usingPref = true;

                    SetBiasedWeight(g_GlobalSettings->parent_population_[compare[best_index]]->obj_.data());
                    // cout << "lambda" << endl;
                    // for(int i = 0; i < weight_num_; i++)
                    // {
                    //     for(int j = 0; j < g_GlobalSettings->obj_num_; j++)
                    //     {
                    //         cout << lambda_[i][j];
                    //     }
                    //     cout << endl;

                    // }
                    
                    // UpdateNeighbour();
                }
            }
            
			// generate offspring population
			Crossover(g_GlobalSettings->parent_population_.data(), g_GlobalSettings->offspring_population_.data());
			PolynomialMutation(g_GlobalSettings->offspring_population_.data(), 2 * g_GlobalSettings->population_num_ / 2, g_GlobalSettings->dec_lower_bound_, g_GlobalSettings->dec_upper_bound_, mutation_para_);
			EvaluatePop(g_GlobalSettings->offspring_population_.data(), 2 * g_GlobalSettings->population_num_ / 2);
			MergePopulation(g_GlobalSettings->parent_population_.data(), g_GlobalSettings->population_num_, g_GlobalSettings->offspring_population_.data(),
				2 * g_GlobalSettings->population_num_ / 2, g_GlobalSettings->mixed_population_.data());
			
            // update ideal point
            UpdateIdealpoint(g_GlobalSettings->offspring_population_.data(),g_GlobalSettings->population_num_, ideal_point, g_GlobalSettings-> obj_num_);


			// select next generation's population
			EnvironmentalSelection(g_GlobalSettings->parent_population_.data(), g_GlobalSettings->mixed_population_.data());
		}
        // cout << lambda_[0][0] << lambda_[0][1] << endl;
        printf("total consultation is %d times\n", count);
        CalculateMinDis(weight, g_GlobalSettings->parent_population_.data(), g_GlobalSettings->population_num_, g_GlobalSettings->obj_num_, g_GlobalSettings->dec_num_, g_GlobalSettings->problem_name_, g_GlobalSettings->algorithm_name_);
        CalculateExpectDis(weight, g_GlobalSettings->parent_population_.data(), g_GlobalSettings->population_num_, g_GlobalSettings->obj_num_, g_GlobalSettings->dec_num_, g_GlobalSettings->problem_name_, g_GlobalSettings->algorithm_name_);
    }

	void R2_IBEA_RUCB_AL::Initialization()
	{
        weight_num_ = g_GlobalSettings->population_num_;
		// initialize parent population
		g_GlobalSettings->InitializePopulation(g_GlobalSettings->parent_population_.data(), g_GlobalSettings->population_num_);
		EvaluatePop(g_GlobalSettings->parent_population_.data(), g_GlobalSettings->population_num_);

        weight = SetWeight(g_GlobalSettings->weight_StringType_);

        // initialize recommend point
		recommend_point = new double*[max_consultation];
		for(int i = 0; i < max_consultation; i++)
		{
			recommend_point[i] = new double[g_GlobalSettings->obj_num_];
		}
        
        // char file[256];
        // sprintf(file,"./UniformWeights/%dd_%d.txt",g_GlobalSettings->obj_num_,g_GlobalSettings->population_num_);
        // lambda_=LoadUniformWeights(g_GlobalSettings->population_num_,g_GlobalSettings->obj_num_,file);
        lambda_=UniformPoint(g_GlobalSettings->population_num_, &weight_num_,g_GlobalSettings->obj_num_);


        UpdateIdealpoint(g_GlobalSettings->parent_population_.data(), g_GlobalSettings->population_num_, ideal_point, g_GlobalSettings->obj_num_);

		// set mutation parameter
		mutation_para_.pro = 1.0 / g_GlobalSettings->dec_num_;
		mutation_para_.index1 = 20.0;

		// set crossover parameter
		cross_para_.pro = 1.0;
		cross_para_.index1 = 20.0;
	}

	void R2_IBEA_RUCB_AL::Crossover(Individual **parent_pop, Individual **offspring_pop)
	{
		// generate random permutation index for tournment selection
		std::vector<int> index1(g_GlobalSettings->population_num_);
		std::vector<int> index2(g_GlobalSettings->population_num_);
		random_permutation(index1.data(), g_GlobalSettings->population_num_);
		random_permutation(index2.data(), g_GlobalSettings->population_num_);

		for (int i = 0; i < g_GlobalSettings->population_num_ / 2; ++i)
		{
			// Individual *parent1 = TournamentByFitness(parent_pop[index1[2 * i]], parent_pop[index1[2 * i + 1]]);
			// Individual *parent2 = TournamentByFitness(parent_pop[index2[2 * i]], parent_pop[index2[2 * i + 1]]);
            Individual *parent1 = TournamentByBiR2Indicator(parent_pop[index1[2 * i]], parent_pop[index1[2 * i + 1]]);
            Individual *parent2 = TournamentByBiR2Indicator(parent_pop[index2[2 * i]], parent_pop[index2[2 * i + 1]]);
			SBX(parent1, parent2, offspring_pop[2 * i], offspring_pop[2 * i + 1],
				g_GlobalSettings->dec_lower_bound_, g_GlobalSettings->dec_upper_bound_, cross_para_);
		}
    
	}

	double R2_IBEA_RUCB_AL::CalEpsIndicator(Individual *ind1, Individual *ind2)
	{
		double max_eps = 0.0, temp_eps = 0.0;
		double r = g_GlobalSettings->dec_upper_bound_[0] - g_GlobalSettings->dec_lower_bound_[0];

		max_eps = (ind1->obj_[0] - g_GlobalSettings->dec_upper_bound_[0]) / r - (ind2->obj_[0] - g_GlobalSettings->dec_upper_bound_[0]) / r;
		for (int i = 1; i < g_GlobalSettings->obj_num_; ++i)
		{
			r = g_GlobalSettings->dec_upper_bound_[i] - g_GlobalSettings->dec_lower_bound_[1];
			temp_eps = (ind1->obj_[i] - g_GlobalSettings->dec_upper_bound_[i]) / r - (ind2->obj_[i] - g_GlobalSettings->dec_upper_bound_[i]) / r;

			if (temp_eps > max_eps)
				max_eps = temp_eps;
		}

		return max_eps;
	}

	void R2_IBEA_RUCB_AL::CalFitness(Individual **pop, int pop_num, std::vector<double> &fitness)
	{
		// determine indicator values and their maximum
		double max_fitness = 0;
		for (int i = 0; i < pop_num; ++i)
		{
			for (int j = 0; j < pop_num; ++j)
			{
				// fitness[i * pop_num + j] = CalEpsIndicator(pop[i], pop[j]);
                fitness[i * pop_num + j] = CalBiR2Indicator(pop[i], pop[j]);
				if (max_fitness < fabs(fitness[i * pop_num + j]))
					max_fitness = fabs(fitness[i * pop_num + j]);
			}
		}

		// calculate for each pair of individuals the corresponding value component
		for (int i = 0; i < pop_num;++i)
			for (int j = 0; j < pop_num; j++)
				fitness[i * pop_num + j] = exp((-fitness[i * pop_num + j] / max_fitness) / kappa);

		// set individual's fitness
		for (int i = 0; i < pop_num; ++i)
		{
			double sum = 0;
			for (int j = 0; j < pop_num; ++j)
				if (i != j)
					sum += fitness[j * pop_num + i];

			pop[i]->fitness_ = sum;
		}
	}

	void R2_IBEA_RUCB_AL::EnvironmentalSelection(Individual **parent_pop, Individual **mixed_pop)
	{
		int mixed_popnum = g_GlobalSettings->population_num_ + 2 * g_GlobalSettings->population_num_ / 2;

		// calculate fitness and store it in fitness array
		std::vector<double> fitness(mixed_popnum * mixed_popnum);
		CalFitness(mixed_pop, mixed_popnum, fitness);

		// select next generation's individuals
		int worst = -1;
		std::vector<int> flag(mixed_popnum,0);

		for (int i = g_GlobalSettings->population_num_; i > 0; --i)
		{
			int j = 0;
			for (j = 0; j < mixed_popnum && flag[j] == 1; ++j);
			worst = j;

			for (j = j + 1; j < mixed_popnum; ++j)
			{
				if (flag[j] != 1)
				{
					if (mixed_pop[j]->fitness_ > mixed_pop[worst]->fitness_)
						worst = j;
				}
			}

			for (j = 0; j < mixed_popnum; ++j)
			{
				if (flag[j] != 1)
				{
					mixed_pop[j]->fitness_ -= fitness[worst * mixed_popnum + j];
				}
			}

			flag[worst] = 1;
		}

		int count = 0;
		for (int i = 0; i < mixed_popnum; ++i)
		{
			if (flag[i] != 1)
				CopyIndividual(mixed_pop[i], parent_pop[count++]);
		}
	}

    double R2_IBEA_RUCB_AL::CalR2Indicator(Individual *x)
    {
        double res = 0;
        double a, ChebycheffValue;
        for (int i = 0; i < g_GlobalSettings->population_num_; ++i)
        {
            ChebycheffValue = -INF;
            for (int j = 0; j < g_GlobalSettings->obj_num_; ++j)
            {
                a = fabs(ideal_point[j] - x->obj_[j]) / lambda_[i][j];
                if (a > ChebycheffValue)
                {
                    ChebycheffValue = a;
                }
            }
            res += ChebycheffValue;
        }
        res /= g_GlobalSettings->population_num_;
        return res;
    }

    double R2_IBEA_RUCB_AL::CalR2Indicator(Individual *x, Individual *y)
    {
        double res = 0;
        double a, b, ChebycheffValue_x, ChebycheffValue_y;
        for (int i = 0; i < g_GlobalSettings->population_num_; ++i)
        {
            ChebycheffValue_x = -INF;
            ChebycheffValue_y = -INF;
            for (int j = 0; j < g_GlobalSettings->obj_num_; ++j)
            {
                a = fabs(ideal_point[j] - x->obj_[j]) / lambda_[i][j];
                b = fabs(ideal_point[j] - y->obj_[j]) / lambda_[i][j];
                if (a > ChebycheffValue_x)
                {
                    ChebycheffValue_x = a;
                }
                if (b > ChebycheffValue_y)
                {
                    ChebycheffValue_y = b;
                }
            }
            if (ChebycheffValue_x < ChebycheffValue_y)
            {
                res += ChebycheffValue_x;
            }
            else
            {
                res += ChebycheffValue_y;
            }
        }
        res /= g_GlobalSettings->population_num_;
        return res;
    }

    double R2_IBEA_RUCB_AL::CalBiR2Indicator(Individual *x, Individual *y)
    {
        return CalR2Indicator(x) - CalR2Indicator(x,y);
    }

    Individual* R2_IBEA_RUCB_AL::TournamentByBiR2Indicator(Individual *ind1, Individual *ind2)
    {
        double I_R2_1,I_R2_2;
        I_R2_1 = CalBiR2Indicator(ind1, ind2);
        I_R2_2 = CalBiR2Indicator(ind2, ind1);
        /*if (I_R2_1 < I_R2_2)
        {
            #if DEBUG
            std::cout<<"[INFO] Select parent 1."<<std::endl;
            #endif
            return ind1;
        }
        else if(I_R2_1 > I_R2_2)
        {
            #if DEBUG
            std::cout<<"[INFO] Select parent 2."<<std::endl;
            #endif
            return ind2;
        }*/
        if (I_R2_1 == 0)
        {
            return ind1;
        }
        else if(I_R2_2 == 0)
        {
            return ind2;
        }
        else
        {
            #if DEBUG
            std::cout<<"[INFO] Select parent randomly."<<std::endl;
            #endif
            if (randomperc() <= 0.5)
            {
                return (ind1);
            }
            else
            {
                return (ind2);
            }
        }
    }

    // get the utility function value of all solutions
    void  R2_IBEA_RUCB_AL::UtilityFunction_Gaussian(Individual **pop)
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
    void R2_IBEA_RUCB_AL::StorePreference(Individual **pop)
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
                PVals(i, j) = 1 / (1 + exp(-1000 * diff));
                // std::cout << PVals(i,j) << std::endl;
            }
        }
        PVals.save("compare.dat",raw_ascii);
    }

    void R2_IBEA_RUCB_AL::ActiveStorePreference(Individual **pop)
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


    // consult the RUCB dueling bandit and return the best point index
    int R2_IBEA_RUCB_AL::Consult_DM()
    {
        // mat PVals;
        int best;
        PVals.load("compare.dat");

        double learning_rate = 0.51;
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

    // arrange the weight vectors uniformally to the 10 best weight
    void R2_IBEA_RUCB_AL::SetBiasedWeight(double *best)
    {
        std::vector<double>dis(weight_num_);
        for(int i = 0; i < weight_num_; ++i)
        {
            dis[i] = CalEuclidianDistance(best,lambda_[i],g_GlobalSettings->obj_num_);
            // std::cout<<i<<":"<<dis[i]<<endl;
        }
        std::vector<size_t>index(dis.size());
        iota(index.begin(),index.end(),0);
        sort(index.begin(),index.end(),
            [&dis](size_t index_1,size_t index_2){return dis[index_1]<dis[index_2];});
        //find the 10 best lambda_ according to our best
        //the 10 best are index[0]~index[9]

        bool flag[weight_num_];
        memset(flag, true, weight_num_ * sizeof(bool));
        for(int i = 0; i < nPromisingWeight; ++i)
        {
            flag[index[i]] = false;//10 best need no change
            // std::cout<<index[i]<<endl;
        }
        int nSolveNum = 0;//weights have been changed
        double tempdis,minDis;
        int near_index;
        nSolveNum = nPromisingWeight;
        int nMaxChange = ceil((double)(weight_num_ - nPromisingWeight) / nPromisingWeight);
        // std::cout<<nSolveNum<<"  "<<nMaxChange<<endl;
        for(int i = 0; i < nPromisingWeight && nSolveNum < weight_num_; ++i)
        {
            int nCurrentTuned = 0;
            
            while(nCurrentTuned < nMaxChange && nSolveNum < weight_num_)
            {
                minDis = INF;
                for(int j = 0; j < weight_num_; ++j)
                {
                    if(flag[j])//flag=true haven't been changed yet
                    {
                        tempdis = CalEuclidianDistance(lambda_[index[i]], lambda_[j], g_GlobalSettings->obj_num_);
                        if(tempdis < minDis)
                        {
                            minDis = tempdis;
                            near_index = j;
                            // std::cout<<"near_index"<<near_index;
                        }
                    }
                }

                for(int j = 0; j < g_GlobalSettings->obj_num_; ++j)
                {
                    lambda_[near_index][j] += step_size_ * (lambda_[index[i]][j] - lambda_[near_index][j]);
                    // lambda_[near_index][j]=lambda_[index[i]][j];

                }
                flag[near_index] = false;
                nSolveNum++;
                nCurrentTuned++;
            }
            // std::cout<<nSolveNum<<endl;
        }
    }

    float R2_IBEA_RUCB_AL::CalculateKLDivergence(Individual **pop)
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

    double R2_IBEA_RUCB_AL::UpdateUtilityPrediction(double obj, int index)
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

    

}