# Exercise 2

- Download and build the OSU Micro-Benchmarks available at http://mvapich.cse.ohio-state.edu/download/mvapich/osu-micro-benchmarks-5.8.tgz. Do not forget to set the compiler parameters for `configure`, e.g. `./configure CC=mpicc CXX=mpic++ ...`

- After building, submit SLURM jobs that run the `osu_latency` and `osu_bw` executables.

- Create a table and figures that illustrate the measured data and study them. What effects can you observe?

- Find out more about the hardware that you're running on, e.g. by using `lstopo --of txt` (available via `module load hwloc`). Modify your experiment such that the 2 MPI ranks are placed on

  - different cores of the same socket,
  - different sockets of the same node, and
  - different nodes.

- Amend your table and figures to include these additional measurements. What effects can you observe? How can you verify rank placement without looking at performance?

- How stable are the measurements when running the experiments multiple times?

- Insert the measured time for latency (size 0) and bandwidth (size 1048576) into the comparison spreadsheet: https://docs.google.com/spreadsheets/d/1E-9kRGMV8Py1Qp32wuVHs7dWSXIWigBHc3Ba1iTheFc/edit?usp=sharing
