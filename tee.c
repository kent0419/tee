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
	if (argc < 3 || strcmp(argv[1], "--help") == 0)
		usageErr("%s file {r<length>|R<length>|w<string>|s<offset>}...\n",
				argv[0]);
	fd = open(argv[1], O_RDWR | O_CREAT,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
			S_IROTH | S_IWOTH); /* rw-rw-rw- */
	if (fd == -1)
		exit(EXIT_FAILURE);
	for (ap = 2; ap < argc; ap++) {
		switch (argv[ap][0]) {
			case 'r': /* Display bytes at current offset, as text */
			case 'R': /* Display bytes at current offset, in hex */
				len = getLong(&argv[ap][1], GN_ANY_BASE, argv[ap]);
	
					buf = malloc(len);
				if (buf == NULL)
					exit(EXIT_FAILURE);
				numRead = read(fd, buf, len);
				if (numRead == -1)
					exit(EXIT_FAILURE);
				if (numRead == 0) {
					printf("%s: end-of-file\n", argv[ap]);
				} else {
					printf("%s: ", argv[ap]);
					for (j = 0; j < numRead; j++) {
						if (argv[ap][0] == 'r')
							printf("%c", isprint((unsigned char) buf[j]) ?
									buf[j] : '?');
						else
							printf("%02x ", (unsigned int) buf[j]);
					}
					printf("\n");
				}
				free(buf);
				break;
			case 'w': /* Write string at current offset */
				numWritten = write(fd, &argv[ap][1], strlen(&argv[ap][1]));
				if (numWritten == -1)
					exit(EXIT_FAILURE);
				printf("%s: wrote %ld bytes\n", argv[ap], (long) numWritten);
				break;
			case 's': /* Change file offset */
				offset = getLong(&argv[ap][1], GN_ANY_BASE, argv[ap]);
				if (lseek(fd, offset, SEEK_SET) == -1)
					exit(EXIT_FAILURE);
				printf("%s: seek succeeded\n", argv[ap]);
				break;
			default:
				printf("Argument must start with [rRws]: %s\n", argv[ap]);
		}
	}
	exit(EXIT_SUCCESS);
}
