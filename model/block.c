#include <stdlib.h>
#include "block.h"

Block* createBlock(int len, Block* next) {
	Block* b = (Block*) malloc (sizeof(Block));
	b->data = (char*) malloc(sizeof(len));
	b->nextBlock = next;
	return b;
}

void deleteBlock(Block* this) {
	free(this);
}

void setData(Block* this, char* data) {
	this->data = data;
}

void setNext(Block* this, Block* next) {
	this->nextBlock = next;
}

char* getData(Block* this) {
	return this->data;
}

Block* getNextBlock(Block* this) {
	return this->nextBlock;
}
