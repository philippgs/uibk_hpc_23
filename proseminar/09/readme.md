# Assignment 9

The goal of this assignment is to investigate energy consumption characteristics.

## Exercise 1 (1 Point)

This exercise consists in familiarizing yourself with energy and power instrumentation on your system.

### Tasks

- Check your CPU model and investigate what power and/or energy instrumentation capabilities it provides
- Briefly describe the scope of the instrumentation in terms of topology that is covered (i.e. which components), resolution/accuracy, etc.
- Find at least one tool that allows you to obtain these measurements for a specific program that you run

## Exercise 2 (1 Point)

This exercise consists in investigating the effect of (not) busy waiting on time, energy and power.

### Description

As you know, MPI is designed to operate at very high performance. As a result, blocking calls such as `MPI_Wait()` are *busy waiting*, i.e. looping at full CPU load until the corresponding non-blocking operation finished. Many MPI implementations allow to change this behavior - OpenMPI features a command line parameter `--mca mpi_yield_when_idle 1` for `mpiexec` that changes this to yielding the CPU instead of busy waiting. Note that the effectiveness of this setting might depend on your OpenMPI version and your operating system. Alternatively, one could also use non-blocking communication along with `MPI_Test()` in a loop where the process yields the current OS scheduling slice using `nanosleep()` in case the operation has not finished yet.

### Tasks

- Write a small MPI program that runs in parallel, but only has one rank do actual work while all ranks need to wait at the end with a collective communication call or barrier. Consider e.g. modifying your solution to the PI approximation such that only one rank participates in the computation, while all the others immediatly reach the `MPI_Gather()` operation and wait there. Ensure an execution time of around 5 seconds.
- Run the program for both cases, busy waiting and yielding. Measure the wall time and energy consumption of your entire program in both cases. Also compute the average power consumption.
- Discuss the effects. What can you observe? How stable are the time and energy measurements?
- Try to oversubscribe, i.e. start more ranks than you have hardware threads available. What can you observe?
- Insert the measured wall time and energy consumption for the number of ranks that equals the number of hardware threads of your system into the comparison spreadsheet: https://docs.google.com/spreadsheets/d/18WIigEWPM3htroCkLbLoiVKf2x4J2PtxDbtuYUPTRQQ/edit
- Note: You can use `--use-hwthread-cpus` to allow OpenMPI to discover hardware threads instead of just physical cores

## Exercise 3 (1 Point)

This exercise consists in investigating the effects of frequency and voltage scaling.

### Tasks

- Download the [stream memory benchmark](https://github.com/jeffhammond/STREAM) and briefly familiarize yourself with it. When you run it, make sure to enable OpenMP to use all hardware threads. Also, consider fixing the thread affinity for best results.
- Find out how to set the CPU core clock frequencies for your hardware platform. Consider e.g. [this page](https://wiki.archlinux.org/title/CPU_frequency_scaling#Setting_maximum_and_minimum_frequencies) in case you need pointers.
- Select at least 3 frequencies and run the benchmark for each of those frequencies. Ideally, try all settings offered by your CPU. Examine the performance offered vs. the power and energy consumed. What can you observe?
- Insert the data points with the highest and lowest measured energy consumption into the comparison spreadsheet: https://docs.google.com/spreadsheets/d/18WIigEWPM3htroCkLbLoiVKf2x4J2PtxDbtuYUPTRQQ/edit