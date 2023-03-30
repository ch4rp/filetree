/*
 * Title   : filetree (for unix)
 * Date    : 12 February 2023
 * Author  : ch4rp
 * 
 * Example usage:
 * main.c:
		#include <stdio.h>
		int tree(const char *path, void (*func)(const char *, int));
		
		void printfile(const char *string, int filetype)
		{
			printf("%c %s\n", filetype ? 'D' : '-', string);
		}
		
		int main(int argc, char **argv)
		{
			void (*ret)(const char *, int) = printfile;
			tree(argv[1], ret);
			return 0;
		} 
  $ gcc -c main.c
  $ gcc -c filetree.c
  $ gcc main.o filetree.o -o tree
  $ ./tree /home
 */


#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#define MAX_PATH_LENG	1024
typedef enum _Ftype {
	// directory = 1, not directory = 0
	KFILE = 0, 
	KDIR = 1
}Ftype;

int tree(const char *path, void (*func)(const char *, int))
{
	
	
	DIR *currdir;
	struct dirent *direnv;
	struct stat    envinf;
	char filepath[MAX_PATH_LENG];
	Ftype filetype;
	
	if ((currdir = opendir(path)) == NULL)
		return 1;

	while (errno = 0, (direnv = readdir(currdir)) != NULL) {
		filetype = KFILE;
		
		// skip directories '.' and '..'
		if (!strcmp(direnv->d_name, ".") || !strcmp(direnv->d_name, ".."))
			continue;

		memset(filepath, 0, MAX_PATH_LENG);
		strcpy(filepath, path);
		strcat(filepath, "/");
		strcat(filepath, direnv->d_name);

		if (stat(filepath, &envinf) < 0) 
			continue;
		
		// if file is a directory
		if (envinf.st_mode & S_IFDIR) {
			tree(filepath, func);
			filetype = KDIR;
		}
		
		(*func)(filepath, filetype);
	}

	if (errno != 0)
		return errno;

	closedir(currdir);

	return 0;
}


