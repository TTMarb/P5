#include "dummy.h"

void callexecve(char path[]) {
    char* const args[] = {"ls", NULL};
    char* const envp[] = {NULL};

    execve(path, args, envp);
    perror("execve");
    exit(EXIT_FAILURE); /* Exit the child process if it fails*/
}