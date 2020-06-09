#include <omp.h>

#include <iostream>

#define N 16

using namespace std;

int a[] = {8, 9, 1, 4, -4, 19, 4, 6, 9, 2, 10, 11, -22, 20, 15, 0};

void *BM_M2(long i, long j) {
  if (a[i] < a[j]) {
    auto temp = a[i];
    a[i] = a[j];
    a[j] = temp;
  }
}

void *BM_P2(long i, long j) {
  if (a[i] > a[j]) {
    auto temp = a[i];
    a[i] = a[j];
    a[j] = temp;
  }
}

void *MergeUp(long low, long s) {
  auto step = s / 2;

  while (step > 0) {
    for (auto i = low; i < low + s; i += step * 2) {
#pragma omp parallel for
      for (auto j = 0; j < step; j++) {
        auto k = i + j;
        BM_P2(k, k + step);
      }
    }

    step /= 2;
  }
}

void *MergeDown(long low, long s) {
  auto step = s / 2;

  while (step > 0) {
    for (auto i = low; i < low + s; i += step * 2) {
#pragma omp parallel for
      for (auto j = 0; j < step; j++) {
        auto k = i + j;
        BM_M2(k, k + step);
      }
    }

    step /= 2;
  }
}

int main() {
  for (long s = 2; s <= N; s *= 2) {
    for (long i = 0; i < N; i += s * 2) {
      MergeUp(i, s);

      if (i + s < N) MergeDown(i + s, s);
    }
  }

  cout << "Sorted Array: ";
  for (long i = 0; i < N; i++) cout << a[i] << " ";
  cout << "\n";
  return 0;
}