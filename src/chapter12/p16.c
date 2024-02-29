#include "../csapp.c"

#define MAX_THREADS 32

void *thread(void *vargp);

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <nthreads>\n", argv[0]);
        exit(0);
    }

    pthread_t tids[MAX_THREADS];
    int nthreads = atoi(argv[1]);

    if (nthreads > MAX_THREADS) {
        fprintf(stderr, "nthreads should not exceed %d\n", MAX_THREADS);
        exit(0);
    }

    for (int i = 0; i < nthreads; ++i) {
        Pthread_create(&tids[i], NULL, thread, (void *)i);
    }

    for (int i = 0; i < nthreads; ++i) {
        Pthread_join(tids[i], NULL);
    }
    
    exit(0);
}

void *thread(void *vargp)
{
    int tid = (int *)vargp;
    printf("Hello, world from thread = %d!\n", tid);

    return NULL;
}
