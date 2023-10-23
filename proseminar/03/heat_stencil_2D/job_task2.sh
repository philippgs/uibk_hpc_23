#!/bin/bash

# Execute job in the partition "lva" unless you have special requirements.
#SBATCH --partition=lva
# Name your job to be able to identify it later
#SBATCH --job-name heat-seq 
# Redirect output stream to this file
#SBATCH --output=output.log
# Maximum number of tasks (=processes) to start in total
#SBATCH --exclusive

module load openmpi/4.1.4-oneapi-2022.2.1-oj6kipv

make all

echo "12 Core - 1024"

mpirun -np 12 --mca btl_openib_allow_ib 1 ~/build/heat_stencil_1D_mpi 1024
mpirun -np 12 --mca btl_openib_allow_ib 1 ~/build/heat_stencil_1D_mpi_block 1024
mpirun -np 12 --mca btl_openib_allow_ib 1 ~/build/heat_stencil_2D_mpi 1024
mpirun -np 12 --mca btl_openib_allow_ib 1 ~/build/heat_stencil_2D_mpi_block 1024


echo "12 Core - 2048"

mpirun -np 12 --mca btl_openib_allow_ib 1 ~/build/heat_stencil_1D_mpi 2048
mpirun -np 12 --mca btl_openib_allow_ib 1 ~/build/heat_stencil_1D_mpi_block 2048
mpirun -np 12 --mca btl_openib_allow_ib 1 ~/build/heat_stencil_2D_mpi 2048
mpirun -np 12 --mca btl_openib_allow_ib 1 ~/build/heat_stencil_2D_mpi_block 2048

echo "12 Core - 4096"

mpirun -np 12 --mca btl_openib_allow_ib 1 ~/build/heat_stencil_1D_mpi 4096
mpirun -np 12 --mca btl_openib_allow_ib 1 ~/build/heat_stencil_1D_mpi_block 4096
mpirun -np 12 --mca btl_openib_allow_ib 1 ~/build/heat_stencil_2D_mpi 4096
mpirun -np 12 --mca btl_openib_allow_ib 1 ~/build/heat_stencil_2D_mpi_block 4096

echo "36 Core - 1024"

mpirun -np 36 --mca btl_openib_allow_ib 1 ~/build/heat_stencil_1D_mpi 1024
mpirun -np 36 --mca btl_openib_allow_ib 1 ~/build/heat_stencil_1D_mpi_block 1024
mpirun -np 36 --mca btl_openib_allow_ib 1 ~/build/heat_stencil_2D_mpi 1024
mpirun -np 36 --mca btl_openib_allow_ib 1 ~/build/heat_stencil_2D_mpi_block 1024


echo "36 Core - 2048"

mpirun -np 36 --mca btl_openib_allow_ib 1 ~/build/heat_stencil_1D_mpi 2048
mpirun -np 36 --mca btl_openib_allow_ib 1 ~/build/heat_stencil_1D_mpi_block 2048
mpirun -np 36 --mca btl_openib_allow_ib 1 ~/build/heat_stencil_2D_mpi 2048
mpirun -np 36 --mca btl_openib_allow_ib 1 ~/build/heat_stencil_2D_mpi_block 2048

echo "36 Core - 4096"

mpirun -np 36 --mca btl_openib_allow_ib 1 ~/build/heat_stencil_1D_mpi 4096
mpirun -np 36 --mca btl_openib_allow_ib 1 ~/build/heat_stencil_1D_mpi_block 4096
mpirun -np 36 --mca btl_openib_allow_ib 1 ~/build/heat_stencil_2D_mpi 4096
mpirun -np 36 --mca btl_openib_allow_ib 1 ~/build/heat_stencil_2D_mpi_block 4096

echo "96 Core - 1024"

mpirun -np $SLURM_NTASKS --mca btl_openib_allow_ib 1 ~/build/heat_stencil_1D_mpi 1024
mpirun -np $SLURM_NTASKS --mca btl_openib_allow_ib 1 ~/build/heat_stencil_1D_mpi_block 1024
mpirun -np $SLURM_NTASKS --mca btl_openib_allow_ib 1 ~/build/heat_stencil_2D_mpi 1024
mpirun -np $SLURM_NTASKS --mca btl_openib_allow_ib 1 ~/build/heat_stencil_2D_mpi_block 1024


echo "96 Core - 2048"

mpirun -np $SLURM_NTASKS --mca btl_openib_allow_ib 1 ~/build/heat_stencil_1D_mpi 2048
mpirun -np $SLURM_NTASKS --mca btl_openib_allow_ib 1 ~/build/heat_stencil_1D_mpi_block 2048
mpirun -np $SLURM_NTASKS --mca btl_openib_allow_ib 1 ~/build/heat_stencil_2D_mpi 2048
mpirun -np $SLURM_NTASKS --mca btl_openib_allow_ib 1 ~/build/heat_stencil_2D_mpi_block 2048

echo "96 Core - 4096"

mpirun -np $SLURM_NTASKS --mca btl_openib_allow_ib 1 ~/build/heat_stencil_1D_mpi 4096
mpirun -np $SLURM_NTASKS --mca btl_openib_allow_ib 1 ~/build/heat_stencil_1D_mpi_block 4096
mpirun -np $SLURM_NTASKS --mca btl_openib_allow_ib 1 ~/build/heat_stencil_2D_mpi 4096
mpirun -np $SLURM_NTASKS --mca btl_openib_allow_ib 1 ~/build/heat_stencil_2D_mpi_block 4096
