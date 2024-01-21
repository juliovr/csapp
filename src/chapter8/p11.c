#include "../csapp.c"

int main()
{
    int i;
    printf("main pid = %d\n", getpid());

    for (i = 0; i < 2; i++) {
        printf("fork from pid = %d\n", getpid());
        Fork();
    }

    pid_t pid = getpid();

    printf("hello from pid = %d\n", pid);
    exit(0);
}
