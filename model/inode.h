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

Inode* createInode(char * name, long fileSize, int type, Inode* parent);
void deleteInode(Inode* this);

void setSibling(Inode* this, Inode* sibling);
void setChild(Inode* this, Inode* child);

Inode* getParent(Inode* this);
Inode* getSibling(Inode* this);
Inode* getChild(Inode* this);
Block* getBlock(Inode* this);

// ME
void setBlocksData(char* data);
char* getBlocksData();

#endif /* INODE_H_ */
