#include <bits/stdc++.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

int parse(char *input, char **parsed) {
  int i = 0;
  while (true) {
    parsed[i] = strsep(&input, " ");
    if (parsed[i] == NULL) break;

    if (strlen(parsed[i]) == 0) i--;

    i++;
  }

  return i;
}

void cd(char **parsed) {
  if (chdir(parsed[1]) != 0) {
    perror("shell");
  }
}

void execute(char **parsed, int N) {
  if (strcmp(parsed[0], "cd") == 0) {
    cd(parsed);
    return;
  }

  pid_t pid = fork();

  if (pid == -1) {
    perror("shell");

  } else if (pid == 0) {
    if (execvp(parsed[0], parsed) < 0) {
      perror("shell");
    }

    exit(0);
  } else {
    if (strcmp(parsed[N - 1], "&")) {
      int status;
      wait(&status);

      if (WIFSIGNALED(status)) {
        perror("Error in exiting child : " + WTERMSIG(status));
      }
    }
  }
}

void ctrlC(int x) { cout << "\nProcess exited by User\n"; }

void printDirectory() {
  char cwd[1024];
  getcwd(cwd, sizeof(cwd));
  cout << "\nanshul@anshul:" << cwd << "~$ ";
}

void shell() {
  char str[100], *parsed[100];

  while (true) {
    printDirectory();

    // Handle Ctrl-C signal
    signal(SIGINT, ctrlC);
    cin.getline(str, 100);

    // Handle Ctrl-D signal
    if (cin.eof()) {
      cout << "Exited using Ctrl-D\n";
      break;
    }

    int N = parse(str, parsed);

    if (strcmp(parsed[0], "exit") == 0) break;

    execute(parsed, N);
  }
}

int main() { shell(); }