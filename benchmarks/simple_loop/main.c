#include <stdio.h>
#include <stdlib.h>
#include "get_time.h"
#include <omp.h>
#define N 100

int main() {
    BENCH_INIT_TIMERS;
    int isHost = 1;
    int aVar = 0;

    BENCH_INIT_TEST;
    for (int i = 0; i < NUM_REP; i ++) {
        aVar = 0;
        BENCH_START_TIMER;
        for (int k = 0; k < N; k ++) {
            #pragma omp target defaultmap(tofrom:scalar)
            {
                isHost = omp_is_initial_device();
                aVar++;
            }
        }
        BENCH_STOP_TIMER;
        if (aVar != N) printf("  === ERROR!!! aVar = %d, it should have been %d\n", aVar, N);
        BENCH_REGISTER_TEST;
    }
    BENCH_TIMER_RESULT("simple loop scalar")


    printf("Executing in the %s\n", isHost? "host": "device");
    return 0;
}
