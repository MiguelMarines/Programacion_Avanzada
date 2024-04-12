/*----------------------------------------------------------------

* Programación Avanzada: Manejo de Directorios

* Autor: Miguel Marines

* Actividad: Busqueda de directorios de manera recursiva o no.

--------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <limits.h>
#include <fcntl.h>

/*
* Funcion para encontrar la palabra en cada archivo de cada
* directorio de forma recursiva.
*/
void findSubstring(char *directory, char *substring, int recursive) {
  DIR *dir;
  struct dirent *entry;
  struct stat info;
  char pathname[PATH_MAX + NAME_MAX + 1];

  // Revisar que el parametro directory sea un directorio y que exista.
  if( (dir = opendir(directory)) == NULL ) {
    perror("opendir");
    exit(-2);
  }

  // Buscar substring en el directorio inicial.
  while ( (entry = readdir(dir)) != NULL ) {
    if (strstr(entry->d_name, substring) != NULL) {
      printf("Name: %s\n", entry->d_name);
      printf("Path: %s\n\n", directory);
    }
  }

  // Hacer busqueda recursiva.
  if (recursive) {
    rewinddir(dir);
    while ( (entry = readdir(dir)) != NULL ) {
      if ( (strcmp(".", entry->d_name) != 0) && (strcmp("..", entry->d_name) != 0) ) {
        sprintf(pathname, "%s/%s", directory, entry->d_name);
        stat(pathname, &info);

        if ((info.st_mode & S_IFMT) == S_IFDIR) {
          findSubstring(pathname, substring, 1);
        }
      }
    }
  }

  closedir(dir);
}

int main(int argc, char * argv[]) {

  int recursive;
  char initial_directory[PATH_MAX + 1];
  char *substring;

  /* Revisa el número de parametros.
   * Admite a partir de 3
   * $ .\find string initial_directory [-r]
  */
  if (argc < 3 || argc > 4) {
    fprintf(stderr, "usage: %s string initial_directory [-r]\n", argv[0]);
    return -1;
  }
  if (argc == 4 && strcmp(argv[3], "-r") != 0) {
    fprintf(stderr, "usage: %s string initial_directory [-r]\n", argv[0]);
    return -3;
  }

  substring = (char*) malloc( strlen(argv[1]) );
  strcpy(substring, argv[1]);
  strcpy(initial_directory, argv[2]);

  // Revisar si se utilizara recursion.
  recursive = (argc == 4)? 1 : 0;

  // Hacer la llamada a findSubstring.
  findSubstring(initial_directory, substring, recursive);

  free(substring);
  
  return 0;
}
