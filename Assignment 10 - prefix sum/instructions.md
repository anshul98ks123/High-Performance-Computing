# Instructions to run

### 1. Prefix Sum using MPI

Files:

> 10a_prefix_sum_mpi.c

Compile and run the program:

```
mpicc 10a_prefix_sum_mpi.c -o prefix_sum
mpirun -np 4 ./prefix_sum 
```

### 2. Prefix Sum using OpenMP

Files:

> 10b_prefix_sum_openmp.cpp

Compile and run the program:

```
g++ -fopenmp 10b_prefix_sum_openmp.cpp
./a.out 
```