/*
gcc -Og -g -o bin/p26 src/chapter8/p26.c && bin/p26

bin/foo_2 10 &
asd
Ctrl+c
*/

#include "../csapp.c"

#define MAXARGS 128

#define MAXJOBS 20


int parseline(char *buf, char **argv)
{
    char *delim;    /* Points to first space delimiter */
    int argc;       /* Number of args */
    int bg;         /* Background job? */

    buf[strlen(buf) - 1] = ' '; /* Replace trailing '\n' with space */
    while (*buf && (*buf == ' ')) { /* Ignore leading spaces */
        buf++;
    }

    /* Build argv list */
    argc = 0;
    while ((delim = strchr(buf, ' '))) {
        argv[argc++] = buf;
        *delim = '\0';
        buf = delim + 1;
        while (*buf && (*buf == ' ')) { /* Ignore spaces */
            buf++;
        }
    }

    argv[argc] = NULL;

    if (argc == 0) { /* Ignore blank line */
        return 1;
    }

    /* Should the job run in the background? */
    if ((bg = (*argv[argc - 1] == '&')) != 0) {
        argv[--argc] = NULL;
    }

    return bg;
}

typedef enum {
    Running,
    Stopped,
} JobStatus;

typedef struct {
    int jid;
    int pid;
    char command[MAXLINE];
    JobStatus status;
    int in_use;
    int bg;
} Job;

static Job jobs[MAXJOBS];

static volatile sig_atomic_t fg_pid;

static void init_jobs()
{
    memset(jobs, 0, sizeof(jobs));
}

Job *get_available_job()
{
    for (int i = 0; i < MAXJOBS; ++i) {
        Job *job = jobs + i;
        if (!job->in_use) {
            job->jid = i + 1;
            return job;
        }
    }

    return NULL;
}

Job *get_job_by_pid(int pid)
{
    for (int i = 0; i < MAXJOBS; ++i) {
        Job *job = jobs + i;
        if (job->pid == pid) {
            return job;
        }
    }

    return NULL;
}

Job *get_job_by_jid(int jid)
{
    for (int i = 0; i < MAXJOBS; ++i) {
        Job *job = jobs + i;
        if (job->jid == jid) {
            return job;
        }
    }

    return NULL;
}

char *get_status_name(JobStatus status)
{
    switch(status) {
        case Running: return "Running";
        case Stopped: return "Stopped";
        default: "";
    }
}

void print_job(Job *job)
{
    char bg_char = (job->bg) ? '&' : ' ';
    printf("[%d] %4d %s\t%s %c\n", job->jid, job->pid, get_status_name(job->status), job->command, bg_char);
}

void print_jobs()
{
    for (int i = 0; i < MAXJOBS; ++i) {
        Job *job = jobs + i;
        if (job->in_use) {
            print_job(job);
        }
    }
}

int is_number(char *s)
{
    while (*s) {
        if (!(*s >= '0' && *s <= '9')) {
            return 0;
        }

        s++;
    }

    return 1;
}

int parse_id(char *s)
{
    int error = -1;
    if (!s) {
        return error;
    }

    // for jid format: %dddd
    if (s[0] == '%') {
        s++;
    }

    if (!is_number(s)) {
        return error;
    }

    return atoi(s);
}

int builtin_command(char **argv)
{
    if (!strcmp(argv[0], "quit")) {
        exit(0);
    }

    if (!strcmp(argv[0], "&")) {
        return 1;
    }

    if (!strcmp(argv[0], "jobs")) {
        print_jobs();
        return 1;
    }

    if (!strcmp(argv[0], "bg")) {
        int id = parse_id(argv[1]);
        if (id != -1) {
            sigset_t mask_one, prev_one;
            Sigemptyset(&mask_one);
            Sigaddset(&mask_one, SIGCHLD);
            Sigprocmask(SIG_BLOCK, &mask_one, &prev_one);

            pid_t pid = id;
            
            if (*argv[1] == '%') {
                Job *job = get_job_by_jid(id);
                pid = job->pid;
            }

            if (pid) {
                fg_pid = 0;
                printf("kill SIGCONT pid = %d\n", pid);
                Kill(pid, SIGCONT);
            } else {
                printf("Job not found.\n");
            }
            
            Sigprocmask(SIG_SETMASK, &prev_one, NULL);
        } else {
            printf("Error format. Use 'bg %%<d>' for jid or 'bg <d>' for pid, where d is the id number\n");
        }

        return 1;
    }

    if (!strcmp(argv[0], "fg")) {
        int id = parse_id(argv[1]);
        if (id != -1) {
            sigset_t mask_one, prev_one;
            Sigemptyset(&mask_one);
            Sigaddset(&mask_one, SIGCHLD);
            Sigprocmask(SIG_BLOCK, &mask_one, &prev_one);

            pid_t pid = id;
            
            if (*argv[1] == '%') {
                Job *job = get_job_by_jid(id);
                pid = job->pid;
            }

            if (pid) {
                fg_pid = pid;
                printf("kill SIGCONT pid = %d\n", pid);
                Kill(pid, SIGCONT);

                while (fg_pid) {
                    sigsuspend(&prev_one);
                }
            } else {
                printf("Job not found.\n");
            }
            
            Sigprocmask(SIG_SETMASK, &prev_one, NULL);
        } else {
            printf("Error format. Use 'fg %%<d>' for jid or 'fg <d>' for pid, where d is the id number\n");
        }

        return 1;
    }

    return 0;
}

void sigchld_handler(int sig)
{
    // Sio_puts("sigchld_handler\n");
    int old_errno = errno;

    sigset_t mask, prev_mask;

    pid_t pid;
    int status;
    
    Sigfillset(&mask);
    while ((pid = waitpid(-1, &status, 0)) > 0) {
        if (WIFEXITED(status) || WIFSIGNALED(status)) {
            // Sio_puts("signaled\n");
            Sigprocmask(SIG_BLOCK, &mask, &prev_mask); // Block sigs        
            Job *job = get_job_by_pid(pid);
            job->in_use = 0;
            if (pid == fg_pid) {
                fg_pid = 0;
            }
            Sigprocmask(SIG_SETMASK, &prev_mask, NULL); // Restore sigs
        }

        if (WIFCONTINUED(status) != 0) {
            // Sio_puts("continue\n");
            Sigprocmask(SIG_BLOCK, &mask, &prev_mask); // Block sigs
        
            Job *job = get_job_by_pid(pid);
            job->bg = 0;

            Sigprocmask(SIG_SETMASK, &prev_mask, NULL); // Restore sigs
        }
    }

    errno = old_errno;
}

void sigint_handler(int sig)
{
    // Sio_puts("sigint_handler\n");
    int old_errno = errno;

    sigset_t mask, prev_mask;

    Sigfillset(&mask);
    Sigprocmask(SIG_BLOCK, &mask, &prev_mask);
    if (fg_pid != 0) {
        Kill(fg_pid, SIGSTOP);
        Sio_puts("\n");
    } else {
        Sio_puts("\n> ");
    }
    Sigprocmask(SIG_SETMASK, &prev_mask, NULL);

    errno = old_errno;
}

void eval(char *cmdline)
{
    char *argv[MAXARGS];    /* Argument list execve() */
    char command[MAXLINE];  /* Holds modified command line */
    int bg;                 /* Should the job run in bg or fg */
    pid_t pid;              /* Process id */

    sigset_t mask_all, mask_one, prev_one, prev_all;

    strcpy(command, cmdline);
    bg = parseline(command, argv);
    
    if (argv[0] == NULL) {
        return; /* Ignore empty lines */
    }

    if (!builtin_command(argv)) {
        Sigemptyset(&mask_one);
        Sigaddset(&mask_one, SIGCHLD);
        Sigaddset(&mask_one, SIGINT);

        Sigprocmask(SIG_BLOCK, &mask_one, &prev_one); // Block SIGCHLD
        if ((pid = Fork()) == 0) { /* Child runs user job */
            Sigprocmask(SIG_SETMASK, &prev_one, NULL); // Unblock SIGCHLD in child process

            if (execve(argv[0], argv, environ) < 0) {
                printf("%s: Command not found.\n", argv[0]);
                exit(0);
            }
        }

        Sigfillset(&mask_all);
        Sigprocmask(SIG_BLOCK, &mask_all, &prev_all);
        Job *job = get_available_job();
        if (!job) {
            fprintf(stderr, "There are no jobs available to handle the command\n");
        }

        job->pid = pid;
        strcpy(job->command, command);
        job->bg = bg;
        job->in_use = 1;
        Sigprocmask(SIG_SETMASK, &prev_all, NULL);

        /* Parent waits for foreground job to terminate */
        if (!job->bg) {
            fg_pid = pid;

            int status;
            if (waitpid(pid, &status, 0) < 0) {
                unix_error("waitfg: waitpid error");
            }
            
            fg_pid = 0;
            job->in_use = 0;
        } else {
            char bg_char = (job->bg) ? '&' : ' ';
            printf("[%d] %4d %s %c\n", job->jid, job->pid, job->command, bg_char);
        }

        Sigprocmask(SIG_SETMASK, &prev_one, NULL); // Unblock SIGCHLD in parent process
    }
}

int main()
{
    Signal(SIGCHLD, sigchld_handler);
    Signal(SIGINT, sigint_handler);
    
    init_jobs();

    char cmdline[MAXLINE];

    while (1) {
        /* Read */
        printf("> ");
        Fgets(cmdline, MAXLINE, stdin);
        if (feof(stdin)) {
            exit(0);
        }

        eval(cmdline);
    }
}
