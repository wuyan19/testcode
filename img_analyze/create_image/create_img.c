#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <arpa/inet.h>

unsigned long get_file_size(const char *path)
{
	unsigned long filesize = -1;	
	struct stat statbuff;
	if(stat(path, &statbuff) < 0)
	{
		return filesize;
	}else
	{
		filesize = statbuff.st_size;
	}
	return filesize;
}

int main (int argc, char* argv[])
{
	unsigned int header_size = 200;
	unsigned int block0_size = 0;
	unsigned int block1_size = 0;
	unsigned int block2_size = 0;
	unsigned int block3_size = 0;
	unsigned int total_size = 0;
	unsigned char fill_size = 0;
	unsigned int crc_result = 0;
	char *crc_argv[4];
	char temp_str[20] = {0};

	char command[200];	

	if (0)
	{
		printf("Usage:\n\t%s [ToolsPath] [OutFileName]\n", argv[0]);
		return 0;
	}

	block0_size = get_file_size ("./block0");
	block1_size = get_file_size ("./block1");
	block2_size = get_file_size ("./block2");
	block3_size = get_file_size ("./block3");

	printf("block0_size = %u,%#x\n", block0_size, block0_size);
	printf("block1_size = %u,%#x\n", block1_size, block1_size);
	printf("block2_size = %u,%#x\n", block2_size, block2_size);
	printf("block3_size = %u,%#x\n", block3_size, block3_size);

	total_size = header_size + block0_size + block1_size + block2_size + block3_size;
	printf("total_size = %u\n", total_size);

	memset(command, 0, sizeof(command));
	sprintf(command, "./tools/extract_data_append ./header bitway.img 0 %u", header_size);
	system(command);
	
	if((header_size % 4) != 0)
	{
		fill_size = 4 - (header_size % 4);
	}
	printf("fill_size = %u\n", fill_size);

	memset(command, 0, sizeof(command));
	sprintf(command, "./tools/extract_data_append ./block0 bitway.img 0 %u", block0_size);
	system(command);

	if((block0_size % 4) != 0)
	{
		fill_size = 4 - (block0_size % 4);
	}
	printf("fill_size = %u\n", fill_size);

	memset(command, 0, sizeof(command));
	sprintf(command, "./tools/extract_data_append ./block1 bitway.img 0 %u", block1_size);
	system(command);

	if((block1_size % 4) != 0)
	{
		fill_size = 4 - (block1_size % 4);
	}
	printf("fill_size = %u\n", fill_size);

	memset(command, 0, sizeof(command));
	sprintf(command, "./tools/extract_data_append ./block2 bitway.img 0 %u", block2_size);
	system(command);

	if((block2_size % 4) != 0)
	{
		fill_size = 4 - (block2_size % 4);
	}
	printf("fill_size = %u\n", fill_size);

	memset(command, 0, sizeof(command));
	sprintf(command, "./tools/extract_data_append ./block3 bitway.img 0 %u", block3_size);
	system(command);

	memset(command, 0, sizeof(command));
	sprintf(command, "./tools/edit_file ./bitway.img 180 %#x", htonl(block0_size));
	system(command);
	printf("htonl(block0_size) = %#x\n", htonl(block0_size));

	memset(command, 0, sizeof(command));
	sprintf(command, "./tools/edit_file ./bitway.img 184 %#x", htonl(block1_size));
	system(command);
	printf("htonl(block1_size) = %#x\n", htonl(block1_size));

	memset(command, 0, sizeof(command));
	sprintf(command, "./tools/edit_file ./bitway.img 188 %#x", htonl(block2_size));
	system(command);
	printf("htonl(block2_size) = %#x\n", htonl(block2_size));

	memset(command, 0, sizeof(command));
	sprintf(command, "./tools/edit_file ./bitway.img 192 %#x", htonl(block3_size));
	system(command);
	printf("htonl(block3_size) = %#x\n", htonl(block3_size));

	memset(command, 0, sizeof(command));
	sprintf(command, "./tools/edit_file ./bitway.img 128 %#x", htonl(total_size - 180));
	system(command);

	crc_argv[0] = "./crc32";
	crc_argv[1] = "./bitway.img";
	crc_argv[2] = "180";
	sprintf(temp_str, "%u", total_size - 180);
	crc_argv[3] = temp_str;
	crc_result = compute(4, crc_argv);
	printf("image crc = %#x\n", crc_result);

	memset(command, 0, sizeof(command));
	sprintf(command, "./tools/edit_file ./bitway.img 140 %#x", htonl(crc_result));
	system(command);

	memset(command, 0, sizeof(command));
	sprintf(command, "./tools/edit_file ./bitway.img 120 0");
	system(command);

	crc_argv[0] = "./crc32";
	crc_argv[1] = "./bitway.img";
	crc_argv[2] = "116";
	crc_argv[3] = "64";
	crc_result = compute(4, crc_argv);
	printf("image header crc = %#x\n", crc_result);

	memset(command, 0, sizeof(command));
	sprintf(command, "./tools/edit_file ./bitway.img 120 %#x", htonl(crc_result));
	system(command);

	return 0;
}
