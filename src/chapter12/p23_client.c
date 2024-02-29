#include "../csapp.c"

int main(int argc, char **argv)
{
    char *host = "localhost";
    char *port = "3000";

    int clientfd = Open_clientfd(host, port);


    char *buf = "Test data";

    rio_t rio;
    Rio_readinitb(&rio, clientfd);
    Rio_writen(clientfd, buf, strlen(buf));

    exit(0);
}
