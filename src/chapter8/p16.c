#include "../csapp.c"

int counter = 1;

int main()
{
    if (Fork() == 0) {
        counter--;
        exit(0);
    } else {
        Wait(NULL);
        printf("counter = %d\n", ++counter);
    }

    exit(0);
}
