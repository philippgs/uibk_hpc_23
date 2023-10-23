#!/bin/bash

CORES=12
SIZE=1024

# Execute job in the partition "lva" unless you have special requirements.
#SBATCH --partition=lva
# Name your job to be able to identify it later
#SBATCH --job-name heat-seq 
# Redirect output stream to this file
# Maximum number of tasks (=processes) to start in total
#SBATCH --ntasks=$CORES
#SBATCH --output="output-$CORES-$SIZE.log"
# Maximum number of tasks (=processes) to start in total
#SBATCH --exclusive

module load openmpi/4.1.4-oneapi-2022.2.1-oj6kipv


echo "$CORES Core - $SIZE"

mpirun -np $CORES --mca btl_openib_allow_ib 1 ~/uibk_hpc_23/proseminar/03/heat_stencil_2D/build/heat_stencil_1D_mpi $SIZE
mpirun -np $CORES --mca btl_openib_allow_ib 1 ~/uibk_hpc_23/proseminar/03/heat_stencil_2D/build/heat_stencil_1D_mpi_block $SIZE
mpirun -np $CORES --mca btl_openib_allow_ib 1 ~/uibk_hpc_23/proseminar/03/heat_stencil_2D/build/heat_stencil_2D_mpi $SIZE
mpirun -np $CORES --mca btl_openib_allow_ib 1 ~/uibk_hpc_23/proseminar/03/heat_stencil_2D/build/heat_stencil_2D_mpi_block $SIZE
