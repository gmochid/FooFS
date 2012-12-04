/*
 * inode.h
 *
 *  Created on: Dec 3, 2012
 *      Author: gmochid
 */

#ifndef INODE_H_
#define INODE_H_

#include "block.h"

typedef struct _Inode {
	char* name; // nama file ataupun direktori ini
	long fileSize; // besarnya ukuran node ini bila merupakan sebuah file
	int type; // menyatakan direktori bila 0 dan file bila 1
	struct _Inode* parent; // parent dari inode ini, bila null maka root
	struct _Inode* sibling; // menyatakan next inode yang berada pada direktori yang sama
	struct _Inode* child; // menyatakan child dari inode ini
	Block* block; // block yang berisi data
} Inode;

void freeInode(Inode* this);
void printInode(Inode* this);

#endif /* INODE_H_ */
