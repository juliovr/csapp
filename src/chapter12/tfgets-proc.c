/*
gcc -Og -g -o bin/tfgets-proc src/chapter12/tfgets-proc.c && bin/tfgets-proc
*/

#include "../csapp.c"

static sigjmp_buf sigbuf;

void handler(int sig)
{
    siglongjmp(sigbuf, 1);
}

char *tfgets(char *s, int size, FILE *stream)
{
    if (Fork() == 0) {
        Sleep(5);
        exit(0);
    }

    if (sigsetjmp(sigbuf, 1)) {
        // When 5 seconds has passed, the OS signal SIGCHLD, where "siglongjmp" re-execute from the initial "sigsetjmp", 
        // but now it returns non-zero, so it continues from here.
        // See: https://pubs.opengroup.org/onlinepubs/7908799/xsh/sigsetjmp.html
        // Return value: If the return is from a successful direct invocation, sigsetjmp() returns 0. If the return is from a call to siglongjmp(), sigsetjmp() returns a non-zero value. 
        return NULL;
    } else {
        // First execution follow this path
        Signal(SIGCHLD, handler);
        return fgets(s, size, stream);
    }
}

int main()
{
    char buf[MAXLINE];

    if (tfgets(buf, MAXLINE, stdin) == NULL) {
        printf("BOOM\n");
    } else {
        printf("%s", buf);
    }

    return 0;
}
