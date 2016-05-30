#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

int main (int argc, char* argv[])
{
	int fd = -1;
	unsigned int addr = 0;
	unsigned int data = 0;
	if (argc < 4)
	{
		printf("Usage:\n\t%s <InFileName> <Addr> <data>\n", argv[0]);
		return -1;
	}
	fd = open(argv[1], O_RDWR);
	if (fd < 0)
	{
		printf("open %s is failed!\n", argv[1]);
		return -2;
	}
	
	addr = strtoul(argv[2], NULL, 0);
	data = strtoul(argv[3], NULL, 0);
	lseek(fd, addr, SEEK_SET);
	write(fd, &data, sizeof(data));
	
	close(fd);
	return 0;
}
