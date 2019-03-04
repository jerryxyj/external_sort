#!/bin/sh
#SBATCH --partition=compute
#SBATCH --job-name=linsort
#SBATCH --nodes=1
#SBATCH --ntasks=1
##SBATCH --time=0:20:00
##SBATCH --wait-all-node=1
#SBATCH --output=linsort2GB.log
echo 'Time is'
srun ./linsort2GB.sh
