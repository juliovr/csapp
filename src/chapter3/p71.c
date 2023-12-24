#include <stdio.h>

#define BUFFER_SIZE 20
#define true 1

void good_echo()
{
    char buffer[BUFFER_SIZE];

    while (true) {
        char *p = fgets(buffer, sizeof(buffer), stdin);
        if (!p) {
            break;
        }

        printf("input = %s\n", buffer);
    }
}

int main()
{
    good_echo();
    
    return 0;
}
