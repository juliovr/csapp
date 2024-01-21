#include "../csapp.c"

int mysystem(char *command)
{
    pid_t pid;
    if ((pid = Fork()) == 0) {
        char *argv[] = { "/bin/sh", "-c", command, NULL };
        if (execve(argv[0], argv, environ) < 0) {
            printf("%s: Command not found\n", command);
            exit(1);
        }
    }

    int status;
    if (waitpid(pid, &status, 0) < 0) {
        unix_error("waitpid error");
    }

    // Terminated normally
    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    }

    // Terminated abnormally
    if (WIFSIGNALED(status)) {
        return WTERMSIG(status);
    }
}

int main()
{
    printf("Code p22_test1 = %d\n", mysystem("bin/p22_test1"));
    printf("Code p22_test2 = %d\n", mysystem("bin/p22_test2"));

    return 0;
}
