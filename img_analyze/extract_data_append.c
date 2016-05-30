#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

#define PAGE_LEN 1024

int main (int argc, char* argv[])
{
	int fd_in = -1;
	int fd_out = -1;
	unsigned int start_addr = 0;
	unsigned int size = 0;
	unsigned char data[PAGE_LEN] = {0};
	unsigned int loop = 0;
	if (argc < 5)
	{
		printf("Usage:\n\t%s <InFileName> <OutFileName> <StartAddr> <Size>\n", argv[0]);
		return -1;
	}
	fd_in = open(argv[1], O_RDONLY);
	if (fd_in < 0)
	{
		printf("open %s is failed!\n", argv[0]);
		return -2;
	}
	fd_out = open(argv[2], O_WRONLY | O_APPEND | O_CREAT, 0666);
	if (fd_out < 0)
	{
		printf("create %s is failed!\n", argv[0]);
		return -3;
	}
	start_addr = strtoul(argv[3], NULL, 0);
	size = strtoul(argv[4], NULL, 0);
	lseek(fd_in, start_addr, SEEK_SET);
	for(loop = 0; loop < size/PAGE_LEN; loop++)
	{
		read(fd_in, data, PAGE_LEN);
		write(fd_out, data, PAGE_LEN);
	}
	read(fd_in, data, size%PAGE_LEN);
	write(fd_out, data, size%PAGE_LEN);
	close(fd_in);
	close(fd_out);
	return 0;
}
