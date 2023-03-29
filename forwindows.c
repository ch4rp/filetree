/*
Title   : filetree (for windows)
Author  : ch4rp
Date    : 29 Mar 2023

Example usage:
  main.c:
    #include <stdio.h>
    #include <string.h>
    
    int tree(const char *path, void (*func)(const char *, int));
    
    void printfile(const char *string, int filetype)
    {
      printf("%c %s\n", filetype ? 'D' : '-', string);
    }
    
    #define MAX_PATH_LENG 1024
    
    int main(int argc, char **argv)
    {
      char fullpath[MAX_PATH_LENG];
      memset(fullpath, 0, MAX_PATH_LENG);
      strcpy(fullpath, argv[1]);
      strcat(fullpath, "\\");
    
      void (*ret)(const char *, int) = printfile;
      tree(fullpath, ret);
      return 0;
    }


  $ gcc -c main.c
  $ gcc -c filetree.c
  $ gcc main.o filetree.o -o tree
  $ .\tree C:\\Users\\x\\Documents
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define MAX_PATH_LENG	1024

typedef enum _Filetype {
  KFILE = 0,
  KDIR = 1
}Filetype; // directory = 1, not directory = 0

BOOL tree(LPCSTR path, VOID (*func)(LPCSTR, DWORD))
{

  char filepath[MAX_PATH_LENG];
  Filetype ftype;

  WIN32_FIND_DATA fileinfo;
  HANDLE hFindFile;

  memset(filepath, 0, MAX_PATH_LENG);
  strcpy(filepath, path);
  strcat(filepath, "\\\\");
  strcat(filepath, "*");

  //printf("pwd: %s\n|\n", filepath);

  if ((hFindFile = FindFirstFile(filepath, &fileinfo)) == INVALID_HANDLE_VALUE)
  return 1;

  do {
    ftype = KFILE;

    if (!(strcmp(fileinfo.cFileName, ".") && strcmp(fileinfo.cFileName, "..")))
      continue;

    filepath[strlen(filepath)-1] = '\0';
    strcat(filepath, fileinfo.cFileName);

    if (fileinfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
      strcat(filepath, "\\\\");
      tree(filepath, func);
      ftype = KDIR;
    }

    (*func)(filepath, ftype);

  }while (FindNextFile(hFindFile, &fileinfo));

  FindClose(hFindFile);

  return 0;
}


