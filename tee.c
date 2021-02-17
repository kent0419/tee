#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/types.h>  /* Type definitions used by many programs */
#include <stdio.h>      /* Standard I/O functions */
#include <stdlib.h>     /* Prototypes of commonly used library functions,
						   plus EXIT_SUCCESS and EXIT_FAILURE constants */
#include <unistd.h>     /* Prototypes for many system calls */
#include <errno.h>      /* Declares errno and defines error constants */
#include <string.h>     /* Commonly used string-handling functions */
#include <stdbool.h>    /* 'bool' type plus 'true' and 'false' constants */
#include <stdarg.h>
#include "get_num.h"
#define MAXLEN  1024

void usageErr(const char *format, ...)
{
	va_list argList;

	fflush(stdout);           /* Flush any pending stdout */

	fprintf(stderr, "Usage: ");
	va_start(argList, format);
	vfprintf(stderr, format, argList);
	va_end(argList);

	fflush(stderr);           /* In case stderr is not line-buffered */
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
	size_t len;
	off_t offset;
	int fd, ap, j;
	char *buf;
	ssize_t numRead, numWritten;
	int ret;
	char *p;


	if (argc < 2 || strcmp(argv[1], "--help") == 0)
		usageErr("%s file {-a}...\n",
				argv[0]);

	p = (char*)malloc(MAXLEN*(sizeof(char*)));
	memset(p,0,MAXLEN);
	ret = read(0,p,MAXLEN);

	if (argc == 3 && (strcmp(argv[2],"-a")==0)) {
		fd = open(argv[1], O_RDWR | O_APPEND,
				S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
				S_IROTH | S_IWOTH ); /* rw-rw-rw- */
		if (fd == -1)
			exit(EXIT_FAILURE);
	}
	else{
		fd = open(argv[1], O_RDWR | O_CREAT,
				S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
				S_IROTH | S_IWOTH); /* rw-rw-rw- */
		if (fd == -1)
			exit(EXIT_FAILURE);
	}
	numWritten = write(fd, p, strlen(p));
	if (numWritten == -1)
		exit(EXIT_FAILURE);
	printf("%s: wrote %ld bytes\n", argv[1], (long) numWritten);

	exit(EXIT_SUCCESS);
}
