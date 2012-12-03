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

SuperBlock* createSuperBlock(long capacity, char* name, long sizeBlock, long totalBlock);

void deleteInode(Inode* inode);

//ME
void addInode(Inode* parent); // kalau parent==null, maka root
void convertToFile(char* filename);
Inode* getInodeFromPath(char* path);
void setBlocksData(SuperBlock* sb, Inode* this, char* data);
char* getBlocksData(SuperBlock* sb, Inode* this);;

#endif /* SUPERBLOCK_H_ */
