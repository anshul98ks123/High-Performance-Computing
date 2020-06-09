#include <iostream>

#define N 16

using namespace std;

int a[] = {8, 9, 1, 4, -4, 19, 4, 6, 9, 2, 10, 11, -22, 20, 15, 0};

void *Swap(void *arg) {
  auto i = (long)arg;
  auto temp = a[i];
  a[i] = a[i + 1];
  a[i + 1] = temp;
}

void *CompareAndSwap(void *arg) {
  auto i = (long)arg;
  if (a[i] > a[i + 1]) {
    auto temp = a[i];
    a[i] = a[i + 1];
    a[i + 1] = temp;
  }
}

void *merge(void *arg) {
  auto low = (long)arg / N, high = (long)arg % N, mid = (low + high) / 2;
  pthread_t threads[N];

  if (high - low <= 1) {
    CompareAndSwap((void *)(low));
    return (void *)(low);
  }

  //  unshuffle
  for (long i = low + 1, c = (high - low) / 2; i <= mid; i++, c--) {
    for (auto j = 0; j < c; j++)
      pthread_create(&threads[j], NULL, Swap, (void *)(i + 2 * j));

    for (auto j = 0; j < c; j++) pthread_join(threads[j], NULL);
  }

  //  recursive calls
  pthread_create(&threads[0], NULL, merge, (void *)(low * N + mid));
  pthread_create(&threads[1], NULL, merge, (void *)((mid + 1) * N + high));
  pthread_join(threads[0], NULL);
  pthread_join(threads[1], NULL);

  //  shuffle
  for (long i = mid, c = 1; i > low; i--, c++) {
    for (auto j = 0; j < c; j++)
      pthread_create(&threads[j], NULL, Swap, (void *)(i + 2 * j));

    for (auto j = 0; j < c; j++) pthread_join(threads[j], NULL);
  }

  //  Compare and Swap
  for (auto i = low + 1; i < high; i += 2)
    pthread_create(&threads[i], NULL, CompareAndSwap, (void *)(i));

  for (auto i = low + 1; i < high; i += 2) pthread_join(threads[i], NULL);
}

void *mergesort(void *arg) {
  auto low = (long)arg / N, high = (long)arg % N, mid = (low + high) / 2;
  pthread_t threads[2];

  if (high - low == 1) {
    CompareAndSwap((void *)(low));
    return (void *)(low);
  }

  //  Recursive Calls
  pthread_create(&threads[0], NULL, mergesort, (void *)(low * N + mid));
  pthread_create(&threads[1], NULL, mergesort, (void *)((mid + 1) * N + high));
  pthread_join(threads[0], NULL);
  pthread_join(threads[1], NULL);

  //  Merge
  merge((void *)(low * N + high));
}

int main() {
  mergesort((void *)(long)(N - 1));

  cout << "Sorted: ";
  for (int i = 0; i < N; i++) cout << a[i] << " ";
  cout << "\n";
  return 0;
}