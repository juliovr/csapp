#include "../csapp.c"

int main()
{
    if (Fork() == 0) {
        printf("a"); fflush(stdout);
        exit(0);
    } else {
        printf("b"); fflush(stdout);
        Waitpid(-1, NULL, 0);
    }

    printf("c"); fflush(stdout);
    exit(0);
}
