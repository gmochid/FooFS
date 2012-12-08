/*
 * shm.h
 *
 *  Created on: Dec 8, 2012
 *      Author: rosyid
 */

#ifndef SHM_H_
#define SHM_H_

#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include "helper.h"

#define SHMFILE "/tmp/foofsshm"
#define SHMSIZE (1 << 10)

#define SHM_OPEN (1 << 0)
#define SHM_INIT (1 << 1)
#define SHM_END  (1 << 2)

char* open_shm();
char* open_and_init_shm();
char* close_shm();

#endif /* SHM_H_ */
