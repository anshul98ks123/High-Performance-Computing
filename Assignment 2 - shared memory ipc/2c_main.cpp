#include "sharedmem.h"

int main() {
  key = ftok(".", 'a');
  createSharedMem(1);
  attachSharedMem();

  createSem(3);
  initSem(FULL, 0);
  initSem(MUTEX, 1);
  initSem(EMPTY, 1);
  int count;

  cout << "Sleep while producer and consumer process execute \n";
  usleep(30e6);
  cout << "Sleep completed\n";

  removeSem();
  shmdt(sharedmem_ptr);
  shmctl(sharedmem_id, IPC_RMID, NULL);
}