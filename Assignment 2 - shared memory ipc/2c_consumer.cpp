#include "sharedmem.h"

int main() {
  key = ftok(".", 'a');
  createSharedMem(1);
  attachSharedMem();

  createSem(3);
  int count;

  for (int i = 0; i <= 10; i++) {
    wait(FULL);
    wait(MUTEX);

    cout << "Consumer: " << *sharedmem_ptr << "\n";

    signal(MUTEX);
    signal(EMPTY);
  }
}