#ifndef __BENCH_TIMING__
#define __BENCH_TIMING__

#include <math.h>
#include <sys/time.h>
#include <stdint.h>

#include <stdio.h>

#if GET_TIME == 1
#ifndef NUM_REP
#define NUM_REP 3
#endif
#define BENCH_GET_TIME(timer) \
{ \
  struct timeval time;\
  gettimeofday(&time, NULL);\
  timer = (uint64_t)time.tv_sec*1e6 + (uint64_t)time.tv_usec; \
}

#define BENCH_PRINT_TIME_LAPSED(start, stop) \
{ \
  printf("Time(us) = %ju", stop - start)\
}


#define BENCH_INIT_TIMERS \
  uint64_t _BENCH_aux, _BENCH_tmp, _BENCH_start, _BENCH_stop, _BENCH_partial, _BENCH_cur_result = 0, _BENCH_all_results[NUM_REP]; \
  uint64_t _BENCH_accum; \
  double _BENCH_average, _BENCH_std_dev, _BENCH_median;

#define BENCH_TIMING_LOAD \
{ \
  uint64_t volatile __BENCH_b = 0;\
    __BENCH_b++; \
}

#define BENCH_START_TIMER BENCH_GET_TIME(_BENCH_start)
#define BENCH_STOP_TIMER BENCH_GET_TIME(_BENCH_stop)
#define BENCH_GET_TIME_LAPSED (_BENCH_stop - _BENCH_start)

#define BENCH_INIT_TEST \
  for (_BENCH_aux = 0; _BENCH_aux < NUM_REP; _BENCH_aux++) { _BENCH_all_results[_BENCH_aux] = -1; } \
  _BENCH_cur_result = 0; \
  _BENCH_accum = 0; 

// Implementing insertion sort 
#define BENCH_REGISTER_TEST \
   _BENCH_partial = BENCH_GET_TIME_LAPSED; \
  for (_BENCH_aux = 0; _BENCH_aux < _BENCH_cur_result+1; _BENCH_aux++) { \
    if (_BENCH_partial < _BENCH_all_results[_BENCH_aux]) { \
      _BENCH_tmp = _BENCH_all_results[_BENCH_aux]; \
      _BENCH_all_results[_BENCH_aux] = _BENCH_partial; \
      _BENCH_partial = _BENCH_tmp; \
    } \
  } \
  _BENCH_cur_result++;
  


// Find average, median and standard deviation ignore first and last (list of results is sorted)
#define BENCH_TIMER_RESULT(clause) \
  for (_BENCH_aux = 1; _BENCH_aux < NUM_REP-1; _BENCH_aux++) \
    _BENCH_accum += _BENCH_all_results[_BENCH_aux]; \
  _BENCH_average = (double)_BENCH_accum / (NUM_REP-2); \
  _BENCH_accum = 0; \
  for (_BENCH_aux = 1; _BENCH_aux < NUM_REP-1; _BENCH_aux++) \
    _BENCH_accum += pow(_BENCH_all_results[_BENCH_aux] - _BENCH_average, 2); \
  _BENCH_std_dev = sqrt(_BENCH_accum/(NUM_REP-2)); \
  _BENCH_median = ((NUM_REP-2) % 2 == 0) ? (_BENCH_all_results[(NUM_REP-2)/2] + _BENCH_all_results[(NUM_REP-2)/2 + 1])/2 : _BENCH_all_results[(NUM_REP-2)/2 + 1];\
  printf("["clause"] AVG_TIME = %f us, STD_DEV = %f us, MEDIAN = %f us, MAX_TIME = %ju us, MIN_TIME = %ju us\n", _BENCH_average, _BENCH_std_dev, _BENCH_median, _BENCH_all_results[NUM_REP-1], _BENCH_all_results[1]);

#define BENCH_PRINT_VALUES \
  for (_BENCH_aux = 0; _BENCH_aux < NUM_REP; _BENCH_aux++) { printf("%ju", _BENCH_all_results[_BENCH_aux]); }

#elif GET_TIME == 2
// CUDA PROFILER VERSION

#include <cuda.h>
#include <cupti.h>
#include "bench_cupti.h"
#define NUM_REP 1

#define BENCH_INIT_TIMERS  \
initTrace();

#define BENCH_PRINT_TIME_LAPSED(start, stop) 

#define BENCH_TIMING_LOAD \
{ \
  uint64_t volatile __BENCH_b = 0;\
    __BENCH_b++; \
}

#define BENCH_START_TIMER \
  cuptiActivityFlushAll(0); \
  ignoreEventsPrinting = 0; \
  _BENCH_accum_driver = 0; \
  _BENCH_accum_kernel = 0; \
  _BENCH_accum_runtime = 0; \
  _BENCH_accum_memory = 0; \
  _BENCH_accum_others = 0; \
  CUPTI_CALL(cuptiGetTimestamp(&startTimestamp));

#define BENCH_STOP_TIMER  \
      cuptiActivityFlushAll(0);

#define BENCH_INIT_TEST \
  printf("Starting test");

#define BENCH_REGISTER_TEST \
  ignoreEventsPrinting = 1;

#define BENCH_TIMER_RESULT(clause) \
  do { \
    uint64_t stopTimestamp; \
    uint64_t testDuration, cudaDuration; \
    CUPTI_CALL(cuptiGetTimestamp(&stopTimestamp)); \
    testDuration = stopTimestamp - startTimestamp; \
    cudaDuration = _BENCH_accum_driver + _BENCH_accum_kernel + _BENCH_accum_runtime + _BENCH_accum_memory + _BENCH_accum_others; \
    printf("TEST_END \t %s \t %lu \t %lu \t %lu\n", clause, startTimestamp, stopTimestamp, testDuration); \
    printf("TEST_SUMMARY\t%s\t%s\t%s\t%lu\t%lu\n", BENCH_COMPILER_NAME, BENCH_COMPILER_VERSION, clause, testDuration, cudaDuration); \
    fflush(stdout); \
  } while (0);
#else
#define NUM_REP 1
#define BENCH_GET_TIME(timer) 
#define BENCH_PRINT_TIME_LAPSED(start, stop) 
#define BENCH_INIT_TIMERS 
#define BENCH_TIMING_LOAD 
#define BENCH_START_TIMER 
#define BENCH_STOP_TIMER 
#define BENCH_GET_TIME_LAPSED
#define BENCH_INIT_TEST 
#define BENCH_REGISTER_TEST
#define BENCH_TIMER_RESULT(clause) 
#define BENCH_PRINT_VALUES 
#define BENCH_GET_TIME(timer)
#define BENCH_PRINT_TIME_LAPSED(start, stop) 
#define BENCH_INIT_TIMERS 
#define BENCH_TIMING_LOAD 
#define BENCH_START_TIMER 
#define BENCH_STOP_TIMER 
#define BENCH_GET_TIME_LAPSED
#define BENCH_INIT_TEST 
#define BENCH_REGISTER_TEST 
#define BENCH_TIMER_RESULT(clause) 
#define BENCH_PRINT_VALUES 
#endif

#endif /* GUARD */