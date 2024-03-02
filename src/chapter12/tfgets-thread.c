/*
gcc -Og -g -o bin/tfgets-thread src/chapter12/tfgets-thread.c && bin/tfgets-thread
*/

#include "../csapp.c"


static int timeout = -1;
static char *fgets_result = NULL;

typedef struct {
    char *s;
    int size;
    FILE *stream;
    pthread_t *tid_sleep;
} Thread_params;

void *thread_fgets(void *vargp)
{
    Thread_params *params = (Thread_params *)vargp;
    fgets_result = fgets(params->s, params->size, params->stream);
    
    Pthread_cancel(*params->tid_sleep);
}

void *thread_sleep(void *vargp)
{
    Sleep(5);

    return NULL;
}

char *tfgets(char *s, int size, FILE *stream)
{
    pthread_t tid_fgets;
    pthread_t tid_sleep;
    
    Thread_params params = {};
    params.s = s;
    params.size = size;
    params.stream = stream;
    params.tid_sleep = &tid_sleep;

    Pthread_create(&tid_fgets, NULL, thread_fgets, &params);
    Pthread_create(&tid_sleep, NULL, thread_sleep, NULL);

    Pthread_join(tid_sleep, NULL);
    Pthread_cancel(tid_fgets);
    Pthread_join(tid_fgets, NULL);

    return fgets_result;
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
