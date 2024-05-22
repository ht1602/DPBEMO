#pragma once

#include "stdafx.h"

class ArmAL {
public:
    ArmAL();
    ArmAL(const vec& probabilities);

    friend ostream& operator<<(ostream& os, const ArmAL& a);

    //Will return true if this arm won
    bool compareWithArm(size_t index);

    /* Will give you the double value for probability. Used for calculation of
     * regret, and evaluating performance but should not be used for the algorithm
     */
    double getProbability(size_t index);

    ~ArmAL();


private:
    //Defined as the probability that THIS arm will win against the arm in the index
    vec probabilities;
};


ostream& operator<<(ostream& os, const ArmAL& a);
