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

void printInode(Inode* this) {
    printf("--%s--\n", this->name);
    if(this->parent == NULL) {
        printf("-parent: ROOT\n");
    } else {
        printf("-parent: %s\n", this->parent->name);
    }

    if(this->child == NULL) {
            printf("-child: LEAF\n");
        } else {
            printf("-child: %s\n", this->child->name);
        }
}

