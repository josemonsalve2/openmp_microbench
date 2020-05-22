cmake_minimum_required(VERSION 2.8.9)
project(benchmarks)

include_directories(include)

option(GET_TIME "If 1 we do regular timing measurement, if 2 we get CUPTI values (no repetitions), otherwise no timing meassurement" 0)
add_definitions( -DGET_TIME=${GET_TIME})

if (${GET_TIME} EQUAL 2)
  if( NOT CUDA_HOME)
    set(CUDA_HOME "/usr/lib/cuda")
  endif()

  include_directories(${CUDA_HOME}/extras/CUPTI/include)
  link_directories(${CUDA_HOME}/extras/CUPTI/lib64)
endif()

if (NUM_REP)
  add_definitions( -DNUM_REP=${NUM_REP})
  add_definitions( -DNUM_REP=${NUM_REP})
endif()

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_C_COMPILER clang)

# FLAGS FOR ALL CASES 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -fopenmp -fopenmp-targets=nvptx64-nvidia-cuda")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -fopenmp -fopenmp-targets=nvptx64-nvidia-cuda")


## CHECK FOR DEBUG MODE
if (DEBUG)
  set(CMAKE_BUILD_TYPE Debug)
else ()
  set(CMAKE_BUILD_TYPE Release)
endif (DEBUG)

## CHECK FOR VERBOSE EXECUTION MODE
if (VERBOSE_MODE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVERBOSE_MODE=${VERBOSE_MODE}")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DVERBOSE_MODE=${VERBOSE_MODE}")
endif (VERBOSE_MODE)


## COMPILER SPECIFIC
if (NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  message(FATAL_ERROR " It should be clang only ${CMAKE_CXX_COMPILER_ID}")
endif()
if (NOT "${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
  message(FATAL_ERROR " It should be clang only ${CMAKE_C_COMPILER_ID}")
endif()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -std=c++17")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0 -std=c11")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g -O3 -std=c++17")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -g -O3")


add_subdirectory (benchmarks)
