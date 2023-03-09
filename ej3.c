#include <stdio.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();

    if (pid == 0) {
        printf("Proceso hijo ejecutándose.\n");
    } else {
        while(1);
    }

    return 0;
}
