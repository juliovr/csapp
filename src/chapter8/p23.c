/*
The first signal is handled, the second is pending. Due to sleep(1), the remaining signals will be discarded.
If the for loop terminante-condition is increased to, let's say, 5000, and the sleep(1) is removed, 
the counter will be have a different value each time.
*/
#include "../csapp.c"

int counter = 0;

void handler(int sig)
{
    counter++;
    sleep(1);
    return;
}

int main()
{
    int i;

    Signal(SIGUSR2, handler);

    if (Fork() == 0) {
        for (i = 0; i < 5; i++) {
            Kill(getppid(), SIGUSR2);
            printf("sent SIGUSR2 to parent\n");
        }

        exit(0);
    }

    Wait(NULL);
    printf("counter=%d\n", counter);
    exit(0);
}
