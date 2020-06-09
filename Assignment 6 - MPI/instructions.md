# Instructions to run

### Sum of Array using MPI

Files:

> 6a_sum_array_mpi.c

Compile and run the program:

```
mpicc 6a_sum_array_mpi.c -o sum
mpirun -np 4 ./sum 
```