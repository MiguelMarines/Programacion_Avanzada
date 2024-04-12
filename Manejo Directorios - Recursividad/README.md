# Actividad Manejo de Directorios
Escribe un programa llamado *find* que recibe de línea de comando una cadena de texto a buscar, un directorio base y, de forma opcional, si queremos que la búsqueda sea recursiva o no.

```
$ .\find string initial_directory [-r]
```

Ejemplos de uso:
```
$ .\find
usage: find string initial_directory [-r]
--------------------------------------------------------------------------
$ .\find apt no_existe
find: No such file or directory
--------------------------------------------------------------------------
$ find apt find.c
find: Not a directory
--------------------------------------------------------------------------
$ .\find apt dir1 -x
usage: find string initial_directory [-r]
--------------------------------------------------------------------------
$ .\find apt dir1
Name: chapter2.txt
Path: dir1

Name: chapter1.txt
Path: dir1
--------------------------------------------------------------------------
$ .\find apt dir1 -r
Name: chapter2.txt
Path: dir1

Name: chapter1.txt
Path: dir1

Name: chapter3.txt
Path: dir1/dir2

Name: chapter4.txt
Path: dir1/dir2

Name: chapter6.txt
Path: dir1/dir2/dir3

Name: chapter5.txt
Path: dir1/dir2/dir3

Name: chapter8.txt
Path: dir1/dir2/dir3/dir4

Name: chapter7.txt
Path: dir1/dir2/dir3/dir4

Name: chapter9.txt
Path: dir1/dir2/dir3/dir4/dir5

Name: chapter10.txt
Path: dir1/dir2/dir3/dir4/dir5
```