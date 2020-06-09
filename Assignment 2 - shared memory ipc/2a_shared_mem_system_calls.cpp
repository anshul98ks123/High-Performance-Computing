#include <bits/stdc++.h>
#include <sys/shm.h>

using namespace std;

int sharedmem_id, *sharedmem_ptr;
key_t key;

void createSharedMem(int n) {
  sharedmem_id = shmget(key, n * sizeof(int), IPC_CREAT | 0666);
  if (sharedmem_id < 0) {
    cout << "Error in creating shared memory\n";
    exit(1);
  }
}

void attachSharedMem() {
  sharedmem_ptr = (int *)shmat(sharedmem_id, NULL, 0);
  if ((long)sharedmem_ptr == -1) {
    cout << "Error in attaching shared memory\n";
    exit(1);
  }
}

int main() {
  key = ftok(".", 'a');

  createSharedMem(1);
  attachSharedMem();

  cout << "Writing into shared memory \n";
  *sharedmem_ptr = 10;

  cout << "Reading from shared memory = " << *sharedmem_ptr << "\n";

  // Detach and remove shared memory
  shmdt(sharedmem_ptr);
  shmctl(sharedmem_id, IPC_RMID, NULL);

  return 0;
}