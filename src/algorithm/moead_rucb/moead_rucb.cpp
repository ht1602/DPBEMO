#include "algorithm/moead_rucb/moead_rucb.h"

#include <cmath>
#include <ctime>
#include <iostream>
#include <algorithm>

#include "core/global.h"
#include "core/utility.h"
#include "core/uniform_point.h"
#include "core/emoc_manager.h"
#include "operator/sbx.h"
#include "operator/polynomial_mutation.h"
#include "random/random.h"

#include <numeric>  //iota
#include "RUCB/stdafx.h"
#include "RUCB/Run.h"


namespace emoc{
    MOEADRUCB::MOEADRUCB( int thread_id ):
    Algorithm(thread_id),
    lambda_(nullptr),
    weight_num_(0),
    neighbour_(nullptr),
    ideal_point_(new double[g_GlobalSettings->obj_num_]), 
	aggregation_type_(0),
    neighbour_selectpro_(0.9),
	pbi_theta_(5.0),
    replace_num_(2),
    nPromisingWeight(10),
    step_size_(0.5),
    c(new double[g_GlobalSettings->obj_num_]),
    utl(new double[g_GlobalSettings->population_num_]),
    select_sum(20),
    compare(new size_t[select_sum])
    {

    }

    MOEADRUCB::~MOEADRUCB()
    {
        for (int i = 0; i < weight_num_; ++i)
		{
			delete[] lambda_[i];		
			lambda_[i] = nullptr;
            delete[] neighbour_[i];
            neighbour_[i]=nullptr;
		}
        
		delete[] lambda_;
		delete[] neighbour_;
		delete[] ideal_point_;
        delete[] c;
        delete[] utl;
        delete[] compare;
		lambda_ = nullptr;
		neighbour_ = nullptr;
		ideal_point_ = nullptr;
        c = nullptr;
        utl = nullptr;
        compare = nullptr;
    }

    void MOEADRUCB::Solve()
    {
        Initialization();
        Individual *offspring = g_GlobalSettings->offspring_population_[0];
        first_tau = 100;
        tau_ = 20;
        while(!IsTermination())
        {
            if((g_GlobalSettings->iteration_num_ > first_tau) && (g_GlobalSettings->iteration_num_ % tau_ == 0) && (g_GlobalSettings->iteration_num_ > 0) && tau_ && first_tau)
            {
                UtilityFunction(g_GlobalSettings->parent_population_.data());
                StorePreference(g_GlobalSettings->parent_population_.data());
                int best_index = Consult_DM();
                printf("the recommend population index is %d\n",compare[best_index]);
                for(int i = 0 ;i < g_GlobalSettings->obj_num_; i++)
                {
                    // printf("%f\n",lambda_[compare[best_index]][i]);
                    printf("%.4f\n", g_GlobalSettings->parent_population_[compare[best_index]]->obj_[i]);
                }
                SetBiasedWeight(lambda_[compare[best_index]]);
                
            }
            UpdateNeighbour();
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
    }

    void MOEADRUCB::Initialization()
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


        SetNeighbours();

        UpdateIdealpoint(g_GlobalSettings->parent_population_.data(),weight_num_,ideal_point_,g_GlobalSettings->obj_num_);

        mutation_para_.pro=1.0/g_GlobalSettings->dec_num_;
        mutation_para_.index1=20.0;

        cross_para_.pro=1.0;
        cross_para_.index1=20.0;
        // for(int i = 0;i< g_GlobalSettings->obj_num_; i++)
        // {
        //     c[i] = 0.5;
        // }
        if(g_GlobalSettings->obj_num_ == 3)
        {
            c[0] = 0.3;
            c[1] = 0.3;
            c[2] = 0.3;
        }
        else if(g_GlobalSettings->obj_num_ == 2)
        {
            c[0] = 0.5;
            c[1] = 0.5;
        }
        

    }

    void MOEADRUCB::SetNeighbours()
    {
        neighbour_num_=weight_num_/10;
        neighbour_=new int*[weight_num_];
        for(int i=0;i<weight_num_;++i)
        {
            neighbour_[i]=new int[neighbour_num_];
        }

        std::vector<DistanceInfo>sort_list(weight_num_);
        for(int i=0;i<weight_num_;++i)
        {
            for(int j=0;j<weight_num_;++j)
            {
                double distance_temp=0;
                for(int k=0;k<g_GlobalSettings->obj_num_;k++)
                {
                    distance_temp+=(lambda_[i][k]-lambda_[j][k])*(lambda_[i][k]-lambda_[j][k]);
                }
                sort_list[i].distance=sqrt(distance_temp);
                sort_list[j].index=j;
            }
            std::sort(sort_list.begin(),sort_list.end(),[](const DistanceInfo &left,const DistanceInfo &right){
                return left.distance<right.distance;
            });
            for(int j=0;j<neighbour_num_;++j)
            {
                neighbour_[i][j]=sort_list[j+1].index;
            }
        }
    }

    void MOEADRUCB::Crossover(Individual **parent_pop,int current_index,Individual *offspring)
    {
        int size=neighbour_type_==NEIGHBOUR?neighbour_num_:weight_num_;
        int parent1_index=0,parent2_index=0;

        if(neighbour_type_==NEIGHBOUR)
        {
            int k=rnd(0,size-1);
            int l=rnd(0,size-1);
            parent1_index=neighbour_[current_index][k];
            parent2_index=neighbour_[current_index][l];
        }
        else
        {
            parent1_index=rnd(0,size-1);
            parent2_index=rnd(0,size-1);
        }
        Individual *parent1=parent_pop[parent1_index];
        Individual *parent2=parent_pop[parent2_index];

        SBX(parent1,parent2,g_GlobalSettings->offspring_population_[1],offspring,
        g_GlobalSettings->dec_lower_bound_,g_GlobalSettings->dec_upper_bound_,cross_para_);

    }

    void MOEADRUCB::UpdateSubproblem(Individual *offspring,int current_index,int aggregration_type)
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
                    offspring_fitness=CalWeightedSum(current_ind,lambda_[weight_index],ideal_point_,g_GlobalSettings->obj_num_);
                    neighbour_fitness=CalWeightedSum(offspring,lambda_[weight_index],ideal_point_,g_GlobalSettings->obj_num_);
                    break;
                case2:
                    offspring_fitness=CalPBI(current_ind,lambda_[weight_index],ideal_point_,g_GlobalSettings->obj_num_,pbi_theta_);
                    neighbour_fitness=CalPBI(offspring,lambda_[weight_index],ideal_point_,g_GlobalSettings->obj_num_,pbi_theta_);
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

    void MOEADRUCB::UpdateNeighbour()
    {
        DistanceInfo *sort_list=new DistanceInfo[weight_num_];
        // std::vector<DistanceInfo>sort_list(weight_num_);
        for(int i=0;i<weight_num_;++i)
        {
            for(int j=0;j<weight_num_;++j)
            {
                double distance_temp=0;
                for(int k=0;k<g_GlobalSettings->obj_num_;++k)
                {
                    distance_temp+=(lambda_[i][k]-lambda_[j][k])*(lambda_[i][k]-lambda_[j][k]);
                }
                sort_list[j].distance=sqrt(distance_temp);
                sort_list[j].index=j;
            }
            std::sort(sort_list,sort_list+weight_num_,[](const DistanceInfo &left, const DistanceInfo &right){
                return left.distance<right.distance;
            });

           for(int j=0;j<neighbour_num_;j++)
           {
            neighbour_[i][j]=sort_list[j+1].index;
           }


        }
        delete[] sort_list;
    }

    // consult the RUCB dueling bandit and return the best point index
    int MOEADRUCB::Consult_DM()
    {
        mat PVals;
        int best;
        PVals.load("compare.dat");

        double learning_rate= 0.51;
        int T_round = 1000;
        Run_RUCB run(PVals, learning_rate);
    for(int i = 0; i < T_round ; i++)
    {
        // cout << "the " << i+1 << " round" << endl;
        run.timestep();

        // cout << run << endl;
        
        // best = run.winner();
        
    }
    best = run.winner();
    printf("recommend index is %d\n", best);
    return best;
    }

    // arrange the weight vectors uniformally to the 10 best weight
    void MOEADRUCB::SetBiasedWeight(double *best)
    {
        std::vector<double>dis(weight_num_);
        for(int i=0;i<weight_num_;++i)
        {
            dis[i]=CalEuclidianDistance(best,lambda_[i],g_GlobalSettings->obj_num_);
            // std::cout<<i<<":"<<dis[i]<<endl;
        }
        std::vector<size_t>index(dis.size());
        iota(index.begin(),index.end(),0);
        sort(index.begin(),index.end(),
            [&dis](size_t index_1,size_t index_2){return dis[index_1]<dis[index_2];});
        //find the 10 best lambda_ according to our best
        //the 10 best are index[0]~index[9]

        bool flag[weight_num_];
        memset(flag,true,weight_num_*sizeof(bool));
        for(int i=0;i<nPromisingWeight;++i)
        {
            flag[index[i]]=false;//10 best need no change
            // std::cout<<index[i]<<endl;
        }
        int nSolveNum=0;//weights have been changed
        double tempdis,minDis;
        int near_index;
        nSolveNum=nPromisingWeight;
        int nMaxChange=ceil((double)(weight_num_-nPromisingWeight)/nPromisingWeight);
        // std::cout<<nSolveNum<<"  "<<nMaxChange<<endl;
        for(int i=0;i<nPromisingWeight && nSolveNum<weight_num_; ++i)
        {
            int nCurrentTuned=0;
            
            while(nCurrentTuned<nMaxChange && nSolveNum<weight_num_)
            {
                minDis=INF;
                for(int j=0;j<weight_num_;++j)
                {
                    if(flag[j])//flag=true haven't been changed yet
                    {
                        tempdis=CalEuclidianDistance(lambda_[index[i]],lambda_[j],g_GlobalSettings->obj_num_);
                        if(tempdis<minDis)
                        {
                            minDis=tempdis;
                            near_index=j;
                            // std::cout<<"near_index"<<near_index;
                        }
                    }
                }

                for(int j=0;j<g_GlobalSettings->obj_num_;++j)
                {
                    lambda_[near_index][j]+=step_size_*(lambda_[index[i]][j]-lambda_[near_index][j]);
                    // lambda_[near_index][j]=lambda_[index[i]][j];

                }
                flag[near_index]=false;
                nSolveNum++;
                nCurrentTuned++;
            }
            // std::cout<<nSolveNum<<endl;
        }
    }

    // get the utility function value of all solutions
    void  MOEADRUCB::UtilityFunction(Individual **pop)
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

    // uniformly choose 10 compare objects, and store there utility function difference in a dat file
    void MOEADRUCB::StorePreference(Individual **pop)
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
                PVals(i, j) = diff;
                // std::cout << PVals(i,j) << std::endl;
            }
        }
        PVals.save("compare.dat",raw_ascii);
    }

}