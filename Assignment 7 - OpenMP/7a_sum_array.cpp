#include <bits/stdc++.h>
#include <omp.h>

using namespace std;

int main() {
  int sum = 0, size = 50;
  int arr[size];

  for (int i = 0; i < size; i++) {
    arr[i] = i + 1;
  }

  omp_set_num_threads(8);
#pragma omp parallel for reduction(+ : sum)
  for (int i = 0; i < size; i++) {
    sum += arr[i];
  }

  cout << "Sum of array = " << sum << "\n";
  return 0;
}