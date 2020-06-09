#include <iostream>

#define N 16

using namespace std;

int a[] = {8, 9, 1, 4, -4, 19, 4, 6, 9, 2, 10, 11, -22, 20, 15, 0};

void *BM_M2(void *arg) {
  auto i = (long)arg / (N + 1), j = (long)arg % (N + 1);
  if (a[i] < a[j]) {
    auto temp = a[i];
    a[i] = a[j];
    a[j] = temp;
  }
}

void *BM_P2(void *arg) {
  auto i = (long)arg / (N + 1), j = (long)arg % (N + 1);
  if (a[i] > a[j]) {
    auto temp = a[i];
    a[i] = a[j];
    a[j] = temp;
  }
}

void *MergeUp(void *arg) {
  auto low = (long)arg / (N + 1), s = (long)arg % (N + 1);
  auto step = s / 2;

  pthread_t threads[N];

  while (step > 0) {
    for (auto i = low; i < low + s; i += step * 2) {
      for (auto j = 0; j < step; j++) {
        auto k = i + j;
        pthread_create(&threads[i], NULL, BM_P2,
                       (void *)(k * (N + 1) + (k + step)));
      }

      for (auto j = 0; j < step; j++) pthread_join(threads[i], NULL);
    }

    step /= 2;
  }
}

void *MergeDown(void *arg) {
  auto low = (long)arg / (N + 1), s = (long)arg % (N + 1);
  auto step = s / 2;

  pthread_t threads[N];

  while (step > 0) {
    for (auto i = low; i < low + s; i += step * 2) {
      for (auto j = 0; j < step; j++) {
        auto k = i + j;
        pthread_create(&threads[i], NULL, BM_M2,
                       (void *)(k * (N + 1) + (k + step)));
      }

      for (auto j = 0; j < step; j++) pthread_join(threads[i], NULL);
    }

    step /= 2;
  }
}

int main() {
  pthread_t threads[N];

  for (long s = 2; s <= N; s *= 2) {
    for (long i = 0; i < N; i += s * 2) {
      pthread_create(&threads[i], NULL, MergeUp, (void *)(i * (N + 1) + s));

      if (i + s < N)
        pthread_create(&threads[i + s], NULL, MergeDown,
                       (void *)((i + s) * (N + 1) + s));
    }

    for (long i = 0; i < N; i += s * 2) {
      pthread_join(threads[i], NULL);
      if (i + s < N) pthread_join(threads[i + s], NULL);
    }
  }

  cout << "Sorted Array: ";
  for (long i = 0; i < N; i++) cout << a[i] << " ";
  cout << "\n";
  return 0;
}