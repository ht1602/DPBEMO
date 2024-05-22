#include "Run.h"
#include "stdafx.h"



/*******************************************************************************
 *************************** CONSTRUCTORS **************************************
 *******************************************************************************/
Run_RUCB::Run_RUCB(const vector<Arm_RUCB>& arms, double learningRate): 
arms(arms), 
wins(arms.size(), arms.size(), fill::zeros),
ucb(arms.size(), arms.size(), fill::ones),
learningRate(learningRate),
t(0)
{
    best_choose = 0;
}

Run_RUCB::Run_RUCB(const mat& vals, double learningRate):
arms(vals.n_rows), 
wins(arms.size(), arms.size(), fill::zeros),
learningRate(learningRate), 
t(0)
{
    assert(vals.n_rows == vals.n_cols);

    for(size_t i = 0; i < vals.n_rows; i++)
    {
        vec pVals = trans(vals.row(i));
        // pVals.print();
        // printf("\n");
        arms[i] = Arm_RUCB(pVals);
    }
    best_choose = 0;
}

Run_RUCB::~Run_RUCB()
{


}


/*******************************************************************************
 ********************************* ACTIONS *************************************
 *******************************************************************************/
void Run_RUCB::timestep()
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
    // cout << "c=" << select1 << endl;
    
    // pick d = argmax ucb(i,j)
    // vec temp2 = trans(ucb.row(select1));
    vec temp2 = ucb.col(select1);
    select2 = temp2.index_max();
    // cout << "d=" << select2 << endl;

    //compare arm c with d, increment w metric

    bool compare = arms[select1].compareWithArm(select2);
    if(compare)
    {
        wins(select1,select2)+=1;
    }
    else
    {
        wins(select2,select1)+=1;
    }

}

void Run_RUCB::updateUCB()
{
    mat totalWins = wins + trans(wins);

    ucb = wins / totalWins + sqrt(learningRate * log(t) / totalWins);

    //Something tells me this kills all the optimization but ok...
    //Also, THERE IS PROBABLY A VECTORIZATION FOR THIS CODE
    //TODO: Find vectorization
    for(size_t i = 0; i < ucb.n_rows; i++) {
        for(size_t j = 0; j < ucb.n_cols; j++) {
            if(totalWins(i, j) == 0)
                ucb(i, j) = 2;
            if (i == j)
                ucb(i,j)=0.5;
        }
    }
}

int Run_RUCB::winner()
{   int max = 0;
    size_t best = 0;
    mat totalWins = wins + trans(wins);
    mat win_prob = wins / totalWins;
    int optimal_max = 0;
    
    for(size_t i = 0; i < wins.n_rows; i++)
    {
        int optimal_count = 0;
        for(size_t j = 0; j < wins.n_cols; j++)
        {
            double prob = arms[i].getProbability(j);
            if(prob > 0.0e-6)
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
    printf("the optimal arm is %zu\n", optimal_arm);
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
    printf("the best arm is :%zu\n",best);
    printf("the accuracy is : %.2f % \n", accuracy*100);    
    return best;
}

ostream& operator<<(ostream& os, const Run_RUCB& r)
{
    os << "Run arms: " << endl;
    for(size_t i = 0; i < r.arms.size(); i++) {
        os << "\t[" << i << "]: " << r.arms[i] << endl;
    }

    os << endl << "UCB: " << endl;
    os << r.ucb << endl;

    return os;
}