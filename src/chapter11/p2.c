#include "../csapp.c"

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <hex>\n", argv[0]);
        return 0;
    }

    uint32_t addr;
    sscanf(argv[1], "%x", &addr);

    struct in_addr naddr;
    naddr.s_addr = htonl(addr);

    char buf[MAXBUF];
    if (!inet_ntop(AF_INET, &naddr, buf, MAXBUF)) {
        unix_error("inet_ntop");
    }

    fprintf(stdout, "%s\n", buf);

    return 0;
}
