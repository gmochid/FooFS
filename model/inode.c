#include "inode.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


void freeInode(Inode* this) {
	Block* now = this->block;
	Block* before;
	while(now != NULL) {
		before = now;
		now = now->nextBlock;
		deleteBlock(before);
	}
	free(this);
}

