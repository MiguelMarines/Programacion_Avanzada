/*----------------------------------------------------------------

* Programación Avanzada: Manejo de Archivos

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
  ulong input_file_size;
  uchar buffer, reg;

  int input_file, output_file;

  // Revisa el número de parametros.
  if (argc != 3)
  {
    printf("usage: %s source destination\n", argv[0]);
    return -1;
  }

  // Abre los archivos ingresados y checa si existen.
  if((input_file = open(argv[1], O_RDONLY)) < 0)
  {
    printf("%s: No such file or directory\n", argv[0]);
    return -2;
  }


  // Abre el archivo de output y sheca que exista.
  if((output_file = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666 )) < 0)
  {
    printf("%s: No such file or directory\n", argv[0]);
    return -3;
  }
  
  //Abre el archivo ingresado y genera el archivo desencriptado.
  input_file_size = lseek(input_file, 0, SEEK_END);
  lseek(input_file, 0, SEEK_SET);

  int uchar_size = sizeof(uchar);

  for (int i = 0; i < input_file_size; i++)
  {
    read(input_file, &buffer, uchar_size);
    reg = (buffer & 0x0F)<<4 | (buffer & 0xF0)>>4;
    write(output_file, &reg, uchar_size);
  }
  
  close(input_file);
  close(output_file);

  return 0;
}