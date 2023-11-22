#include "test.h"
#include <stdlib.h>

/* Allocates memory for an array of nmemb elements of size bytes each. 
   The memory is set to 0. */
void * calloc(size_t nmemb, size_t size)
{
    if (nmemb == 0 || size == 0) {
        return NULL;
    }

    size_t buffer_size = nmemb * size;
    
    // Check for multiplication overflow
    if (nmemb != buffer_size / size) {
        return NULL;
    }

    void *ptr = malloc(buffer_size);
    if (ptr) {
        memset(ptr, 0, buffer_size);
    }

    return ptr;
}

int main()
{
    void* p;
    p = calloc(0x1234, 1);
    assert(p != NULL);
    free(p);

    p = calloc(SIZE_MAX, 2);
    assert(p == NULL);
    free(p);
    
    print_test_ok("Tests passed\n");
    return 0;
}
