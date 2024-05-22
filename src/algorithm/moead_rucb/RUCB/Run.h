#pragma once

#include "Arm.h"
#include "stdafx.h"

class Run_RUCB {
public:
    Run_RUCB();
    Run_RUCB(const vector<Arm_RUCB>& arms, double learningRate = 1);
    Run_RUCB(const mat& vals, double learningRate = 1);
    ~Run_RUCB();

    void timestep();

    void updateUCB();

    int winner();

    friend ostream& operator<<(ostream& os, const Run_RUCB& a);

private:
    vector<Arm_RUCB> arms;

    mat wins;
    mat ucb;

    double learningRate;

    long t;

    long best_choose;
    size_t optimal_arm;
    size_t select1;
    size_t select2;
};

ostream& operator<<(ostream& os, const Run_RUCB& a);
