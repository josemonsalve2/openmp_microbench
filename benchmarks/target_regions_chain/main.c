#include <stdio.h>
#include <stdlib.h>
#include "get_time.h"
#include <omp.h>

#define STEPS 200
#define N 1000

int main() {
    BENCH_INIT_TIMERS;
    double A[N];

    BENCH_INIT_TEST;
    for (int i = 0; i < NUM_REP; i ++) {
        BENCH_START_TIMER;
        for (uint32_t k = 0; k < N; k ++) {
            A[k] = 0;
        }
        for (int j = 0; j < STEPS; j++) {
            #pragma omp target data map (tofrom: A)
            {
                #pragma omp target teams distribute parallel for
                for (uint32_t k=0; k < N; k++ )
                    A[k] = 0;
                #pragma omp target teams distribute parallel for
                for (uint32_t k=0; k < N; k++ )
                    A[k] += k;
                #pragma omp target teams distribute parallel for
                for (uint32_t k=0; k < N; k++ )
                    A[k] *= 2;
                #pragma omp target teams distribute parallel for
                for (uint32_t k=0; k < N; k++ )
                    A[k] /= 3;
                #pragma omp target teams distribute parallel for
                for (uint32_t k=0; k < N; k++ )
                    A[k] *= 5;
                #pragma omp target teams distribute parallel for
                for (uint32_t k=0; k < N; k++ )
                    A[k] /= 7;
                #pragma omp target teams distribute parallel for
                for (uint32_t k=0; k < N; k++ )
                    A[k] *= 9;
                #pragma omp target teams distribute parallel for
                for (uint32_t k=0; k < N; k++ )
                    A[k] *= 11;
            }
        }
        BENCH_STOP_TIMER;
        BENCH_REGISTER_TEST
        for (uint32_t k = 0; k < N; k++ ) {
            double val = (((((((double)k*2)/3)*5)/7)*9)*11);
            if (fabs(A[k] - val ) > 0.00001) {
                printf("ERROR! A[%d] = %f != %f \n", k, A[k], val );
                //break;
            }
        }
    }
    BENCH_STOP_TIMER;
    BENCH_TIMER_RESULT("target_region_chain")

    return 0;
}
