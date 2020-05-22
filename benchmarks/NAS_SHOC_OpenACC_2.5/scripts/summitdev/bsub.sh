#!/usr/bin/env bash

USAGE="USAGE: bsub.sh /path/to/app"

if [ -z "$1" ]; then
    echo $USAGE
    exit -1
fi

export APP_PATH=$1

# bsub ~/scripts/run_batch.sh
# bsub -alloc_flags gpumps,smt8 -W 30 -nnodes 1 -P GEN010sollve -o jobs`date +%m%d%y_%H%M%S`.o ~/scripts/run_batch.sh
bsub -alloc_flags gpumps -W 2:30 -nnodes 1 -P GEN010sollve -o jobs`date +%m%d%y_%H%M%S`.o ./run_batch.sh
