#include <stdlib.h>
#include <string.h>
#include "block.h"

Block* createBlock(int len, Block* next) {
	Block* b = (Block*) malloc (sizeof(Block));
	b->data = (char*) malloc(sizeof(len));
	b->nextBlock = next;
	return b;
}

Block* createBlockData(int len, char* data, Block* next) {
    Block* b = (Block*) malloc (sizeof(Block));
    b->data = (char*) malloc(sizeof(len));
    strcpy(b->data, data);
    b->nextBlock = next;
    return b;
}

void deleteBlock(Block* this) {
	free(this->data);
	free(this);
}

