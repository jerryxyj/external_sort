#!/bin/sh
#SBATCH --partition=compute
#SBATCH --job-name=mysort
#SBATCH --nodes=1
#SBATCH --ntasks=1
##SBATCH --time=0:20:00
##SBATCH --wait-all-node=1

hostname

srun ./mysort 20G



echo 'Done'

