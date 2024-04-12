/*----------------------------------------------------------------

* Programación Avanzada: Examen Final

* Autor: Miguel Marines

* Actividad: Desencriptar

*--------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

typedef unsigned char uchar;
typedef unsigned long ulong;

int main(int argc, char * argv[])
{
  ulong source_file_size;
  uchar buffer, reg;

  /* Variables para manejar los archivos origen y
  destino. */
  int source_file, destination_file;

  /* Revisa que el número de parametros ingresado sea el
  correcto (nombre del programa, archivo fuente y archivo destino). */
  if (argc != 3)
  {
    printf("usage: %s origen destination\n", argv[0]);
    return -1;
  }

  /* Abre el archivo ingresado como fuente y si no existe 
   o no se abre manda un mensaje de error. */
  if((source_file = open(argv[1], O_RDONLY)) < 0)
  {
    printf("%s: No such file or directory\n", argv[0]);
    return -2;
  }

  /* Abre el archivo ingresado como destino y si no existe 
  lo crea y verifica que exista, en este archivo se va a
  escribir la información desencriptada del archivo fuente. */
  if((destination_file = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666 )) < 0)
  {
    printf("%s: No such file or directory\n", argv[0]);
    return -3;
  }
  
  /* Abre el archivo encritado ingresado como fuente y 
  escribe en el archivo destino la información desencriptada. */
  source_file_size = lseek(source_file, 0, SEEK_END);
  lseek(source_file, 0, SEEK_SET);

  int uchar_size = sizeof(uchar);

  for (int i = 0; i < source_file_size; i++)
  {
    read(source_file, &buffer, uchar_size);

    reg = (buffer & 0x0F)<<4 | (buffer & 0xF0)>>4;

    write(destination_file, &reg, uchar_size);
  }
  
  /* Cierra los archivos fuente y destino despues de 
  que se hiceron las operaciones correspondientes para
  la desecnriptación. */
  close(source_file);
  close(destination_file);

  /* Indica al usuario que se terminaron de ejecutar de 
  forma correcta las operaciones para realizar la desencriptación. */
  puts("done");

  // Termina el programa cuando se ejecuta correctamente.
  return 0;

}