# Task 1

## SLURM

- scheduling a job: `sbatch`
- monitor a job: `squeue` or `sq`
- cancel a job: `scancel <job-id>`

## Job Script

`./job.sh`

## Options

Options are passed as `sbatch --<option>`

- `output=<filename_pattern>`
  - filename of the output file
  - there exists some patterns, for dynamic output names
- `input=<filename>`
  - to define an input file
  - maybe useful to pipe jobs together
  - pattern are allowed
- `chdir=<directory`>
  - to define the current working directory for relative input and output files
- `test-only`
  - validates the job script and estimates execution start
- `ntasks=<number>`
  - requests CPU resources for the number of tasks
    - default 'filled up' behavior
      - to avoid that use `exclusive` Options
- `ntasks-per-node=<number>`
  - can be a range or a single number
  - restricts the scheduling algorithm

## Parallel Jobs

Slurm executes parallel jobs in a master/slave setup. The master is started, when the scheduler starts the job and then the master distributes the work across the worker. The worker then executed the job, which is in Slurm a `job step`. A job can contain multiple job steps.

To start a job step, use `srun`.

There are some additional options, which are relevant for the parallel job execution:

- `export=<ALL | NONE>`
  - controls the behavior of environment variables
- `cpus-per-task=<number>`
  - fine-tuning the CPU provision
- `mpi`
  - ensuring ranks and communication
