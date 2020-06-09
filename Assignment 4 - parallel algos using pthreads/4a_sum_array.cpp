#include <iostream>

#define MAX 16
#define MAX_THREAD (MAX / 2)

using namespace std;

int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
int b[16] = {0};
int k = 0;

void *sum_array(void *arg) {
  auto thread_part = (long)arg;
  if (k) {
    a[thread_part] = 0;
  } else {
    b[thread_part] = 0;
  }

  for (auto i = thread_part * 2; i < (thread_part + 1) * 2; i++) {
    if (k) {
      a[thread_part] += b[i];
    } else {
      b[thread_part] += a[i];
    }
  }
}

int main() {
  pthread_t threads[MAX_THREAD];

  int iter = (MAX / 4), t = 1;

  cout << "array : ";
  for (int i = 0; i < 16; i++) {
    cout << a[i] << " ";
  }

  while (iter--) {
    for (int i = 0; i < MAX_THREAD / t; i++)
      pthread_create(&threads[i], NULL, sum_array, (void *)(long)i);

    for (int i = 0; i < MAX_THREAD / t; i++) pthread_join(threads[i], NULL);

    k = k ^ 1;
    t *= 2;
  }

  cout << "\nsum is " << a[0] << "\n";
  return 0;
}