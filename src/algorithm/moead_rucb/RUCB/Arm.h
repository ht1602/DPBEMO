#pragma once

#include "stdafx.h"

class Arm_RUCB {
public:
    Arm_RUCB();
    Arm_RUCB(const vec& probabilities);

    friend ostream& operator<<(ostream& os, const Arm_RUCB& a);

    //Will return true if this Arm_RUCB won
    bool compareWithArm(size_t index);

    /* Will give you the double value for probability. Used for calculation of
     * regret, and evaluating performance but should not be used for the algorithm
     */
    double getProbability(size_t index);

    ~Arm_RUCB();


private:
    //Defined as the probability that THIS Arm_RUCB will win against the Arm_RUCB in the index
    vec probabilities;
};


ostream& operator<<(ostream& os, const Arm_RUCB& a);
