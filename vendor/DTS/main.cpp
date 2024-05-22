#include "stdafx.h"
#include "Run.h"
clock_t start,finish;

using namespace std;

int main(int argc, char* argv[])
{
    srand(time(NULL));
    

    // mat pVals = mat(3, 3);
    mat pVals ;
    // std::vector<string> problem {"Booth", "Easom", "Hartmann", "Sphere", "StyblinskiTang", "ThreeHumpCamel"};
    pVals.load("./res/test.dat");
    

    double learning_rate= 0.51;
    int T_round = 1000;
    int trial = 10;
    Run run(pVals, learning_rate);
    start = clock();
    run.timestep(T_round, "toy", 0);
    finish = clock();
    double endtime = (double)(finish - start) / CLOCKS_PER_SEC;
    cout << "Total time" << endtime*1000 << "ms" << endl;
    // for(int index = 0; index < 6; index++)
    // {
    //     for(int i = 0; i < trial; i++)
    //     {
    //         start = clock();
    //         pVals.load("../problem/" + problem[index] + "/" + problem[index] + "_" + to_string(i) + ".dat");
    //         Run run(pVals, learning_rate);
    //         run.timestep(T_round, problem[index], i);
    //         finish = clock();
    //         double endtime = (double)(finish - start)/CLOCKS_PER_SEC;
    //         printf("Trial %d total time: %.2f ms\n", i, endtime*1000);
    //         // cout << "Total time: "<< endtime*1000 << "ms" <<endl;
    //     }
    // }
    
    
    return 0;
}
