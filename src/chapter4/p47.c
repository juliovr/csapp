
/* Bubble sort: Array version */
void bubble_a(long *data, long count)
{
    long i, last;
    for (last = count - 1; last > 0; last--) {
        for (i = 0; i < last; i++) {
            if (data[i + 1] < data[i]) {
                /* Swap adjacent elements */
                long t = data[i + 1];
                data[i + 1] = data[i];
                data[i] = t;
            }
        }
    }
}

/* Bubble sort: Pointer version */
void bubble_p(long *data, long count)
{
    long *current;
    long *last;
    for (last = data + count - 1; last != data; last--) {
        for (current = data; current != last; current++) {
            if (*(current + 1) < *current) {
                long t = *(current + 1);
                *(current + 1) = *current;
                *current = t;
            }
        }
    }
}

void dump_array(long *data, long count)
{
    printf("Array:");
    for (long i = 0; i < count; ++i) {
        printf(" %d", data[i]);
    }

    printf("\n");
}

int main()
{
    long data[] = {
        5, 1, 10, 2, 4,
    };

    int count = sizeof(data) / sizeof(data[0]);

    printf("Before ");
    dump_array(data, count);

    bubble_p(data, count);

    printf("After ");
    dump_array(data, count);
}
