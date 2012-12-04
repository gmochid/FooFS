#include "superblock.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

SuperBlock* createSuperBlock(char* name, long capacity, long sizeBlock, long totalBlock) {
	SuperBlock* sb = (SuperBlock*) malloc (sizeof(SuperBlock));
	sb->sizeBlock = sizeBlock;
	sb->name = name;
	sb->capacity = capacity;
	sb->availableCapacity = sb->capacity;
	sb->usedCapacity = 0;
	sb->totalBlock = totalBlock;
	sb->availableBlock = sb->totalBlock;
	sb->usedBlock = 0;
	sb->root = NULL;
	return sb;
}

void setBlocksData(SuperBlock* sb, Inode* this, char* data) {
	Block* now = this->block;
	int size = sb->sizeBlock;
	int fsize = strlen(data);
	int offset = 0;
	while(now != NULL) {
		if(fsize >= size) {
			memcpy(now->data, data + offset, sb->sizeBlock);
			(*(now->data + size)) = '\0';
		} else {
			memcpy(now->data, data + offset, fsize);
			(*(now->data + fsize)) = '\0';
		}
		printf("%d\n", strlen(now->data));
		offset += sb->sizeBlock;
		fsize -= sb->sizeBlock;
		now = now->nextBlock;
	}
}

char* getBlocksData(SuperBlock* sb, Inode* this) {
	char* buff = (char*) malloc(this->fileSize * sizeof(char));
	Block* now = this->block;
	while(now != NULL) {
		strcat(buff, now->data);
		now = now->nextBlock;
	}
	return buff;
}

int createInode(SuperBlock * sb, Inode* parent, char * name, long fileSize, int type) {
	if(sb->availableCapacity < fileSize) {
		return 1;
	}

	Inode* inode = (Inode*) malloc(sizeof(Inode));
	inode->fileSize = fileSize;
	inode->name = name;
	inode->type = type;
	//Alokasi Block (bingung)

	sb->usedCapacity += fileSize;
	sb->availableCapacity -= fileSize;

	if(parent == NULL) { // direktori masih kosong
		sb->root = inode;
	} else if(parent != NULL) {
		inode ->sibling = parent->child;
		parent->child = inode;
		inode->parent = parent;
	}

	return 0;
}

void deleteInode(SuperBlock* sb, Inode* now, Inode* inode) {
	if(now == inode) {
		if(now == sb->root) {
			removeInode(sb, now);
			sb->root = NULL;
		} else {
			removeInode(sb, now);
		}
	} else {
		if(now->sibling != NULL) {
			if(now->sibling == inode) {
				now->sibling = now->sibling->sibling;
				deleteInode(sb, inode, inode);
				return;
			} else {
				deleteInode(sb, now->sibling, inode);
			}
		}
		if(now->child != NULL) {
			if(now->child == inode) {
				now->child = now->child->sibling;
				deleteInode(sb, inode, inode);
				return;
			} else {
				deleteInode(sb, now->child, inode);
			}
		}
	}
}

void removeInode(SuperBlock* sb, Inode* now) {
	if(now->sibling != NULL) {
		removeInode(sb, now->sibling);
	}
	if(now->child != NULL) {
		removeInode(sb, now->child);
	}
	sb->usedCapacity -= now->fileSize;
	sb->availableCapacity += now->fileSize;
	freeInode(now);
}
