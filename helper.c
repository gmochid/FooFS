#include "helper.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int tokenizer(char* str, char*** arr_token, char tok, int max_arr_token) {
	int len, i, j, k;

	*arr_token = (char**) calloc(max_arr_token, sizeof(char*));

	len = strlen(str);

	for (i = 0; i < len; i++) {
		str[i] = (str[i] == tok) ? '\0' : str[i];
	}

	k = 1; j = 0;
	for (i = 0; i < len; i++) {
		if(str[i] != 0 && k == 1) {
			(*arr_token)[j++] = &str[i];
			k = 0;
		} else if(str[i] == '\0') {
			k = 1;
		}
	}

	return j;
}

void bufferFile(char* path, char** buff, int size_buff) {
	*buff = (char*) malloc(size_buff * sizeof(char));
	int fd, fdA, fdB;
	int size;

	fdA = open(path, O_RDONLY);
	size = lseek(fdA, 0, SEEK_END);

	lseek(fdA, 0, SEEK_SET);
	read(fdA, *buff, size);
}

void bufferFileTo(char* path, char*toPath, char** buff, int size_buff) {
	*buff = (char*) malloc(size_buff * sizeof(char));
	int fd, fdA, fdB;
	int size;
	unlink(toPath);

	fdA = open(path, O_RDONLY);
	size = lseek(fdA, 0, SEEK_END);
	fdB = open(toPath, O_RDWR | O_APPEND | O_CREAT, 0664);

	lseek(fdA, 0, SEEK_SET);
	read(fdA, *buff, size);
	write(fdB, *buff, size);
}

/*
int main()
{
	int size = 1 << 13;
	char* buffer;
	bufferFileTo("x.png", "z.png", &buffer, size);

	return 0;
}
*/


/*
void bufferFile()
{
    // Copy file dengan buffer 1kb
    char buff[1 << 10];
    int fd, fdA, fdB;
    int size;
    int iread;

    // Mendapatkan file-size
    fd = open(FRNAME, O_RDONLY);
    size = lseek(fd, 0, SEEK_END);
    close(fd);

    iread = 0;

    unlink(FWNAME);

    fdA = open(FRNAME, O_RDONLY);
    fdB = open(FWNAME, O_RDWR | O_APPEND | O_CREAT, 0664);

#if 0
    // Unchunked, langsung sebesar BUFFSIZE
    char buff[BUFFERSIZE];
    lseek(fdA, 0, SEEK_SET);
    read(fdA, buff, sizeof(buff);

    write(fdB, buff, sizeof(buff));
#endif
}
*/
