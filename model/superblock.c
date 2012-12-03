#include "superblock.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void setBlocksData(SuperBlock* sb, Inode* this, char* data) {
	Block* now = getBlock(this);
	int size = sb->sizeBlock;
	int fsize = (int) this->fileSize;
	int offset = 0;
	while(now != NULL) {
		if(fsize >= size) {
			memcpy(now->data, data + offset, size);
			(*(now->data + size)) = '\0';
		} else {
			memcpy(now->data, data + offset, fsize);
			(*(now->data + fsize)) = '\0';
		}
		offset += size;
		fsize -= size;
		now = now->nextBlock;
	}
}
