#pragma once

#include "Arm.h"
#include "stdafx.h"

class RunAL {
public:
    RunAL();
    RunAL(const vector<ArmAL>& arms, double learningRate = 1, int Budget = 1, double kappa =1);
    RunAL(const mat& vals, double learningRate = 1, int Budget =1, double kappa = 1);
    ~RunAL();

    void timestep();

    void timestep(int T);

    void updateUCB();

    void updateProb();

    void updateProb_c();

    void updateProb_c(std::vector<int> index_save);

    void MSELossFunction();

    void CrossEntropyLossFunction();

    int winner();

    friend ostream& operator<<(ostream& os, const RunAL& a);

private:
    vector<ArmAL> arms;

    mat wins;
    mat ucb;
    mat prob;
    vec prob_c;
    mat Q;              // record whether has been picked onced
    mat L;

    double learningRate;

    long t;

    long best_choose;
    size_t optimal_arm;
    size_t select1;
    size_t select2;
    double cum_regret;
    
    int B;
    int num_query;
    double kappa_;
    double p_min;
};

ostream& operator<<(ostream& os, const RunAL& a);
