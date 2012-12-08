/*
 * shm.c
 *
 *  Created on: Dec 8, 2012
 *      Author: rosyid
 */

#include "shm.h"

char* _open_shm(int flags) {
    char *cmd;// = (char*) malloc(SHMSIZE * sizeof(char));
    int fd;

    if(flags & SHM_INIT) {
        fd = open(SHMFILE, O_CREAT | O_RDWR | O_TRUNC, 0666);
        lseek(fd, SHMSIZE, SEEK_END);
        write(fd, "", 1);
    } else if((flags & SHM_OPEN )||( flags & SHM_END)) {
        fd = open(SHMFILE, O_RDWR);
    }

    cmd = (char *) mmap(NULL, SHMSIZE, PROT_READ | PROT_WRITE, MAP_SHARED,
            fd, 0);

    close(fd);
    return cmd;
}

char* open_shm() {
    return _open_shm(SHM_OPEN);
}
char* open_and_init_shm() {
    return _open_shm(SHM_INIT);
}
char* close_shm() {
    return _open_shm(SHM_END);
}
