#include "../csapp.c"

int main(int argc, char **argv)
{
    char buf[MAXLINE];
    int rc;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <domain name>\n", argv[0]);
        exit(0);
    }

    /* Get a list of addrinfo records */
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;          /* IPv4 only */
    hints.ai_socktype = SOCK_STREAM;    /* Connections only */

    struct addrinfo *listp;
    if ((rc = getaddrinfo(argv[1], NULL, &hints, &listp)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s \n", gai_strerror(rc));
        exit(1);
    }

    /* Walk the list and display each IP address */
    int flags = NI_NUMERICHOST; /* Display address string instead of domain name */
    for (struct addrinfo *p = listp; p; p = p->ai_next) {
        Getnameinfo(p->ai_addr, p->ai_addrlen, buf, MAXLINE, NULL, 0, flags);
        printf("%s\n", buf);
    }

    /* Clean up */
    Freeaddrinfo(listp);

    return 0;
}
