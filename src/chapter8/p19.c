#include "../csapp.c"

void foo(int n)
{
    int i;

    for (i = 0; i < n; i++) {
        Fork();
    }

    printf("hello\n");
    exit(0);
}

int main()
{
    foo(3);
}
