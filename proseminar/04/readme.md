# Assignment 4

The goal of this assignment is to analyze and possibly optimize the heat stencil application.

## Exercise 1 (3 Points)

This exercise consists in profiling and analyzing the performance of the heat stencil application.

### Tasks

- Do a detailed performance analysis on your 1D and 2D heat stencil implementation. Use either tools discussed in the lecture (`perf`, `gprof`, `gperftools`, etc.) or any tools you deem fit for generating a performance profile.
- Provide a report that discusses the most expensive source code locations ("hot spots") along with explaining why they are expensive and how to possibly improve on that. Compare blocking and non-blocking if possible, as well as 1D and 2D.
- Consider improving your 2D stencil using the gathered information. If you do, document the improvement you managed to achieve. If you don't, argue why the application performance cannot be feasibly improved in your opinion.
- Insert the final walltime and speedup of the 2D stencil for 96 cores for N=768x768 and T=768x768x100 into the comparison spreadsheet: https://docs.google.com/spreadsheets/d/18WIigEWPM3htroCkLbLoiVKf2x4J2PtxDbtuYUPTRQQ/edit