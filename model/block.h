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
Block* createBlockData(int len, char* data, Block* next);
void deleteBlock(Block* this);

#endif /* BLOCK_H_ */
