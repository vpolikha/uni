#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	char *addr;
	int fd;
	struct stat sb;

	if (argc != 2) {
		fprintf(stderr, "Usage error: %s file\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	fd = open(argv[1], O_RDONLY);

	if (fd == -1) {
		fprintf(stderr, "open error");
		exit(EXIT_FAILURE);
	}

	/* Obtain the size of the file and use it to specify the size of
	the mapping and the size of the buffer to be written */
	if (fstat(fd, &sb) == -1) {
		fprintf(stderr, "fstat error");
		exit(EXIT_FAILURE);
	}

	addr = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

	if (addr == MAP_FAILED) {
		fprintf(stderr, "mmap error");
		exit(EXIT_FAILURE);
	}

	if (write(STDOUT_FILENO, addr, sb.st_size) != sb.st_size) {
		fprintf(stderr, "partial/failed write");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}
