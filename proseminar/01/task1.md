Team collaboration of Hendrik Munske and Florian Neururer
# Task 1

## SLURM

- scheduling a job: `sbatch [scriptname]`
- monitor a job: `squeue` or `sq`
- cancel a job: `scancel <job-id>`

## Job Script

script provided in `job1.sh`

## Options

1. **`--ntasks`**: This is the number of tasks to run for the job and directly correlates with how many cores will be used. 
2. **`--cpus-per-task`**: Specifies the number of CPUs allocated per task. This is useful for multi-threaded applications and can improve CPU utilization, thus potentially improving performance.
3. **`--ntasks-per-node`**: Controls the number of tasks that can be run per node. For some parallel jobs, performance can be optimized by controlling how tasks are distributed across nodes.
4. **`--mem`**: Allocates a specific amount of memory for the job. If the job is memory-intensive, ensuring that it has enough memory can significantly affect performance. Running out would mean swapping to disk, which is much slower.
5. **`--exclusive`**: Requests exclusive use of a node, preventing other jobs from running on the same node. This ensures that no other jobs will contend for the resources on the node your job is running on, which can result in more consistent and potentially better performance.

#### Possible settings and effects:

- `--ntasks`: An integer specifying the number of tasks, determining how many cores are used for the job.

- `--cpus-per-task`: An integer specifying the number of CPUs per task.
  
- `--ntasks-per-node`: An integer that limits the number of tasks per node, useful for optimizing data locality in some parallel algorithms.
  
- `--mem`: Specifies memory in megabytes or can be suffixed with `G` for gigabytes, e.g., `--mem=20G`.
  
- `--exclusive`: No argument needed. Ensures that the node is exclusively used for this job, eliminating resource contention.

## Parallel Jobs

Running the program in parallel depends on how the program is written. For example, in our case if it is written using MPI, we can use mpirun in the SLURM script to start the parallel execution.

```
#!/bin/bash
#SBATCH --ntasks=4
mpiexec -n $SLURM_NTASKS program
```

For environment setup, we need to load appropriate modules or set environment variables, depending on the software stack required for our parallel program.

In our case we need to load the `openmpi` module in the `job.sh` script before we run it using `mpiexec` in order to run the our program using MPI.

```
module load openmpi/3.1.6-gcc-12.2.0-d2gmn55
```
