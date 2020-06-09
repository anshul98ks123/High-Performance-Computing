#include <iostream>

using namespace std;

int a[3][4] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

int b[4][2] = {1, 2, 3, 4, 5, 6, 7, 8};

int c[3][2];
int n = 3, k = 4, m = 2;

void *multiply(void *arg) {
  auto p = (long)arg;
  auto i = p / m, j = p % m;

  int sum = 0;
  for (int x = 0; x < k; x++) sum += a[i][x] * b[x][j];

  c[i][j] = sum;
}

int main() {
  pthread_t threads[n * m];

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      pthread_create(&threads[i * m + j], NULL, multiply,
                     (void *)(long)(i * m + j));
    }
  }

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      pthread_join(threads[i * m + j], NULL);
    }
  }

  cout << "A: \n";
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < k; j++) {
      cout << a[i][j] << " ";
    }
    cout << "\n";
  }

  cout << "\nB: \n";
  for (int i = 0; i < k; i++) {
    for (int j = 0; j < m; j++) {
      cout << b[i][j] << " ";
    }
    cout << "\n";
  }

  cout << "\nC = A * B: \n";
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      cout << c[i][j] << " ";
    }
    cout << "\n";
  }
  return 0;
}