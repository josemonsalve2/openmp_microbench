#!/usr/bin/env bash
# Begin LSF directives
#BSUB -P GEN010SOLLVE
#BSUB -J batch_test
#BSUB -o job.o
#BSUB -W 2
#BSUB -nnodes 1
#BSUB -env "all"
#BSUB -alloc_flags gpumps
# End LSF directives and begin shell commands

echo "APP_PATH: $APP_PATH, using debug: '$DEBUG'"

# module add cuda/8.0.61-1 cmake clang/20170925
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/ibm/spectrum_mpi/lib/:/sw/summitdev/cuda/9.0.69/extras/CUPTI/lib64

# jsrun -n1 -a1 -g1 -c$nt -r1 -E LD_LIBRARY_PATH=$LD_LIBRARY_PATH -E OMP_NUM_THREADS $DEBUG $APP_PATH
jsrun -n1 -a1 -g1 -lgpu-cpu $DEBUG $APP_PATH
# jsrun -n1 -a1 -g1 -r1 -E LD_LIBRARY_PATH=$LD_LIBRARY_PATH $DEBUG $APP_PATH
