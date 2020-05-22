#include <stdio.h>
#include <stdlib.h>
#include "get_time.h"
#include <omp.h>
#define N 100
#define STEPS 2

int main() {
    BENCH_INIT_TIMERS;
    uint32_t A[N];

    BENCH_INIT_TEST;
    for (uint32_t i = 0; i < NUM_REP; i ++) {
        BENCH_START_TIMER;
        // Init A
        for (uint32_t k = 0; k < N; k++)
            A[k] = 0;
        for (uint32_t j = 0; j < STEPS; j++) {
            #pragma omp target data map(tofrom: A) 
            {
                #pragma omp target teams distribute parallel for
                for (uint32_t k = 0; k < N; k ++) {
                    A[k]+=k;
                }
            }
        }
        BENCH_STOP_TIMER;
        BENCH_REGISTER_TEST;
        //Check results
        for (uint32_t k = 0; k < N; k++) {
            if (A[k] != k*STEPS) {
                printf("  === ERROR!!! A[%d] = %d, it should have been %d\n", k, A[k], k*STEPS);
                //break;
            }
        }
    }
    BENCH_TIMER_RESULT("simple_loop_target_data")

    return 0;
}
