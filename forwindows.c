/*
Title   : filetree (for windows)
Author  : ch4rp
Date    : 30 Mar 2023

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
    
    int main(void)
    {
      void (*ret)(const char *, int) = printfile;
      const char *startpath = "C:\\Users\\x\\Documents";
    
      tree(startpath, ret);
    
      return 0;
    }

  $ gcc -c main.c
  $ gcc -c forwindows.c
  $ gcc main.o forwindows.o -o tree
  $ .\tree 
*/


#include <stdio.h>
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

  memset(filepath, '\0', MAX_PATH_LENG);
  strcpy(filepath, path);
  strcat(filepath, "\\");
  filepath[strlen(filepath)] = '*';

  if ((hFindFile = FindFirstFile(filepath, &fileinfo)) == INVALID_HANDLE_VALUE)
    return 1;

  filepath[strlen(filepath)-1] = '\0';


  do {
    ftype = KFILE;

    if (!(strcmp(fileinfo.cFileName, ".") && strcmp(fileinfo.cFileName, "..")))
      continue;
    
    memset(filepath, '\0', MAX_PATH_LENG);
    strcpy(filepath, path);
    strcat(filepath, "\\");
	strcat(filepath, fileinfo.cFileName);

    if (fileinfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
      tree(filepath, func);
      ftype = KDIR;
    }

    (*func)(filepath, ftype);

  }while (FindNextFile(hFindFile, &fileinfo));

  FindClose(hFindFile);

  return 0;
}


