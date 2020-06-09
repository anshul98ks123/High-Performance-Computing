#include "sharedmem.h"
#define N 5
#define M 10

void *Producer(void *args) {
  int count = 0;

  for (int x = 0; x <= M; x++) {
    wait(EMPTY);
    wait(MUTEX);

    int i = *(sharedmem_ptr + N), j = *(sharedmem_ptr + N + 1);

    cout << "Producer: " << count << "\n";

    *(sharedmem_ptr + j % N) = count++;
    *(sharedmem_ptr + N + 1) = (j + 1) % N;

    signal(MUTEX);
    signal(FULL);
  }
}

void *Consumer(void *args) {
  for (int x = 0; x <= M; x++) {
    wait(FULL);
    wait(MUTEX);

    int i = *(sharedmem_ptr + N), j = *(sharedmem_ptr + N + 1);

    cout << "Consumer: " << *(sharedmem_ptr + i % N) << "\n";
    *(sharedmem_ptr + N) = (i + 1) % N;

    signal(MUTEX);
    signal(EMPTY);
  }
}

int main() {
  pthread_t producer, consumer;

  key = ftok(".", 'a');
  createSharedMem(N + 2);
  attachSharedMem();

  createSem(3);
  initSem(FULL, 0);
  initSem(MUTEX, 1);
  initSem(EMPTY, N);

  pthread_create(&producer, NULL, Producer, (void *)NULL);
  pthread_create(&consumer, NULL, Consumer, (void *)NULL);

  pthread_join(producer, NULL);
  pthread_join(consumer, NULL);

  removeSem();
  shmdt(sharedmem_ptr);
  shmctl(sharedmem_id, IPC_RMID, NULL);
}