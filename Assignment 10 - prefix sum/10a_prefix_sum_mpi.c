#include <mpi.h>
#include <stdio.h>

#define max_rows 100000
#define send_data_tag 2001
#define return_data_tag 2002

int array[max_rows];
int temp[max_rows];
int array2[max_rows];
int prefix[max_rows];

main(int argc, char **argv) {
  int sum, partial_sum, k;
  MPI_Status status;
  int my_id, root_process, ierr, i, num_rows, num_procs, an_id,
      num_rows_to_receive, avg_rows_per_process, sender, num_rows_received,
      start_row, end_row, num_rows_to_send;

  ierr = MPI_Init(&argc, &argv);

  root_process = 0;

  ierr = MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
  ierr = MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

  if (my_id == root_process) {
    printf("please enter the size of array to find prefix sum: ");
    scanf("%i", &num_rows);

    avg_rows_per_process = num_rows / num_procs;

    for (i = 0; i < num_rows; i++) array[i] = 1;

    for (an_id = 1; an_id < num_procs; an_id++) {
      start_row = an_id * avg_rows_per_process;
      end_row = (an_id + 1) * avg_rows_per_process;

      if (an_id == num_procs - 1) end_row = num_rows;

      num_rows_to_send = end_row - start_row;

      ierr = MPI_Send(&num_rows_to_send, 1, MPI_INT, an_id, send_data_tag,
                      MPI_COMM_WORLD);

      ierr = MPI_Send(&array[start_row], num_rows_to_send, MPI_INT, an_id,
                      send_data_tag, MPI_COMM_WORLD);
    }

    sum = 0;
    for (i = 0; i < avg_rows_per_process; i++) {
      sum += array[i];
    }

    temp[0] = 0;
    temp[1] = sum;

    printf("Partial sum %i calculated by root process\n", sum);

    for (an_id = 1; an_id < num_procs; an_id++) {
      ierr = MPI_Recv(&partial_sum, 1, MPI_INT, MPI_ANY_SOURCE, return_data_tag,
                      MPI_COMM_WORLD, &status);

      sender = status.MPI_SOURCE;

      printf("Partial sum %i returned from process %i\n", partial_sum, sender);

      if (sender < num_procs - 1) {
        temp[sender + 1] = partial_sum;
      }
    }

    printf("T array = %d ", temp[0]);
    for (int i = 1; i < num_procs; i++) {
      temp[i] += temp[i - 1];
      printf("%d ", temp[i]);
    }
    printf("\n");

    for (an_id = 1; an_id < num_procs; an_id++) {
      start_row = an_id * avg_rows_per_process;
      end_row = (an_id + 1) * avg_rows_per_process;

      if (an_id == num_procs - 1) end_row = num_rows;

      num_rows_to_send = end_row - start_row;

      ierr = MPI_Send(&num_rows_to_send, 1, MPI_INT, an_id, send_data_tag,
                      MPI_COMM_WORLD);

      ierr = MPI_Send(&array[start_row], num_rows_to_send, MPI_INT, an_id,
                      send_data_tag, MPI_COMM_WORLD);

      ierr = MPI_Send(&temp[an_id], 1, MPI_INT, an_id, send_data_tag,
                      MPI_COMM_WORLD);
    }

    k = 0;
    for (int i = 0; i < avg_rows_per_process; i++) {
      k += array[i];
      prefix[i] = k;
    }

    for (an_id = 1; an_id < num_procs; an_id++) {
      start_row = an_id * avg_rows_per_process;
      end_row = (an_id + 1) * avg_rows_per_process;

      if (an_id == num_procs - 1) end_row = num_rows;

      num_rows_to_receive = end_row - start_row;

      ierr = MPI_Recv(&array2, num_rows_to_receive, MPI_INT, an_id,
                      return_data_tag, MPI_COMM_WORLD, &status);

      sender = status.MPI_SOURCE;

      for (int i = start_row; i < end_row; i++) {
        prefix[i] = array2[i - start_row];
      }
    }

    printf("Prefix sums = ");
    for (int i = 0; i < num_rows; i++) {
      printf("%d ", prefix[i]);
    }
    printf("\n");

  } else {
    ierr = MPI_Recv(&num_rows_to_receive, 1, MPI_INT, root_process,
                    send_data_tag, MPI_COMM_WORLD, &status);

    ierr = MPI_Recv(&array2, num_rows_to_receive, MPI_INT, root_process,
                    send_data_tag, MPI_COMM_WORLD, &status);

    num_rows_received = num_rows_to_receive;

    partial_sum = 0;
    for (i = 0; i < num_rows_received; i++) {
      partial_sum += array2[i];
    }

    ierr = MPI_Send(&partial_sum, 1, MPI_INT, root_process, return_data_tag,
                    MPI_COMM_WORLD);

    int tobeadded, start;
    ierr = MPI_Recv(&num_rows_to_receive, 1, MPI_INT, root_process,
                    send_data_tag, MPI_COMM_WORLD, &status);

    ierr = MPI_Recv(&array2, num_rows_to_receive, MPI_INT, root_process,
                    send_data_tag, MPI_COMM_WORLD, &status);

    ierr = MPI_Recv(&tobeadded, 1, MPI_INT, root_process, send_data_tag,
                    MPI_COMM_WORLD, &status);

    num_rows_received = num_rows_to_receive;

    array2[0] += tobeadded;
    for (int i = 1; i < num_rows_received; i++) {
      array2[i] += array2[i - 1];
    }

    ierr = MPI_Send(&array2, num_rows_received, MPI_INT, root_process,
                    return_data_tag, MPI_COMM_WORLD);
  }

  ierr = MPI_Finalize();
}