#!/bin/bash

# Execute job in the partition "lva" unless you have special requirements.
#SBATCH --partition=lva
# Name your job to be able to identify it later
#SBATCH --job-name heat-mpi
# Redirect output stream to this file
#SBATCH --output=output.log
# Maximum number of tasks (=processes) to start in total
#SBATCH --ntasks=64
# Enforce exclusive node allocation, do not share with other jobs
#SBATCH --exclusive

module load openmpi/3.1.6-gcc-12.2.0-d2gmn55
mpiexec ~/heat_stencil_1D_mpi 4096
