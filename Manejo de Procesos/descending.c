/*----------------------------------------------------------------

* Programación Avanzada

* Autor: Miguel Marines

* Actividad: Manejo de Procesos

*--------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <limits.h>
#include <sys/wait.h>

// Función para generar procesos hijo.
void child_process(int num, int nivel_actual, int x)
{
	int pid;

    // Caso de que el nivel actual se igual al número de procesos a generar introducido por el usuario.
    if (nivel_actual == num)
    {
        srand(getpid());
        // El proceso duerme 1 segundo.
        sleep(1);

        // Imprime espacios según el número de proceso.
        for(int i = 0; i < nivel_actual; i++)
        {
            printf("\t");
        }

        // Despliega información sobre el proceso (id del pádre, su propio id y su nivel).
	    printf("PPID = %i PID = %i NIVEL = %i\n", getppid()-x, getpid()-x, nivel_actual);

        // Termina la ejecución de la función.
	    exit(0);
    }

    else
    {
        // Imprime espacios según el número de proceso.
        for(int i = 0; i < nivel_actual; i++)
        {
            printf("\t");
        }
				// Imprimir si el nivel actual = 0
				if (!nivel_actual)
                {
					printf("PPID = %i PID = %i NIVEL = %i\n", getppid()-getppid(), getpid()-getpid(), nivel_actual);
					x = getpid();
				}
				else
				    // Despliega información sobre el proceso (id del pádre, su propio id y su nivel).
					printf("PPID = %i PID = %i NIVEL = %i\n", getppid()-x, getpid()-x, nivel_actual);

        // Crea los procesos.
        for (int i = 0; i <= nivel_actual; i++)
        {
            if ( (pid = fork()) < 0 )
            {
			    perror("fork");
			    exit(-3);
		    }

            else if (pid == 0)
            {
                nivel_actual++;
			    child_process(num, nivel_actual,x);
            }

            else
            {
                wait(NULL);
            }

        }

        // Termina la ejecución de la función.
        exit(0);

    }

}

int main(int argc, char* argv[])
{
    // Variable para almacenar el valor del segundo parámetro.
    int num;

    // Verifica que la cantidad de argumentos sea 2.
    if (argc != 2)
    {
		fprintf(stderr, "usage: %s number\n", argv[0]);
		return -1;
	}

    // Convierte un string(segundo parámetro) a su valor numérico.
    num = atoi(argv[1]);

    // Verifica que el segundo parámetro sea un valor positivo mayor a 1.
    if (num <= 1)
    {
		fprintf(stderr, "%s: the parameter must be a positive integer number\n", argv[0]);
		return -2;
	}

    // Llamada a la función.
    child_process(num, 0,0);

    // Termina el programa.
	return 0;
}
