#include <stdio.h>
#include <stdlib.h>
#include "get_time.h"
#include <omp.h>

int main() {
    BENCH_INIT_TIMERS;
    int isHost = 1;

    BENCH_INIT_TEST;
    for (int i = 0; i < NUM_REP; i ++) {
        isHost = 1;
        BENCH_START_TIMER;
        // For the current cuda graph implementation there must be a loop
        for (int i = 0; i < 2; i++) {
            #pragma omp target defaultmap(tofrom:scalar)
                isHost = omp_is_initial_device();
        }
        BENCH_STOP_TIMER;
        BENCH_REGISTER_TEST
    }
    BENCH_STOP_TIMER;
    BENCH_TIMER_RESULT("target")


    printf("Executing in the %s\n", isHost? "host": "device");
    return 0;
}
