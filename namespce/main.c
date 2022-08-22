#define _GNU_SOURCE

#include <sys/wait.h>

#include <sys/utsname.h>

#include <sched.h>

#include <stdio.h>

#define STACK_SIZE (1024 * 1024)

static char stack[STACK_SIZE];

static char *const child_args[] = {"/bin/bash", NULL};

static int child(void *arg) {

    execv("/bin/bash", child_args);

    return 0;

}

int main(int argc, char *argv[]) {

    pid_t pid;

    pid = clone(child, stack + STACK_SIZE, SIGCHLD | CLONE_NEWUTS, NULL);

    waitpid(pid, NULL, 0);

}
