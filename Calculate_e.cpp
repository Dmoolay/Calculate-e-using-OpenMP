#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
//===============================================================================================================
#define GRAINS_PER_RUN 100000
#define OSCILLATIONS 10000
#define PROBABILITY 0.0001
#define NUM_RUNS 128
//===============================================================================================================
int main() {
    // Parameter Initialization
    const int num_runs = NUM_RUNS;
    const int grains_per_run = GRAINS_PER_RUN;
    const int oscillations = OSCILLATIONS;
    double sum = 0.0;
    double start_time, end_time;
    unsigned int seed = time(NULL); // Random seed
    int numThreads[] = {1, 2, 4, 8};
    int num_thread_count = 4;
//===============================================================================================================
    //This for loop does a run for a different number of processes each time
    for (int t = 0; t < num_thread_count; ++t) {
        omp_set_num_threads(numThreads[t]);
        sum = 0.0;
        start_time = omp_get_wtime(); // Start measure time

        //This for loop runs for 128 different calculations of e
        for (int i = 0; i < num_runs; ++i) {
            int total_num_of_fall = 0;
// Run parallel loop for independent iterations and sum the count variable at the end
#pragma omp parallel for reduction(+:total_num_of_fall)
            // This for loop runs over all the grains
            for (int j = 0; j < grains_per_run; ++j) {
                int flag1 = 0;   //flag that indicate if a grain fall in one of the oscillations
                // This for loop simulate all the oscillations for each grain
                for (int k = 0; k < oscillations; ++k) {
                    double rand1 = (double)rand_r(&seed) / RAND_MAX; // get a random number in range [0,1]
                    if (rand1 <= 0.0001) { // if number fall inside
                        flag1 = 1; // indicate the grain fell
                        break; //break the for loop for this grain (count him 1 time max)
                    }
                }
                total_num_of_fall += flag1; //add the falling grain to the total sum
            }
//===============================================================================================================
            // Calculate the estimate e for this iteration (1 of 128)
            double estimate = (double)grains_per_run / (grains_per_run-total_num_of_fall);
            sum += estimate; // add it to the sum of all iterations
//===============================================================================================================
        }
        end_time = omp_get_wtime(); // Stop measure time
        // Calculate the final average estimate for e (avg of 128 runs)
        double average = sum / num_runs;
//===============================================================================================================
        // print the result for every different number of thread
        printf("Number of threads: %d\n", numThreads[t]);
        printf("Estimated value of e: %f\n", average);
        printf("Running time: %f seconds\n\n", end_time - start_time);
    }
    return 0;
}

