#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LINE 1024
#define MAX_ARGS 64

int main(void) {
    char line[MAX_LINE];
    char *args[MAX_ARGS];

    while(1) {
        printf("myshell> ");
        fflush(stdout);

        if(fgets(line, MAX_LINE, stdin) == NULL) {
            break; // Si no hay entrada salir
        }

        // Quitar el salto de línea al final
        line[strcspn(line, "\n")] = '\0';

        // salir de shell con el comando "exit"
        if(strcmp(line, "exit") == 0) {
            break;
        }

        int i = 0;
        char *token = strtok(line, " ");
        while(token != NULL && i < MAX_ARGS - 1) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }

        args[i] = NULL; // Finalizar la lista de argumentos

        // Crear un proceso hijo
        pid_t pid = fork();
        if(pid < 0) {
            perror("fork");
            continue;
        }

        if(pid == 0) {
            // Proceso hijo: intentar ejecutar el comando
            if(execvp(args[0], args) == -1) {
                perror("execvp");
                exit(EXIT_FAILURE);
            }
        } else {
            // Esperar que termine el hijo
            wait(NULL);
        }
    }

    return 0;
}