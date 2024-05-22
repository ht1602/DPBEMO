#include "algorithm/moead_rucb_al/moead_rucb_al.h"

#include <cmath>
#include <ctime>
#include <iostream>
#include <algorithm>

#include "core/file.h"
#include "core/global.h"
#include "core/utility.h"
#include "core/uniform_point.h"
#include "core/emoc_manager.h"
#include "operator/sbx.h"
#include "operator/polynomial_mutation.h"
#include "random/random.h"

#include <numeric>  //iota
#include "../vendor/Bandit/stdafx.h"
#include "../vendor/Bandit/RUCB-AL.h"
// #include "RUCB-AL/stdafx.h"
// #include "RUCB-AL/Run.h"
using namespace std;

namespace emoc{
    MOEAD_RUCB_AL::MOEAD_RUCB_AL( int thread_id ):
    Algorithm(thread_id),
    lambda_(nullptr),
    weight_num_(0),
    neighbour_(nullptr),
    ideal_point_(new double[g_GlobalSettings->obj_num_]),
	aggregation_type_(0),
    neighbour_selectpro_(0.9),
	pbi_theta_(5.0),
    count(0),
    replace_num_(2),
    nPromisingWeight(10),
    step_size_(0.3),
    c(new double[g_GlobalSettings->obj_num_]),
    utl(new double[g_GlobalSettings->population_num_]),
    select_sum(10),
    compare(new size_t[select_sum]),
    weight(new double[g_GlobalSettings->obj_num_]),
    sigma(0.5),
    best_index(0),
    max_consultation(10),
    util_prob1(new double[g_GlobalSettings->population_num_]),
	util_prob2(new double[g_GlobalSettings->population_num_]),
    gamma(0.9)
    {

    }

    MOEAD_RUCB_AL::~MOEAD_RUCB_AL()
    {
        for (int i = 0; i < weight_num_; ++i)
		{
			delete[] lambda_[i];		
			lambda_[i] = nullptr;
            delete[] neighbour_[i];
            neighbour_[i]=nullptr;
		}
        for(int i = 0; i < max_consultation; ++i)
        {
            delete[] recommend_point[i];
            recommend_point[i] = nullptr;
        }
        
        
		delete[] lambda_;
		delete[] neighbour_;
		delete[] ideal_point_;
        delete[] c;
        delete[] utl;
        delete[] compare;
        delete[] weight;
        delete[] recommend_point;
        delete[] util_prob1;
        delete[] util_prob2;
		lambda_ = nullptr;
		neighbour_ = nullptr;
		ideal_point_ = nullptr;
        c = nullptr;
        utl = nullptr;
        compare = nullptr;
        weight = nullptr;
        recommend_point = nullptr;
        util_prob1 = nullptr;
        util_prob2 = nullptr;
    }

    void MOEAD_RUCB_AL::Solve()
    {
        Initialization();
        Individual *offspring = g_GlobalSettings->offspring_population_[0];
        int first_tau = 0.5 * g_GlobalSettings->max_evaluation_ / g_GlobalSettings->population_num_;

        while(!IsTermination())
        {
            // first time to consult when run 50% iteration budget
            if(g_GlobalSettings->iteration_num_ >= first_tau && count == 0)
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

                SetBiasedWeight(lambda_[compare[best_index]]);
                // UpdateNeighbour();

            }
            // the 2nd and afterwards consultation
            if(count && count < max_consultation)
            {
                double KL = CalculateKLDivergence(g_GlobalSettings->parent_population_.data());
                if(KL > 1e-3)
                {
                    printf("KL = %f \n", KL);
					printf("the %d time to consult in generation %d\n", ++count, g_GlobalSettings->iteration_num_);
					UtilityFunction_Gaussian(g_GlobalSettings->parent_population_.data());
					// StorePreference(g_GlobalSettings->parent_population_.data());
					ActiveStorePreference(g_GlobalSettings->parent_population_.data());
					best_index = Consult_DM();
					// printf("the recommend population index is %d\n",compare[best_index]);
					// printf("the recommend population is :");
					// for(int i = 0; i < g_GlobalSettings->obj_num_; i++)
					// {
					// 	cout << g_GlobalSettings->parent_population_[compare[best_index]]->obj_[i];
					// 	recommend_point[count][i] = g_GlobalSettings->parent_population_[compare[best_index]]->obj_[i];
					// }
					// cout << endl;
					// count++;

                    SetBiasedWeight(lambda_[compare[best_index]]);
                    // UpdateNeighbour();
                }
            }
            // UpdateNeighbour();
            
            for(int i = 0; i < weight_num_ ; i++)
            {
                if(randomperc() < neighbour_selectpro_)
                {
                    neighbour_type_ = NEIGHBOUR;
                }
                else
                {
                    neighbour_type_ = GLOBAL;
                }
                Crossover(g_GlobalSettings->parent_population_.data(),i,offspring);
                PolynomialMutation(offspring,g_GlobalSettings->dec_lower_bound_,g_GlobalSettings->dec_upper_bound_,mutation_para_);
                EvaluateInd(offspring);

                UpdateIdealpoint(offspring,ideal_point_,g_GlobalSettings->obj_num_);
                UpdateSubproblem(offspring,i,aggregation_type_);
            }
        }
        printf("total consultation is %d times\n", count + 1);
        CalculateMinDis(weight);
    }

    void MOEAD_RUCB_AL::Initialization()
    {
        weight_num_=g_GlobalSettings->population_num_;
        // std::cout<<"weight_num_="<<weight_num_<<endl;
        g_GlobalSettings->InitializePopulation(g_GlobalSettings->parent_population_.data(),g_GlobalSettings->population_num_);
        EvaluatePop(g_GlobalSettings->parent_population_.data(),g_GlobalSettings->population_num_);
        // std::cout<<"weight_num_="<<weight_num_<<endl;

        char file[256];
        sprintf(file,"./UniformWeights/%dd_%d.txt",g_GlobalSettings->obj_num_,g_GlobalSettings->population_num_);
        // lambda_=LoadUniformWeights(g_GlobalSettings->population_num_,g_GlobalSettings->obj_num_,file);
        lambda_=UniformPoint(g_GlobalSettings->population_num_,&weight_num_,g_GlobalSettings->obj_num_);
        real_popnum_=weight_num_;

        weight = SetWeight(g_GlobalSettings->weight_StringType_);
        cout << "weight: ";
        for(int i = 0; i < g_GlobalSettings->obj_num_; i++)
        {
            cout << weight[i] << " " ;
        }
        cout<< endl;

        // initialize recommend point
		recommend_point = new double*[max_consultation];
		for(int i = 0; i < max_consultation; i++)
		{
			recommend_point[i] = new double[g_GlobalSettings->obj_num_];
		}

        SetNeighbours();

        UpdateIdealpoint(g_GlobalSettings->parent_population_.data(), weight_num_, ideal_point_, g_GlobalSettings->obj_num_);

        mutation_para_.pro = 1.0 / g_GlobalSettings->dec_num_;
        mutation_para_.index1 = 20.0;

        cross_para_.pro = 1.0;
        cross_para_.index1 = 20.0;     

    }

    void MOEAD_RUCB_AL::SetNeighbours()
    {
        neighbour_num_=weight_num_/10;
        neighbour_=new int*[weight_num_];
        for(int i=0;i<weight_num_;++i)
        {
            neighbour_[i]=new int[neighbour_num_];
        }

        std::vector<DistanceInfo>sort_list(weight_num_);
        for(int i = 0; i < weight_num_; ++i)
        {
            for(int j = 0; j < weight_num_; ++j)
            {
                double distance_temp = 0;
                for(int k = 0;k < g_GlobalSettings->obj_num_; k++)
                {
                    distance_temp += (lambda_[i][k]-lambda_[j][k])*(lambda_[i][k]-lambda_[j][k]);
                }
                sort_list[i].distance = sqrt(distance_temp);
                sort_list[j].index = j;
            }
            std::sort(sort_list.begin(),sort_list.end(),[](const DistanceInfo &left,const DistanceInfo &right){
                return left.distance < right.distance;
            });
            for(int j = 0; j < neighbour_num_; ++j)
            {
                neighbour_[i][j] = sort_list[j+1].index;
            }
        }
    }

    void MOEAD_RUCB_AL::Crossover(Individual **parent_pop,int current_index,Individual *offspring)
    {
        int size=neighbour_type_ == NEIGHBOUR ? neighbour_num_:weight_num_;
        int parent1_index = 0, parent2_index = 0;

        if(neighbour_type_ == NEIGHBOUR)
        {
            int k = rnd(0,size-1);
            int l = rnd(0,size-1);
            parent1_index = neighbour_[current_index][k];
            parent2_index = neighbour_[current_index][l];
        }
        else
        {
            parent1_index = rnd(0,size-1);
            parent2_index = rnd(0,size-1);
        }
        Individual *parent1 = parent_pop[parent1_index];
        Individual *parent2 = parent_pop[parent2_index];

        SBX(parent1,parent2,g_GlobalSettings->offspring_population_[1],offspring,
        g_GlobalSettings->dec_lower_bound_,g_GlobalSettings->dec_upper_bound_,cross_para_);

    }

    void MOEAD_RUCB_AL::UpdateSubproblem(Individual *offspring,int current_index,int aggregration_type)
    {
        int size = neighbour_type_ == NEIGHBOUR ? neighbour_num_ : weight_num_;
		std::vector<int> perm_index(size);
		random_permutation(perm_index.data(), size);

		int count = 0, weight_index = 0;
		double offspring_fitness = 0.0;
		double neighbour_fitness = 0.0;

		// calculate fitness and update subproblem;
		for (int i = 0; i < size; ++i)
		{
			if (count >= replace_num_)
				break;
			
			if (neighbour_type_ == NEIGHBOUR)
            {
                // std::cout<<i<<":"<<current_index<<"  "<<perm_index[i]<<endl;
                weight_index = neighbour_[current_index][perm_index[i]];
            }
			else
				weight_index = perm_index[i];

			Individual *current_ind = g_GlobalSettings->parent_population_[weight_index];

            switch(aggregation_type_)
            {
                case 0:
                    offspring_fitness = CalInverseChebycheff(offspring, lambda_[weight_index], ideal_point_, g_GlobalSettings->obj_num_);
			        neighbour_fitness = CalInverseChebycheff(current_ind, lambda_[weight_index], ideal_point_, g_GlobalSettings->obj_num_);
                    break;
                
                case 1:
                    offspring_fitness = CalWeightedSum(current_ind,lambda_[weight_index],ideal_point_,g_GlobalSettings->obj_num_);
                    neighbour_fitness = CalWeightedSum(offspring,lambda_[weight_index],ideal_point_,g_GlobalSettings->obj_num_);
                    break;
                case2:
                    offspring_fitness = CalPBI(current_ind,lambda_[weight_index],ideal_point_,g_GlobalSettings->obj_num_,pbi_theta_);
                    neighbour_fitness = CalPBI(offspring,lambda_[weight_index],ideal_point_,g_GlobalSettings->obj_num_,pbi_theta_);
                    break;
                default:
                    break;
            }
			
			if (offspring_fitness < neighbour_fitness)
			{
				CopyIndividual(offspring, g_GlobalSettings->parent_population_[weight_index]);
				count++;
			}
		}
    }

    void MOEAD_RUCB_AL::UpdateNeighbour()
    {
        DistanceInfo *sort_list = new DistanceInfo[weight_num_];
        // std::vector<DistanceInfo>sort_list(weight_num_);
        for(int i = 0; i < weight_num_; ++i)
        {
            for(int j = 0; j < weight_num_; ++j)
            {
                double distance_temp = 0;
                for(int k = 0; k<g_GlobalSettings->obj_num_; ++k)
                {
                    distance_temp += (lambda_[i][k] - lambda_[j][k]) * (lambda_[i][k] - lambda_[j][k]);
                }
                sort_list[j].distance = sqrt(distance_temp);
                sort_list[j].index = j;
            }
            std::sort(sort_list,sort_list + weight_num_,[](const DistanceInfo &left, const DistanceInfo &right){
                return left.distance < right.distance;
            });

           for(int j = 0; j < neighbour_num_; j++)
           {
            neighbour_[i][j] = sort_list[j+1].index;
           }


        }
        delete[] sort_list;
    }

    // consult the RUCB dueling bandit and return the best point index
    int MOEAD_RUCB_AL::Consult_DM()
    {
        mat PVals;
        int best;
        PVals.load("compare.dat");
        cout << PVals << endl;


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
    void MOEAD_RUCB_AL::SetBiasedWeight(double *best)
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

    // get the utility function value of all solutions
    void  MOEAD_RUCB_AL::UtilityFunction(Individual **pop)
    {
        double **f = new double*[g_GlobalSettings->population_num_];
        double max = 0.0;
        int max_index = 0;
        for(int i = 0; i < g_GlobalSettings->population_num_; i++)
        {
            f[i] = new double[g_GlobalSettings->obj_num_];
        }

        for(int i = 0;i < g_GlobalSettings->population_num_; i++)
        {
            for(int j = 0;j < g_GlobalSettings->obj_num_; j++)
            {
                f[i][j] = pow(c[j] * pop[i]->obj_[j] - 1, 3);
            }
        }

        for(int i = 0;i < g_GlobalSettings->population_num_; i++)
        {
            utl[i] = 0;
            for(int j = 0;j < g_GlobalSettings->obj_num_; j++)
            {
                utl[i] += 0.33 * f[i][j];
            }
            if(utl[i] > max)
            {
                max = utl[i];
                max_index = i;
            }
            if(g_GlobalSettings->obj_num_ == 3)
                utl[i] *= 1e6;
        }
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
        f = nullptr;
    }

    // get the utility function value of all solutions
    void  MOEAD_RUCB_AL::UtilityFunction_Gaussian(Individual **pop)
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
    void MOEAD_RUCB_AL::StorePreference(Individual **pop)
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
                // PVals(i, j) = diff;
                PVals(i,j) = 1 / (1 + exp(-1000 * diff));
                // std::cout << PVals(i,j) << std::endl;
            }
        }
        PVals.save("compare.dat",raw_ascii);
    }

    void MOEAD_RUCB_AL::ActiveStorePreference(Individual **pop)
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
                // PVals(i, j) = diff;
                PVals(i,j) = 1 / (1 + exp(-1000 * diff));
                // std::cout << PVals(i,j) << std::endl;
            }
        }
        PVals.save("compare.dat",raw_ascii);
	}

    float MOEAD_RUCB_AL::CalculateKLDivergence(Individual **pop)
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

    double MOEAD_RUCB_AL::UpdateUtilityPrediction(double obj, int index)
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

    void MOEAD_RUCB_AL::CalculateMinMaxDis(Individual **pop)
    {
        minDis = INF;
        maxDis = 0;
        for(int i = 0; i < g_GlobalSettings->population_num_; i++)
        {
            double tempDis = CalEuclidianDistance(pop[i]->obj_.data(), weight, g_GlobalSettings->obj_num_);
            if(tempDis < minDis)
            {
                minDis = tempDis;
            }
            if(tempDis > maxDis)
            {
                maxDis = tempDis;
            }
        }

    }

    void MOEAD_RUCB_AL::CalculateMinDis(double *best)
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

    // void MOEAD_RUCB_AL::CalculateMaxDis(Individual **pop)
    // {
        //  maxDis = 0;
        // std::vector<size_t>index(g_GlobalSettings->population_num_);
        // // std::vector<size_t>compare(select_sum);
        // iota(index.begin(),index.end(),0);
        // std::sort(index.begin(),index.end(),
        //     [&pop](size_t index1, size_t index2){return pop[index1]->obj_[0] > pop[index2]->obj_[0];});
        
        // for(int i = 0; i < g_GlobalSettings->obj_num_; i++)
        // {
        //     volume *= abs(pop[index[0]]->obj_[i] - pop[index[g_GlobalSettings->population_num_ - 1]]->obj_[i]);
        // }
        // for(int i = 0; i < g_GlobalSettings->population_num_; i++)
        // {
        //     double tempDis = CalEuclidianDistance(pop[i]->obj_.data(), weight, g_GlobalSettings->obj_num_);
        //     if(tempDis < minDis)
        //     {
        //         minDis = tempDis;
        //     }
        // }
        // printf("min_dist=%f\n", minDis);

    // }

}