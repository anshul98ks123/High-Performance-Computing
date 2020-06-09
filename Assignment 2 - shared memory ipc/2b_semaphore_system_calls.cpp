#include <bits/stdc++.h>
#include <sys/sem.h>

using namespace std;

int main() {
  key_t key = 10;

  int semaphore_id, val;

  cout << "creating a semaphore \n";
  semaphore_id = semget(key, 1, IPC_CREAT | 0666);

  if (semaphore_id < 0) {
    cout << "Error in creating semaphore\n";
    exit(1);
  }

  cout << "semaphore id = " << semaphore_id << "\n";

  cout << "initializing semaphore to 10 \n";
  if (semctl(semaphore_id, 0, SETVAL, 10) < 0) {
    cout << "Error in initializing semaphore\n";
    exit(1);
  }

  val = semctl(semaphore_id, 0, GETVAL);
  cout << "Value in semaphore: " << val << "\n";

  sembuf temp;
  temp.sem_flg = 0;
  temp.sem_num = 0;
  temp.sem_op = -5;

  cout << "updating semaphore by decreasing by 5\n";
  if (semop(semaphore_id, &temp, 1) == -1) {
    cout << "Error in updating value\n";
  }

  val = semctl(semaphore_id, 0, GETVAL);
  cout << "Updated value: " << val << "\n";

  cout << "removing semaphore \n";
  if (semctl(semaphore_id, 0, IPC_RMID) < 0) {
    cout << "Error in initializing semaphore\n";
    exit(1);
  }

  return 0;
}
