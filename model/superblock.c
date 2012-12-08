#include "superblock.h"

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
	sb->directoryCount = 0;
	sb->fileCount = 0;
	return sb;
}

void useResources(SuperBlock* sb, long cap) {
    sb->availableCapacity -= cap;
    sb->usedCapacity += cap;
    int uB = cap / sb->sizeBlock;
    sb->availableBlock -= uB;
    sb->usedBlock += uB;
}

void releaseResources(SuperBlock* sb, long cap) {
    sb->availableCapacity += cap;
    sb->usedCapacity  -= cap;
    int uB = cap / sb->sizeBlock;
    sb->availableBlock += uB;
    sb->usedBlock = uB;
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

Inode* createInode(SuperBlock * sb, Inode* parent, char * name, long fileSize, int type) {
	if(sb->availableCapacity < fileSize) {
		return NULL;
	}

	Inode* inode = (Inode*) malloc(sizeof(Inode));
	inode->name = (char*) malloc(sizeof(char) * STDSIZE);
	inode->fileSize = fileSize;
	strcpy(inode->name, name);
	inode->type = type;
	inode->sibling = NULL;
	inode->parent = NULL;
	inode->child = NULL;
	if(type) {
	    sb->fileCount++;
	} else {
	    sb->directoryCount++;
	}

	if(inode->type) {
        long i, totalBlock = fileSize / sb->sizeBlock;
        Block* now = createBlock(sb->sizeBlock, NULL);
        for(i = 0; i < totalBlock; i++) {
            if(i == (totalBlock - 1)) {
                inode->block = now;
            } else {
                now = createBlock(sb->sizeBlock, now);
            }
        }
	}

	useResources(sb, fileSize);

	if(parent == NULL) { // direktori masih kosong
		sb->root = inode;
	} else if(parent != NULL) {
		inode ->sibling = parent->child;
		parent->child = inode;
		inode->parent = parent;
	}

	return inode;
}

void deleteInode(SuperBlock* sb, Inode* now, Inode* inode) {
	if(now == inode) {
	    if(inode->type) {
            sb->fileCount--;
        } else {
            sb->directoryCount--;
        }

		if(now == sb->root) {
		    releaseResources(sb, now->fileSize);
			removeInode(sb, now->child);
			freeInode(now);
			sb->root = NULL;
		} else {
		    releaseResources(sb, now->fileSize);
			removeInode(sb, now->child);
			freeInode(now);
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
	releaseResources(sb, now->fileSize);
	freeInode(now);
}

char* getPathFromInode(SuperBlock* sb, Inode* inode) {
    //printf("--%s--%d\n", inode->name, inode == sb->root);
    if(inode == sb->root) {
        return NULL;
    } else if(inode != NULL) {
        char* pathRekursif = getPathFromInode(sb, inode->parent);
        char* path;
        int len;
        if(inode->parent != sb->root) {
            len = strlen(pathRekursif) + 1 + strlen(inode->name);
            path = (char*) malloc(sizeof(char) * len);
            strcpy(path, pathRekursif);
            strcat(path, "/");
            strcat(path, inode->name);
        } else {
            len = 1 + strlen(inode->name);
            path = (char*) malloc(sizeof(char) * len);
            strcpy(path, "/");
            strcat(path, inode->name);
        }
        return path;
    }
    return NULL;
}

Inode* getInodeFromPath(SuperBlock* sb, char* path) {
    if(strcmp(path, "/") == 0) {
        return sb->root;
    }
    char** arr_path; int i;

    //membuat array dari token-token filepath
    char z[STDSIZE];
    strcpy(z, path);
    int len = tokenizer(z, &arr_path, '/', 100);

    //printf("||len = %d - %s\n", len, );

    Inode* now = sb->root;
    for(i = 0; i < len; i++) {
        //printf("%s\n", *(arr_path + i));
        if(now == NULL)
            return NULL;
        now = now->child;
        while(now != NULL) {
            if(strcmp(now->name, *(arr_path + i)) == 0) {
                break;
            }
            now = now->sibling;
        }
    }
    return now;
}

void moveInode(SuperBlock*sb, Inode* from, Inode* parentTo) {
}
