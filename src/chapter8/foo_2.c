#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Usage: %s <seconds_to_sleep>\n", argv[0]);
        return 0;
    }

    int seconds = atoi(argv[1]);
    sleep(seconds);

    return 0;
}