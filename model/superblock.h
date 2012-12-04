/*
 * superblock.h
 *
 *  Created on: Dec 3, 2012
 *      Author: gmochid
 */

#ifndef SUPERBLOCK_H_
#define SUPERBLOCK_H_

#include "inode.h"

typedef struct {
	long capacity; // kapasitas total sebuah blok
	long usedCapacity; // kapasitas yang telah digunakan
	long availableCapacity; // kapasitas yang masih kosong
	char* name; // nama dari blok ini
	long sizeBlock; // besar ukuran dari sebuah blok pada filesystem ini
	long totalBlock; // total block pada filesystem ini
	long usedBlock; // total block yang telah dialokasi
	long availableBlock; // total block yang masih dapat dialokasi
	Inode* root; // pointer pada struktur data yg dimiliki superblock ini
} SuperBlock;

/**
 * Membuat SuperBlock baru
 * Relasi (capacity = sizeBlock * totalBlock) harus dipenuhi
 */
SuperBlock* createSuperBlock(char* name, long capacity, long sizeBlock, long totalBlock); // DONE

void deleteInode(SuperBlock* sb, Inode* now, Inode* inode); // DONE
void removeInode(SuperBlock* sb, Inode* now); // DONE

int createInode(SuperBlock * sb, Inode* parent, char * name, long fileSize, int type); // kalau parent==null, maka root, bingung
void convertToFile(SuperBlock* sb, char* filename); // mikir dulu ngesave ke filenya

Inode* getInodeFromPath(SuperBlock* sb, char* path);
char* getPathFromInode(SuperBlock* sb, Inode* inode);

void setBlocksData(SuperBlock* sb, Inode* this, char* data); // DONE
char* getBlocksData(SuperBlock* sb, Inode* this); // DONE

#endif /* SUPERBLOCK_H_ */
