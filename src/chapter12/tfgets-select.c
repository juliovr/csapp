/*
gcc -Og -g -o bin/tfgets-select src/chapter12/tfgets-select.c && bin/tfgets-select
*/

#include "../csapp.c"

static sigjmp_buf sigbuf;

void handler(int sig)
{
    siglongjmp(sigbuf, 1);
}

char *tfgets(char *s, int size, FILE *stream)
{
    fd_set read_set;

    FD_ZERO(&read_set);
    FD_SET(STDIN_FILENO, &read_set);
    
    struct timeval time;
    time.tv_sec = 5;
    int rc = Select(1, &read_set, NULL, NULL, &time);
    
    if (FD_ISSET(STDIN_FILENO, &read_set)) {
        return fgets(s, size, stream);
    } else {
        return NULL;
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
