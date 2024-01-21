#include "../csapp.c"

int main(int argc, char *argv[])
{
    char *name = "/bin/ls";
    argv[0] = name;
    if (execve(name, argv, environ) < 0) {
        printf("%s: Command not found\n", argv[0]);
        exit(1);
    }

    return 0;
}
