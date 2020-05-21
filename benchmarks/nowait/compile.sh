#!/bin/bash

VANILLA_ROOT="$HOME/Documents/iwomp_async/build/vanilla"
MS_ROOT="$HOME/Documents/iwomp_async/build/ms"
NW_ROOT="$HOME/Documents/iwomp_async/build/nw"

OLD_LD_LIBRARY_PATH=$LD_LIBRARY_PATH

CXXFLAGS_BASE="-fopenmp -fopenmp-targets=nvptx64 -std=c++14"

# Different combinations
for CMPLR in "nw" "ms" "vanilla"
do
  if [ "$CMPLR" == "nw" ]; then
    ROOT=$NW_ROOT
  elif [ "$CMPLR" == "ms" ]; then
    ROOT=$MS_ROOT
  else
    ROOT=$VANILLA_ROOT
  fi

  CXX=$ROOT/bin/clang++

  for NOWAIT in "nowait" "nonowait"
  do
    if [ "$NOWAIT" == "nowait" ]; then
      CXXFLAGS1="$CXXFLAGS_BASE -DUSE_NOWAIT=1"
    else
      CXXFLAGS1="$CXXFLAGS_BASE"
    fi
    for OUT in "out" "in"
    do
      if [ "$OUT" == "out" ]; then
        CXXFLAGS="$CXXFLAGS1 -DOUTER_MAP=1"
      else
        CXXFLAGS="$CXXFLAGS1"
      fi

      EXE_NAME=bench_$CMPLR\_$NOWAIT\_$OUT

      # echo "$CXX $CXXFLAGS bench.cpp -o $EXE_NAME -O2 -L $ROOT/lib -I $ROOT/include"

      $CXX $CXXFLAGS bench.cpp -o $EXE_NAME -O2 -L $ROOT/lib -I $ROOT/include
    done
  done
done
