#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void *basic_memset(void *s, int c, size_t n)
{
    size_t cnt = 0;
    unsigned char *schar = s;
    while (cnt < n) {
        *schar++ = (unsigned char) c;
        cnt++;
    }

    return s;
}

void *memset_8x1(void *s, int c, size_t n)
{
    int K = sizeof(unsigned long);
    
    // Replicate c in all K-sized bytes to be set
    unsigned long sc = 0;
    for (int shift = 0; shift < K; ++shift) {
        sc |= (c << (shift*8));
    }

    
    unsigned char *schar = s;

    // 1.- Align
    size_t cnt = 0;
    while (cnt < n && (size_t)schar % K != 0) {
        *schar++ = (unsigned char)c;
        cnt++;
    }

    // 2.- Apply memset
    unsigned long *schar_long = (unsigned long *)schar;

    size_t loop_times = (n - cnt) / K;
    for (size_t i = 0; i < loop_times; ++i) {
        *schar_long++ = sc;
        cnt += K;
    }

    // 3.- Set remaining bytes
    while (cnt < n) {
        *schar++ = (unsigned char)c;
        cnt++;
    }

    return s;
}

void dump_buffer(void *s, size_t n)
{
    unsigned char *buffer = s;
    for (int i = 0; i < n; ++i) {
        printf("%.2X ", buffer[i]);
    }
    printf("\n");
}

int main()
{
    size_t n = 2048;
    void *buffer1 = malloc(n);
    void *buffer2 = malloc(n);

    basic_memset(buffer1, 0xAB, n);
    memset_8x1(buffer2, 0xAB, n);

    // dump_buffer(buffer1, n);
    // dump_buffer(buffer2, n);

    int cmp = memcmp(buffer1, buffer2, n);
    assert(cmp == 0);

    free(buffer2);
    free(buffer1);

    return 0;
}
