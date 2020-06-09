#include <bits/stdc++.h>
#include <omp.h>

using namespace std;

int main() {
  int size = 16;
  int arr[size], prefix[size], t[size];
  int num_proc = 4;

  for (int i = 0; i < size; i++) {
    arr[i] = 1;
    t[i] = 0;
  }

  omp_set_num_threads(num_proc);
#pragma omp parallel
  {
    int id = omp_get_thread_num(), temp = 0;
    int blocksize = size / num_proc;
    int start = blocksize * id, end = (id + 1) * blocksize;

    if (id == num_proc - 1) end = size;

    for (int i = start; i < end; i++) {
      temp += arr[i];
      prefix[i] = temp;
    }

    t[id + 1] = temp;
  }

  for (int i = 1; i < num_proc; i++) {
    t[i] += t[i - 1];
  }

#pragma omp parallel
  {
    int id = omp_get_thread_num(), temp = t[id];
    int blocksize = size / num_proc;
    int start = blocksize * id, end = (id + 1) * blocksize;

    if (id == num_proc - 1) end = size;

    for (int i = start; i < end; i++) {
      prefix[i] += temp;
    }
  }

  cout << "Prefix sums = ";
  for (int i = 0; i < size; i++) {
    cout << prefix[i] << " ";
  }
  cout << "\n";
  return 0;
}