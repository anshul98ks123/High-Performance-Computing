#include <bits/stdc++.h>
#include <omp.h>

using namespace std;

int main() {
  int largest = INT_MIN, size = 20;
  int arr[size];

  for (int i = 0; i < size; i++) {
    arr[i] = i + 1;
  }

  omp_set_num_threads(8);
#pragma omp parallel for reduction(max : largest)
  for (int i = 0; i < size; i++) {
    largest = max(largest, arr[i]);
  }

  cout << "Largest of array = " << largest << "\n";
  return 0;
}