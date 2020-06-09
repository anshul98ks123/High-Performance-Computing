#include "sharedmem.h"

void *Producer(void *args) {
  int count = 0;

  for (int i = 0; i <= 10; i++) {
    wait(EMPTY);
    wait(MUTEX);

    *sharedmem_ptr = count++;
    cout << "Producer : " << *sharedmem_ptr << "\n";

    signal(MUTEX);
    signal(FULL);
  }
}

void *Consumer(void *args) {
  for (int i = 0; i <= 10; i++) {
    wait(FULL);
    wait(MUTEX);

    cout << "Consumer: " << *sharedmem_ptr << "\n";

    signal(MUTEX);
    signal(EMPTY);
  }
}

int main() {
  pthread_t producer, consumer;

  key = ftok(".", 'a');
  createSharedMem(1);
  attachSharedMem();

  createSem(3);
  initSem(FULL, 0);
  initSem(MUTEX, 1);
  initSem(EMPTY, 1);

  pthread_create(&consumer, NULL, Consumer, (void *)NULL);
  pthread_create(&producer, NULL, Producer, (void *)NULL);

  pthread_join(producer, NULL);
  pthread_join(consumer, NULL);

  shmdt(sharedmem_ptr);
  shmctl(sharedmem_id, IPC_RMID, NULL);
}