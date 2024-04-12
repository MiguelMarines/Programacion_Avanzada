/*----------------------------------------------------------------

* Programación Avanzada: Manejo de Threads

* Autor: Miguel Marines

* Actividad: Hilos

*--------------------------------------------------------------*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <ctype.h>
#include "utils.h"


double **MATRIX_A;
double **MATRIX_B;
double **MATRIX_C;


int SIZE_MATRIZ_A, SIZE_MATRIZ_B;

typedef struct
{
	int start, end;
} Block;

void multiplicarRenglon(int row)
{
	int i, j;

	for (i = 0; i < SIZE_MATRIZ_A; i++)
	{
		double res = 0;
		
		for (j = 0; j < SIZE_MATRIZ_A; j++)
		{
			res += (MATRIX_A[row][j] * MATRIX_B[j][i]);
		}

		MATRIX_C[row][i] = res;
	}
}


void *partial_matrix(void *param)
{
	Block *block;
	int i;

	block = (Block *)param;

	for (i = block->start; i < block->end; i++)
	{
		multiplicarRenglon(i);
	}

	return ((void *)0);
}


void printMatrix(double **m)
{
	int i, j;

	for (i = 0; i < SIZE_MATRIZ_A; i++)
	{
		for (j = 0; j < SIZE_MATRIZ_A; j++)
		{
			printf(" %f ", m[i][j]);
		}
		printf("\n");
	}
}

int main(int argc, char **argv)
{

	int i, j, threads;
	char number_threads[5], size_matrix_A[10], size_matrix_B[10];
	FILE *fileA, *fileB;
	double concurrent;


	
	// Validación de los parametros de entrada al ejecutar el programa.

	if (argc != 7)
	{
		printf("Error: You need to introduce 6 parameters after the name of the program!\n");
		printf("Example: %s -a matrix_file_a.txt -b matrix_file_b.txt -t thread_amount\n", argv[0]);
		return -1;
	}

	if (strcmp(argv[1], "-a") != 0)
	{
		printf("Error: the first parameter must be -a\n");
		printf("Example: %s -a matrix_file_a.txt -b matrix_file_b.txt -t thread_amount\n", argv[0]);
		return -1;
	}

	if (strcmp(argv[3], "-b") != 0)
	{
		printf("Error: the third parameter must be -b\n");
		printf("Example: %s -a matrix_file_a.txt -b matrix_file_b.txt -t thread_amount\n", argv[0]);
		return -1;
	}

	if (strcmp(argv[5], "-t") != 0)
	{
		printf("Error: the fifth parameter must be -b\n");
		printf("Example: %s -a matrix_file_a.txt -b matrix_file_b.txt -t thread_amount\n", argv[0]);
		return -1;
	}


	
	// Valida los archivos de entrada.

	if ((fileA = fopen(argv[2], "r")) == NULL)
	{
		printf("%s: No such file or directory %s\n", argv[0], argv[1]);
		return -2;
	}

	if ((fileB = fopen(argv[4], "r")) == NULL)
	{
		printf("%s: No such file or directory %s\n", argv[0], argv[3]);
		return -2;
	}



	// Valida que el número de hilos sea un número positivo.
	
	stpcpy(number_threads, argv[6]);

	for (i = 0; i < strlen(number_threads); i++)
	{
		if (isdigit(number_threads[i]) == 0)
		{
			printf("Error: the number of threads must be an integer value!\n");
			return -3;
		}
	}

	threads = atoi(number_threads);



	// Valida que las matrices sean del mismo tamaño.

	fgets(size_matrix_A, 10, fileA);
	fgets(size_matrix_B, 10, fileB);

	if (strcmp(size_matrix_A, size_matrix_B) != 0)
	{
		printf("Error: The matrices must be the same size!\n");
		return -4;
	}

	// Imprime el nombre de los archivos que contienen las matrices, el tamaño de las matrices y el número de hilos.

	printf("File Matrix A: %s\n", argv[2]);
	printf("File Matrix B: %s\n", argv[4]);
	
	printf("Threads: %i \n", threads);

	SIZE_MATRIZ_A = atoi(size_matrix_A);
	SIZE_MATRIZ_B = atoi(size_matrix_B);
	printf("Size Matrix a: %i\n", SIZE_MATRIZ_A);
	printf("Size Matrix b: %i\n", SIZE_MATRIZ_B);

	
	// Llena las matrices.

	MATRIX_A = (double **)malloc(sizeof(double *) * SIZE_MATRIZ_A);

	for (int i = 0; i < SIZE_MATRIZ_A; i++)
		MATRIX_A[i] = (double *)malloc(sizeof(double) * SIZE_MATRIZ_A);

	for (i = 0; i < SIZE_MATRIZ_A; i++)
	{
		for (int j = 0; j < SIZE_MATRIZ_A; j++)
		{
			fscanf(fileA, "%lf", &MATRIX_A[i][j]);
		}
	}

	
	MATRIX_B = (double **)malloc(sizeof(double *) * SIZE_MATRIZ_A);

	for (int i = 0; i < SIZE_MATRIZ_A; i++)
		MATRIX_B[i] = (double *)malloc(sizeof(double) * SIZE_MATRIZ_A);

	for (int i = 0; i < SIZE_MATRIZ_A; i++)
	{

		for (int j = 0; j < SIZE_MATRIZ_A; j++)
		{
			fscanf(fileB, "%lf", &MATRIX_B[i][j]);
		}
	}

	MATRIX_C = (double **)malloc(sizeof(double *) * SIZE_MATRIZ_A);

	for (i = 0; i < SIZE_MATRIZ_A; i++)
		MATRIX_C[i] = (double *)malloc(sizeof(double) * SIZE_MATRIZ_A);


	printf("\nResult:\n");
	Block *blocks = (Block *)malloc(sizeof(Block) * threads);
	pthread_t *tids = (pthread_t *)malloc(sizeof(pthread_t) * threads);


	for (i = 0; i < threads; i++)
	{
		if (SIZE_MATRIZ_A % threads == 0)
		{
			int chunk = SIZE_MATRIZ_A / threads;
			blocks[i].start = chunk * i;
			blocks[i].end = blocks[i].start + chunk;
		}
		else
		{
			int normal_chunk = (SIZE_MATRIZ_A / threads);
			int chunk = SIZE_MATRIZ_A / threads;
			if (i < threads - 1)
			{
				blocks[i].start = normal_chunk * i;
				blocks[i].end = blocks[i].start + chunk;
			}
			else
			{
				blocks[i].start = normal_chunk * i;
				blocks[i].end = blocks[i].start + SIZE_MATRIZ_A - (normal_chunk * (threads - 1));
			}
		}
	}

	// Hace el calculo del resultado e imprime el tiempo.

	start_timer();
	for (i = 0; i < threads; i++)
	{
		pthread_create(&tids[i], NULL, partial_matrix, (void *)&blocks[i]);
	}
	for (i = 0; i < threads; i++)
	{
		pthread_join(tids[i], NULL);
	}
	concurrent = stop_timer();
	printMatrix(MATRIX_C);
	printf("\nTime = %.5lf ms\n", concurrent);
	

	// Libera la memoria.

	free(MATRIX_A);
	free(MATRIX_B);
	free(MATRIX_C);
	fclose(fileA);
	fclose(fileB);
    
    // Termina el programa exitosamente.
	
	return 0;
	
}


/*
Resultados:
1. 
    ./multiply -a a1181.txt -b b1181.txt -t 1
    Time = 8919.90300 ms
2.
    ./multiply -a a1181.txt -b b1181.txt -t 2
	Time = 4837.08600 ms
3.
    ./multiply -a a1181.txt -b b1181.txt -t 3
    Time = 4632.75600 ms
4.
    ./multiply -a a1181.txt -b b1181.txt -t 4
	Time = 4605.15600 ms
*/