#!/bin/bash
#SBATCH --job-name=proj3
#SBATCH --output=base.txt
#SBATCH -N 8
#SBATCH -t 00:59:00

mpirun -np 32 ./cs211_proj3_2 10000000000
