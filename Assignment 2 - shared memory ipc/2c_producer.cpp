#include "sharedmem.h"

int main() {
  key = ftok(".", 'a');
  createSharedMem(1);
  attachSharedMem();

  createSem(3);
  int count = 0;

  for (int i = 0; i <= 10; i++) {
    wait(EMPTY);
    wait(MUTEX);

    *sharedmem_ptr = count++;
    cout << "Producer: " << *sharedmem_ptr << "\n";

    signal(MUTEX);
    signal(FULL);
  }
}