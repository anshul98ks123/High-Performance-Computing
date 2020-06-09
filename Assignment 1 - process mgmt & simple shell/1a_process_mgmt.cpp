#include <bits/stdc++.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

int main() {
  pid_t id;

  // create a child process
  id = fork();

  if (id < 0) {
    cout << "Fork failed \n";

    // exit with status failure
    exit(1);

  } else if (id == 0) {
    // if process is child

    cout << "(child) PID of parent process: " << getppid() << "\n";
    cout << "(child) PID of child process: " << getpid() << "\n";

    sleep(2);

    // exit with success status
    exit(0);

  } else {
    // if process is parent
    pid_t id = getpid();

    cout << "(parent) PID of parent (itself): " << id << "\n";
    cout << "(parent) waiting for child process to finish\n";

    // wait for child process to finish execution
    pid_t PID_child = wait(NULL);

    if (PID_child == -1) {
      cout << "(parent) No child process\n";
    } else {
      cout << "(parent) Child process completed\n";
      cout << "(parent) PID of returned child: " << PID_child << "\n";
    }
  }
}