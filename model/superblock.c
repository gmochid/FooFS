#include "superblock.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void setBlocksData(SuperBlock* sb, Inode* this, char* data) {
	Block* now = this->block;
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

char* getBlocksData(SuperBlock* sb, Inode* this) {
	char* buff = (char*) malloc(this->fileSize * sizeof(char));
	Block* now = this->block;
	while(now != NULL) {
		strcat(buff, now->data);
		now = now->nextBlock;
	}
	return buff;
}

void createInode(SuperBlock * sb, Inode* parent, char * name, long fileSize, int type) {
	Inode* inode = (Inode*) malloc(sizeof(Inode));
	inode->fileSize = fileSize;
	inode->name = name;
	inode->type = type;
	//Alokasi Block (bingung)

	if(parent == NULL) { // direktori masih kosong
		sb->root = inode;
	} else if(parent != NULL) {
		inode ->sibling = parent->child;
		parent->child = inode;
		inode->parent = parent;
	}
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
	freeInode(now);
}
