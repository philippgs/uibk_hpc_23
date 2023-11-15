# Assignment 7

The goal of this assignment is to implement your parallelization and optimization plan of the n-body simulation of Assignment 6 and experiment with load imbalance.

## Exercise 1 (2 Points)

### Tasks

- Provide an MPI implementation of your parallelization and optimization plan for the n-body simulation of Assignment 6.
- Measure the speedup and efficiency for multiple problem and machine sizes as in previous exercises. If your parallelization and optimization are orthogonal code modifications, try to measure the impact of your optimization separately.
- Illustrate the data in appropriate figures and discuss them. What can you observe? Did the implementation meet your expectations from Assignment 6?
- Add your best parallel wall time for 96 cores, 5000 particles, and 100 time steps into the comparison spreadsheet: https://docs.google.com/spreadsheets/d/18WIigEWPM3htroCkLbLoiVKf2x4J2PtxDbtuYUPTRQQ/edit.

## Exercise 2 (1 Point)

### Tasks

- Modify your n-body simulation to introduce spatial load imbalance.
- Try to mitigate your newly introduced load imbalance using any means discussed so far, and measure your efforts.
- Illustrate the data in appropriate figures and discuss them. What can you observe?
