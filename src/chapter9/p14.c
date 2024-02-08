#include "../csapp.c"

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <txt file>\n", argv[0]);
        return 0;
    }

    char *filename = argv[1];
    int fd = Open(filename, O_RDWR, 0);
    if (fd != -1) {
        struct stat fd_stat;
        if (fstat(fd, &fd_stat) == 0) {
            int size = fd_stat.st_size;
            char *buf = Mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
            *buf = 'J';
            Munmap(buf, size);
        }
    }

    return 0;
}
