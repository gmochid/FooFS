/*
 * block.h
 *
 *  Created on: Dec 3, 2012
 *      Author: gmochid
 */

#ifndef BLOCK_H_
#define BLOCK_H_

typedef struct _Block {
	char* data; // data yang disimpan
	struct _Block* nextBlock; // block selanjutnya yang berada pada inode yang sama
} Block;

Block* createBlock(int len, Block* next);
void deleteBlock(Block* this);

void setData(Block* this, char* data);
void setNext(Block* this, Block* next);

char* getData(Block* this);
Block* getNextBlock(Block* this);

#endif /* BLOCK_H_ */
