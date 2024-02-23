#include "../csapp.c"

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <dotted-decimal>\n", argv[0]);
        return 0;
    }

    struct in_addr naddr;
    int rc = inet_pton(AF_INET, argv[1], &naddr);
    if (rc == 0) {
        app_error("Invalid dotted-decimal address");
    } else if (rc < 0) {
        unix_error("inet_pton");
    }

    fprintf(stdout, "0x%x\n", ntohl(naddr.s_addr));
}
