#include <stdio.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();

    if (pid == 0) {
        for (int i = 0; i < 4000000; i++){
            printf("%d\n", i);
        }
    } else {
        while(1);
    }

    return 0;
}
