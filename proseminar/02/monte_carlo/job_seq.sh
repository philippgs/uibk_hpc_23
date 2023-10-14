#!/bin/bash 
 
# Execute job in the partition "lva" unless you have special requirements. 
#SBATCH --partition=lva 
# Name your job to be able to identify it later 
#SBATCH --job-name pi 
# Redirect output stream to this file 
#SBATCH --output=output.log 
# Enforce exclusive node allocation, do not share with other jobs 
#SBATCH --exclusive 
~/mpi_seq 1000000000
