#include <iostream>
#define N 8
using namespace std;

int a[N] = {3, 2, 3, 8, 5, 6, 4, 1};

void *compare(void *arg) {
  auto p = (long)arg;

  if (a[p] > a[p + 1]) {
    int t = a[p];
    a[p] = a[p + 1];
    a[p + 1] = t;
  }
}

int main() {
  pthread_t threads[N / 2];

  cout << "Initial array: ";
  for (int i = 0; i < N; i++) cout << a[i] << " ";
  cout << "\n";

  for (int i = 0; i < N; i++) {
    int t = i & 1;

    for (int j = 0; j < N / 2 - (t ? 1 : 0); j++) {
      pthread_create(&threads[j], NULL, compare, (void *)(long)(2 * j + t));
    }

    for (int j = 0; j < N / 2 - (t ? 1 : 0); j++) {
      pthread_join(threads[j], NULL);
    }
  }

  cout << "Sorted array: ";
  for (int i = 0; i < N; i++) cout << a[i] << " ";
  cout << "\n";
  return 0;
}