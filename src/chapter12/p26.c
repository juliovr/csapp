#include "../csapp.c"


// struct hostent {
//     char  *h_name;            /* official name of host */
//     char **h_aliases;         /* alias list */
//     int    h_addrtype;        /* host address type */
//     int    h_length;          /* length of address */
//     char **h_addr_list;       /* list of addresses */
// }


static sem_t mutex;

void free_hostent(struct hostent *hostent)
{

}

struct hostent *gethostbyname_ts(const char *name, struct hostent *hostname)
{
    P(&mutex);

    struct hostent *source = gethostbyname(name);

    hostname->h_name = (char *)malloc(strlen(source->h_name) + 1);
    strcpy(hostname->h_name, source->h_name);

    int n_aliases = 0;
    while (source->h_aliases[n_aliases]) {
        ++n_aliases;
    }
    hostname->h_aliases = (char **)malloc(sizeof(char *) * n_aliases);

    for (int i = 0; i < n_aliases; ++i) {
        hostname->h_aliases[i] = (char *)malloc(strlen(source->h_aliases[i] + 1));
        strcpy(hostname->h_aliases[i], source->h_aliases[i]);
    }
    hostname->h_aliases[n_aliases] = NULL;

    hostname->h_addrtype = source->h_addrtype;
    hostname->h_length = source->h_length;

    int n_addr_list = 0;
    while (source->h_addr_list[n_addr_list]) {
        ++n_addr_list;
    }
    hostname->h_addr_list = (char **)malloc(sizeof(char *) * n_addr_list);

    for (int i = 0; i < n_addr_list; ++i) {
        hostname->h_addr_list[i] = (char *)malloc(strlen(source->h_addr_list[i] + 1));
        strcpy(hostname->h_addr_list[i], source->h_addr_list[i]);
    }
    hostname->h_addr_list[n_addr_list] = NULL;

    V(&mutex);

    return hostname;
}

int main()
{
    Sem_init(&mutex, 0, 1);
    struct hostent hostname;
    gethostbyname_ts("localhost", &hostname);
    printf("name = %s\n", hostname.h_name);
    printf("aliases = %s\n", hostname.h_aliases);
    printf("addrtype = %d\n", hostname.h_addrtype);
    printf("length = %d\n", hostname.h_length);
    printf("addr_list = %s\n", *hostname.h_addr_list);

    return 0;
}
