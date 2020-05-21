#!/bin/bash

VANILLA_ROOT="$HOME/Documents/iwomp_async/build/vanilla"
MS_ROOT="$HOME/Documents/iwomp_async/build/ms"
NW_ROOT="$HOME/Documents/iwomp_async/build/nw"

OLD_LD_LIBRARY_PATH=$LD_LIBRARY_PATH

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
  export LD_LIBRARY_PATH=$ROOT/lib:$OLD_LD_LIBRARY_PATH

  for NOWAIT in "nowait" "nonowait"
  do
    for OUT in "out" "in"
    do
      EXE_NAME=bench_$CMPLR\_$NOWAIT\_$OUT
      ./$EXE_NAME | tee $EXE_NAME.log
    done
  done
done
