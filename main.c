#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "get_next_line.h"

int main(int argc, char **argv)
{
	if (argc < 2) {
		fprintf(stderr, "Usage: %s <file>\n", argv[0]);
		return 1;
	}
	int fd = open(argv[1], O_RDONLY);
	if (fd < 0) {
		perror("open");
		return 1;
	}
	char *line;
	int i = 1;
	while ((line = get_next_line(fd)) != NULL)
	{
		printf("[%02d] %s", i++, line);
		if (line[0] && line[gnl_strlen(line)-1] != '\n') printf("\n"); /* ensure visible line break */
		free(line);
	}
	close(fd);
	return 0;
}
