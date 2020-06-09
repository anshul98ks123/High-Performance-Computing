#include <bits/stdc++.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>

#define MUTEX 0
#define FULL 1
#define EMPTY 2
using namespace std;

int sharedmem_id, semaphore_id, *sharedmem_ptr;
key_t key;

// SEMAPHORE

void createSem(int n) {
  semaphore_id = semget(key, n, IPC_CREAT | 0666);
  if (semaphore_id < 0) {
    cout << "Error in creating semaphore\n";
    exit(1);
  }
}

void initSem(int sem, int val) {
  if (semctl(semaphore_id, sem, SETVAL, val) < 0) {
    cout << "Error in initializing semaphore\n";
    exit(1);
  }
}

void removeSem() {
  if (semctl(semaphore_id, 0, IPC_RMID) < 0) {
    cout << "Error in initializing semaphore\n";
    exit(1);
  }
}

void wait(int sem) {
  sembuf temp;
  temp.sem_num = (unsigned short)sem;
  temp.sem_op = -1;
  temp.sem_flg = 0;
  if (semop(semaphore_id, &temp, 1) < 0) {
    cout << "Error in wait of semaphore" << sem << "\n";
    exit(2);
  }
}

void signal(int sem) {
  sembuf temp;
  temp.sem_num = (unsigned short)sem;
  temp.sem_op = 1;
  temp.sem_flg = 0;
  if (semop(semaphore_id, &temp, 1) < 0) {
    cout << "Error in signal of semaphore" << sem << "\n";
    exit(2);
  }
}

// SHARED MEMORY

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