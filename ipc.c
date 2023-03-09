#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/shm.h>
#include <sys/mman.h>

#define FD_NAME "/shmem"    // Nombre de memoria para file descriptor
#define MEM_KEY 1234        // Key para la memoria compartida
#define MEM_SIZE 1024       // Tamano de memoria compartida

int main(int argc, char** argv) {
    int id_memoria;
    int* puntero_memoria;

    // Crear espacio de memoria o revisar si ya fue creada
    id_memoria = shmget(MEM_KEY, MEM_SIZE, IPC_EXCL | IPC_CREAT | 0666);
    
    // Si el espacio de memoria ya fue creado
    if (id_memoria == -1)
    {
        // Leer espacio de memoria compartida
        id_memoria = shmget(MEM_KEY, MEM_SIZE, 0666);
        puntero_memoria = (int*) shmat(id_memoria, NULL, 0);

        // Imprimir valor de memoria compartida
        int valor_memoria = (int) *puntero_memoria;
        printf("Valor del espacio de Memoria: %d", valor_memoria);
        shmdt(puntero_memoria);
    }
    else // Si el espacio no ha sido creado
    {
        // Crear espacio de memoria
        id_memoria = shmget(MEM_KEY, MEM_SIZE, IPC_CREAT | 0666);

        // obtener file descriptor
        int file_descriptor = shm_open(
            FD_NAME,
            O_CREAT | O_RDWR,
            S_IRUSR | S_IWUSR
        );

        // Mappear memoria a puntero
        puntero_memoria = (int*) shmat(id_memoria, NULL, 0);

        // Guardar File descriptor en memoria compartida
        *puntero_memoria = file_descriptor;
        shmdt(puntero_memoria);
    }

    // Fork
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("Error al crear proceso hijo");
        return 1;
    }

    // Creacion de Pipe
    int fd[2];
    char buffer[80];

    if (pipe(fd) == -1)
    {
        perror("Error al crear Pipe");
        return 1;
    }

    // Proceso hijo
    if (pid == 0)
    {
        int finish = 0;
        int index = 0;

        while (finish != 1)
        {
            // esperarlo
            close(fd[1]);
            read(fd[0], buffer, sizeof(buffer));
            close(fd[0]);   
            
            if (buffer != "finish")
            {
                finish = 1;        
            }
            else
            {
                // Mappear memoria a puntero
                char **str_arr = (char**) shmat(id_memoria, NULL, 0);

                // Guardar File descriptor en memoria compartida
                str_arr[index] = buffer;
                index += 1;
                shmdt(str_arr);
            }
        }

        exit(0);
    }
    else // Proceso padre
    {
        int n = argc;
        char** x = argv;

        for(int i = 0; i < MEM_SIZE; i++)
        {
            if (i % n == 0)
            {
                // Mandar x en pipe de comunicacion
                close(fd[0]);
                write(fd[1], x, sizeof(x));
                close(fd[1]);
            }
        }

        // Mandar finish en pipe de comunicacion
        close(fd[0]);
        write(fd[1], "finish", sizeof("finish"));
        close(fd[1]);

        wait(NULL);

        // Mappear memoria a puntero
        char **str_arr = (char**) shmat(id_memoria, NULL, 0);

        // Guardar File descriptor en memoria compartida
        printf("Valor del espacio de Memoria: ");

        for(int j = 0; j < sizeof(str_arr); j++)
        {
            printf("%c", str_arr[j]);
        }
        printf("\n");
        shmdt(str_arr);
    }

    // Liberar espacio de memoria
    shmctl(id_memoria, IPC_RMID, NULL);
    return 0;
}
