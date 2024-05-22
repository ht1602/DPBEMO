#include "stdafx.h"
#include <random>
#include "Arm.h"

Arm_RUCB::Arm_RUCB() : Arm_RUCB(mat())
{

}

Arm_RUCB::Arm_RUCB(const vec& probabilities)
    : probabilities(probabilities)
{

}

double Arm_RUCB::getProbability(size_t index) {
    return ((index >= 0 && index < probabilities.n_elem) ? probabilities(index) : -1);
}

ostream& operator<<(ostream& os, const Arm_RUCB& a)
{
    os << "Arm p-values: ";/*{";

    for(size_t i = 0; i < a.probabilities.n_elem; i++) {
        os << fixed << setprecision(2) << a.probabilities[i];
        if(i < a.probabilities.size() - 1)
            os << ", ";
    }

    os << "}";*/

    os << trans(a.probabilities);

    return os;
}

// bool Arm_RUCB::compareWithArm_RUCB(size_t index)
// {
//     /*if(index >= probabilities.size() && index < 0)
//         throw exception;*/

//     double prob = probabilities(index);
//     double randVal = (double)rand() / (double)RAND_MAX;

//     return randVal <= prob;
// }
// TODO：generate Gaussian noise
bool Arm_RUCB::compareWithArm(size_t index)
{
    /*if(index >= probabilities.size() && index < 0)
        throw exception;*/

    double prob = probabilities(index);
    // double randVal = (double)rand() / (double)RAND_MAX;
    double noise;

    std::default_random_engine gen;
    std::normal_distribution<double> distribution(0.0, 0.01);

    noise = distribution(gen);
    // printf("prob = %.4f", prob);
    // printf("noise is %.4f\n", noise);
    // printf("prob + noise = %.4f\n", prob + noise);
    // double noise = 0.0;
    return prob + noise > 0 ? 1 : 0;
}

Arm_RUCB::~Arm_RUCB()
{

}
