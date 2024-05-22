#include "metric/UPCF.h"

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <string.h>

#include "core/macro.h"
#include "core/global.h"
#include "core/utility.h"

int CF_size;		//store the size of composite front


namespace emoc{
    double CalculateIGD_CF(Individual **pop,int pop_num,int obj_num,double* reference_point,double preferred_r)
    {
        int* CF;	//store the index of the composite front
		CF_size=0;
        CF=findPF(pop,pop_num,obj_num,false);
		// printf("CF_size:%d\n",CF_size);

		int* PF=new int[pop_num];	//store the index of the ROI in the composite front, new PF
		int pf_size=0;
		for(int i=0;i<pop_num;i++)
		{
			PF[i]=0;
		}
		// memset(PF, 0, sizeof(int) * pop_num);
		//need to find the ROI
		//find the mid-point
		int index=0;
		Individual *temp_ind = nullptr;
		double min_distance = 0, temp_distance = 0;
		for(int i=0;i<CF_size;++i)
		{
			min_distance=EMOC_INF;
			temp_ind=pop[CF[i]];
			temp_distance=CalEuclidianDistance(reference_point,temp_ind->obj_.data(),obj_num);
			// printf("temp_distance:%f",temp_distance);
			if(temp_distance<min_distance)
			{
				min_distance=temp_distance;
				index=i;	//the closest point in CF to the reference point
			}
		}
		// printf("the nearest index:%d\n",index);

		double R;			//radius of the ROI
		R=preferred_r;
		Individual* mid_point;
		mid_point=pop[CF[index]];
		index=0;
		for(int i=0;i<CF_size;++i)
		{
			temp_distance=CalEuclidianDistance(pop[CF[i]]->obj_.data(),mid_point->obj_.data(),obj_num);
			// printf("temp_distance:%f\n",temp_distance);
			if(temp_distance<=R)	//record ROI in CF
			{
				PF[index]=i;
				index++;
				pf_size++;
			}
		}
		// printf("pf_size:%d\n",pf_size);

		//calculate IGD-CF
		double igd_value = 0;
		temp_ind = nullptr;
		min_distance = 0, temp_distance = 0;

		for (int i = 0; i < pf_size; i++)  
		{
			min_distance = EMOC_INF;
			for (int j = 0; j < pop_num; j++)
			{
				temp_ind = pop[j];
				temp_distance = CalEuclidianDistance(pop[PF[i]]->obj_.data(),temp_ind->obj_.data(), obj_num);
				// printf("temp igd:%f ",temp_distance);
				if(temp_distance!=0)
				{
					if (min_distance > temp_distance)
					{
						min_distance = temp_distance;
					}
				}
				// printf("current min distance:%f\n",min_distance);
			}
			igd_value += min_distance;
			// printf("igd_value:%f\n",igd_value);
		}

		igd_value /= pf_size;
		return 	igd_value;

    }

    int* findPF(Individual **pop, int pop_num, int obj_num, bool is_consider_cons)
    {
        int index = 0; 
		int dominate_relation = 0;
		int uncheck_num = pop_num; 

		int *ni = nullptr;             // store the number of points that dominate i-th solution
		int **si = nullptr;            // store the solution index of which i-th solution dominates
		int	*Q = nullptr;              // store the solution which ni is 0
		int *dominate_num = nullptr;   // store the number of dominate points of i-th solution
		Individual *ind_tempA = nullptr, *ind_tempB = nullptr;


        ni = (int *)malloc(sizeof(int) * pop_num);
		memset(ni, 0, sizeof(int) * pop_num);

		si = (int **)malloc(sizeof(int *) * pop_num);
		for (int i = 0; i < pop_num; i++)
		{
			si[i] = (int *)malloc(sizeof(int) * pop_num);
			memset(si[i], 0, sizeof(int) * pop_num);
		}

		Q = (int *)malloc(sizeof(int) * pop_num);
		memset(Q, 0, sizeof(int) * pop_num);

		dominate_num = (int*)malloc(sizeof(int) * pop_num);
		memset(dominate_num, 0, sizeof(int) * pop_num);

		for (int i = 0; i < pop_num; i++)
		{
			ind_tempA = pop[i];
			index = 0;
			for (int j = 0; j < pop_num; j++)
			{
				if (i == j)
					continue;

				ind_tempB = pop[j];
				dominate_relation = is_consider_cons ? CheckDominanceWithConstraint(ind_tempA, ind_tempB, obj_num) 
					:CheckDominance(ind_tempA, ind_tempB, obj_num);
				if (1 == dominate_relation)
				{
					si[i][index++] = j;
				}
				else if (-1 == dominate_relation)
				{
					ni[i]++;
				}
				else;
			}
			dominate_num[i] = index;
		}

        index = 0;
		for (int i = 0; i < pop_num; i++)
		{
			if (ni[i] == 0)
			{
				Q[index++] = i;
				CF_size++;
				ni[i] = -1;
			}
		}	//find all non-dominated set
        return Q;
    }
}