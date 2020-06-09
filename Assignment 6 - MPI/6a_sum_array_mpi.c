#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define max_rows 100000
#define send_data_tag 2001
#define return_data_tag 2002

int array[max_rows];
int array2[max_rows];

int main(int argc, char **argv) {
  long int sum, partial_sum;
  MPI_Status status;
  int my_id, root_process, ierr, i, num_rows, num_procs, an_id,
      num_rows_to_receive, avg_rows_per_process, sender, num_rows_received,
      start_row, end_row, num_rows_to_send;

  ierr = MPI_Init(&argc, &argv);

  root_process = 0;

  // find id of process and number of processes
  ierr = MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
  ierr = MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

  if (my_id == root_process) {
    printf("please enter the size of array: ");
    scanf("%i", &num_rows);

    if (num_rows > max_rows) {
      printf("Too many numbers.\n");
      exit(1);
    }

    avg_rows_per_process = num_rows / num_procs;

    // initialize
    for (i = 0; i < num_rows; i++) {
      array[i] = i + 1;
    }

    // distribute a portion of array to each slave process
    for (an_id = 1; an_id < num_procs; an_id++) {
      start_row = an_id * avg_rows_per_process + 1;
      end_row = (an_id + 1) * avg_rows_per_process;

      if (an_id == num_procs - 1) end_row = num_rows - 1;
      num_rows_to_send = end_row - start_row + 1;

      ierr = MPI_Send(&num_rows_to_send, 1, MPI_INT, an_id, send_data_tag,
                      MPI_COMM_WORLD);

      ierr = MPI_Send(&array[start_row], num_rows_to_send, MPI_INT, an_id,
                      send_data_tag, MPI_COMM_WORLD);
    }

    // compute sum of the segment assigned to the root process
    sum = 0;
    for (i = 0; i < avg_rows_per_process + 1; i++) {
      sum += array[i];
    }

    printf("Partial sum %i calculated by root process\n", sum);

    // collect the partial sums from the slave processes
    // add them to get the sum of array
    for (an_id = 1; an_id < num_procs; an_id++) {
      ierr = MPI_Recv(&partial_sum, 1, MPI_LONG, MPI_ANY_SOURCE,
                      return_data_tag, MPI_COMM_WORLD, &status);

      sender = status.MPI_SOURCE;

      printf("Partial sum %i returned from process %i\n", partial_sum, sender);

      sum += partial_sum;
    }

    printf("The Sum of array elements is: %i\n", sum);
    printf("Expected Sum: %i\n", (num_rows * (num_rows + 1)) / 2);
  }

  else {
    // slave process - receive array segment and store it in a "local" array
    ierr = MPI_Recv(&num_rows_to_receive, 1, MPI_INT, root_process,
                    send_data_tag, MPI_COMM_WORLD, &status);

    ierr = MPI_Recv(&array2, num_rows_to_receive, MPI_INT, root_process,
                    send_data_tag, MPI_COMM_WORLD, &status);

    num_rows_received = num_rows_to_receive;

    // Calculate the sum of portion of the array
    partial_sum = 0;
    for (i = 0; i < num_rows_received; i++) {
      partial_sum += array2[i];
    }

    // send partial sum to hte root process
    ierr = MPI_Send(&partial_sum, 1, MPI_LONG, root_process, return_data_tag,
                    MPI_COMM_WORLD);
  }

  ierr = MPI_Finalize();
}