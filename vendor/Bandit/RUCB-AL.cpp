#include "RUCB-AL.h"
#include "stdafx.h"



/*******************************************************************************
 *************************** CONSTRUCTORS **************************************
 *******************************************************************************/
RunAL::RunAL(const vector<Arm>& arms, double learningRate, int Budget, double kappa): 
arms(arms), 
wins(arms.size(), arms.size(), fill::zeros),
ucb(arms.size(), arms.size(), fill::ones),
Q(arms.size(), arms.size(), fill::zeros),
prob(arms.size(),arms.size(), fill::zeros),
prob_c(arms.size(), fill::zeros),
L(arms.size(),arms.size(), fill::zeros),
learningRate(learningRate),
t(0),
B(Budget),
num_query(0),
cum_regret(0),
kappa_(kappa)
{
    // wins.print();
    best_choose = 0;
}

RunAL::RunAL(const mat& vals, double learningRate, int Budget, double kappa):
arms(vals.n_rows),
wins(arms.size(), arms.size(), fill::zeros),
learningRate(learningRate), 
ucb(arms.size(), arms.size(), fill::ones),
Q(arms.size(), arms.size(), fill::zeros),
prob(arms.size(), 1, fill::zeros),
prob_c(arms.size(), fill::zeros),
t(0),
B(Budget),
num_query(0),
L(arms.size(),arms.size(), fill::zeros),
cum_regret(0),
kappa_(kappa)
{
    assert(vals.n_rows == vals.n_cols);

    for(size_t i = 0; i < vals.n_rows; i++)
    {
        vec pVals = trans(vals.row(i));
        // pVals.print();
        // printf("\n");
        arms[i] = Arm(pVals);
    }
    best_choose = 0;
    int optimal_max = 0;
    
    for(size_t i = 0; i < wins.n_rows; i++)
    {
        int optimal_count = 0;
        for(size_t j = 0; j < wins.n_cols; j++)
        {
            double probability = arms[i].getProbability(j);
            if(probability > 0.5)
            {
                optimal_count ++;
            }
        }
        if(optimal_count > optimal_max)
        {
            optimal_arm = i;
            optimal_max = optimal_count;
        }        
    }
}

RunAL::~RunAL()
{


}


/*******************************************************************************
 ********************************* ACTIONS *************************************
 *******************************************************************************/
void RunAL::timestep()
{
    t++;

    updateUCB();
    // TODO
    //for all j, pick any arm c with ucb_cj>=1/2, if no satisfying arms, randomly pick c from all arms.
    // mat index_save;
    vector<int> index_save;
    for(size_t i = 0 ; i < ucb.n_rows ; i++)
    {
        vec temp = trans(ucb.row(i));
        // if this line have any emelent less than 0.5, flag == 1
        bool flag = any(temp < 0.5);
        if(!flag)
        {
            index_save.push_back(i);
        } 
    }
    int rnd;
    if(index_save.size())
    {
        rnd = rand() % int(index_save.size());
        select1=index_save[rnd];
    }
    else
    // if there is no available arm, randomly choose one
    {
        rnd = rand() % ucb.n_rows;
        select1 = rnd;
    }
    
    // select1 = index_save[rnd];
    cout << "c=" << select1 << endl;
    
    // pick d = argmax ucb(i,j)
    // vec temp2 = trans(ucb.row(select1));
    vec temp2 = ucb.col(select1);
    select2 = temp2.index_max();
    cout << "d=" << select2 << endl;

    //compare arm c with d, increment w metric

    bool compare = arms[select1].compareWithArm(select2);
    // double regret1 = arms[optimal_arm].getProbability(select1) - 0.5;
    // double regret2 = arms[optimal_arm].getProbability(select2) - 0.5;
    // cum_regret += (regret1 + regret2) / 2 ;

    if(compare)
    {
        wins(select1,select2)+=1;
    }
    else
    {
        wins(select2,select1)+=1;
    }

}

void RunAL::timestep(int T)
{
    while(t < T && num_query < B)
    {
        
        t++;
        p_min = 1 / (arms.size()* pow(t,kappa_));
        // update UCB
        updateProb();
        // cout << "winnning probability" << endl;
        // prob.print();
        MSELossFunction();
        // cout << "loss matrix" << endl;
        // L.print();
        updateUCB();
        // cout << "ucb matrix" << endl;
        // ucb.print();

        // choose the first arm a_c
        vector<int> index_save;
        for(size_t i = 0 ; i < ucb.n_rows ; i++)
        {
            vec temp = trans(ucb.row(i));
            // if this line have any emelent less than 0.5, flag == 1
            bool flag = any(temp < 0.5);
            if(!flag)
            {
                index_save.push_back(i);
            } 
        }
        // printf("index_save:");
        // for(int i = 0 ; i < index_save.size(); i++)
        // {
        //     printf("%d ",index_save[i]);
        // }
        // cout << endl;
        if(index_save.size() == 1)
        {
            select1 = index_save[0];
        }
        if(index_save.size() == 0)
        {
            // choose from the whole K arms according to probability
            updateProb_c();
            select1 = prob_c.index_max();
        }
        if(index_save.size() > 1)
        {
            // choose from the index_save
            updateProb_c(index_save);
            select1 = prob_c.index_max();
        }
        // cout << "probability of c" << endl;
        // prob_c.print();
        // cout << "c=" << select1 << endl;

        // choose the second arm d, d = argmax u_cj
        vec prob_d;
        prob_d = trans(ucb.row(select1));
        // prob_d = ucb.col(select1)
        select2 = prob_d.index_max();
        // cout << "d=" <<select2 << endl;

        // compare a_c and a_d
        bool compare;
        // double regret1 = arms[optimal_arm].getProbability(select1) - 0.5;
        // double regret2 = arms[optimal_arm].getProbability(select2) - 0.5;
        // cum_regret += (regret1 + regret2) / 2 ;
        if(Q(select1,select2) == 1 || Q(select2, select2) == 1)
        {
            // once compared
            compare = arms[select1].compareWithArm(select2);
        }
        else
        {
            // never compared, query the Oracle
            compare = arms[select1].compareWithArm(select2);
            num_query ++;
            Q(select1,select2) = 1;
        }
        if(compare)
        {
            wins(select1,select2)+=1;
        }
        else
        {
            wins(select2,select1)+=1;
        }

    }
}

void RunAL::updateUCB()
{
    // double p_min = 1 / (arms.size()* pow(t,kappa_));
    // mat totalWins = wins + trans(wins);

    
    for(size_t i = 0; i < ucb.n_rows; ++i)
    {
        for(size_t j = 0; j < ucb.n_cols; ++j)
        {
            vec sum_row = sum(L,1);
            // cout << "sum_row" << endl;
            // sum_row.print();
            if(i == j)
                ucb(i,j) = 0.0;
            else
            {
                if(sum_row[i] == 0.0)
                    ucb(i,j) = p_min + (1 - arms.size() * p_min) * 1;
                else
                    ucb(i,j) = p_min + (1 - arms.size() * p_min) * L(i,j) / sum_row[i];
            }
                
        }
    }
    
    // ucb = wins / totalWins + sqrt(learningRate * log(t) / totalWins);

    //Something tells me this kills all the optimization but ok...
    //Also, THERE IS PROBABLY A VECTORIZATION FOR THIS CODE
    //TODO: Find vectorization
    // for(size_t i = 0; i < ucb.n_rows; i++) {
    //     for(size_t j = 0; j < ucb.n_cols; j++) {
    //         // if(totalWins(i, j) == 0)
    //         //     ucb(i, j) = 2;
    //         if (i == j)
    //             ucb(i,j)=0.5;
    //     }
    // }
}

// generate the winning probability matrix
void RunAL::updateProb()
{
    mat totalWins = wins + trans(wins);

    prob = wins / totalWins;

    for(size_t i = 0; i < prob.n_rows; ++i)
    {
        for(size_t j = 0; j < prob.n_cols; ++j)
        {
            if(totalWins(i,j) == 0)
                prob(i,j) = 0.5;
            if(i == j)
                prob(i,j) = 0.5;
        }
    }
}

void RunAL::updateProb_c()
{
    double totalLoss = accu(L);
    // printf("total loss = %f\n", totalLoss);
    // double p_min = 1 / (arms.size()* pow(t,kappa_));
    vec sum_row = sum(L,1);
    for(size_t i = 0; i < arms.size(); ++ i )
    {
        if(totalLoss != 0)
            prob_c[i] = p_min + (1 - arms.size() * p_min) * sum_row[i] / totalLoss;
        else
            prob_c[i] = p_min + (1 - arms.size()* p_min) * 1;
    }

}

void RunAL::updateProb_c(std::vector<int> index_save)
{
    double totalLoss = accu(L);
    // double p_min = 1 / (arms.size()* pow(t,kappa_));
    vec sum_row = sum(L,1);
    bool flag;    // true means row i in the index_save
    for(size_t i = 0; i < arms.size(); ++i)
    {
        flag = false;
        // printf("%d",i);
        for(size_t j = 0; j < index_save.size(); ++j)
        {
            // printf("%d", index_save[j]);
            if(i == index_save[j])
            {
                flag = true;
                break;
            }
        }
        if(flag)
        {
            if(totalLoss != 0)
                prob_c[i] = p_min + (1 - arms.size() * p_min) * sum_row[i] / totalLoss;
            else
                prob_c[i] = p_min + (1 - arms.size()* p_min) * 1;
        }
        else
            prob_c[i] = 0.0;
        
    }
    prob_c.print();
}

// calculate loss matrix in MSE loss function form
void RunAL::MSELossFunction()
{
    for(size_t i = 0; i < arms.size(); ++i)
    {
        for(size_t j = 0; j < arms.size(); ++j)
        {
            if(arms[i].getProbability(j) > 0)
                L(i,j) = pow(prob(i,j) - 1, 2);
            else
                L(i,j) = pow(prob(i,j) - 0, 2);
            // L(i,j) = pow(prob(i,j) - arms[i].getProbability(j), 2);
        }
    }
}

void RunAL::CrossEntropyLossFunction()
{
    for(size_t i = 0; i < arms.size(); ++i)
    {
        for(size_t j = 0; j < arms.size(); ++j)
        {
            L(i,j) = - arms[i].getProbability(j) * log(prob(i,j));
        }
    }
}


int RunAL::winner()
{   int max = 0;
    size_t best = 0;
    mat totalWins = wins + trans(wins);
    mat win_prob = wins / totalWins;
    
    // printf("the optimal arm is %zu\n", optimal_arm);
    for(size_t i = 0; i < wins.n_rows; i++)
    {
        for(size_t j = 0; j < wins.n_cols; j++)
        {
            if(totalWins(i,j) == 0)
                win_prob(i,j) = 1;
            if(i == j)
                win_prob(i,j) = 0.5;
        }
    }
    // printf("wins:\n");
    // wins.print();
    // printf("\n winning probability:\n");
    // win_prob.print();
    for(size_t i = 0; i < win_prob.n_rows; i++ )
    {
        int count = 0;
        
        for(size_t j = 0; j < win_prob.n_cols; j++)
        {
            if(win_prob(i,j) > 0.5000)
                count ++;
        }
        if(count > max)
        {
            max = count;
            best = i;
        }
    }
    if(best == optimal_arm)
    {
        best_choose++;
    }
    double accuracy = 1.0000*best_choose / t;
    
    // cout << "the best arm is" << index << endl;
    // printf("the best arm is :%zu\n",best);
    // printf("the accuracy is : %.2f % \n", accuracy*100);
    // printf("the cummulative regret is : %.4f \n", cum_regret);  
    return best;  
}

ostream& operator<<(ostream& os, const RunAL& r)
{
    os << "Run arms: " << endl;
    for(size_t i = 0; i < r.arms.size(); i++) {
        os << "\t[" << i << "]: " << r.arms[i] << endl;
    }

    os << endl << "UCB: " << endl;
    os << r.ucb << endl;

    os << endl << "Winning Probability" << endl;
    os << r.prob << endl;

    return os;
}