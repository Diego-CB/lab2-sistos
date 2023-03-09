#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/shm.h>
#include <sys/mman.h>

int main()
{
    pid_t pid = fork();

    if (pid == 0){
        execl("ipc", "ipc", "2", "A");
    }
    else
    {
        execl("ipc", "ipc", "2", "B");
    }
    return 0;
}