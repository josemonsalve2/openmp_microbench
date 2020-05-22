#!/usr/bin/env bash

#module load cuda clang
#./bsub.sh "./execExperiments.sh conf_files/omp_offloading_mt/LU.conf"
#module load cuda pgi/17.10
#module load cuda clang
#export OMP_NUM_THREADS=160
#./execExperiments.sh conf_files/omp_offloading_mt/LU.conf
#./bsub.sh "./execExperiments.sh conf_files/omp_offloading/LU.conf"
#sleep 60
#module load pgi/17.10 cuda
#export OMP_NUM_THREADS=160
./bsub.sh "./execExperiments.sh conf_files/omp_offloading/LU.conf"
#sleep 60
#./execExperiments.sh conf_files/omp_summitdev/LU.conf
#sleep 60
#./execExperiments.sh conf_files/acc_multicore/LU.conf
