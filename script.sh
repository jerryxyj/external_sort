#!/bin/bash

#SBATCH --partition=interactive
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --output=1QP.out
#SBATCH --wait-all-nodes=1

hostname

srun ./mycpu 1 QP
